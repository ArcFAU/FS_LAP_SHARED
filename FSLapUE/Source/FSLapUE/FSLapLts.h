#pragma once

#include "CoreMinimal.h"

/**
 * The team's lap time simulation (LTS, MATLAB) as the reference lap: the real LTS computes a lap for the current
 * setup with the chosen model family, and the game compares the driven lap with it.
 *
 * A MATLAB worker (lts.viewer.worker of the LTS, file queue) is started in the background on first use and kept
 * running; jobs (schema homo-lts.job.v1) go to Saved/LTS/queue, results (homo-lts.run.v1) to Saved/LTS/runs/<job>.
 * A result folder that already exists is a cached result and loads at once. The LTS folder is only read: it is put on
 * MATLAB's path, its MATLAB project is not opened.
 * Where things are: FSLAP_LTS_ROOT (default D:\VD\laptimesimulation), FSLAP_MATLAB (default the newest
 * C:\Program Files\MATLAB\R20xx\bin\matlab.exe).
 */
class FFSLapLts
{
public:
	struct FLap
	{
		TArray<double> Distance, Time, Speed;   // m, s, m/s along the LTS track
		double LapTime = 0, Length = 0;
		FString Model;
	};

	static const TCHAR* const Models[4];        // "" (the stored lap), pointMass, singleTrack, dualTrack
	static const TCHAR* const ModelLabels[4];

	~FFSLapLts();
	// Asks for the lap of Model (index into Models, 1..3) with these LTS parameter overrides (homoLtsParameters ids).
	void Request(int32 Model, const TArray<TPair<FString, double>>& Overrides);
	// True once, when the requested lap has arrived.
	bool Poll(FLap& Out);
	FString Status() const { return StatusText; }
	bool IsBusy() const { return !JobId.IsEmpty(); }

	static bool ParseRun(const FString& Json, FLap& Out);   // public for the self-check

private:
	FString Folder(const TCHAR* Sub) const;
	bool WorkerAlive() const;
	bool StartWorker();

	FString JobId, StatusText;
	double JobStarted = 0, NextPoll = 0;
	bool bWorkerStarted = false;
};
