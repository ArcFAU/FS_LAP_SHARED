// Headless acceptance test of the C++ physics: the AI lap must be 77.4 s +- 0.1 s on the stock car.
//   ailap [arcade|free|city|events] [trace.csv]    exit code 0 = pass (events: the tracks after FSG; city: Erlangen, from FSLAP_CITY)
// The same core runs in the game, so what passes here is what drives the car there.
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "HoMoLapCore.h"

int main(int argc, char** argv)
{
	using namespace HoMoLap;
	Mode mode = Mode::Sim;
	const char* tracePath = nullptr;
	int event = 0;   // ailap events: every event track after FSG
	for (int i = 1; i < argc; ++i)
	{
		if (!std::strcmp(argv[i], "arcade")) mode = Mode::Arcade;
		else if (!std::strcmp(argv[i], "free")) mode = Mode::Free;
		else if (!std::strcmp(argv[i], "city")) mode = Mode::City;
		else if (!std::strcmp(argv[i], "events")) event = 1;
		else tracePath = argv[i];
	}
	if (event)
	{
		// the tracks rebuilt from the cars' logs: the AI must lap each one; next to it the car's lap there. At the power of
		// a Tau endurance (32 kW, 10.4 Nm, tools\logs\README.md) it must also lap cleanly.
		bool ok = true;
		Setup endurance;
		endurance.Pmax = 32; endurance.Tm = 10.4;
		for (int i = 1; i < EventTrackCount(); ++i)
		{
			const Track track = MakeEventTrack(i);
			std::printf("%s: N=%d LEN=%.3f cones=%zu, %s lap %.2f s (S1 %.2f, S2 %.2f)\n", track.name, track.N, track.LEN, track.cones.size(), track.refName, track.ltsLap, track.ltsSplit[0], track.ltsSplit[1]);
			if (!track.terrain.empty())
			{
				// a hill: rolling 1 s down the steepest part of the line from 5 m/s, no pedals, must gain speed as gravity says
				int steep = 0;
				double grade = 0;
				for (int k = 0; k < track.N; ++k)
				{
					const double g = track.HeightAt(track.x[(k + 1) % track.N], track.y[(k + 1) % track.N]) - track.HeightAt(track.x[k], track.y[k]);
					if (g / track.SEG[k] < grade) { grade = g / track.SEG[k]; steep = k; }
				}
				Track flat = track;
				flat.terrain.clear();   // the same roll without the hill: the same rolling resistance and drag
				for (const double dir : { 1.0, -1.0 })
				{
					double v[2], d[2] = { 0, 0 }, dz = 0;
					for (int f = 0; f < 2; ++f)
					{
						Simulation hill(f ? flat : track, Setup{}, Mode::Sim);
						hill.PlaceAt(track.S[steep] - dir * 4);
						if (dir < 0) hill.c.h += 3.141592653589793;   // facing uphill
						hill.c.vx = 5;
						Input none;
						none.human = true;
						const double z0 = track.HeightAt(hill.c.x, hill.c.y);
						for (int k = 0; k < 600; ++k)
						{
							const double px = hill.c.x, py = hill.c.y;
							hill.Step(&none);
							d[f] += std::hypot(hill.c.x - px, hill.c.y - py);
						}
						v[f] = std::hypot(hill.c.vx, hill.c.vy);
						if (!f) dz = track.HeightAt(hill.c.x, hill.c.y) - z0;
					}
					// the energy: v^2 = 5^2 - 2 g dz - 2 a d, with the resistance a per metre from the flat roll
					const double a = (25 - v[1] * v[1]) / (2 * d[1]);
					const double expect = std::sqrt(std::max(0.0, 25 - 2 * 9.81 * dz - 2 * a * d[0]));
					std::printf("  rolling %s the steepest slope (%.1f %%) for 1 s from 5 m/s: %.2f m %s over %.2f m, %.3f m/s (energy says %.3f; flat %.3f)\n", dir > 0 ? "down" : "up",
						-grade * 100, std::abs(dz), dz < 0 ? "down" : "up", d[0], v[0], expect, v[1]);
					// within 10 % of what the hill does (drag grows with speed and rolling resistance shrinks with the tilt, which a constant per metre leaves out)
					ok = ok && std::abs(dz) > 0.2 && std::abs(v[0] - expect) < 0.1 * std::abs(expect - v[1]);
				}
			}
			for (const Setup& v : { Setup{}, endurance })
			{
				Simulation sim(track, v, Mode::Sim);
				for (long long k = 0; k < 200 * 600 && sim.laps.size() < 2; ++k) sim.Step(nullptr);
				if (sim.laps.size() < 2) { std::printf("  %.0f kW: no second lap\n", v.Pmax); ok = false; continue; }
				const LapResult& lap = sim.laps[1];
				std::printf("  %.0f kW, %.1f Nm: lap %.3f s (S1 %.3f, S2 %.3f, cones %d, off %d, valid %d)\n", v.Pmax, v.Tm, lap.time, lap.split1, lap.split2,
					lap.cones, lap.offCourse, lap.valid ? 1 : 0);
				ok = ok && lap.valid && (v.Pmax != endurance.Pmax || (lap.cones == 0 && lap.offCourse == 0));
			}
		}
		std::printf(ok ? "PASS\n" : "FAIL\n");
		return ok ? 0 : 1;
	}
	const char* cityFile = std::getenv("FSLAP_CITY");   // run.bat: FSLapUE\Content\FS\Data\Erlangen.bin
	const Track track = mode == Mode::City ? MakeCityTrack(cityFile ? cityFile : "Erlangen.bin") : MakeFsgTrack();
	if (track.N == 0) { std::printf("no city data (%s)\nFAIL\n", cityFile ? cityFile : "FSLAP_CITY not set"); return 1; }
	Simulation sim(track, Setup{}, mode);
	int freeCones = 0;
	for (const Cone& cone : track.cones) freeCones += cone.freeOnly ? 1 : 0;
	std::printf("track: N=%d LEN=%.3f cones=%zu (free roam only %d)\n", track.N, track.LEN, track.cones.size(), freeCones);

	if (mode == Mode::Free)
	{
		// the web game's arcade car does 0-100 km/h in 1.59 s from the strip
		Input full;
		full.thr = 1;
		full.human = true;
		for (int k = 0; k < 20 * 600; ++k) sim.Step(&full);
		std::printf("0-100 km/h %.3f s, 75 m %.3f s, top %.1f km/h\n", sim.fr.t100, sim.fr.t75, sim.fr.top * 3.6);
		const bool ok = std::abs(sim.fr.t100 - 1.59) < 0.05;
		std::printf(ok ? "PASS\n" : "FAIL\n");
		return ok ? 0 : 1;
	}

	if (mode == Mode::City)
	{
		// a bridge's deck is the surface for a car on it, the ground beneath for one under it
		int overpasses = 0, decksOk = 0;
		for (const Deck& d : track.decks)
		{
			const double mx = (d.x0 + d.x1) / 2, my = (d.y0 + d.y1) / 2, zd = (d.z0 + d.z1) / 2, zg = track.HeightAt(mx, my);
			if (zd - zg < 3) continue;
			++overpasses;
			decksOk += std::abs(track.HeightAt(mx, my, zd) - zd) < 1e-6 && std::abs(track.HeightAt(mx, my, zg) - zg) < 1e-6 ? 1 : 0;
		}
		std::printf("city: %zu wall edges, %zu bridge deck pieces (%d over 3 m above the ground, %d of them right both ways), ground %d x %d at %.0f m\n",
			track.edges.size(), track.decks.size(), overpasses, decksOk, track.tnx, track.tny, track.tstep);
		// the AI must lap without touching anything, never jumping between surfaces, and a car driven flat out into the
		// scenery must stay outside it
		int contacts = 0;
		double jump = 0, zPrev = sim.c.z;
		for (long long k = 0; k < 200 * 600; ++k)
		{
			sim.Step(nullptr);
			contacts += sim.c.hitWall ? 1 : 0;
			static FILE* aiTrace = tracePath ? std::fopen(tracePath, "w") : nullptr;
			if (aiTrace && k % 30 == 0) std::fprintf(aiTrace, "%.3f,%.3f,%.3f,%.1f,%.3f,%.2f,%d\n", sim.clock, sim.c.x, sim.c.y, sim.c.s, sim.c.lat, std::hypot(sim.c.vx, sim.c.vy), sim.c.hitWall ? 1 : 0);
			jump = std::max(jump, std::abs(sim.c.z - zPrev));
			zPrev = sim.c.z;
		}
		// inside a building: on the solid side of the nearest wall edge
		const auto InsideEdges = [&track](double px, double py)
		{
			const int ci = static_cast<int>(std::floor((px - track.gridX0) / 8.0)), cj = static_cast<int>(std::floor((py - track.gridY0) / 8.0));
			if (track.cellStart.empty() || ci < 0 || cj < 0 || ci >= track.gridW || cj >= track.gridH) return false;
			const int cell = cj * track.gridW + ci;
			double best = 1e9, side = 1;
			for (int item = track.cellStart[cell]; item < track.cellStart[cell + 1]; ++item)
			{
				const Edge& e = track.edges[track.cellItems[item]];
				const double ex = e.x1 - e.x0, ey = e.y1 - e.y0, len2 = ex * ex + ey * ey;
				if (len2 < 1e-12) continue;
				const double t = std::clamp(((px - e.x0) * ex + (py - e.y0) * ey) / len2, 0.0, 1.0);
				const double d = std::hypot(px - e.x0 - ex * t, py - e.y0 - ey * t);
				if (d < best) { best = d; side = (px - e.x0) * ey - (py - e.y0) * ex; }   // > 0: the edge's right, outside
			}
			return best < 1e8 && best > 0.05 && side < 0;
		};
		Simulation crash(track, Setup{}, mode);
		Input full;
		full.thr = 1;
		full.human = true;
		bool inside = false, finite = true;
		double topAfter = 0;
		int crashContacts = 0;
		for (int k = 0; k < 40 * 600; ++k)
		{
			crash.Step(&full);
			crashContacts += crash.c.hitWall ? 1 : 0;
			finite = finite && std::isfinite(crash.c.x) && std::isfinite(crash.c.vx) && std::isfinite(crash.c.r);
			for (const Wall& w : track.walls) inside = inside || (crash.c.x > w.x0 + 0.05 && crash.c.x < w.x1 - 0.05 && crash.c.y > w.y0 + 0.05 && crash.c.y < w.y1 - 0.05);
			inside = inside || (InsideEdges(crash.c.x, crash.c.y) && crash.c.z - track.HeightAt(crash.c.x, crash.c.y, std::nan("")) < 0.5);   // on a bridge, over the outlines, is fine
			if (crashContacts) topAfter = std::max(topAfter, std::hypot(crash.c.vx, crash.c.vy));
		}
		// R, 1.5 km from the circuit in eight directions: onto the nearest street (in a forest a few hundred metres), not back to the circuit, stopped, outside the buildings
		int resetsOk = 0;
		double resetMax = 0;
		for (int k = 0; k < 8; ++k)
		{
			Simulation lost(track, Setup{}, mode);
			lost.c.x = track.x[0] + 1500 * std::cos(k * 0.785398); lost.c.y = track.y[0] + 1500 * std::sin(k * 0.785398);
			const double x0 = lost.c.x, y0 = lost.c.y, toCircuit = std::abs(track.NearestPoint(x0, y0).lat);
			lost.BackOnTrack();
			const double moved = std::hypot(lost.c.x - x0, lost.c.y - y0);
			resetMax = std::max(resetMax, moved);
			resetsOk += moved < 0.5 * toCircuit && lost.c.vx == 0 && std::isfinite(lost.c.z) && !InsideEdges(lost.c.x, lost.c.y) ? 1 : 0;
		}
		// R beside every 100 m of the main roads, then flat out the way the car faces and turned round: it must get away
		// (on the bridge at (1217, 1013), over a road with noise walls, their flat outlines held a car fast)
		const auto Trapped = [&track, mode](double px, double py)
		{
			double moved = 0;
			for (const double turn : { 0.0, 3.14159265 })
			{
				Simulation r(track, Setup{}, mode);
				r.c.x = px; r.c.y = py;
				r.BackOnTrack();
				r.c.h += turn;
				const double x0 = r.c.x, y0 = r.c.y;
				Input full;
				full.thr = 1;
				full.human = true;
				for (int k = 0; k < 3 * 600; ++k) r.Step(&full);
				moved = std::max(moved, std::hypot(r.c.x - x0, r.c.y - y0));
			}
			return moved < 10;
		};
		int roadResets = 0, roadTrapped = 0;
		double run = 1e9;
		for (size_t i = 0; i + 1 < track.roadX.size(); ++i)
		{
			const double ax = track.roadX[i], ay = track.roadY[i], bx = track.roadX[i + 1], by = track.roadY[i + 1], len = std::hypot(bx - ax, by - ay);
			if (ax > 1e8 || bx > 1e8) { run = 1e9; continue; }
			for (double d = 0; d < len; d += 1)
			{
				if ((run += 1) < 100) continue;
				run = 0;
				++roadResets;
				const double px = ax + (bx - ax) * d / len, py = ay + (by - ay) * d / len;
				if (Trapped(px + 3 * (by - ay) / len, py - 3 * (bx - ax) / len))
				{
					++roadTrapped;
					std::printf("trapped after R near (%.0f, %.0f)\n", px, py);
				}
			}
		}
		const bool bridgeFree = !Trapped(1217.5, 1013.1);
		for (const LapResult& lap : sim.laps) std::printf("lap %d: %.3f s (S1 %.3f, S2 %.3f, valid %d)\n", lap.n, lap.time, lap.split1, lap.split2, lap.valid ? 1 : 0);
		std::printf("AI wall contacts %d, biggest height step %.3f m; flat out from the grid: contacts %d, inside a wall %d, numbers finite %d, top speed after the first hit %.1f m/s\n",
			contacts, jump, crashContacts, inside ? 1 : 0, finite ? 1 : 0, topAfter);
		std::printf("R 1.5 km from the circuit: %d of 8 onto a street close by (farthest move %.0f m)\n", resetsOk, resetMax);
		std::printf("R on the main roads: %d places, %d where the car could not drive away; the noise-wall bridge at (1217, 1013) %s\n", roadResets, roadTrapped, bridgeFree ? "free" : "TRAPPED");
		const bool ok = !sim.laps.empty() && sim.laps[0].valid && contacts == 0 && crashContacts > 0 && !inside && finite && jump < 0.1 && overpasses > 0 && decksOk == overpasses && resetsOk == 8 && roadTrapped == 0 && bridgeFree;
		std::printf(ok ? "PASS\n" : "FAIL\n");
		return ok ? 0 : 1;
	}

	FILE* trace = tracePath ? std::fopen(tracePath, "w") : nullptr;
	double traceT = 0;
	for (long long k = 0; k < 165 * 600; ++k)
	{
		sim.Step(nullptr);
		if (trace && sim.clock - traceT >= 0.02)
		{
			traceT = sim.clock;
			std::fprintf(trace, "%.17g,%.17g,%.17g,%.17g,%.17g,%.17g,%.17g\n", sim.clock, sim.c.x, sim.c.y, std::hypot(sim.c.vx, sim.c.vy), sim.c.ax, sim.c.ay, sim.c.s);
		}
	}
	if (trace) std::fclose(trace);

	bool pass = !sim.laps.empty() && !sim.lastRec.empty();
	for (const LapResult& lap : sim.laps)
	{
		std::printf("lap %d: %.3f s (raw %.3f, S1 %.3f, S2 %.3f, penalty %.0f, cones %d, off %d, valid %d)\n", lap.n, lap.time, lap.raw, lap.split1, lap.split2, lap.pen, lap.cones, lap.offCourse, lap.valid ? 1 : 0);
		if (mode == Mode::Sim && std::abs(lap.time - 77.4) > 0.1) pass = false;
	}
	std::printf("ghost samples %zu, LTS at 10 s: %.2f m\n", sim.lastRec.size(), track.LtsDistAt(10.0));
	std::printf(pass ? "PASS\n" : "FAIL\n");
	return pass ? 0 : 1;
}
