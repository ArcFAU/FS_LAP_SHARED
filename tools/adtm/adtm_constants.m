function c = adtm_constants() %#codegen
%ADTM_CONSTANTS [steering wheel angle at full lock (rad), front and rear brake torque per bar (Nm), tyre radius (m),
%   motor torque (Nm), gear ratio, number of states, number of outputs, wheelbase (m)]
P = coder.const(@feval, 'adtm_load_params');
c = [P.veh.u_max / abs(P.veh.i_S), P.veh.i_p2TBFA, P.veh.i_p2TBRA, P.tFL.r_0, P.veh.T_MMax, P.veh.i_G(1), P.nStates, P.nOutputs, P.veh.wb];
end
