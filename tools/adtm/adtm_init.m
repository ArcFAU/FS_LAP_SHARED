function x = adtm_init(car, rz, r0) %#codegen
%ADTM_INIT The ADTM standing still at the origin, settled on its springs (f_calcInitState), heading along +x, on the
%   road around the origin given by rz, r0 (adtm_road): its height, pitch and roll follow that road.
P = coder.const(@feval, 'adtm_load_params');
veh = P.veh; v = adtm_vehicle(P.veh, car);
veh.A_A = 1; veh.C_D = car(1); veh.C_LFA = car(2); veh.C_LRA = car(3);
veh.m = v(1); veh.m_C = v(2); veh.l_FA = v(3); veh.l_RA = v(4);
veh.F_SFL0 = v(5); veh.F_SFR0 = v(5); veh.F_SRL0 = v(6); veh.F_SRR0 = v(6); veh.z_cg = v(7); veh.T_MMax = v(8);
veh.c_RollFA = v(9); veh.c_RollRA = v(10); veh.i_G = veh.i_G * car(9);
x = zeros(P.nStates, 1);
[x(1), x(2), x(3), x(4), x(5), x(7), x(8), x(9), x(10)] = f_calcInitState(veh, P.tFL, P.tFR, P.tRL, P.tRR, P.env, adtm_road(rz, r0), P.opts);
end
