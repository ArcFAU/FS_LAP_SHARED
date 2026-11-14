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
	double maxSteerT = 0;
	long long steered = 0, aligning = 0;   // force feedback: T_Steering must pull the wheel back to the centre
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
			maxSteerT = std::max(maxSteerT, std::abs(sim.c.steerT));
			if (std::abs(sim.c.delta) > 0.02) { ++steered; aligning += sim.c.steerT * sim.c.delta < 0 ? 1 : 0; }
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
	const double aligningShare = steered ? 100.0 * aligning / steered : 0;
	std::printf("steering torque: largest %.1f Nm, against the steering angle %.1f %% of the steered time\n", maxSteerT, aligningShare);
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

	// the setup reaches the ADTM (adtm_vehicle.m): more mass and less motor torque both slow the standing start
	const auto to100 = [&](const Setup& changed)
	{
		Simulation run(track, changed, Mode::Free);
		const std::unique_ptr<Simulation::VehicleModel> model = MakeAdtm(changed);
		run.external = model.get();
		for (int k = 0; k < 10 * 600; ++k) run.Step(&full);
		return run.fr.t100;
	};
	Setup heavy, weak;
	heavy.mCar += 40;
	weak.Tm *= 0.75;
	const double heavy100 = to100(heavy), weak100 = to100(weak);
	const bool setupActs = heavy100 > drag.fr.t100 + 0.05 && weak100 > drag.fr.t100 + 0.05;
	std::printf("setup on the ADTM: 0-100 km/h %.3f s stock, %.3f s with 40 kg more, %.3f s with 75 %% motor torque\n", drag.fr.t100, heavy100, weak100);

	// a steering wheel with Wheel rotation set: no smoothing, and a fixed ratio at any speed; the keys stay smoothed
	Input wheel = full;
	wheel.steer = 0.1; wheel.wheel = wheel.direct = true;
	drag.Step(&wheel);
	const bool direct = drag.c.st == 0.1 && drag.c.direct && std::abs(drag.c.delta - 0.1 * drag.C.delMax) < 1e-12;
	Input keys = full;
	keys.steer = -0.5;
	drag.Step(&keys);
	const bool smoothed = drag.c.st > -0.5 && !drag.c.direct;
	std::printf("wheel 1:1 at %.0f km/h: st %.3f, road angle %.3f deg; keys after one step: st %.3f (asked -0.5)\n", drag.c.vx * 3.6, 0.1, 0.1 * drag.C.delMax * 180 / 3.14159265358979, drag.c.st);

	const bool pass = !sim.laps.empty() && finite && follows && realTime > 2 && drag.fr.t100 > 0 && std::abs(drag.fr.t100 - 1.577) > 0.002
		&& maxSteerT > 0 && aligningShare > 50   // the ADTM really drove
		&& direct && smoothed && setupActs;
	std::printf(pass ? "PASS\n" : "FAIL\n");
	return pass ? 0 : 1;
}
