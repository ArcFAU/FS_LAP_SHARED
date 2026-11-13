// Headless test of the team's ADTM as FS Lap's vehicle model (HoMoLapAdtm, code from tools\adtm\adtm_codegen.m):
// FS Lap's AI drives the FSG track for 165 s on the ADTM, then a standing start flat out in free roam.
//   adtmlap [trace.csv]    exit code 0 = pass: at least one lap, every number finite, faster than real time
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>

#include "HoMoLapAdtm.h"

int main(int argc, char** argv)
{
	using namespace HoMoLap;
	if (!*AdtmSource())
	{
		std::printf("no ADTM code: run tools\\adtm\\adtm_codegen.m first\nFAIL\n");
		return 1;
	}
	std::printf("ADTM: %s\n", AdtmSource());
	const Track track = MakeFsgTrack();
	Simulation sim(track, Setup{}, Mode::Sim);
	const std::unique_ptr<Simulation::VehicleModel> adtm = MakeAdtm(Setup{});
	sim.external = adtm.get();
	FILE* trace = argc > 1 ? std::fopen(argv[1], "w") : nullptr;
	bool finite = true;
	double maxLat = 0, traceT = 0;
	long long lapSteps = 0, slideSteps = 0;   // FS Lap's own model: a tyre slides on about 3 % of the AI lap
	const auto start = std::chrono::steady_clock::now();
	for (long long k = 0; k < 165 * 600; ++k)
	{
		sim.Step(nullptr);
		finite = finite && std::isfinite(sim.c.x) && std::isfinite(sim.c.vx) && std::isfinite(sim.c.r);
		if (sim.IsLapActive())
		{
			maxLat = std::max(maxLat, std::abs(sim.c.lat));
			++lapSteps;
			slideSteps += sim.c.slipF || sim.c.slipR ? 1 : 0;
		}
		if (trace && sim.clock - traceT >= 0.02)
		{
			traceT = sim.clock;
			std::fprintf(trace, "%.6f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f\n", sim.clock, sim.c.x, sim.c.y, std::hypot(sim.c.vx, sim.c.vy), sim.c.ax, sim.c.ay, sim.c.s, sim.c.lat);
		}
	}
	const double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
	if (trace) std::fclose(trace);
	for (const LapResult& lap : sim.laps)
	{
		std::printf("lap %d: %.3f s (raw %.3f, S1 %.3f, S2 %.3f, penalty %.0f, cones %d, off %d, valid %d)\n", lap.n, lap.time, lap.raw, lap.split1, lap.split2, lap.pen, lap.cones, lap.offCourse, lap.valid ? 1 : 0);
	}
	const double realTime = 165 / seconds;
	std::printf("largest distance from the centreline on a lap %.2f m, a tyre sliding %.1f %% of the time, numbers finite %d, %.1fx real time (%.1f us per 600 Hz step)\n",
		maxLat, lapSteps ? 100.0 * slideSteps / lapSteps : 0.0, finite ? 1 : 0, realTime, seconds / (165 * 600) * 1e6);

	// the game moves the car (restart, back on track): the ADTM must carry on from there
	const double target = track.LEN * 0.5;
	sim.PlaceAt(target);
	for (int k = 0; k < 600; ++k) sim.Step(nullptr);
	double moved = sim.c.s - target;
	if (moved < -track.LEN / 2) moved += track.LEN;
	const bool follows = moved > 0 && moved < 60 && std::abs(sim.c.lat) < 2;
	std::printf("placed at %.0f m, 1 s later %.1f m further, %.2f m from the centreline\n", target, moved, sim.c.lat);

	Simulation drag(track, Setup{}, Mode::Free);
	const std::unique_ptr<Simulation::VehicleModel> adtm2 = MakeAdtm(Setup{});
	drag.external = adtm2.get();
	Input full;
	full.thr = 1;
	full.human = true;
	full.assist = true;   // as in the game, where the assists are on by default: they must not act in free roam
	for (int k = 0; k < 10 * 600; ++k) drag.Step(&full);
	std::printf("free roam flat out: 0-100 km/h %.3f s, 75 m %.3f s, top %.1f km/h (FS Lap's own model: 1.577 s, 3.075 s)\n", drag.fr.t100, drag.fr.t75, drag.fr.top * 3.6);

	const bool pass = !sim.laps.empty() && finite && follows && realTime > 2 && drag.fr.t100 > 0 && std::abs(drag.fr.t100 - 1.577) > 0.002;   // the last: the ADTM really drove
	std::printf(pass ? "PASS\n" : "FAIL\n");
	return pass ? 0 : 1;
}
