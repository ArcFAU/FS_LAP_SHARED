#include "FSLapHUD.h"

#include "RenderUtils.h"

#include "CanvasItem.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/Font.h"
#include "FSLapVehiclePawn.h"

#include <algorithm>
#include <cmath>

namespace
{
	FLinearColor Hex(uint8 R, uint8 G, uint8 B, float A = 1.0f)
	{
		FLinearColor Color = FLinearColor::FromSRGBColor(FColor(R, G, B));
		Color.A = A;
		return Color;
	}

	// the web game's palette
	const FLinearColor Paper = Hex(0xf3, 0xf1, 0xea), Muted = Hex(0xa6, 0xad, 0xb5), Signal = Hex(0xe8, 0x20, 0x2a), Gold = Hex(0xff, 0xc8, 0x3d), Fast = Hex(0x3d, 0xdc, 0x84), Best = Hex(0xb4, 0x6c, 0xff);
	const FLinearColor Ink = Hex(0x0b, 0x0d, 0x0f, 0.64f), Chip = Hex(0x22, 0x26, 0x2b, 0.9f);

	FString LapTime(double T)
	{
		const int32 Minutes = FMath::FloorToInt32(T / 60);
		return FString::Printf(TEXT("%d:%06.3f"), Minutes, T - Minutes * 60);
	}

	FString Signed(double D, int32 Decimals)
	{
		return FString::Printf(TEXT("%s%.*f"), D < 0 ? TEXT("-") : TEXT("+"), Decimals, FMath::Abs(D));
	}
}

void AFSLapHUD::Label(const FString& Text, float X, float Y, float Size, const FLinearColor& Color, bool bBold, bool bCentre)
{
	const FSlateFontInfo Info(Font, FMath::Max(1, FMath::RoundToInt32(Size * S)), bBold ? FName(TEXT("Bold")) : FName(TEXT("Regular")));
	FCanvasTextItem Item(FVector2D(X * S, Y * S), FText::FromString(Text), Info, Color);
	Item.bCentreX = bCentre;
	Item.EnableShadow(FLinearColor(0, 0, 0, 0.55f));
	Canvas->DrawItem(Item);
}

void AFSLapHUD::Box(float X, float Y, float W, float H, const FLinearColor& Color)
{
	DrawRect(Color, X * S, Y * S, W * S, H * S);
}

void AFSLapHUD::Slant(float X, float Y, float W, float H, const FLinearColor& Color, float Lean)
{
	const FVector2D A((X + Lean) * S, Y * S), B((X + W + Lean) * S, Y * S), C((X + W) * S, (Y + H) * S), D(X * S, (Y + H) * S);
	FCanvasTriangleItem Upper(A, B, D, GWhiteTexture), Lower(B, C, D, GWhiteTexture);
	Upper.SetColor(Color);
	Lower.SetColor(Color);
	Upper.BlendMode = Lower.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(Upper);
	Canvas->DrawItem(Lower);
}

void AFSLapHUD::Panel(float X, float Y, float W, float H)
{
	Slant(X, Y, W, H, Ink);
	Slant(X - 12, Y, 5, H, Signal);
}

void AFSLapHUD::Ring(float X, float Y, float Radius, const FLinearColor& Color)
{
	const int32 Sides = 36;
	for (int32 i = 0; i < Sides; ++i)
	{
		const float A = 2 * PI * i / Sides, B = 2 * PI * (i + 1) / Sides;
		DrawLine((X + Radius * FMath::Cos(A)) * S, (Y + Radius * FMath::Sin(A)) * S, (X + Radius * FMath::Cos(B)) * S, (Y + Radius * FMath::Sin(B)) * S, Color, 1.0f);
	}
}

void AFSLapHUD::ShowMessage(const FString& Title, const FString& Sub, const FLinearColor& Color, float Seconds, bool bSmall)
{
	MessageTitle = Title;
	MessageSub = Sub;
	MessageColor = Color;
	MessageTime = Seconds;
	bMessageSmall = bSmall;
}

void AFSLapHUD::DrawMenu(const AFSLapVehiclePawn& Vehicle)
{
	static const TCHAR* const Names[] = { TEXT("HOT LAP"), TEXT("ARCADE"), TEXT("CITY"), TEXT("FREE ROAM"), TEXT("SETUP") };
	static const TCHAR* const Notes[] = {
		TEXT("Team data and FS Rules penalties."),
		TEXT("More grip and power on the FSG track.\nHandbrake to slide. No penalties."),
		TEXT("A made-up city about 3 km across, with traffic\nand ring roads. A timed circuit runs through the middle."),
		TEXT("Open paddock with skidpad and acceleration strip.\n0-100 km/h and 75 m timed."),
		TEXT("Change the aero package, mass, power, tyres,\nbrakes and chassis."),
	};
	Slant(-40, 0, 680, ScreenH, Ink, 60);
	Slant(640, 0, 6, ScreenH, Signal, 60);
	Label(TEXT("FS LAP"), 56, 56, 72, Paper, true);
	Label(TEXT("FAUmax on the FSG endurance track"), 58, 158, 20, Muted);
	for (int32 i = 0; i < UE_ARRAY_COUNT(Names); ++i)
	{
		const bool bSelected = i == Vehicle.GetMenuIndex();
		if (bSelected)
		{
			Slant(44, 214 + i * 66, 500, 58, Signal, 14);
		}
		Label(Names[i], 64, 222 + i * 66, 36, bSelected ? Paper : Muted, true);
	}
	Label(Notes[Vehicle.GetMenuIndex()], 58, 214 + UE_ARRAY_COUNT(Names) * 66 + 20, 18, Paper);
	Label(TEXT("UP / DOWN  choose      ENTER  start"), 58, ScreenH - 62, 18, Muted);
}

void AFSLapHUD::DrawSetup(const AFSLapVehiclePawn& Vehicle)
{
	Box(0, 0, 860, ScreenH, Ink);
	Box(856, 0, 4, ScreenH, Signal);
	Label(TEXT("SETUP"), 56, 40, 52, Paper, true);
	const int32 Rows = Vehicle.GetSetupRowCount();
	for (int32 Row = 0; Row < Rows; ++Row)
	{
		const float Y = 118 + Row * 38;   // 23 rows above the key help
		const bool bSelected = Row == Vehicle.GetSetupRow();
		if (bSelected)
		{
			Box(44, Y - 4, 780, 36, Chip);
			Box(44, Y - 4, 4, 36, Signal);
		}
		Label(Vehicle.GetSetupRowText(Row), 64, Y, 24, bSelected ? Paper : Muted, bSelected);
	}
	Label(TEXT("UP / DOWN  choose     LEFT / RIGHT  change     R  stock car     ENTER  back"), 58, ScreenH - 62, 18, Muted);
}

void AFSLapHUD::DrawWheel(const AFSLapVehiclePawn& Vehicle)
{
	Box(0, 0, 860, ScreenH, Ink);
	Box(856, 0, 4, ScreenH, Signal);
	Label(TEXT("STEERING WHEEL"), 56, 40, 52, Paper, true);
	Label(Vehicle.GetWheelText(), 58, 130, 24, Paper);
	Label(TEXT("ENTER  next step     ESC  back to setup"), 58, ScreenH - 62, 18, Muted);
}

void AFSLapHUD::DrawTower(const AFSLapVehiclePawn& Vehicle, const HoMoLap::Simulation& Sim)
{
	const float X = 28, Y = 28, W = 430;
	const HoMoLap::Mode Mode = Vehicle.GetRaceMode();
	if (Mode == HoMoLap::Mode::Free)
	{
		Panel(X, Y, W, 240);
		Label(TEXT("FREE ROAM"), X + 18, Y + 10, 22, Muted, true);
		Label(LapTime(Sim.fr.run || Sim.fr.got100 || Sim.fr.got75 ? Sim.fr.t : 0), X + 18, Y + 38, 58, Paper, true);
		const auto Time = [](double Value, double BestValue) { return Value < 0 ? FString(TEXT("-")) : FString::Printf(TEXT("%.2f s   best %.2f"), Value, BestValue); };
		const float Row = Y + 122;
		Label(TEXT("0-100 km/h"), X + 18, Row, 20, Muted);
		Label(Time(Sim.fr.t100, Sim.fr.b100), X + 170, Row, 20, Paper);
		Label(TEXT("75 m"), X + 18, Row + 34, 20, Muted);
		Label(Time(Sim.fr.t75, Sim.fr.b75), X + 170, Row + 34, 20, Paper);
		Label(TEXT("Top speed"), X + 18, Row + 68, 20, Muted);
		Label(FString::Printf(TEXT("%.0f km/h"), Sim.fr.top * 3.6), X + 170, Row + 68, 20, Paper);
		return;
	}

	const bool bActive = Sim.IsLapActive(), bValid = Sim.IsCurrentLapValid(), bHasLts = Sim.T.ltsLap > 0;
	Panel(X, Y, W, bHasLts ? 344 : 312);
	Label(bActive ? FString::Printf(TEXT("LAP %d"), static_cast<int32>(Sim.laps.size()) + 1) : FString(TEXT("OUT LAP")), X + 18, Y + 10, 22, Muted, true);
	if (bActive && Sim.CurrentLapPenalty() > 0)
	{
		Box(X + 300, Y + 10, 112, 28, Signal);
		Label(FString::Printf(TEXT("+%.0f s"), Sim.CurrentLapPenalty()), X + 356, Y + 12, 20, Paper, true, true);
	}
	else if (Mode != HoMoLap::Mode::Sim)
	{
		Box(X + 300, Y + 10, 112, 28, Chip);
		Label(Mode == HoMoLap::Mode::City ? TEXT("CITY") : TEXT("ARCADE"), X + 356, Y + 12, 18, Gold, true, true);
	}
	Label(LapTime(Sim.CurrentLapTime()), X + 18, Y + 38, 58, bActive && !bValid ? Signal : Paper, true);

	// live gaps
	double Gap = 0;
	const bool bLive = bActive && bValid && Sim.CurrentLapTime() > 0.5;
	Label(TEXT("vs your best"), X + 18, Y + 122, 16, Muted);
	if (bLive && Vehicle.GetGapToBest(Gap))
	{
		Label(Signed(Gap, 2), X + 18, Y + 142, 28, Gap < 0 ? Fast : Signal, true);
	}
	else
	{
		Label(!bActive ? TEXT("from the line") : (bValid ? TEXT("no lap yet") : TEXT("invalid")), X + 18, Y + 148, 20, Muted);
	}
	if (bHasLts)
	{
		Label(TEXT("vs LTS lap"), X + 228, Y + 122, 16, Muted);
		if (bLive)
		{
			const double ToLts = Sim.CurrentLapTime() + Sim.CurrentLapPenalty() - Sim.T.LtsTimeAt(Sim.LapProgress());
			Label(Signed(ToLts, 2), X + 228, Y + 142, 28, ToLts < 0 ? Fast : Signal, true);
		}
		else
		{
			Label(bActive ? TEXT("-") : TEXT("from the line"), X + 228, Y + 148, 20, Muted);
		}
	}

	// sectors: green when faster than the best lap's, gold when slower, red on an invalid lap; the one you are in is outlined
	const float ChipW = (W - 44) / 3, ChipY = Y + 190;
	for (int32 i = 0; i < 3; ++i)
	{
		const float CX = X + 18 + i * (ChipW + 4);
		const double Done = i < 2 ? Sim.Split(i) : 0;
		const double Sector = i == 1 ? Sim.Split(1) - Sim.Split(0) : Sim.Split(0);
		FLinearColor Back = Chip;
		if (Done > 0)
		{
			const double BestSector = i == 1 ? Vehicle.GetBestSplit(1) - Vehicle.GetBestSplit(0) : Vehicle.GetBestSplit(0);
			Back = !bValid ? Signal : (Vehicle.GetBestSplit(1) <= 0 || Sector < BestSector ? Fast : Gold);
			Back.A = 0.9f;
		}
		Box(CX, ChipY, ChipW, 36, Back);
		if (bActive && Sim.CurrentSector() == i + 1)
		{
			Box(CX, ChipY + 33, ChipW, 3, Paper);
		}
		Label(Done > 0 ? FString::Printf(TEXT("%.2f"), Sector) : FString::Printf(TEXT("S%d"), i + 1), CX + ChipW / 2, ChipY + 6, 20, Done > 0 ? Hex(0x0b, 0x0d, 0x0f) : Muted, true, true);
	}

	// last, best, reference
	float Row = Y + 238;
	const auto Item = [&](const TCHAR* Name, const FString& Value, const FLinearColor& Color)
	{
		Label(Name, X + 18, Row, 20, Muted);
		Label(Value, X + 170, Row, 20, Color, true);
		Row += 32;
	};
	if (Sim.laps.empty())
	{
		Item(TEXT("Last"), TEXT("-:--.---"), Muted);
	}
	else
	{
		const HoMoLap::LapResult& Last = Sim.laps.back();
		Item(TEXT("Last"), LapTime(Last.time) + (Last.valid ? TEXT("") : TEXT("  invalid")), Last.valid ? Paper : Signal);
	}
	Item(TEXT("Best"), Vehicle.GetBestLap() > 0 ? LapTime(Vehicle.GetBestLap()) : FString(TEXT("-:--.---")), Best);
	if (bHasLts)
	{
		Item(*Vehicle.GetLtsLabel(), LapTime(Sim.T.ltsLap), Gold);
	}
}

void AFSLapHUD::DrawMinimap(const HoMoLap::Simulation& Sim)
{
	const HoMoLap::Track& T = Sim.T;
	const bool bGrid = !T.streetX.empty();   // the city maps show the whole city, not just the circuit
	const double X0 = bGrid ? T.streetX.front() : (T.city ? T.APRON[0] : *std::min_element(T.x.begin(), T.x.end())), X1 = bGrid ? T.streetX.back() : (T.city ? T.APRON[2] : *std::max_element(T.x.begin(), T.x.end()));
	const double Y0 = bGrid ? T.streetY.front() : (T.city ? T.APRON[1] : *std::min_element(T.y.begin(), T.y.end())), Y1 = bGrid ? T.streetY.back() : (T.city ? T.APRON[3] : *std::max_element(T.y.begin(), T.y.end()));
	const float Width = 300, Height = 190, Left = ScreenW - Width - 42, Top = 42;
	const double Scale = FMath::Min(Width / (X1 - X0), Height / (Y1 - Y0));
	const float OffsetX = Left + (Width - (X1 - X0) * Scale) / 2, OffsetY = Top + (Height - (Y1 - Y0) * Scale) / 2;
	const auto Point = [&](double X, double Y) { return FVector2D((OffsetX + (X - X0) * Scale) * S, (OffsetY + (Y - Y0) * Scale) * S); };
	Panel(Left - 14, Top - 14, Width + 28, Height + 62);
	for (const double X : T.streetX)
	{
		const FVector2D A = Point(X, Y0), B = Point(X, Y1);
		DrawLine(A.X, A.Y, B.X, B.Y, Muted * 0.45f, 1.0f * S);
	}
	for (const double Y : T.streetY)
	{
		const FVector2D A = Point(X0, Y), B = Point(X1, Y);
		DrawLine(A.X, A.Y, B.X, B.Y, Muted * 0.45f, 1.0f * S);
	}
	const auto Lines = [&](const std::vector<double>& Xs, const std::vector<double>& Ys, const FLinearColor& Color, float Width)
	{
		for (size_t i = 0; i + 1 < Xs.size(); ++i)
		{
			if (Xs[i] < 1e8 && Xs[i + 1] < 1e8)   // 1e9 marks a break between lines
			{
				const FVector2D A = Point(Xs[i], Ys[i]), B = Point(Xs[i + 1], Ys[i + 1]);
				DrawLine(A.X, A.Y, B.X, B.Y, Color, Width * S);
			}
		}
	};
	Lines(T.roadX, T.roadY, Gold * 0.8f, 2.0f);   // the main roads
	const int32 Step = FMath::Max(1, T.N / 240);   // about 240 line pieces, whatever the lap length
	for (int32 i = 0; i < T.N; i += Step)
	{
		const int32 Next = (i + Step) % T.N;
		const FVector2D A = Point(T.x[i], T.y[i]), B = Point(T.x[Next], T.y[Next]);
		DrawLine(A.X, A.Y, B.X, B.Y, Paper, 2.0f * S);
	}
	const FVector2D Start = Point(T.x[0], T.y[0]), Car = Point(Sim.c.x, Sim.c.y);
	DrawRect(Gold, Start.X - 3 * S, Start.Y - 6 * S, 6 * S, 12 * S);
	DrawRect(Signal, Car.X - 6 * S, Car.Y - 6 * S, 12 * S, 12 * S);
	Label(FString::Printf(TEXT("%s  -  %s m"), T.ltsLap > 0 ? TEXT("FSG endurance") : TEXT("City circuit"), *FString::FormatAsNumber(FMath::RoundToInt32(T.LEN))), Left, Top + Height + 12, 16, Muted);
}

void AFSLapHUD::DrawSpeedo(const HoMoLap::Simulation& Sim)
{
	const float X = 44, Y = ScreenH - 196, W = 430;
	const double Top = Sim.C.vMotor * (Sim.arcade ? 1.45 : 1.0);
	const float Rev = FMath::Clamp(static_cast<float>(FMath::Abs(Sim.c.vx) / Top), 0.0f, 1.0f);
	const float Power = FMath::Clamp(static_cast<float>(Sim.c.power / (Sim.C.Pmax * (Sim.arcade ? 1.75 : 1.0))), 0.0f, 1.0f);
	Slant(X, Y + 20, W, 116, Ink, 16);
	Slant(X - 14, Y + 20, 6, 116, Signal, 16);
	Label(FString::FromInt(FMath::RoundToInt32(FMath::Abs(Sim.c.vx) * 3.6)), X + 26, Y + 20, 92, Paper, true);
	Label(TEXT("KM/H"), X + 262, Y + 92, 20, Muted, true);
	Label(FString::Printf(TEXT("%.0f kW"), Sim.c.power / 1000), X + 262, Y + 44, 32, Paper, true);
	// the rev bar: slanted segments, red near the motor's limit
	const int32 Segments = 26;
	for (int32 i = 0; i < Segments; ++i)
	{
		const bool bLit = (i + 0.5f) / Segments < Rev;
		Slant(X + 8 + i * 16.5f, Y + 144, 12, 16, bLit ? (i >= Segments - 5 ? Signal : Paper) : Chip, 5);
	}
	Slant(X + 8, Y + 166, 430 * Power, 5, Signal, 2);
}

void AFSLapHUD::DrawAero(const HoMoLap::Simulation& Sim)
{
	const float W = 486, H = 186, X = ScreenW - W - 28, Y = ScreenH - H - 56;
	Panel(X, Y, W, H);
	const float CX = X + 98, CY = Y + H / 2, R = 78, PerG = R / 3;   // 3 g to the edge
	for (int32 G = 1; G <= 3; ++G)
	{
		Ring(CX, CY, G * PerG, FLinearColor(1, 1, 1, 0.16f));
	}
	Box(CX - R, CY, 2 * R, 1, FLinearColor(1, 1, 1, 0.16f));
	Box(CX, CY - R, 1, 2 * R, FLinearColor(1, 1, 1, 0.16f));
	const double Limit = (Sim.c.capF + Sim.c.capR) / (Sim.C.m * 9.81);
	Ring(CX, CY, FMath::Min(static_cast<float>(Limit), 3.2f) * PerG, Gold);
	for (int32 i = 0; i < GTrail.Num(); ++i)
	{
		FLinearColor Dot = Signal;
		Dot.A = 0.1f + 0.6f * i / GTrail.Num();
		Box(CX + GTrail[i].X * PerG - 1.5f, CY - GTrail[i].Y * PerG - 1.5f, 3, 3, Dot);
	}
	if (GTrail.Num())
	{
		Box(CX + GTrail.Last().X * PerG - 3, CY - GTrail.Last().Y * PerG - 3, 6, 6, Paper);
	}
	float Row = Y + 14;
	const auto Item = [&](const TCHAR* Name, const FString& Value)
	{
		Label(Name, X + 200, Row, 16, Muted);
		Label(Value, X + 376, Row - 1, 18, Paper, true);
		Row += 33;
	};
	Item(TEXT("Downforce"), FString::Printf(TEXT("%.0f N"), Sim.c.Fdown));
	Item(TEXT("Drag"), FString::Printf(TEXT("%.0f N"), Sim.c.Fdrag));
	Item(TEXT("ClA with yaw loss"), FString::Printf(TEXT("%.2f m2"), Sim.c.ClAe));
	Item(TEXT("Body slip"), FString::Printf(TEXT("%.1f deg"), FMath::RadiansToDegrees(FMath::Abs(Sim.c.beta))));
	Item(TEXT("Grip limit"), FString::Printf(TEXT("%.2f g"), Limit));
}

void AFSLapHUD::DrawLights(const AFSLapVehiclePawn& Vehicle)
{
	const float Radius = 34, Gap = 26, Width = 5 * 2 * Radius + 4 * Gap, Y = 150;
	const float X0 = (ScreenW - Width) / 2;
	Box(X0 - 26, Y - Radius - 22, Width + 52, 2 * Radius + 44, Ink);
	for (int32 i = 0; i < 5; ++i)
	{
		const FLinearColor Color = i < Vehicle.GetLights() ? Signal : Hex(0x22, 0x26, 0x2b);
		FCanvasNGonItem Light(FVector2D((X0 + Radius + i * (2 * Radius + Gap)) * S, Y * S), FVector2D(Radius * S, Radius * S), 32, Color);
		Canvas->DrawItem(Light);
	}
}

void AFSLapHUD::UpdateMessages(const AFSLapVehiclePawn& Vehicle, const HoMoLap::Simulation& Sim, float DeltaSeconds)
{
	MessageTime = FMath::Max(0.0f, MessageTime - DeltaSeconds);
	const uint8 Flow = static_cast<uint8>(Vehicle.GetFlow());
	if (LastSim != &Sim)   // a new race
	{
		LastSim = &Sim;
		LastSplits[0] = LastSplits[1] = 0;
		LastLaps = 0;
		LastCones = LastOff = 0;
		bLastValid = true;
		bLastGot100 = bLastGot75 = false;
		GTrail.Reset();
		MessageTime = 0;
	}
	if (Vehicle.GetFlow() == EFSLapFlow::Race && LastFlow == static_cast<uint8>(EFSLapFlow::Countdown))
	{
		ShowMessage(TEXT("GO"), FString(), Fast, 0.9f, false);
	}
	LastFlow = Flow;
	if (Vehicle.GetFlow() != EFSLapFlow::Race)
	{
		return;
	}

	if (Sim.fr.got100 && !bLastGot100)
	{
		ShowMessage(TEXT("0-100 km/h"), FString::Printf(TEXT("%.2f s"), Sim.fr.t100), Fast, 1.6f, true);
	}
	if (Sim.fr.got75 && !bLastGot75)
	{
		ShowMessage(TEXT("75 m"), FString::Printf(TEXT("%.3f s  -  %.0f km/h"), Sim.fr.t75, FMath::Abs(Sim.c.vx) * 3.6), Fast, 1.6f, true);
	}
	bLastGot100 = Sim.fr.got100;
	bLastGot75 = Sim.fr.got75;
	if (Sim.isFree)
	{
		return;
	}

	for (int32 i = 0; i < 2; ++i)
	{
		if (Sim.Split(i) > 0 && LastSplits[i] == 0 && Sim.IsCurrentLapValid())
		{
			const double BestSplit = Vehicle.GetBestSplit(i);
			const FString Sub = Sim.T.ltsLap > 0 ? Signed(Sim.Split(i) - Sim.T.ltsSplit[i], 2) + TEXT(" to the LTS lap") : FString();
			if (BestSplit > 0)
			{
				const double Gap = Sim.Split(i) - BestSplit;
				ShowMessage(FString::Printf(TEXT("SECTOR %d  %s"), i + 1, *Signed(Gap, 2)), Sub, Gap < 0 ? Fast : Gold, 1.6f, true);
			}
			else
			{
				ShowMessage(FString::Printf(TEXT("SECTOR %d"), i + 1), Sub, Gold, 1.6f, true);
			}
		}
		LastSplits[i] = Sim.Split(i);
	}
	if (Sim.laps.size() > LastLaps)
	{
		const HoMoLap::LapResult& Lap = Sim.laps.back();
		const FString Penalty = Lap.pen > 0 ? FString::Printf(TEXT("  incl. +%.0f s"), Lap.pen) : FString();
		if (!Lap.valid)
		{
			ShowMessage(TEXT("LAP INVALID"), LapTime(Lap.time) + TEXT("  -  not counted"), Signal, 2.4f, false);
		}
		else if (FMath::Abs(Vehicle.GetBestLap() - Lap.time) < 1e-6)
		{
			ShowMessage(TEXT("NEW BEST LAP"), LapTime(Lap.time) + Penalty, Best, 2.6f, false);
		}
		else
		{
			ShowMessage(LapTime(Lap.time), Signed(Lap.time - Vehicle.GetBestLap(), 3) + TEXT(" to your best") + Penalty, Gold, 2.4f, false);
		}
		LastLaps = Sim.laps.size();
		LastCones = LastOff = 0;
	}
	if (Sim.CurrentLapCones() > LastCones)
	{
		ShowMessage(TEXT("CONE DOWN"), TEXT("+2 s  -  D 10.1.4"), Gold, 1.2f, true);
	}
	if (Sim.CurrentLapOffCourse() > LastOff)
	{
		ShowMessage(TEXT("OFF COURSE"), TEXT("+10 s  -  all four wheels outside the cones, D 10.1.5"), Gold, 1.6f, true);
	}
	if (Sim.IsLapActive() && !Sim.IsCurrentLapValid() && bLastValid && !Sim.T.city)   // the city is open: leaving the circuit is not a fault
	{
		ShowMessage(TEXT("TRACK CUT"), TEXT("This lap will not count"), Signal, 1.8f, true);
	}
	LastCones = Sim.CurrentLapCones();
	LastOff = Sim.CurrentLapOffCourse();
	bLastValid = !Sim.IsLapActive() || Sim.IsCurrentLapValid();
}

void AFSLapHUD::DrawHUD()
{
	Super::DrawHUD();

	const AFSLapVehiclePawn* Vehicle = Cast<AFSLapVehiclePawn>(GetOwningPawn());
	const HoMoLap::Simulation* Sim = Vehicle ? Vehicle->GetSimulation() : nullptr;
	if (!Font)
	{
		Font = LoadObject<UFont>(nullptr, TEXT("/Engine/EngineFonts/Roboto.Roboto"));
	}
	if (!Sim || !Canvas || !Font)
	{
		return;
	}
	S = Canvas->ClipY / 1080.0f;
	ScreenW = Canvas->ClipX / S;
	ScreenH = 1080.0f;
	const float DeltaSeconds = GetWorld()->GetDeltaSeconds();

	if (Vehicle->GetFlow() == EFSLapFlow::Menu)
	{
		DrawMenu(*Vehicle);
		return;
	}
	if (Vehicle->GetFlow() == EFSLapFlow::Setup)
	{
		DrawSetup(*Vehicle);
		return;
	}
	if (Vehicle->GetFlow() == EFSLapFlow::Wheel)
	{
		DrawWheel(*Vehicle);
		return;
	}

	if (Vehicle->GetFlow() == EFSLapFlow::Replay)
	{
		Box(ScreenW / 2 - 230, 30, 460, 84, Ink);
		Label(FString::Printf(TEXT("REPLAY  %s"), *LapTime(Vehicle->GetBestLap())), ScreenW / 2, 38, 38, Paper, true, true);
		Label(TEXT("ESC  back"), ScreenW / 2, 86, 18, Muted, false, true);
		return;
	}

	UpdateMessages(*Vehicle, *Sim, DeltaSeconds);
	if (Vehicle->GetFlow() == EFSLapFlow::Race)
	{
		GTrail.Add(FVector2f(Sim->c.ay / 9.81, Sim->c.ax / 9.81));
		if (GTrail.Num() > 90)
		{
			GTrail.RemoveAt(0);
		}
	}

	if (!Sim->T.city || (Sim->IsLapActive() && Sim->IsCurrentLapValid()))   // roaming the city needs no lap panel, only driving the circuit does
	{
		DrawTower(*Vehicle, *Sim);
	}
	DrawSpeedo(*Sim);
	if (Vehicle->GetRaceMode() == HoMoLap::Mode::Sim)   // engineering data: the sim wants it, arcade and the city do not
	{
		DrawAero(*Sim);
	}
	if (Vehicle->GetRaceMode() != HoMoLap::Mode::Free)
	{
		DrawMinimap(*Sim);
		// wrong way: moving backwards along the circuit for most of a second
		const double Along = Sim->c.vx * FMath::Cos(Sim->c.h - Sim->T.PoseAt(Sim->c.s).h);
		WrongWayTime = Along < -1.5 ? WrongWayTime + DeltaSeconds : 0;
		if (WrongWayTime > 0.8f && MessageTime <= 0 && !Sim->T.city)
		{
			ShowMessage(TEXT("WRONG WAY"), TEXT("Turn around or press R"), Gold, 0.6f, false);
		}
	}
	if (MessageTime > 0)
	{
		const float BannerH = bMessageSmall ? 112 : 164;
		Slant(ScreenW / 2 - 440, 226, 880, BannerH, Ink, 24);
		Slant(ScreenW / 2 - 458, 226, 10, BannerH, MessageColor, 24);
		Label(MessageTitle, ScreenW / 2, 238, bMessageSmall ? 46 : 86, MessageColor, true, true);
		Label(MessageSub, ScreenW / 2, bMessageSmall ? 298 : 342, 24, Paper, true, true);
	}
	if (FMath::Abs(Sim->c.vx) < 2) Label(TEXT("WASD / ARROWS  drive     SPACE  handbrake     C  chase / onboard     R  back on track     ESC  pause"), ScreenW / 2, ScreenH - 36, 16, Muted, false, true);

	if (Vehicle->GetFlow() == EFSLapFlow::Countdown)
	{
		DrawLights(*Vehicle);
	}
	else if (Vehicle->GetFlow() == EFSLapFlow::Paused)
	{
		Box(0, 0, ScreenW, ScreenH, FLinearColor(0, 0, 0, 0.6f));
		Label(TEXT("PAUSED"), ScreenW / 2, 400, 96, Paper, true, true);
		Label(Vehicle->CanReplay() ? TEXT("ENTER  resume        R  restart        B  replay best lap        M  menu") : TEXT("ENTER  resume        R  restart        M  menu"), ScreenW / 2, 530, 26, Muted, false, true);
	}
}
