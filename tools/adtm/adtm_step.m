function [x, out] = adtm_step(x, u, car, t, dt) %#codegen
%ADTM_STEP One Runge-Kutta step of the team's ADTM (f_vehicleModel), as its own f_solveODE does it.
%   x: the 42 states, u: [delta_D (rad at the steering wheel), p_BFA, p_BRA (bar), throttle, gear, mu_N],
%   car: FS Lap's setup, see adtm_vehicle (7 values), t: time (s), dt: step (s).
P = coder.const(@feval, 'adtm_load_params');
veh = P.veh; v = adtm_vehicle(P.veh, car);
veh.A_A = 1; veh.C_D = car(1); veh.C_LFA = car(2); veh.C_LRA = car(3);
veh.m = v(1); veh.m_C = v(2); veh.l_FA = v(3); veh.l_RA = v(4);
veh.F_SFL0 = v(5); veh.F_SFR0 = v(5); veh.F_SRL0 = v(6); veh.F_SRR0 = v(6); veh.z_cg = v(7); veh.T_MMax = v(8);
in = struct('delta_D', u(1), 'p_BFA', u(2), 'p_BRA', u(3), 'throttle', u(4), 'gear', u(5), 'mu_N', u(6));
[k1, out] = f_vehicleModel(in, t, x, veh, P.tFL, P.tFR, P.tRL, P.tRR, P.env, P.road, P.opts);
k2 = f_vehicleModel(in, t + dt / 2, x + dt / 2 * k1, veh, P.tFL, P.tFR, P.tRL, P.tRR, P.env, P.road, P.opts);
k3 = f_vehicleModel(in, t + dt / 2, x + dt / 2 * k2, veh, P.tFL, P.tFR, P.tRL, P.tRR, P.env, P.road, P.opts);
k4 = f_vehicleModel(in, t + dt, x + dt * k3, veh, P.tFL, P.tFR, P.tRL, P.tRR, P.env, P.road, P.opts);
x = x + dt * (k1 + 2 * k2 + 2 * k3 + k4) / 6;
end
