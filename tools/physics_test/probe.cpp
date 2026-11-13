// Handling probe: scripted braking and steering in arcade (or hot lap) on open tarmac. Prints what the car does, so a tuning change can be measured.
//   probe [sim|arcade] [v0 km/h] [trace.csv]
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "HoMoLapCore.h"

using namespace HoMoLap;

namespace
{
	const double kPi = 3.141592653589793;

	// Gets the car up to speed on a straight, then holds the scripted input until it stops or `seconds` pass.
	struct Result { double stopDist, heading, maxBeta, maxYaw, jerk, minR, tStop, maxAy, endV; };

	Result Run(const Track& track, bool sim, double v0, double brake, double steer, double seconds, double steerFlip, const char* tracePath, double throttle = 0, bool handbrake = false)
	{
		Simulation s(track, Setup{}, Mode::Free);
		s.arcade = !sim;
		s.c.x = track.PADDOCK[0] + 60; s.c.y = (track.PADDOCK[1] + track.PADDOCK[3]) / 2; s.c.h = 0;
		s.c.vx = v0;
		Input in;
		in.human = true;
		in.assist = sim;
		in.brk = brake;
		in.thr = throttle;
		in.hb = handbrake;
		FILE* trace = tracePath ? std::fopen(tracePath, "w") : nullptr;
		Result r{};
		r.minR = 1e9;
		double dist = 0, prevR = 0, t = 0;
		const double h0 = s.c.h;
		for (; t < seconds; t += DT)
		{
			in.steer = (steerFlip > 0 && t > steerFlip) ? -steer : steer;
			s.Step(&in);
			const double v = std::hypot(s.c.vx, s.c.vy);
			dist += v * DT;
			const double beta = std::atan2(s.c.vy, std::max(std::abs(s.c.vx), 1.0)) * 180 / kPi;
			r.maxBeta = std::max(r.maxBeta, std::abs(beta));
			r.maxYaw = std::max(r.maxYaw, std::abs(s.c.r));
			r.maxAy = std::max(r.maxAy, std::abs(s.c.ay));
			r.jerk += std::abs(s.c.r - prevR);
			prevR = s.c.r;
			if (std::abs(s.c.r) > 0.05 && v > 2) r.minR = std::min(r.minR, v / std::abs(s.c.r));
			if (trace && std::fmod(t, 0.02) < DT) std::fprintf(trace, "%.3f,%.3f,%.3f,%.3f,%.4f,%.2f,%.3f,%.3f,%.3f\n", t, v, s.c.ax, s.c.ay, s.c.r, beta, s.c.delta, s.c.x, s.c.y);
			if (v < 1.0) break;
		}
		if (trace) std::fclose(trace);
		r.stopDist = dist; r.tStop = t; r.heading = (s.c.h - h0) * 180 / kPi; r.endV = std::hypot(s.c.vx, s.c.vy);
		return r;
	}

	void Print(const char* name, const Result& r)
	{
		std::printf("%-30s dist %6.1f m  t %5.2f s  heading %7.1f deg  max slip %5.1f deg  max yaw %5.2f rad/s  tightest R %6.1f m  max ay %5.1f  yaw jitter %6.2f  end v %4.1f\n",
			name, r.stopDist, r.tStop, r.heading, r.maxBeta, r.maxYaw, r.minR, r.maxAy, r.jerk, r.endV);
	}
}

int main(int argc, char** argv)
{
	const bool sim = argc > 1 && !std::strcmp(argv[1], "sim");
	const double kmh = argc > 2 ? std::atof(argv[2]) : 100;
	const char* trace = argc > 3 ? argv[3] : nullptr;
	const double v0 = kmh / 3.6;
	const Track track = MakeFsgTrack();
	std::printf("%s from %.0f km/h\n", sim ? "hot lap (assists on)" : "arcade", kmh);
	const Result straight = Run(track, sim, v0, 1, 0, 8, 0, nullptr);
	const Result turning = Run(track, sim, v0, 1, 1, 8, 0, trace);
	Print("straight, full brake", straight);
	Print("full brake + full right", turning);
	Print("full brake + half right", Run(track, sim, v0, 1, 0.5, 8, 0, nullptr));
	Print("half brake + full right", Run(track, sim, v0, 0.5, 1, 8, 0, nullptr));
	Print("full brake, right then left", Run(track, sim, v0, 1, 1, 8, 0.5, nullptr));
	Print("no brake, coast + full right", Run(track, sim, v0, 0, 1, 2.5, 0, nullptr));
	Print("full throttle + full right", Run(track, sim, v0, 0, 1, 3.0, 0, nullptr, 1.0));
	Print("full throttle, right then left", Run(track, sim, v0, 0, 1, 3.0, 1.0, nullptr, 1.0));
	Print("handbrake + full right, on power", Run(track, sim, v0, 0, 1, 1.5, 0, nullptr, 1.0, true));
	if (sim || kmh != 100) return 0;
	// arcade at 100 km/h: braking must stop the car in a straight line and still let it turn, without the nose swinging round
	const bool ok = straight.stopDist > 18 && straight.stopDist < 25 && std::abs(straight.heading) < 0.1 && turning.heading > 90 && turning.maxBeta < 25 && turning.maxAy > 15;
	std::printf(ok ? "PASS\n" : "FAIL\n");
	return ok ? 0 : 1;
}
