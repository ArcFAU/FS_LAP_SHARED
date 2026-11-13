function result = adtmReplay(runFile, options)
%ADTMREPLAY Offline co-simulation: drives a lap logged in FS Lap again with the team's Advanced Dual Track Model (ADTM).
%   adtmReplay(RUN) reads FS Lap's lap_NN.run.json, turns the driver's steering, throttle and brake into ADTM inputs
%   (steering wheel angle, brake pressures, throttle), runs the ADTM open loop (no driver correcting the car, as the
%   team replays real-car logs) and plots both cars over time: speed, yaw rate, lateral acceleration and the path.
%   It also writes lap_NN.adtm_input.csv in the ADTM's input format, which the ADTM's own start script can load.
%
%   Conversions:
%   - steering: the ADTM's steering wheel angle that gives FS Lap's road wheel angle (mean of the two front wheels),
%     from the ADTM's own steering kinematics;
%   - brake: FS Lap's brake force (pedal x Fb, split by brakeFront) as pressure through the ADTM's brake data;
%   - throttle: FS Lap's pedal, capped at FS Lap's system power (the ADTM limits each motor only, not the system).
%   The ADTM uses its own vehicle file (mass, tyres, suspension). FS Lap's aero (ClA, CdA, balance) is copied over.
%
%   Options: AdtmRoot (default: env FSLAP_ADTM_ROOT or D:\FSLap_VD\advanced-dual-track-model), Vehicle
%   ("FAUmaxTau.m"), EnvConditions ("default_mu0.7", the team's usual road scaling), SimFreq (200 Hz), SavePng.
arguments
    runFile (1, 1) string
    options.AdtmRoot (1, 1) string = defaultRoot()
    options.Vehicle (1, 1) string = "FAUmaxTau.m"
    options.EnvConditions (1, 1) string = "default_mu0.7"
    options.SimFreq (1, 1) double {mustBePositive} = 200
    options.SavePng (1, 1) string = ""
end
run = jsondecode(fileread(runFile));
assert(isfield(run, 'fslap'), 'adtmReplay:NotDriven', '%s is not a lap driven in FS Lap.', runFile);
c = run.fslap.channels; setup = run.fslap.setup;
t = c.time_s(:) - c.time_s(1);

% the ADTM, with its project open (its parameter files find their folders through it); opening runs only its path setup
project = matlab.project.rootProject;
if isempty(project) || ~strcmpi(string(project.RootFolder), options.AdtmRoot)
    openProject(options.AdtmRoot);
end
opts = struct('vehicle', char(options.Vehicle), 'envConditions', char(options.EnvConditions), 'simFreq', options.SimFreq, ...
    'dataFreq', 100, 'solver', 'Runge-Kutta', 'tireModelName', 'TMeasy', 'tireModel', 2, 'tireDyn', true, ...
    'damperModelName', 'passive', 'damperModel', 1, 'aeromap', false, 'isSimulink', false, 'forceSkidPad', false, ...
    'driver_separated', false, 'use3DRoad', false, 'road', 'flat.mat');
[xNames, outNames] = f_getIndiceNaming();
xNames = fieldnames(xNames); outNames = fieldnames(outNames);
opts.nStates = numel(xNames); opts.nOutputs = numel(outNames);
veh = f_vehicleParameters(opts);
tir = cellfun(@f_tireParameters, veh.tires(1:4), 'UniformOutput', false);
env = f_environmentParameters(opts.envConditions);
road = struct.empty();
% FS Lap's aero package (A = 1 m^2, coefficients negative for downforce, as in the ADTM's vehicle files)
veh.A_A = 1; veh.C_D = -setup.CdA; veh.C_LFA = -setup.ClA * setup.bal; veh.C_LRA = -setup.ClA * (1 - setup.bal);

% steering: front wheel angle against steering wheel angle, from the ADTM's kinematics at standstill height
x0 = zeros(opts.nStates, 1); x0(15) = 10; opts.x_0 = x0;
[x0(1), x0(2), x0(3), x0(4), x0(5), x0(7), x0(8), x0(9), x0(10)] = f_calcInitState(veh, tir{:}, env, road, opts);
wheelMax = veh.u_max / abs(veh.i_S);
handTable = linspace(-wheelMax, wheelMax, 81).';
roadTable = zeros(size(handTable));
iFL = find(strcmp(outNames, 'delta_FL')); iFR = find(strcmp(outNames, 'delta_FR'));
for k = 1:numel(handTable)
    [~, out] = f_vehicleModel(inputAt(handTable(k), 0, 0, 0), 0, x0, veh, tir{:}, env, road, opts);
    roadTable(k) = (out(iFL) + out(iFR)) / 2;
end
[roadTable, order] = sort(roadTable); handTable = handTable(order);
assert(all(diff(roadTable) > 0), 'adtmReplay:Steering', 'The ADTM front wheel angle is not monotonic in the steering wheel angle.');
handAngle = interp1(roadTable, handTable, c.roadWheelAngle_rad(:), 'linear', 'extrap');
outOfRange = mean(abs(handAngle) > wheelMax);

% brake force (N, whole car) as pressure (bar): torque per wheel = force per wheel x tyre radius
r0 = tir{1}.r_0;
brakeForce = c.brake(:) * setup.Fb;
pFront = brakeForce * setup.brakeFront / 2 * r0 / veh.i_p2TBFA;
pRear = brakeForce * (1 - setup.brakeFront) / 2 * r0 / veh.i_p2TBRA;
% throttle: FS Lap's pedal asks for a share of the power-limited drive force; FS Lap ignores it while braking
speed = max(c.vx_mps(:), 0.1);
fullTorquePower = 4 * veh.T_MMax * veh.i_G(1) * speed / r0;
throttle = c.throttle(:) .* min(1, setup.Pmax * 1000 ./ fullTorquePower);   % shortcut: no drivetrain losses, add them if the ADTM gets a system power limit
throttle(c.brake(:) > 0.05) = 0;

% inputs at the simulation rate, start state from the logged lap
opts.tEnd = floor(t(end) * opts.dataFreq) / opts.dataFreq;
nData = round(opts.tEnd * opts.dataFreq) + 1;
tSim = (0:nData * opts.simFreq / opts.dataFreq - 1).' / opts.simFreq;
at = @(v) interp1(t, v, tSim, 'linear', v(end));
input = inputAt(at(handAngle), at(pFront), at(pRear), at(throttle));
opts.x_0 = zeros(opts.nStates, 1);
opts.x_0(15) = c.vx_mps(1);
opts.x_0(25:28) = c.vx_mps(1) / r0;

writeAdtmInput(strrep(runFile, '.run.json', '.adtm_input.csv'), t, handAngle, pFront, pRear, throttle, c);
fprintf('ADTM replay of %s: %.1f s, steering beyond the ADTM lock %.1f %% of the time\n', runFile, opts.tEnd, 100 * outOfRange);
tic; [TTout, opts] = f_solveODE(input, veh, tir{:}, env, road, opts); fprintf('ADTM ran in %.0f s\n', toc);

% compare over time
tA = seconds(TTout.Time);
vA = hypot(TTout.v_x, TTout.v_y);
vF = interp1(t, c.speed_mps(:), tA);
rF = interp1(t, c.yawRate_radps(:), tA);
ayF = interp1(t, c.ay_mps2(:), tA);
distanceA = cumtrapz(tA, vA);
result = struct('time_s', tA, 'adtm', TTout, 'speedFsLap_mps', vF, 'speedAdtm_mps', vA, ...
    'speedRms_mps', sqrt(mean((vA - vF).^2, 'omitnan')), 'yawRateRms_radps', sqrt(mean((TTout.omega_z - rF).^2, 'omitnan')), ...
    'distanceFsLap_m', c.distance_m(end), 'distanceAdtm_m', distanceA(end), 'steeringOutOfRange', outOfRange, 'opts', opts);

figure('Name', 'FS Lap vs ADTM', 'Color', 'w', 'Position', [80 60 1200 900]);
layout = tiledlayout(3, 2, 'TileSpacing', 'compact');
title(layout, sprintf('Open-loop replay of FS Lap %s lap %d through the ADTM (%s, %s)', run.fslap.mode, run.fslap.result.lap, options.Vehicle, options.EnvConditions), 'Interpreter', 'none');
nexttile([1 2]); plot(tA, vF * 3.6, tA, vA * 3.6); ylabel('speed (km/h)'); legend('FS Lap', 'ADTM'); grid on
nexttile; plot(tA, rF, tA, TTout.omega_z); ylabel('yaw rate (rad/s)'); grid on
nexttile; plot(tA, ayF, tA, TTout.a_y); ylabel('a_y (m/s²)'); grid on
nexttile; plot(t, c.roadWheelAngle_rad * 180 / pi, tA, (TTout.delta_FL + TTout.delta_FR) / 2 * 180 / pi); ylabel('front wheel angle (°)'); xlabel('time (s)'); grid on
nexttile; [xF, yF] = startFrame(c.x_m(:), c.y_m(:), c.heading_rad(1)); plot(xF, yF, TTout.x_V - TTout.x_V(1), TTout.y_V - TTout.y_V(1)); axis equal; grid on
xlabel('x from the start (m)'); ylabel('y (m)'); title('path (open loop: drifts off)', 'FontWeight', 'normal');
if options.SavePng ~= "", exportgraphics(gcf, options.SavePng, 'Resolution', 110); end
fprintf('speed RMS difference %.2f m/s, yaw rate RMS difference %.3f rad/s, distance %.0f m (FS Lap) vs %.0f m (ADTM)\n', ...
    result.speedRms_mps, result.yawRateRms_radps, result.distanceFsLap_m, result.distanceAdtm_m);
end

function in = inputAt(delta, pF, pR, thr)
n = numel(delta);
in = struct('delta_D', num2cell(delta(:)), 'p_BFA', num2cell(pF(:) .* ones(n, 1)), 'p_BRA', num2cell(pR(:) .* ones(n, 1)), ...
    'throttle', num2cell(thr(:) .* ones(n, 1)), 'gear', 1, 'mu_N', 1);
end

function [x, y] = startFrame(x, y, heading)
% the logged path moved to the start and turned so the car starts along +x, like the ADTM
x = x - x(1); y = y - y(1);
rot = [cos(-heading) -sin(-heading); sin(-heading) cos(-heading)] * [x.'; y.'];
x = rot(1, :).'; y = rot(2, :).';
end

function writeAdtmInput(file, t, hand, pF, pR, thr, c)
% the ADTM's input format (see its README): Time as mm:ss.SSS; v_x and wheel speeds set its start state
T = table(string(duration(seconds(t), 'Format', 'mm:ss.SSS')), hand, pF, pR, thr, ones(size(t)), ones(size(t)), c.ax_mps2(:), c.ay_mps2(:), ...
    c.vx_mps(:), zeros(size(t)), zeros(size(t)), 'VariableNames', {'Time', 'delta_D', 'p_BFA', 'p_BRA', 'throttle', 'gear', 'mu_N', 'a_x', 'a_y', 'v_x', 'v_y', 'v_z'});
writetable(T, file);
end

function root = defaultRoot()
root = string(getenv('FSLAP_ADTM_ROOT'));
if root == "", root = "D:\FSLap_VD\advanced-dual-track-model"; end
end
