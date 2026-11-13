#pragma once

#include <memory>

#include "HoMoLapCore.h"

// The team's Advanced Dual Track Model (40 states, TMeasy tyres, double-wishbone kinematics) as FS Lap's vehicle
// model. Its C++ is generated from the team's MATLAB code by tools\adtm\adtm_codegen.m (MATLAB Coder) into
// Private\ADTM; without that code FS Lap builds as before and MakeAdtm returns null.
namespace HoMoLap
{
	// FS Lap's setup gives the aero (ClA, CdA, balance), the brake force and split, and the system power; the rest is
	// the ADTM's own vehicle file, as baked in by the code generation.
	HOMOLAPPHYSICS_API std::unique_ptr<Simulation::VehicleModel> MakeAdtm(const Setup& V);
	HOMOLAPPHYSICS_API const char* AdtmSource();   // which ADTM, vehicle and road friction the code was generated from; "" without
}
