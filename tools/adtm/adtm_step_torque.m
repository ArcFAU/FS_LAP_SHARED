function [x, out] = adtm_step_torque(x, u, car, t, dt) %#codegen
%ADTM_STEP_TORQUE One Runge-Kutta step of the ADTM driven by four motor torque commands, as a VCU sends them.
%   As adtm_step, but u = [delta_D (rad at the steering wheel), p_BFA, p_BRA (bar), throttle (unused), gear, mu_N,
%   T_MCmdFL, T_MCmdFR, T_MCmdRL, T_MCmdRR (Nm at each motor)]. The ADTM's own torque control (f_eMotTorqueCtrl)
%   is skipped: this is its "Simulink" input path (opts.isSimulink), which for hub motors only changes where the
%   torque commands come from. The motor limits (torque, power, speed) stay in the model (f_powertrain).
P = coder.const(@feval, 'adtm_load_params');
veh = P.veh; v = adtm_vehicle(P.veh, car);
veh.A_A = 1; veh.C_D = car(1); veh.C_LFA = car(2); veh.C_LRA = car(3);
veh.m = v(1); veh.m_C = v(2); veh.l_FA = v(3); veh.l_RA = v(4);
veh.F_SFL0 = v(5); veh.F_SFR0 = v(5); veh.F_SRL0 = v(6); veh.F_SRR0 = v(6); veh.z_cg = v(7); veh.T_MMax = v(8);
veh.c_RollFA = v(9); veh.c_RollRA = v(10); veh.i_G = veh.i_G * car(9);
in = struct('delta_D', u(1), 'p_BFA', u(2), 'p_BRA', u(3), 'throttle', u(4), 'gear', u(5), 'mu_N', u(6), ...
    'T_MCmdFL', u(7), 'T_MCmdFR', u(8), 'T_MCmdRL', u(9), 'T_MCmdRR', u(10));
[k1, out] = f_vehicleModel(in, t, x, veh, P.tFL, P.tFR, P.tRL, P.tRR, P.env, P.road, P.optsTorque);
k2 = f_vehicleModel(in, t + dt / 2, x + dt / 2 * k1, veh, P.tFL, P.tFR, P.tRL, P.tRR, P.env, P.road, P.optsTorque);
k3 = f_vehicleModel(in, t + dt / 2, x + dt / 2 * k2, veh, P.tFL, P.tFR, P.tRL, P.tRR, P.env, P.road, P.optsTorque);
k4 = f_vehicleModel(in, t + dt, x + dt * k3, veh, P.tFL, P.tFR, P.tRL, P.tRR, P.env, P.road, P.optsTorque);
x = x + dt * (k1 + 2 * k2 + 2 * k3 + k4) / 6;
end
