#include "FSLapSil.h"

#include "Common/UdpSocketBuilder.h"
#include "HAL/PlatformTime.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "SocketSubsystem.h"
#include "Sockets.h"

#include <algorithm>
#include <cmath>
#include <cstring>

namespace
{
	// the team's inverter model in the closed loop (closed_loop_vcu.Plant): speed loop PI, 1 ms torque lag, 18.8 Nm
	constexpr double Kp = 0.05, Ki = 0.3, Tau = 1e-3, TMax = 18.8;
	constexpr double RadPerSToRpm = 60.0 / (2.0 * 3.141592653589793);

#pragma pack(push, 1)
	struct FOut { char Magic[4]; uint32 Seq, Flags; double D[18]; };
	struct FIn { char Magic[4]; uint32 Seq, Flags; double D[13]; int32 State[4]; };
#pragma pack(pop)
	static_assert(sizeof(FOut) == 156 && sizeof(FIn) == 132, "fslap_sil.py's FSL and VCU structs");
}

bool FFSLapSil::Open(int32 Port)
{
	Close();
	Socket = FUdpSocketBuilder(TEXT("FSLapSil")).AsNonBlocking().BoundToEndpoint(FIPv4Endpoint(FIPv4Address::InternalLoopback, 0)).Build();
	Remote = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Remote->SetLoopbackAddress();
	Remote->SetPort(Port);
	LastAnswer = -1e9;
	Flags = 0;
	for (int32 i = 0; i < 4; ++i)
	{
		NSet[i] = TPos[i] = TNeg[i] = Integral[i] = Torque[i] = 0;
		State[i] = -1;
	}
	return Socket != nullptr;
}

void FFSLapSil::Close()
{
	if (Socket)
	{
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
		Socket = nullptr;
	}
}

void FFSLapSil::Exchange(const HoMoLap::Car& Car, double Time, double SteeringDeg, double Gear, bool bDriving)
{
	if (!Socket)
	{
		return;
	}
	uint32 Pending = 0;
	while (Socket->HasPendingData(Pending))
	{
		FIn In;
		int32 Read = 0;
		if (!Socket->Recv(reinterpret_cast<uint8*>(&In), sizeof(In), Read) || Read != sizeof(In) || std::memcmp(In.Magic, "VCU1", 4) != 0)
		{
			continue;
		}
		Flags = In.Flags;
		VcuTime = In.D[0];
		for (int32 i = 0; i < 4; ++i)
		{
			NSet[i] = In.D[1 + i]; TPos[i] = In.D[5 + i]; TNeg[i] = In.D[9 + i]; State[i] = In.State[i];
		}
		LastAnswer = FPlatformTime::Seconds();
	}
	// the VCU's IMU counts the lateral speed, yaw rate and lateral acceleration to the left; FS Lap's frame to the right
	FOut Out;
	std::memcpy(Out.Magic, "FSL1", 4);
	Out.Seq = ++Seq;
	Out.Flags = bDriving ? 1u : 0u;
	const double Values[18] = { Time, Car.thr, Car.brk, SteeringDeg, Car.vx, -Car.vy, -Car.r, Car.ax, -Car.ay, Car.h * 180.0 / 3.141592653589793,
		Car.wheelOmega[0] * Gear * RadPerSToRpm, Car.wheelOmega[1] * Gear * RadPerSToRpm, Car.wheelOmega[2] * Gear * RadPerSToRpm, Car.wheelOmega[3] * Gear * RadPerSToRpm,
		Torque[0], Torque[1], Torque[2], Torque[3] };
	std::memcpy(Out.D, Values, sizeof(Values));
	int32 Sent = 0;
	Socket->SendTo(reinterpret_cast<const uint8*>(&Out), sizeof(Out), Sent, *Remote);
}

void FFSLapSil::Torques(const HoMoLap::Car& Car, double Gear, double Dt, double Out[4])
{
	const bool bLive = IsLive();   // no answer: the inverters' setpoint timeout, no torque
	for (int32 i = 0; i < 4; ++i)
	{
		const double Up = bLive ? std::min(TPos[i], TMax) : 0.0, Down = bLive ? std::max(TNeg[i], -TMax) : 0.0;
		const double E = NSet[i] - Car.wheelOmega[i] * Gear * RadPerSToRpm;
		const double U = Kp * E + Integral[i];
		const double Set = std::clamp(U, std::min(Down, Up), Up);
		if (!((U > Up && E > 0) || (U < Down && E < 0)))
		{
			Integral[i] += Ki * E * Dt;   // anti-windup as in vehicle.Inverter
		}
		Integral[i] = std::clamp(Integral[i], std::min(Down, Up), Up);
		Torque[i] += (Set - Torque[i]) * std::min(1.0, Dt / Tau);
		Out[i] = Torque[i];
	}
}

bool FFSLapSil::IsLive() const
{
	return FPlatformTime::Seconds() - LastAnswer < 0.25;
}

FString FFSLapSil::Status() const
{
	if (!Socket)
	{
		return TEXT("SIL: no UDP socket");
	}
	if (!IsLive())
	{
		return TEXT("SIL: waiting for the VCU (start tools\\sil\\run.bat)");
	}
	static const TCHAR* const Names[] = { TEXT("safe"), TEXT("reset"), TEXT("enable"), TEXT("drive") };
	FString Inverters;
	for (int32 i = 0; i < 4; ++i)
	{
		Inverters += (i ? TEXT(" ") : TEXT("")) + FString(State[i] >= 0 && State[i] < 4 ? Names[State[i]] : TEXT("?"));
	}
	return FString::Printf(TEXT("SIL: %s, VCU %.1f s, inverters %s"), (Flags & 1) ? TEXT("VCU firmware") : TEXT("stand-in VCU (no firmware)"), VcuTime, *Inverters);
}
