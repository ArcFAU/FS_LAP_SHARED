function v = adtm_vehicle(veh, car) %#codegen
%ADTM_VEHICLE FS Lap's setup applied to the team's vehicle at run time: the values adtm_step and adtm_init then set.
%   car: [C_D, C_LFA, C_LRA with A = 1 m^2 (negative = drag / downforce), mass shift (kg), shift of the mass share on
%   the front axle, CoG height shift (m), motor torque factor]. With [C_D C_LFA C_LRA 0 0 0 1] it is the vehicle file's
%   car exactly; the shifts move the team's values, so FS Lap's stock setup always drives the team's ADTM.
%   v: [m, m_C, l_FA, l_RA, F_SFL0 = F_SFR0, F_SRL0 = F_SRR0, z_cg, T_MMax]
%   The callers set the fields themselves: a vehicle struct returned from here is no longer a constant to MATLAB Coder,
%   and the model picks its branches (motor, driveline, suspension) by constant fields.
m_C = veh.m_C + car(4);   % the unsprung masses stay
l_FA = veh.l_FA - veh.wb * car(5);
l_RA = veh.l_RA + veh.wb * car(5);
% as f_vehicleParameters: the spring pretension carries the chassis
v = [veh.m + car(4), m_C, l_FA, l_RA, 9.80665 * m_C * l_RA / (2 * (l_FA + l_RA)), 9.80665 * m_C * l_FA / (2 * (l_FA + l_RA)), ...
    veh.z_cg + car(6), veh.T_MMax * car(7)];
end
