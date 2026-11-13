function comparison = compareLaps(firstFile, secondFile, options)
%COMPARELAPS Puts two laps side by side over distance: a lap driven in FS Lap against an LTS lap, or two driven laps.
%   compareLaps(FIRST, SECOND) reads two run files (schema homo-lts.run.v1): FS Lap writes one per lap to
%   FSLapUE\Saved\Runs\<session>\lap_NN.run.json, the LTS writes them with lts.io.exportRunJson (FS Lap's LTS
%   reference: FSLapUE\Saved\LTS\runs\<job>\case_001.json). It plots speed, the time gained or lost (FIRST minus
%   SECOND), lateral acceleration and, for a driven lap, throttle, brake and steering, and prints lap and sector times.
%   The SECOND lap's distance is scaled to the FIRST lap's track length (the LTS track and FS Lap's centreline differ
%   by a few metres).
%
%   compareLaps(..., SavePng="compare.png") also writes the figure. Returns the resampled channels.
arguments
    firstFile (1, 1) string
    secondFile (1, 1) string
    options.SavePng (1, 1) string = ""
    options.Points (1, 1) double {mustBeInteger, mustBePositive} = 2000
end
first = readRun(firstFile);
second = readRun(secondFile);
length_m = first.length;
second.distance = second.distance * length_m / second.length;

distance = linspace(0, length_m, options.Points).';
f = resample(first, distance);
s = resample(second, distance);
comparison = struct('distance_m', distance, 'first', f, 'second', s, 'delta_s', f.time - s.time);

figure('Name', 'FS Lap lap comparison', 'Color', 'w', 'Position', [80 60 1100 900]);
layout = tiledlayout(4, 1, 'TileSpacing', 'compact');
title(layout, sprintf('%s  %.3f s   vs   %s  %.3f s', first.name, first.lapTime, second.name, second.lapTime), 'Interpreter', 'none');
nexttile; plot(distance, f.speed * 3.6, distance, s.speed * 3.6); ylabel('speed (km/h)'); legend(first.name, second.name, 'Interpreter', 'none'); grid on
nexttile; plot(distance, comparison.delta_s); yline(0, ':'); ylabel('time delta (s)'); grid on
title('above zero: the first lap is behind', 'FontWeight', 'normal');
nexttile; plot(distance, f.ay, distance, s.ay); ylabel('a_y (m/s²)'); grid on
nexttile
if ~isempty(f.throttle)
    plot(distance, f.throttle, distance, f.brake, distance, f.steer); legend('throttle', 'brake', 'steering', 'Location', 'eastoutside'); ylim([-1.05 1.05])
end
ylabel('inputs (first lap)'); xlabel('distance (m)'); grid on
if options.SavePng ~= "", exportgraphics(gcf, options.SavePng, 'Resolution', 110); end

% lap and sector times, sectors in thirds of the lap as in FS Lap
edges = length_m * [1 2 3] / 3;
ft = diff([0 interp1(distance, f.time, edges)]); st = diff([0 interp1(distance, s.time, edges)]);
comparison.sectors_s = [ft; st];
fprintf('\n%-10s %16s %16s %10s\n', '', first.name, second.name, 'delta');
for k = 1:3
    fprintf('%-10s %16.3f %16.3f %+10.3f\n', sprintf('sector %d', k), ft(k), st(k), ft(k) - st(k));
end
fprintf('%-10s %16.3f %16.3f %+10.3f\n\n', 'lap', first.lapTime, second.lapTime, first.lapTime - second.lapTime);
end

function run = readRun(file)
data = jsondecode(fileread(file));
assert(isfield(data, 'schema') && strcmp(data.schema, 'homo-lts.run.v1'), 'compareLaps:Schema', '%s is not a run file (homo-lts.run.v1).', file);
run.distance = data.lap.distance_m(:);
run.time = data.lap.time_s(:);
run.speed = data.lap.speed_mps(:);
run.ay = data.lap.lateralAcceleration_mps2(:);
run.length = data.track.length_m;
run.lapTime = data.summary.lapTime_s;
[~, name] = fileparts(fileparts(file));
run.name = name;
run.throttle = []; run.brake = []; run.steer = [];
if isfield(data, 'fslap')   % a lap driven in FS Lap: its inputs and its result
    c = data.fslap.channels;
    run.throttle = c.throttle(:); run.brake = c.brake(:); run.steer = c.steer(:);
    run.lapTime = data.fslap.result.rawTime_s;
    run.name = sprintf('%s lap %d', data.fslap.mode, data.fslap.result.lap);
elseif isfield(data, 'model')
    run.name = sprintf('LTS %s', data.model);
end
end

function out = resample(run, distance)
% a driven lap can stand still or roll back a little: keep the distance rising
[d, keep] = unique(cummax(run.distance), 'stable');
pick = @(v) interp1(d, v(keep), distance, 'linear', 'extrap');
out.time = pick(run.time);
out.speed = pick(run.speed);
out.ay = pick(run.ay);
out.throttle = []; out.brake = []; out.steer = [];
if ~isempty(run.throttle)
    out.throttle = pick(run.throttle); out.brake = pick(run.brake); out.steer = pick(run.steer);
end
end
