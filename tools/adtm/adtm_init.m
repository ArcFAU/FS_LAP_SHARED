function x = adtm_init(aero) %#codegen
%ADTM_INIT The ADTM standing still at the origin, settled on its springs (f_calcInitState), heading along +x.
P = coder.const(@feval, 'adtm_load_params');
veh = P.veh; veh.A_A = 1; veh.C_D = aero(1); veh.C_LFA = aero(2); veh.C_LRA = aero(3);
x = zeros(P.nStates, 1);
[x(1), x(2), x(3), x(4), x(5), x(7), x(8), x(9), x(10)] = f_calcInitState(veh, P.tFL, P.tFR, P.tRL, P.tRR, P.env, P.road, P.opts);
end
