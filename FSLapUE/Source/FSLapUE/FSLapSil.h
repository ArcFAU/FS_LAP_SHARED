#pragma once

#include "CoreMinimal.h"
#include "HoMoLapCore.h"

class FSocket;
class FInternetAddr;

// The SIL mode's link to a VCU (tools\sil\fslap_sil.py, which runs the car's firmware through the team's SIL, or a
// stand-in): FS Lap's driver and car state go out over UDP each frame, the inverters' limits come back, and the four
// motor torques are the inverters' speed loop between those limits at every physics step, as the team's
// vehicle_model.Inverter does (the Ephorus inverters run in speed mode: the VCU sends a speed setpoint and two torque
// limits). The packet layout is in fslap_sil.py.
class FFSLapSil
{
public:
	~FFSLapSil() { Close(); }
	bool Open(int32 Port = 8771);
	void Close();
	bool IsOpen() const { return Socket != nullptr; }

	// once a frame: the newest answer in, the state out. Steering wheel deg + right, as the car logs it.
	void Exchange(const HoMoLap::Car& Car, double Time, double SteeringDeg, double Gear, bool bDriving);
	// every physics step: the motor torques (Nm, FL FR RL RR) from the wheel speeds and the latest limits
	void Torques(const HoMoLap::Car& Car, double Gear, double Dt, double Out[4]);

	bool IsLive() const;   // an answer within the last 0.25 s
	FString Status() const;   // for the HUD: which VCU, the inverter states, or what is missing

private:
	FSocket* Socket = nullptr;
	TSharedPtr<FInternetAddr> Remote;
	uint32 Seq = 0, Flags = 0;
	double LastAnswer = -1e9, VcuTime = 0;
	double NSet[4] = {}, TPos[4] = {}, TNeg[4] = {}, Integral[4] = {}, Torque[4] = {};
	int32 State[4] = { -1, -1, -1, -1 };
};
