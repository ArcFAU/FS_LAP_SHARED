function road = adtm_road(rz, r0) %#codegen
%ADTM_ROAD The ADTM's 3D road (f_roadHeight, f_roadMu) as a window of 61 x 61 heights on its 0.1 m grid around the car.
%   rz(i, j): the height (m) at x = (r0(1) + i - 1) / 10, y = (r0(2) + j - 1) / 10 (m), in the frame of the ADTM's x_V, y_V;
%   r0: the window's corner in tenths of a metre (whole numbers). FS Lap moves the window with the car (HoMoLapAdtm.cpp);
%   a flat track gives zeros. The road's friction factor is 1 everywhere: FS Lap's grip stays in mu_N.
road = struct('x', r0(1) + (0:60), 'y', r0(2) + (0:60), 'Z', rz, 'Mu', ones(61));
end
