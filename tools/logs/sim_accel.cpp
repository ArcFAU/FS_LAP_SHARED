// A standing start flat out in free roam, then full braking to a stop, on FS Lap's own model and on the ADTM, for the
// comparison with the car's logs (tools\logs\validate.py). Then circles at a fixed steering angle while the speed
// rises slowly: the lateral acceleration it settles at is the model's grip at that speed. Prints CSV: run,t,v,s,ax,ay
// every 0.01 s; run is fslap, adtm, or the same with _circle<steering in % of full lock>.
//   sim_accel [Pmax_kW Tm_Nm]    the stock car without arguments
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>

#include "HoMoLapAdtm.h"

int main(int argc, char** argv)
{
	using namespace HoMoLap;
	Setup setup;
	if (argc > 2) { setup.Pmax = std::atof(argv[1]); setup.Tm = std::atof(argv[2]); }
	const Track track = MakeFsgTrack();
	std::printf("run,t,v,s,ax,ay\n");
	for (int model = 0; model < 2; ++model)
	{
		const char* name = model ? "adtm" : "fslap";
		Simulation sim(track, setup, Mode::Free);
		const std::unique_ptr<Simulation::VehicleModel> adtm = model ? MakeAdtm(setup) : nullptr;
		if (model && !adtm) break;   // no generated ADTM code
		sim.external = adtm.get();
		sim.arcade = false;   // free roam is arcade on FS Lap's own model (1.75x power, 1.18x grip); here the sim's car is wanted
		const double x0 = sim.c.x, y0 = sim.c.y;
		Input in;
		in.human = true;
		for (int k = 0; k < 14 * 600; ++k)
		{
			const bool braking = k >= 7 * 600;   // 7 s flat out, then the brakes
			in.thr = braking ? 0 : 1;
			in.brk = braking ? 1 : 0;
			sim.Step(&in);
			if (k % 6 == 5) std::printf("%s,%.2f,%.4f,%.3f,%.4f,%.4f\n", name, (k + 1) / 600.0, std::hypot(sim.c.vx, sim.c.vy), std::hypot(sim.c.x - x0, sim.c.y - y0), sim.c.ax, sim.c.ay);
			if (braking && sim.c.vx < 0.2) break;
		}
		// the AI's flying lap of the FSG track with this setup: "<model>_lap", with the lap time in the t column
		Simulation lap(track, setup, Mode::Sim);
		const std::unique_ptr<Simulation::VehicleModel> adtm3 = model ? MakeAdtm(setup) : nullptr;
		lap.external = adtm3.get();
		for (int k = 0; k < 400 * 600 && lap.laps.size() < 2; ++k) lap.Step(nullptr);
		if (lap.laps.size() == 2) std::printf("%s_lap,%.3f,%.4f,0,0,0\n", name, lap.laps[1].time, track.LEN / lap.laps[1].time);
		for (const int lock : { 15, 25, 40, 60, 100 })
		{
			Simulation circle(track, setup, Mode::Free);
			const std::unique_ptr<Simulation::VehicleModel> adtm2 = model ? MakeAdtm(setup) : nullptr;
			circle.external = adtm2.get();
			circle.arcade = false;
			Input turn;
			turn.human = true;
			turn.wheel = turn.direct = true;   // a fixed road wheel angle, without the speed-dependent lock
			turn.steer = lock / 100.0;
			for (int k = 0; k < 60 * 600; ++k)
			{
				const double target = 4 + 26.0 * k / (60 * 600);   // 4 to 30 m/s in 60 s: slow enough to stay near steady state
				turn.thr = std::clamp((target - circle.c.vx) * 0.5, 0.0, 1.0);
				circle.Step(&turn);
				if (circle.c.hitWall) break;   // the paddock ended
				if (k % 6 == 5) std::printf("%s_circle%d,%.2f,%.4f,0,%.4f,%.4f\n", name, lock, (k + 1) / 600.0, std::hypot(circle.c.vx, circle.c.vy), circle.c.ax, circle.c.ay);
			}
		}
	}
	return 0;
}
