function P = adtm_load_params()
%ADTM_LOAD_PARAMS The ADTM's vehicle, tyre and environment data that adtm_codegen.m saved: baked into the generated code.
P = load(fullfile(fileparts(mfilename('fullpath')), 'adtm_params.mat'));
end
