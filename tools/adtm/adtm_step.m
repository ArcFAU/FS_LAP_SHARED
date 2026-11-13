function [x, out] = adtm_step(x, u, aero, t, dt) %#codegen
%ADTM_STEP One Runge-Kutta step of the team's ADTM (f_vehicleModel), as its own f_solveODE does it.
%   x: the 42 states, u: [delta_D (rad at the steering wheel), p_BFA, p_BRA (bar), throttle, gear, mu_N],
%   aero: [C_D, C_LFA, C_LRA] with A = 1 m^2 (negative = drag / downforce), t: time (s), dt: step (s).
P = coder.const(@feval, 'adtm_load_params');
veh = P.veh; veh.A_A = 1; veh.C_D = aero(1); veh.C_LFA = aero(2); veh.C_LRA = aero(3);
in = struct('delta_D', u(1), 'p_BFA', u(2), 'p_BRA', u(3), 'throttle', u(4), 'gear', u(5), 'mu_N', u(6));
[k1, out] = f_vehicleModel(in, t, x, veh, P.tFL, P.tFR, P.tRL, P.tRR, P.env, P.road, P.opts);
k2 = f_vehicleModel(in, t + dt / 2, x + dt / 2 * k1, veh, P.tFL, P.tFR, P.tRL, P.tRR, P.env, P.road, P.opts);
k3 = f_vehicleModel(in, t + dt / 2, x + dt / 2 * k2, veh, P.tFL, P.tFR, P.tRL, P.tRR, P.env, P.road, P.opts);
k4 = f_vehicleModel(in, t + dt, x + dt * k3, veh, P.tFL, P.tFR, P.tRL, P.tRR, P.env, P.road, P.opts);
x = x + dt * (k1 + 2 * k2 + 2 * k3 + k4) / 6;
end
