function files = lapsToMat(target, options)
%LAPSTOMAT Writes FS Lap's lap files as MATLAB .mat files: lap_NN.csv (600 Hz) and lap_NN.run.json -> lap_NN.mat.
%   lapsToMat(TARGET) takes one lap_NN.csv, a session folder (FSLapUE\Saved\Runs\<session>) or a folder of sessions
%   (FSLapUE\Saved\Runs, or a clone of the shared repository's runs branch, where the laps are lap_NN.csv.gz and
%   lap_NN.run.json.gz). Each .mat is written next to its lap and holds:
%     lap     one field per CSV column (time_s, distance_m, x_m, ..., the 600 Hz channels as column vectors)
%     run     the run file (homo-lts.run.v1: session, setup, reference, vehicle model, the 100 Hz lap), if there is one
%     source  the file the lap came from
%   A .mat newer than its lap is kept unless Force=true. Returns the .mat files written.
arguments
    target (1, 1) string
    options.Force (1, 1) logical = false
end
if isfolder(target)
    found = [dir(fullfile(target, '**', 'lap_*.csv')); dir(fullfile(target, '**', 'lap_*.csv.gz'))];
    laps = string(fullfile({found.folder}, {found.name}));
else
    laps = target;
end
files = strings(0, 1);
for csv = laps(:).'
    stem = regexprep(csv, '\.csv(\.gz)?$', '');
    out = stem + ".mat";
    info = dir(csv);
    if ~options.Force && isfile(out) && dir(out).datenum >= info.datenum
        continue
    end
    lap = table2struct(readtable(plain(csv), 'FileType', 'text', 'VariableNamingRule', 'preserve'), 'ToScalar', true);
    run = struct();
    for runFile = [stem + ".run.json", stem + ".run.json.gz"]
        if isfile(runFile)
            run = jsondecode(fileread(plain(runFile)));
        end
    end
    source = csv;
    save(out, 'lap', 'run', 'source');
    files(end + 1, 1) = out; %#ok<AGROW>
end
fprintf('%d .mat file(s) written, %d lap(s) found.\n', numel(files), numel(laps));
end

function file = plain(file)
% a gzipped lap from the shared repository is unpacked to the temporary folder first
if endsWith(file, ".gz")
    file = string(gunzip(file, tempname));
end
end
