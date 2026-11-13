// Headless acceptance test of the C++ physics: the AI lap must be 77.4 s +- 0.1 s on the stock car.
//   ailap [arcade|free|city] [trace.csv]    exit code 0 = pass
// The same core runs in the game, so what passes here is what drives the car there.
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

#include "HoMoLapCore.h"

int main(int argc, char** argv)
{
	using namespace HoMoLap;
	Mode mode = Mode::Sim;
	const char* tracePath = nullptr;
	for (int i = 1; i < argc; ++i)
	{
		if (!std::strcmp(argv[i], "arcade")) mode = Mode::Arcade;
		else if (!std::strcmp(argv[i], "free")) mode = Mode::Free;
		else if (!std::strcmp(argv[i], "city")) mode = Mode::City;
		else tracePath = argv[i];
	}
	const Track track = mode == Mode::City ? MakeCityTrack() : MakeFsgTrack();
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
		// the AI must lap without touching anything, and a car driven flat out into the scenery must stay outside it
		int contacts = 0;
		for (long long k = 0; k < 200 * 600; ++k)
		{
			sim.Step(nullptr);
			contacts += sim.c.hitWall ? 1 : 0;
		}
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
			if (crashContacts) topAfter = std::max(topAfter, std::hypot(crash.c.vx, crash.c.vy));
		}
		for (const LapResult& lap : sim.laps) std::printf("lap %d: %.3f s (S1 %.3f, S2 %.3f, valid %d)\n", lap.n, lap.time, lap.split1, lap.split2, lap.valid ? 1 : 0);
		std::printf("AI wall contacts %d; flat out from the grid: contacts %d, inside a wall %d, numbers finite %d, top speed after the first hit %.1f m/s\n", contacts, crashContacts, inside ? 1 : 0, finite ? 1 : 0, topAfter);
		const bool ok = !sim.laps.empty() && sim.laps[0].valid && contacts == 0 && crashContacts > 0 && !inside && finite;
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
