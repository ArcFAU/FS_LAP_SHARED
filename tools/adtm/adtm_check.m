function adtm_check()
%ADTM_CHECK Compares the compiled ADTM step (adtm_step_mex from adtm_codegen(Mex=true)) with the team's MATLAB code
%   over 3 s of throttle, steering and braking from 15 m/s, and measures how fast the compiled step is.
%   Run after adtm_codegen whenever the ADTM changes; the C++ in the game comes from the same generated code.
P = adtm_load_params();
aero = [P.veh.C_D; P.veh.C_LFA; P.veh.C_LRA; 0; 0; 0; 1];   % the vehicle file's car (adtm_vehicle)
x0 = adtm_init(aero);
x0(15) = 15; x0(25:28) = 15 / P.tFL.r_0;
dt = 1 / 600;
n = 3 * 600;
u = @(k) [0.8 * sin(2 * pi * 0.5 * k * dt); 10 * (k > 1200); 8 * (k > 1200); 0.6 * (k <= 1200); 1; 1];
xm = x0; xc = x0;
worst = 0;
for k = 1:n
    xm = adtm_step(xm, u(k), aero, k * dt, dt);
    xc = adtm_step_mex(xc, u(k), aero, k * dt, dt);
    worst = max(worst, max(abs(xm - xc) ./ max(1, abs(xm))));
end
fprintf('compiled vs MATLAB after %d steps: largest difference %.2e, speed %.3f vs %.3f m/s, yaw rate %.4f vs %.4f rad/s\n', ...
    n, worst, hypot(xm(15), xm(16)), hypot(xc(15), xc(16)), xm(20), xc(20));
assert(worst < 1e-6, 'adtm_check:Mismatch', 'The compiled ADTM differs from the MATLAB code.');
xc = x0; tic;
for k = 1:n, xc = adtm_step_mex(xc, u(k), aero, k * dt, dt); end
fprintf('compiled step (MEX): %.1f us per 600 Hz RK4 step, %.0fx real time\n', toc / n * 1e6, n * dt / toc);

% the torque-driven step (adtm_step_torque): 1 s of equal torque, then 1 s with 8 Nm more on the right motors than on
% the left, with the steering straight, then 1 s of negative torque (recuperation)
ut = @(k) [0; 0; 0; 0; 1; 1; 10 * [1; 1; 1; 1] + (k > 600 && k <= 1200) * [-4; 4; -4; 4] - (k > 1200) * 25 * [1; 1; 1; 1]];
xm = x0; xc = x0;
worst = 0;
yawSplit = 0;
for k = 1:n
    xm = adtm_step_torque(xm, ut(k), aero, k * dt, dt);
    xc = adtm_step_torque_mex(xc, ut(k), aero, k * dt, dt);
    worst = max(worst, max(abs(xm - xc) ./ max(1, abs(xm))));
    if k == 1200, yawSplit = xm(20); vSplit = hypot(xm(15), xm(16)); end
end
fprintf('torque step, compiled vs MATLAB: largest difference %.2e; after 1 s of equal and 1 s of split torque %.2f m/s, yaw rate %+.4f rad/s; after 1 s of -25 Nm %.2f m/s\n', ...
    worst, vSplit, yawSplit, hypot(xm(15), xm(16)));
assert(worst < 1e-6, 'adtm_check:Mismatch', 'The compiled torque-driven ADTM differs from the MATLAB code.');
assert(vSplit > 15 && hypot(xm(15), xm(16)) < vSplit, 'adtm_check:Torque', 'The torque commands do not drive or brake the car.');
assert(yawSplit > 0, 'adtm_check:Yaw', 'More torque on the right motors does not turn the car to the left.');
end
