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
end
