function v = adtm_vehicle(veh, car) %#codegen
%ADTM_VEHICLE FS Lap's setup applied to the team's vehicle at run time: the values adtm_step and adtm_init then set.
%   car: [C_D, C_LFA, C_LRA with A = 1 m^2 (negative = drag / downforce), mass shift (kg), shift of the mass share on
%   the front axle, CoG height shift (m), motor torque factor, shift of the front share of the roll stiffness, gear
%   ratio factor]. With [C_D C_LFA C_LRA 0 0 0 1 0 1] it is the vehicle file's car exactly; the shifts move the team's
%   values, so FS Lap's stock setup always drives the team's ADTM.
%   v: [m, m_C, l_FA, l_RA, F_SFL0 = F_SFR0, F_SRL0 = F_SRR0, z_cg, T_MMax, c_RollFA, c_RollRA]
%   The callers set the fields themselves: a vehicle struct returned from here is no longer a constant to MATLAB Coder,
%   and the model picks its branches (motor, driveline, suspension) by constant fields.
m_C = veh.m_C + car(4);   % the unsprung masses stay
l_FA = veh.l_FA - veh.wb * car(5);
l_RA = veh.l_RA + veh.wb * car(5);
% roll stiffness per axle (N m/rad) as f_suspensionForces has it: (corner spring / 2 + roll spring) * track^2. The
% shift moves stiffness from one axle's roll spring to the other's, so the total and the ride rates stay: a change of
% the split only, as on FS Lap's own model. The team's car has no roll springs, so one of them becomes negative (that
% axle softer in roll than its corner springs alone); the axle stays stiff in roll for a front share between 0 and 1.
kRoll = (veh.c_FA / 2 + veh.c_RollFA) * veh.t_FA^2 + (veh.c_RA / 2 + veh.c_RollRA) * veh.t_RA^2;
% as f_vehicleParameters: the spring pretension carries the chassis
v = [veh.m + car(4), m_C, l_FA, l_RA, 9.80665 * m_C * l_RA / (2 * (l_FA + l_RA)), 9.80665 * m_C * l_FA / (2 * (l_FA + l_RA)), ...
    veh.z_cg + car(6), veh.T_MMax * car(7), veh.c_RollFA + car(8) * kRoll / veh.t_FA^2, veh.c_RollRA - car(8) * kRoll / veh.t_RA^2];
end
