#pragma once

#include <vector>

// HoMo-LAP's physics in plain C++: a line-for-line port of MATALB\+hotlap (physicsStep, aiDriver, planSpeeds,
// nearestPoint, poseAt, loadTrack's centreline and cones, simulateLap's lap rules). Names follow the MATLAB code so
// the two read side by side. No Unreal types, so tools\physics_test builds it on its own.
namespace HoMoLap
{
	constexpr double DT = 1.0 / 600.0;

	// The stock car (hotlap.setups base): engineering units as in the setup editor.
	struct HOMOLAPPHYSICS_API Setup
	{
		double ClA = 5.4, CdA = 1.7, bal = 0.50, mCar = 172, mDrv = 80, wf = 0.50, h = 0.293;
		double Pmax = 80, Tm = 21, G = 12.421, nmax = 20000, mu0 = 1.40, muExp = -0.10, ap = 7;
		double brakeFront = 0.70, Fb = 9000, rollFront = 0.65, tv = 1, delMax = 25;
	};

	struct HOMOLAPPHYSICS_API Params
	{
		double L, tf, tr, Fz0, rr, rho, eta, R, Cc, m, a, b, h, Iz, mu0, muExp, ap, B;
		double G, Pmax, brakeFront, Fb, rollFront, tv, delMax, ClA, CdA, bal, vMotor, FdMax, yawLoss;
	};
	HOMOLAPPHYSICS_API Params MakeParams(const Setup& v);

	struct HOMOLAPPHYSICS_API Input
	{
		double steer = 0, thr = 0, brk = 0;   // steer -1..1, + = right
		bool hb = false, assist = false, human = false, arcade = false;
		bool wheel = false;    // steer comes from a steering wheel: no smoothing, the driver's hands are the rate limit
		bool direct = false;   // and 1:1: steer is the share of the car's steering wheel lock, at any speed, without the steering assist
		// the four motors' torque commands (Nm at each motor; FL, FR, RL, RR), as a VCU sends them: then thr is not used and
		// the vehicle model applies these instead of its own torque control. Only the ADTM takes them.
		bool motorTorques = false;
		double Tm[4] = {};
	};

	struct HOMOLAPPHYSICS_API Car
	{
		double x = 0, y = 0, h = 0, vx = 0, vy = 0, r = 0, st = 0, thr = 0, brk = 0, delta = 0;
		double axp = 0, ayp = 0, s = 0, lat = 0, driftT = 0, dLim = 0.4, outT = 0, FyfP = 0, FyrP = 0;
		double ax = 0, ay = 0;   // body accelerations of the last step (MATLAB: f.ax, f.ay)
		double Fdown = 0, Fdrag = 0, ClAe = 0, beta = 0, capF = 0, capR = 0, power = 0;   // the rest of MATLAB's f, for the HUD
		bool grass = false, assistBrake = false, hitWall = false;
		bool slipF = false, slipR = false;   // axle sliding or locked: skid marks and smoke
		bool direct = false;   // st is the share of the car's steering wheel lock (Input::direct)
		bool motorTorques = false;   // Input::motorTorques and Tm, handed on to the external vehicle model
		double Tm[4] = {};
		double steerT = 0;   // steering wheel torque (Nm, the ADTM's T_Steering; 0 on FS Lap's own model): force feedback
	};

	struct HOMOLAPPHYSICS_API Cone { double x, y; char col; bool big, freeOnly; };   // col: 'b' blue left, 'y' yellow right, 'o' orange start
	struct GhostPoint { double t, x, y, h; };
	struct Wall { double x0, y0, x1, y1; };
	struct CityBox { double x, y, hx, hy, h; };
	struct CityPole { double x, y, z, sx, sy, sz; };
	struct CitySphere { double x, y, z, r; };

	struct HOMOLAPPHYSICS_API Pose { double x, y, h; };
	struct HOMOLAPPHYSICS_API Nearest { double d, s, lat; int i; };

	struct HOMOLAPPHYSICS_API Track
	{
		int N = 0;
		std::vector<double> x, y, SEG, S, TTX, TTY, NX, NY, K;
		double LEN = 0, HALF = 2.0, GRID_S = 0;
		double APRON[4], WORLD_T[4], PADDOCK[4], WORLD_F[4];   // x0 y0 x1 y1
		double ACC_X = 0, ACC_Y = 0;
		std::vector<Cone> cones;
		std::vector<Wall> walls;   // what the car can hit; empty on the FSG track
		std::vector<CityBox> buildings, barriers;
		std::vector<double> streetX, streetY, roadX, roadY;   // the big city's street grid, for the map; empty on the FSG track
		bool city = false;                  // an open city: leaving the circuit is not a fault
		std::vector<CityPole> poles, canopies;   // canopies: FSG paddock tent roofs and the start gantry
		std::vector<CitySphere> treeTops;
		std::vector<double> ltsT, ltsS;   // the LTS reference lap; empty in the city
		double ltsLap = 0, ltsSplit[2] = { 0, 0 }, ltsScale = 1;
		Pose PoseAt(double s) const;
		Nearest NearestPoint(double px, double py) const;
		double LtsDistAt(double t) const;
		double LtsTimeAt(double s) const;
	};
	HOMOLAPPHYSICS_API Track MakeFsgTrack();
	HOMOLAPPHYSICS_API Track MakeCityTrack();   // the generated city

	struct HOMOLAPPHYSICS_API LapResult { int n; double time, raw, pen, split1, split2; bool valid; int cones, offCourse; };

	enum class Mode { Sim, Arcade, Free, City };   // City: arcade handling on the city circuit

	// Free roam: 0-100 km/h and 75 m are timed from every standing start. Times are negative until set.
	struct FreeTiming
	{
		double top = 0, t = 0, d = 0, t100 = -1, t75 = -1, b100 = -1, b75 = -1;
		bool armed = true, run = false, got100 = false, got75 = false;
	};

	class HOMOLAPPHYSICS_API Simulation
	{
	public:
		Simulation(const Track& InTrack, const Setup& V, Mode InMode);

		// Another vehicle model doing the dynamics (the team's ADTM, HoMoLapAdtm.h), with the sim input handling in every
		// mode (the arcade layer is FS Lap's own model). FS Lap still turns the inputs into
		// the commands c.thr, c.brk, c.st and c.delta (road wheel angle); the model then moves the car. Grip: the road's
		// friction factor (grass 0.62). It must follow when the game moves c itself (restart, back on track, walls).
		struct VehicleModel
		{
			virtual ~VehicleModel() = default;
			virtual void Step(Car& c, double Dt, double Grip) = 0;
		};
		VehicleModel* external = nullptr;   // not owned; null: FS Lap's own model

		// One 1/600 s step. Human == nullptr lets the AI drive.
		void Step(const Input* Human);
		void PlaceAt(double s);
		void BackOnTrack();   // the R key, and automatically after 1.5 s outside the world edge

		const Track& T;
		Params C;
		Car c;
		double clock = 0;
		std::vector<LapResult> laps;
		std::vector<char> coneDown;
		std::vector<GhostPoint> lastRec;   // the last finished lap, 30 Hz
		FreeTiming fr;
		bool arcade, isFree;

		bool IsLapActive() const { return lapActive; }
		bool IsCurrentLapValid() const { return lapValid; }
		double CurrentLapPenalty() const { return lapPen; }
		int CurrentLapCones() const { return lapCones; }
		int CurrentLapOffCourse() const { return lapOc; }
		int CurrentSector() const { return lapNext; }
		double CurrentLapTime() const { return lapActive ? clock - lapT0 : 0; }
		double Split(int i) const { return lapSplits[i]; }   // 0 until that sector is done
		double LapProgress() const { return lapProg; }       // metres driven along the lap

	private:
		void PhysicsStep(const Input& In, double Dt);
		Input AiDriver() const;
		void PlaceFreeStart();
		void UpdateFreeTiming();
		void CollideWalls();
		void KeepInWorld(double Dt);

		std::vector<double> vPlan, vPlanA;
		Input held;
		long long k = 0;
		bool lapActive = false, lapValid = true, offCourse = false;
		double lapT0 = 0, lapPen = 0, lapSplits[2] = { 0, 0 }, lapRecT = 0, lapProg = 0;
		int lapNext = 1, lapCones = 0, lapOc = 0, lapN = 0;
		std::vector<GhostPoint> lapRec;
	};
}
