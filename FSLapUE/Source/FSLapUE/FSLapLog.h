#pragma once

#include "CoreMinimal.h"
#include "HoMoLapCore.h"

/**
 * Records every lap at 600 Hz and writes it when it is complete, to Saved/Runs/<session>/:
 *   lap_<n>.csv       every simulation step: inputs and the car's state (columns in Columns below)
 *   lap_<n>.run.json  the lap at 100 Hz in the LTS run format (homo-lts.run.v1: summary, lap.distance_m/time_s/
 *                     speed_mps/curvature_1pm/lateralAcceleration_mps2), plus "fslap" with the setup, the result and
 *                     the full channels, so it can be put next to LTS runs (tools/lts/compareLaps.m, HoMo LTS)
 * Files are written on a worker thread; a lap that is not completed (cut, restart, menu) is dropped.
 * Saved/ is not in git, so updating (Update FS Lap.bat) keeps the laps; Submit runs.bat sends them to the shared repository.
 */
class FFSLapLog
{
public:
	static const TCHAR* const Columns;

	// A new session folder for the next lap: mode name, the setup as a JSON object and the vehicle model, written into every lap file.
	void Begin(const FString& Mode, const FString& SetupJson, const FString& Reference, const FString& Kernel);
	void SetReference(const FString& Reference) { ReferenceJson = Reference; }   // the LTS lap the driver races against
	// After every simulation step of a race; Human is the driver's input, nullptr when the AI drives.
	void Sample(const HoMoLap::Simulation& Sim, const HoMoLap::Input* Human);
	FString LastWritten() const { return LastFile; }

private:
	void Flush(const HoMoLap::Simulation& Sim, const HoMoLap::LapResult& Lap);

	TArray<double> Rows;   // NumColumns values per step
	size_t SeenLaps = 0;
	FString Session, ModeName, Setup, ReferenceJson, KernelName, LastFile, Version;
};
