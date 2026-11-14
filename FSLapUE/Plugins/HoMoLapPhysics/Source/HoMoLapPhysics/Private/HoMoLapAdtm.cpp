#include "HoMoLapAdtm.h"

#if HOMOLAP_WITH_ADTM   // set by HoMoLapPhysics.Build.cs when the generated code is there
#include <algorithm>
#include <cmath>
#include <vector>

#include "ADTM/adtm_indices.h"
#include "ADTM/adtm_step.h"

namespace HoMoLap
{
namespace
{
	using namespace adtm::index;
#ifndef HOMOLAP_ADTM_SLIDE
#define HOMOLAP_ADTM_SLIDE 0.3
#endif
	// TMeasy slip (longitudinal or lateral) above which a tyre counts as sliding: skid marks, smoke. The ADTM's traction
	// control holds 0.15 under power, so the band up to about 0.2 is grip; at 0.3 the AI lap slides 1.7 % of the time
	// (FS Lap's own model 2.9 %; 0.15 gave 16.6 %, 0.2 13.3 %, 0.25 8.5 %)
	constexpr double SlideSlip = HOMOLAP_ADTM_SLIDE;

	class Adtm final : public Simulation::VehicleModel
	{
	public:
		explicit Adtm(const Setup& V)
		{
			adtm::adtm_step_initialize();
			// FS Lap's setup on the team's vehicle (tools/adtm/adtm_vehicle.m): the aero as it is; mass, weight split, CoG
			// height and motor torque as their change from FS Lap's stock car, so the stock setup is the team's ADTM exactly
			const Setup stock;
			car[0] = -V.CdA; car[1] = -V.ClA * V.bal; car[2] = -V.ClA * (1 - V.bal);
			car[3] = (V.mCar + V.mDrv) - (stock.mCar + stock.mDrv); car[4] = V.wf - stock.wf; car[5] = V.h - stock.h; car[6] = V.Tm / stock.Tm;
			tyreGrip = V.mu0 / stock.mu0;   // the tyres' friction scales the ADTM's road friction
			brakeFront = V.brakeFront; brakeForce = V.Fb; systemPower = V.Pmax * 1000;
			double k[9];
			adtm::adtm_constants(k);
			handMax = k[0]; torquePerBarF = k[1]; torquePerBarR = k[2]; r0 = k[3]; motorTorque = k[4] * car[6]; gear = k[5];
			// the ADTM's steering kinematics: mean front wheel angle against steering wheel angle (a step of length 0 only evaluates it)
			adtm::adtm_init(car, x);
			for (int i = 0; i <= 80; ++i)
			{
				double probe[nStates], u[6] = { handMax * (i / 40.0 - 1), 0, 0, 0, 1, 1 };
				std::copy(x, x + nStates, probe);
				adtm::adtm_step(probe, u, car, 0, 0, out);
				hand.push_back(u[0]);
				road.push_back((out[out_delta_FL] + out[out_delta_FR]) / 2);
			}
			if (road.front() > road.back()) { std::reverse(hand.begin(), hand.end()); std::reverse(road.begin(), road.end()); }
		}

		void Step(Car& c, double Dt, double Grip) override
		{
			if (!synced || c.x != last.x || c.y != last.y || c.h != last.h || c.vx != last.vx || c.vy != last.vy || c.r != last.r)
			{
				Follow(c);   // the game moved the car: restart, back on track, a wall
			}
			// FS Lap's commands in the ADTM's terms: steering wheel angle, brake pressures (bar), throttle capped at the system power
			const double brake = std::clamp(c.brk, 0.0, 1.0) * brakeForce;
			double throttle = std::clamp(c.thr, 0.0, 1.0) * std::min(1.0, systemPower / (4 * motorTorque * gear * std::max(c.vx, 0.1) / r0));
			if (c.brk > 0.05 && c.vx > 0.3) throttle = 0;   // as FS Lap: the brake wins
			double u[6] = { c.direct ? std::clamp(c.st, -1.0, 1.0) * handMax : HandAngle(c.delta), brake * brakeFront / 2 * r0 / torquePerBarF, brake * (1 - brakeFront) / 2 * r0 / torquePerBarR, throttle, 1, Grip * tyreGrip };
			adtm::adtm_step(x, u, car, t, Dt, out);
			t += Dt;
			if (!std::all_of(x, x + nStates, [](double v) { return std::isfinite(v); }))
			{
				Follow(c);   // never hand the game a broken state
				return;
			}
			c.x = x[x_x_V]; c.y = x[x_y_V]; c.h = x[x_psi];
			c.vx = x[x_v_x]; c.vy = x[x_v_y]; c.r = x[x_omega_z];
			c.ax = out[out_a_x]; c.ay = out[out_a_y];
			c.axp += (c.ax - c.axp) * std::min(1.0, Dt * 25);
			c.ayp += (c.ay - c.ayp) * std::min(1.0, Dt * 25);
			c.power = std::max(0.0, out[out_T_DriveFL] * x[x_omega_FL] + out[out_T_DriveFR] * x[x_omega_FR] + out[out_T_DriveRL] * x[x_omega_RL] + out[out_T_DriveRR] * x[x_omega_RR]);
			c.FyfP = out[out_F_yFL] + out[out_F_yFR]; c.FyrP = out[out_F_yRL] + out[out_F_yRR];
			const auto sliding = [this](int sx, int sy) { return std::abs(out[sx]) > SlideSlip || std::abs(out[sy]) > SlideSlip; };
			c.slipF = sliding(out_s_xFL, out_s_yFL) || sliding(out_s_xFR, out_s_yFR);
			c.slipR = sliding(out_s_xRL, out_s_yRL) || sliding(out_s_xRR, out_s_yRR);
			c.steerT = out[out_T_Steering];
			last = c;
		}

	private:
		double HandAngle(double roadAngle) const
		{
			if (roadAngle <= road.front()) return hand.front();
			if (roadAngle >= road.back()) return hand.back();
			const size_t i = std::upper_bound(road.begin(), road.end(), roadAngle) - road.begin();
			return hand[i - 1] + (hand[i] - hand[i - 1]) * (roadAngle - road[i - 1]) / (road[i] - road[i - 1]);
		}

		// the ADTM settled on its springs where FS Lap has the car, with FS Lap's velocities
		void Follow(const Car& c)
		{
			adtm::adtm_init(car, x);
			x[x_x_V] = c.x; x[x_y_V] = c.y; x[x_psi] = c.h;
			x[x_v_x] = c.vx; x[x_v_y] = c.vy; x[x_omega_z] = c.r;
			x[x_omega_FL] = x[x_omega_FR] = x[x_omega_RL] = x[x_omega_RR] = c.vx / r0;
			synced = true;
		}

		double x[nStates] = {}, out[nOutputs] = {}, car[7] = {};
		double handMax = 0, torquePerBarF = 1, torquePerBarR = 1, r0 = 0.2, motorTorque = 21, gear = 12.421;
		double brakeFront = 0.7, brakeForce = 9000, systemPower = 80000, tyreGrip = 1, t = 0;
		std::vector<double> hand, road;
		Car last;
		bool synced = false;
	};
}

std::unique_ptr<Simulation::VehicleModel> MakeAdtm(const Setup& V) { return std::make_unique<Adtm>(V); }
const char* AdtmSource() { return adtm::index::source; }
}

#else

namespace HoMoLap
{
std::unique_ptr<Simulation::VehicleModel> MakeAdtm(const Setup&) { return nullptr; }
const char* AdtmSource() { return ""; }
}

#endif
