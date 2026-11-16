//
// Sponsored License - for use in support of a program or activity
// sponsored by MathWorks.  Not for government, commercial or other
// non-sponsored organizational use.
//
// adtm_step.cpp
//
// Code generation for function 'adtm_step'
//

// Include files
#include "adtm_step.h"
#include "rt_nonfinite.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include <emmintrin.h>
#include <math.h>
#include <string.h>

// Type Definitions
namespace adtm
{
  struct struct_T
  {
    double x[61];
    double y[61];
    double Z[3721];
    double Mu[3721];
  };

  struct b_struct_T
  {
    double m_C;
    double z_cg;
    double l_FA;
    double l_RA;
    double c_RollFA;
    double c_RollRA;
    double F_SFL0;
    double F_SFR0;
    double F_SRL0;
    double F_SRR0;
    double C_LFA;
    double C_LRA;
    double C_D;
    double T_MMax;
    double i_G;
  };
}

// Variable Definitions
namespace adtm
{
  static const signed char iv[196] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
  };

  static const double dv3[3] = { 0.765, 0.62, -0.09 };

  static const double dv4[3] = { 0.635, 0.14, -0.14135219963 };

  static const double dv5[3] = { 0.915, 0.11, -0.15869390375999998 };

  static const double dv6[3] = { 0.764175883332, 0.5853777559, -0.17886923377 };

  static const double dv7[3] = { 0.615, 0.24601, -0.014593333799999997 };

  static const double dv8[3] = { 0.935, 0.234581, -0.018252069119999992 };

  static const double dv9[3] = { 0.75520777448, 0.555162973,
    -0.0077475229400000012 };

  static const double dv10[3] = { 0.8250442, 0.14, -0.13 };

  static const double dv11[3] = { 0.82543414787, 0.5823828205, -0.15144758053 };

  static const double dv12[3] = { 0.765, -0.62, -0.09 };

  static const double dv13[3] = { 0.635, -0.14, -0.14135219963 };

  static const double dv14[3] = { 0.915, -0.11, -0.15869390375999998 };

  static const double dv15[3] = { 0.764175883332, -0.5853777559, -0.17886923377
  };

  static const double dv16[3] = { 0.615, -0.24601, -0.014593333799999997 };

  static const double dv17[3] = { 0.935, -0.234581, -0.018252069119999992 };

  static const double dv18[3] = { 0.75520777448, -0.555162973,
    -0.0077475229400000012 };

  static const double dv19[3] = { 0.8250442, -0.14, -0.13 };

  static const double dv20[3] = { 0.82543414787, -0.5823828205, -0.15144758053 };

  static const double dv21[3] = { -0.765, 0.6, -0.09 };

  static const double dv22[3] = { -0.82000000000000006, 0.171,
    -0.17463822676000001 };

  static const double dv23[3] = { -0.545, 0.171, -0.16138069306 };

  static const double dv24[3] = { -0.755, 0.5373777842, -0.18663381170999999 };

  static const double dv25[3] = { -0.96500000000000008, 0.171,
    -0.06383149703999999 };

  static const double dv26[3] = { -0.545, 0.171, -0.07451259982 };

  static const double dv27[3] = { -0.755, 0.4689737556, 0.0013047124400000082 };

  static const double dv28[3] = { -0.865, 0.171, -0.12 };

  static const double dv29[3] = { -0.865, 0.5598025576, -0.084512323714 };

  static const double dv30[3] = { -0.765, -0.6, -0.09 };

  static const double dv31[3] = { -0.82000000000000006, -0.171,
    -0.17463822676000001 };

  static const double dv32[3] = { -0.545, -0.171, -0.16138069306 };

  static const double dv33[3] = { -0.755, -0.5373777842, -0.18663381170999999 };

  static const double dv34[3] = { -0.96500000000000008, -0.171,
    -0.06383149703999999 };

  static const double dv35[3] = { -0.545, -0.171, -0.07451259982 };

  static const double dv36[3] = { -0.755, -0.4689737556, 0.0013047124400000082 };

  static const double dv37[3] = { -0.865, -0.171, -0.12 };

  static const double dv38[3] = { -0.865, -0.5598025576, -0.084512323714 };

  static const double dv39[9] = { 0.05, 0.0, 0.0, 0.0, 0.05, 0.0, 0.0, 0.0, 0.05
  };

  static const double dv40[9] = { 0.05, 0.0, 0.0, 0.0, 0.26, 0.0, 0.0, 0.0, 0.05
  };

  static const signed char iv1[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  static const double dv41[3] = { 0.0, 0.0, 1.0 };

  static const signed char iv2[42] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0 };

  static const signed char iv3[9] = { 20, 0, 0, 0, 79, 0, 0, 0, 91 };

  static const signed char iv4[42] = { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0 };

  static const signed char iv5[42] = { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0 };

  static const signed char iv6[196] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 79, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 91, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0 };

  static boolean_T isInitialized_adtm_step = false;
}

// Function Declarations
namespace adtm
{
  static void adtm_vehicle(const double car[9], double v[10]);
  static double b_f_LUT1D(const double x[5], const double v[5], double xq);
  namespace coder
  {
    static double b_atan2(double y, double x);
    static double b_hypot(double x, double y);
    static double b_norm(const double x[3]);
    static void eye(double b_I[9]);
    namespace internal
    {
      static double b_minimum(const double x[3]);
      static double maximum(const double x[4]);
      static double minimum(const double x[2]);
    }

    static void mldivide(const double A[9], const double B[3], double Y[3]);
    static void mldivide(const double A[196], double B[14]);
  }

  static double f_LUT1D(const double x[61], const double v[61], double xq);
  static void f_aerodynamics(double v_FA, const double v_0FAV[3], double v_RA,
    const double v_0RAV[3], const double e_z[3], double veh_C_LFA, double
    veh_C_LRA, double veh_C_D, double F_DAFA[3], double F_DARA[3], double
    F_LAFA[3], double F_LARA[3], double F_AFAV[3], double F_ARAV[3], double
    out_aero[8]);
  static double f_powertrain(double omega_FL, double omega_FR, double omega_RL,
    double omega_RR, const double x[42], double throttle, double veh_T_MMax,
    double veh_i_G, double &T_MFR, double &T_MRL, double &T_MRR, double &T_DFL,
    double &T_DFR, double &T_DRL, double &T_DRR);
  static double f_powertrain(double omega_FL, double omega_FR, double omega_RL,
    double omega_RR, double T_MCmdFL, double T_MCmdFR, double T_MCmdRL, double
    T_MCmdRR, double veh_T_MMax, double veh_i_G, double &T_MFR, double &T_MRL,
    double &T_MRR, double &T_DFL, double &T_DFR, double &T_DRL, double &T_DRR);
  static double f_roadContact(const double road_x[61], const double road_y[61],
    const double road_Z[3721], const double road_Mu[3721], const double r_0M0[3],
    const double v_0M0[3], const double omega_0U0[3], const double e_yR0[3],
    double v_0QQ[3], double A_0Q[9], double &r_st, double &dz, double &w, double
    r_MQ0[3], double &mu_R);
  static double f_roadHeight(const double road_x[61], const double road_y[61],
    const double road_Z[3721], double x_0R, double y_0R);
  static double f_suspensionDWB(double sus_delta_0, double sus_gamma_0, const
    double sus_r_VWk[3], const double sus_r_VAk[3], const double sus_r_VBk[3],
    const double sus_r_VCk[3], const double sus_r_VDk[3], const double
    sus_r_VEk[3], const double sus_r_VFk[3], const double sus_r_VRk[3], const
    double sus_r_VQk[3], double phi, double u, double A_VW[9], double r_VWV[3],
    double dvdphi[3], double domdphi[3], double dvdu[3], double domdu[3]);
  static double f_suspensionDWB(double sus_delta_0, double sus_gamma_0, const
    double sus_r_VWk[3], const double sus_r_VAk[3], const double sus_r_VBk[3],
    const double sus_r_VCk[3], const double sus_r_VDk[3], const double
    sus_r_VEk[3], const double sus_r_VFk[3], const double sus_r_VRk[3], const
    double sus_r_VQk[3], double phi, double A_VW[9], double r_VWV[3], double
    dvdphi[3], double domdphi[3]);
  static double f_suspensionForces(const double r_VFLV[3], const double r_VFRV[3],
    const double r_VRLV[3], const double r_VRRV[3], const double r_VFLV_dot[3],
    const double r_VFRV_dot[3], const double r_VRLV_dot[3], const double
    r_VRRV_dot[3], double veh_z_cg, double veh_c_RollFA, double veh_c_RollRA,
    double veh_F_SFL0, double veh_F_SFR0, double veh_F_SRL0, double veh_F_SRR0,
    double &z_FR, double &z_RL, double &z_RR, double &z_FL_dot, double &z_FR_dot,
    double &z_RL_dot, double &z_RR_dot, double &F_CrSFL, double &F_CrSFR, double
    &F_CrSRL, double &F_CrSRR, double &F_HvSFA, double &F_HvSRA, double &F_RoSFA,
    double &F_RoSRA, double &F_SFL, double &F_SFR, double &F_SRL, double &F_SRR);
  static double f_tireTMeasy(const double v_0QQ[3], const double omega_0WW[3],
    double r_st, double dz, double w, double b_gamma, double mu_n, const double
    x_T[3], double F_T[3], double T_T[3], double s[4], double x_T_dot[3], double
    &F_yc, double &dF_x0, double &F_xM, double &dF_y0, double &F_yM);
  static void f_vehicleModel(double inputs_delta_D, double inputs_p_BFA, double
    inputs_p_BRA, double inputs_throttle, double inputs_mu_N, const double x[42],
    const b_struct_T &veh, const struct_T &road, double x_dot[42]);
  static void f_vehicleModel(double inputs_delta_D, double inputs_p_BFA, double
    inputs_p_BRA, double inputs_mu_N, double inputs_T_MCmdFL, double
    inputs_T_MCmdFR, double inputs_T_MCmdRL, double inputs_T_MCmdRR, const
    double x[42], const b_struct_T &veh, const struct_T &road, double x_dot[42]);
}

// Function Definitions
namespace adtm
{
  static void adtm_vehicle(const double car[9], double v[10])
  {
    double b_v_tmp;
    double l_FA;
    double l_RA;
    double v_tmp;

    // ADTM_VEHICLE FS Lap's setup applied to the team's vehicle at run time: the values adtm_step and adtm_init then set.
    //    car: [C_D, C_LFA, C_LRA with A = 1 m^2 (negative = drag / downforce), mass shift (kg), shift of the mass share on
    //    the front axle, CoG height shift (m), motor torque factor, shift of the front share of the roll stiffness, gear
    //    ratio factor]. With [C_D C_LFA C_LRA 0 0 0 1 0 1] it is the vehicle file's car exactly; the shifts move the team's
    //    values, so FS Lap's stock setup always drives the team's ADTM.
    //    v: [m, m_C, l_FA, l_RA, F_SFL0 = F_SFR0, F_SRL0 = F_SRR0, z_cg, T_MMax, c_RollFA, c_RollRA]
    //    The callers set the fields themselves: a vehicle struct returned from here is no longer a constant to MATLAB Coder,
    //    and the model picks its branches (motor, driveline, suspension) by constant fields.
    //  the unsprung masses stay
    l_FA = 0.765 - 1.53 * car[4];
    l_RA = 1.53 * car[4] + 0.765;

    //  roll stiffness per axle (N m/rad) as f_suspensionForces has it: (corner spring / 2 + roll spring) * track^2. The
    //  shift moves stiffness from one axle's roll spring to the other's, so the total and the ride rates stay: a change of
    //  the split only, as on FS Lap's own model. The team's car has no roll springs, so one of them becomes negative (that
    //  axle softer in roll than its corner springs alone); the axle stays stiff in roll for a front share between 0 and 1.
    //  as f_vehicleParameters: the spring pretension carries the chassis
    v[0] = car[3] + 240.0;
    v[1] = car[3] + 193.60000000000002;
    v[2] = l_FA;
    v[3] = l_RA;
    v_tmp = 9.80665 * (car[3] + 193.60000000000002);
    b_v_tmp = 2.0 * (l_FA + l_RA);
    v[4] = v_tmp * l_RA / b_v_tmp;
    v[5] = v_tmp * l_FA / b_v_tmp;
    v[6] = car[5] + 0.09;
    v[7] = 21.0 * car[6];
    v[8] = car[7] * 126866.32283582429 / 1.5376;
    v[9] = 0.0 - car[7] * 126866.32283582429 / 1.44;
  }

  static double b_f_LUT1D(const double x[5], const double v[5], double xq)
  {
    double vq;

    //  Linear interpolation for vq(xq) between values v(x)
    if (xq <= x[0]) {
      //  xq is start point
      //  Extrapolation function
      vq = v[0] + (v[1] - v[0]) * (xq - x[0]) / (x[1] - x[0]);
    } else if (xq >= x[4]) {
      //  xq is end point
      //  Extrapolation function
      vq = v[4] + (v[4] - v[3]) * (xq - x[4]) / (x[4] - x[3]);
    } else {
      int i;
      int idx1;

      //  Find last index idx1 where xq >= x
      idx1 = 0;
      i = 0;
      while ((i < 4) && (!(xq < x[i]))) {
        idx1 = i;
        i++;
      }

      if (x[idx1] == xq) {
        vq = v[idx1];
      } else {
        //  Interpolation function
        vq = v[idx1] + (v[idx1 + 1] - v[idx1]) * (xq - x[idx1]) / (x[idx1 + 1] -
          x[idx1]);
      }
    }

    return vq;
  }

  namespace coder
  {
    static double b_atan2(double y, double x)
    {
      double r;
      if (rtIsNaN(y) || rtIsNaN(x)) {
        r = rtNaN;
      } else if (rtIsInf(y) && rtIsInf(x)) {
        int i;
        int i1;
        if (y > 0.0) {
          i = 1;
        } else {
          i = -1;
        }

        if (x > 0.0) {
          i1 = 1;
        } else {
          i1 = -1;
        }

        r = atan2(static_cast<double>(i), static_cast<double>(i1));
      } else if (x == 0.0) {
        if (y > 0.0) {
          r = RT_PI / 2.0;
        } else if (y < 0.0) {
          r = -(RT_PI / 2.0);
        } else {
          r = 0.0;
        }
      } else {
        r = atan2(y, x);
      }

      return r;
    }

    static double b_hypot(double x, double y)
    {
      double b;
      double r;
      r = fabs(x);
      b = fabs(y);
      if (r < b) {
        r /= b;
        r = b * sqrt(r * r + 1.0);
      } else if (r > b) {
        b /= r;
        r *= sqrt(b * b + 1.0);
      } else if (rtIsNaN(b)) {
        r = rtNaN;
      } else {
        r *= 1.4142135623730951;
      }

      return r;
    }

    static double b_norm(const double x[3])
    {
      double absxk;
      double scale;
      double t;
      double y;
      scale = 3.3121686421112381E-170;
      absxk = fabs(x[0]);
      if (absxk > 3.3121686421112381E-170) {
        y = 1.0;
        scale = absxk;
      } else {
        t = absxk / 3.3121686421112381E-170;
        y = t * t;
      }

      absxk = fabs(x[1]);
      if (absxk > scale) {
        t = scale / absxk;
        y = y * t * t + 1.0;
        scale = absxk;
      } else {
        t = absxk / scale;
        y += t * t;
      }

      absxk = fabs(x[2]);
      if (absxk > scale) {
        t = scale / absxk;
        y = y * t * t + 1.0;
        scale = absxk;
      } else {
        t = absxk / scale;
        y += t * t;
      }

      return scale * sqrt(y);
    }

    static void eye(double b_I[9])
    {
      memset(&b_I[0], 0, 9U * sizeof(double));
      b_I[0] = 1.0;
      b_I[4] = 1.0;
      b_I[8] = 1.0;
    }

    namespace internal
    {
      static double b_minimum(const double x[3])
      {
        double ex;
        int idx;
        int k;
        if (!rtIsNaN(x[0])) {
          idx = 1;
        } else {
          boolean_T exitg1;
          idx = 0;
          k = 2;
          exitg1 = false;
          while ((!exitg1) && (k <= 3)) {
            if (!rtIsNaN(x[k - 1])) {
              idx = k;
              exitg1 = true;
            } else {
              k++;
            }
          }
        }

        if (idx == 0) {
          ex = x[0];
        } else {
          ex = x[idx - 1];
          idx++;
          for (k = idx; k < 4; k++) {
            double d;
            d = x[k - 1];
            if (ex > d) {
              ex = d;
            }
          }
        }

        return ex;
      }

      static double maximum(const double x[4])
      {
        double ex;
        int idx;
        int k;
        if (!rtIsNaN(x[0])) {
          idx = 1;
        } else {
          boolean_T exitg1;
          idx = 0;
          k = 2;
          exitg1 = false;
          while ((!exitg1) && (k <= 4)) {
            if (!rtIsNaN(x[k - 1])) {
              idx = k;
              exitg1 = true;
            } else {
              k++;
            }
          }
        }

        if (idx == 0) {
          ex = x[0];
        } else {
          ex = x[idx - 1];
          idx++;
          for (k = idx; k < 5; k++) {
            double d;
            d = x[k - 1];
            if (ex < d) {
              ex = d;
            }
          }
        }

        return ex;
      }

      static double minimum(const double x[2])
      {
        double ex;
        if ((x[0] > x[1]) || (rtIsNaN(x[0]) && (!rtIsNaN(x[1])))) {
          ex = x[1];
        } else {
          ex = x[0];
        }

        return ex;
      }
    }

    static void mldivide(const double A[9], const double B[3], double Y[3])
    {
      double b_A[9];
      double a21;
      double maxval;
      int r1;
      int r2;
      int r3;
      memcpy(&b_A[0], &A[0], 9U * sizeof(double));
      r1 = 0;
      r2 = 1;
      r3 = 2;
      maxval = fabs(A[0]);
      a21 = fabs(A[1]);
      if (a21 > maxval) {
        maxval = a21;
        r1 = 1;
        r2 = 0;
      }

      if (fabs(A[2]) > maxval) {
        r1 = 2;
        r2 = 1;
        r3 = 0;
      }

      b_A[r2] = A[r2] / A[r1];
      b_A[r3] /= b_A[r1];
      b_A[r2 + 3] -= b_A[r2] * b_A[r1 + 3];
      b_A[r3 + 3] -= b_A[r3] * b_A[r1 + 3];
      b_A[r2 + 6] -= b_A[r2] * b_A[r1 + 6];
      b_A[r3 + 6] -= b_A[r3] * b_A[r1 + 6];
      if (fabs(b_A[r3 + 3]) > fabs(b_A[r2 + 3])) {
        int rtemp;
        rtemp = r2;
        r2 = r3;
        r3 = rtemp;
      }

      b_A[r3 + 3] /= b_A[r2 + 3];
      b_A[r3 + 6] -= b_A[r3 + 3] * b_A[r2 + 6];
      Y[1] = B[r2] - B[r1] * b_A[r2];
      Y[2] = (B[r3] - B[r1] * b_A[r3]) - Y[1] * b_A[r3 + 3];
      Y[2] /= b_A[r3 + 6];
      Y[0] = B[r1] - Y[2] * b_A[r1 + 6];
      Y[1] -= Y[2] * b_A[r2 + 6];
      Y[1] /= b_A[r2 + 3];
      Y[0] -= Y[1] * b_A[r1 + 3];
      Y[0] /= b_A[r1];
    }

    static void mldivide(const double A[196], double B[14])
    {
      double b_A[196];
      double smax;
      int a;
      int i;
      int jA;
      signed char ipiv[14];
      memcpy(&b_A[0], &A[0], 196U * sizeof(double));
      for (i = 0; i < 14; i++) {
        ipiv[i] = static_cast<signed char>(i + 1);
      }

      for (int j = 0; j < 13; j++) {
        int A_tmp;
        int b_tmp;
        int jp1j;
        int mmj_tmp;
        signed char i1;
        mmj_tmp = 12 - j;
        b_tmp = j * 15;
        jp1j = b_tmp + 2;
        jA = 15 - j;
        a = 0;
        smax = fabs(b_A[b_tmp]);
        for (int k = 2; k < jA; k++) {
          double s;
          s = fabs(b_A[(b_tmp + k) - 1]);
          if (s > smax) {
            a = k - 1;
            smax = s;
          }
        }

        if (b_A[b_tmp + a] != 0.0) {
          if (a != 0) {
            jA = j + a;
            ipiv[j] = static_cast<signed char>(jA + 1);
            for (int k = 0; k < 14; k++) {
              a = j + k * 14;
              smax = b_A[a];
              A_tmp = jA + k * 14;
              b_A[a] = b_A[A_tmp];
              b_A[A_tmp] = smax;
            }
          }

          i = (b_tmp - j) + 14;
          for (a = jp1j; a <= i; a++) {
            b_A[a - 1] /= b_A[b_tmp];
          }
        }

        jA = b_tmp;
        for (A_tmp = 0; A_tmp <= mmj_tmp; A_tmp++) {
          smax = b_A[(b_tmp + A_tmp * 14) + 14];
          if (smax != 0.0) {
            i = jA + 16;
            a = (jA - j) + 28;
            for (jp1j = i; jp1j <= a; jp1j++) {
              b_A[jp1j - 1] += b_A[((b_tmp + jp1j) - jA) - 15] * -smax;
            }
          }

          jA += 14;
        }

        i1 = ipiv[j];
        if (i1 != j + 1) {
          smax = B[j];
          B[j] = B[i1 - 1];
          B[i1 - 1] = smax;
        }
      }

      for (int k = 0; k < 14; k++) {
        jA = 14 * k;
        if (B[k] != 0.0) {
          i = k + 2;
          for (a = i; a < 15; a++) {
            B[a - 1] -= B[k] * b_A[(a + jA) - 1];
          }
        }
      }

      for (int k = 13; k >= 0; k--) {
        jA = 14 * k;
        smax = B[k];
        if (smax != 0.0) {
          smax /= b_A[k + jA];
          B[k] = smax;
          for (a = 0; a < k; a++) {
            B[a] -= B[k] * b_A[a + jA];
          }
        }
      }
    }
  }

  static double f_LUT1D(const double x[61], const double v[61], double xq)
  {
    double vq;

    //  Linear interpolation for vq(xq) between values v(x)
    if (xq <= x[0]) {
      //  xq is start point
      vq = v[0];
    } else if (xq >= x[60]) {
      //  xq is end point
      vq = v[60];
    } else {
      int i;
      int idx1;

      //  Find last index idx1 where xq >= x
      idx1 = 0;
      i = 0;
      while ((i < 60) && (!(xq < x[i]))) {
        idx1 = i;
        i++;
      }

      if (x[idx1] == xq) {
        vq = v[idx1];
      } else {
        //  Interpolation function
        vq = v[idx1] + (v[idx1 + 1] - v[idx1]) * (xq - x[idx1]) / (x[idx1 + 1] -
          x[idx1]);
      }
    }

    return vq;
  }

  static void f_aerodynamics(double v_FA, const double v_0FAV[3], double v_RA,
    const double v_0RAV[3], const double e_z[3], double veh_C_LFA, double
    veh_C_LRA, double veh_C_D, double F_DAFA[3], double F_DARA[3], double
    F_LAFA[3], double F_LARA[3], double F_AFAV[3], double F_ARAV[3], double
    out_aero[8])
  {
    __m128d r;
    __m128d r1;
    __m128d r2;
    __m128d r3;
    double a;
    double a_tmp;
    double b;
    double b_a;
    double b_b;
    double c_a;
    double d;
    double d1;

    //       Vehicle Aerodynamic Model          %%
    //  ----------------------------------------- %
    //  Version: V1.1 - 2025.10.22                %
    //  Compatible ADTM Release: ADTM_1.4         %
    //  Author: Tim Braungardt                    %
    //  Modified by:                              %
    //  ----------------------------------------- %
    out_aero[0] = rtNaN;
    out_aero[1] = rtNaN;
    out_aero[2] = rtNaN;
    out_aero[3] = rtNaN;
    out_aero[4] = rtNaN;
    out_aero[5] = rtNaN;
    out_aero[6] = rtNaN;
    out_aero[7] = veh_C_LFA + veh_C_LRA;

    //  Output
    a_tmp = 0.60204237944132111 * veh_C_D * 0.5;
    a = a_tmp * v_FA;

    //  Drag front
    a_tmp *= v_RA;

    //  Drag rear
    b_a = 0.60204237944132111 * veh_C_LFA;
    b = v_0FAV[0] * v_0FAV[0];

    //  Lift front
    c_a = 0.60204237944132111 * veh_C_LRA;
    b_b = v_0RAV[0] * v_0RAV[0];

    //  Lift rear
    //  Aero. force front
    r = _mm_mul_pd(_mm_set1_pd(a), _mm_loadu_pd(&v_0FAV[0]));
    _mm_storeu_pd(&F_DAFA[0], r);
    r1 = _mm_mul_pd(_mm_set1_pd(a_tmp), _mm_loadu_pd(&v_0RAV[0]));
    _mm_storeu_pd(&F_DARA[0], r1);
    r2 = _mm_loadu_pd(&e_z[0]);
    r3 = _mm_mul_pd(_mm_mul_pd(_mm_set1_pd(b_a), r2), _mm_set1_pd(b));
    _mm_storeu_pd(&F_LAFA[0], r3);
    r2 = _mm_mul_pd(_mm_mul_pd(_mm_set1_pd(c_a), r2), _mm_set1_pd(b_b));
    _mm_storeu_pd(&F_LARA[0], r2);
    _mm_storeu_pd(&F_AFAV[0], _mm_add_pd(r, r3));
    _mm_storeu_pd(&F_ARAV[0], _mm_add_pd(r1, r2));
    d = a * v_0FAV[2];
    F_DAFA[2] = d;
    d1 = a_tmp * v_0RAV[2];
    F_DARA[2] = d1;
    a = b_a * e_z[2] * b;
    F_LAFA[2] = a;
    a_tmp = c_a * e_z[2] * b_b;
    F_LARA[2] = a_tmp;
    F_AFAV[2] = d + a;
    F_ARAV[2] = d1 + a_tmp;

    //  Aero. force rear
  }

  static double f_powertrain(double omega_FL, double omega_FR, double omega_RL,
    double omega_RR, const double x[42], double throttle, double veh_T_MMax,
    double veh_i_G, double &T_MFR, double &T_MRL, double &T_MRR, double &T_DFL,
    double &T_DFR, double &T_DRL, double &T_DRR)
  {
    double b_veh_T_MMax[3];
    double dv[2];
    double T_MCmdFR;
    double T_MCmdRL;
    double T_MFL;
    double d;
    double d1;
    double facOmegaMLimFR;
    double facOmegaMLimRL;
    double facOmegaMLimRR;
    double v_T;

    //    Vehicle Power- and Drivetrain         %%
    //  ----------------------------------------- %
    //  Version: V1.1 - 2026.10.02                %
    //  Compatible ADTM Release:                  %
    //  Author: Simon Frank                       %
    //  Modified by: Tim Braungardt               %
    //  ----------------------------------------- %
    //  Hub-Motors
    T_MFL = 0.1 * (2094.3951023931954 / veh_i_G - omega_FL);

    //  Limit each absolute value of val0 to limit
    if (fabs(T_MFL) > 1.0) {
      if (rtIsNaN(T_MFL)) {
        T_MFL = rtNaN;
      } else if (T_MFL < 0.0) {
        T_MFL = -1.0;
      } else {
        T_MFL = (T_MFL > 0.0);
      }
    }

    facOmegaMLimFR = 0.1 * (2094.3951023931954 / veh_i_G - omega_FR);

    //  Limit each absolute value of val0 to limit
    if (fabs(facOmegaMLimFR) > 1.0) {
      if (rtIsNaN(facOmegaMLimFR)) {
        facOmegaMLimFR = rtNaN;
      } else if (facOmegaMLimFR < 0.0) {
        facOmegaMLimFR = -1.0;
      } else {
        facOmegaMLimFR = (facOmegaMLimFR > 0.0);
      }
    }

    facOmegaMLimRL = 0.1 * (2094.3951023931954 / veh_i_G - omega_RL);

    //  Limit each absolute value of val0 to limit
    if (fabs(facOmegaMLimRL) > 1.0) {
      if (rtIsNaN(facOmegaMLimRL)) {
        facOmegaMLimRL = rtNaN;
      } else if (facOmegaMLimRL < 0.0) {
        facOmegaMLimRL = -1.0;
      } else {
        facOmegaMLimRL = (facOmegaMLimRL > 0.0);
      }
    }

    facOmegaMLimRR = 0.1 * (2094.3951023931954 / veh_i_G - omega_RR);

    //  Limit each absolute value of val0 to limit
    if (fabs(facOmegaMLimRR) > 1.0) {
      if (rtIsNaN(facOmegaMLimRR)) {
        facOmegaMLimRR = rtNaN;
      } else if (facOmegaMLimRR < 0.0) {
        facOmegaMLimRR = -1.0;
      } else {
        facOmegaMLimRR = (facOmegaMLimRR > 0.0);
      }
    }

    //  Use code when NOT running in Simulink
    //  All wheel drive torque control
    //       Electric motor torque control      %%
    //  ----------------------------------------- %
    //  Version: V1.1 - 2024.12.26                %
    //  Compatible ADTM Release: ADTM_1.4         %
    //  Author: Simon Frank simon.sf.frank@tum.de %
    //  Modified by:                              %
    //  Info: Add custom functions here.          %
    //  Input:                                    %
    //    veh     -   struct containing vehicle   %
    //                parameters                  %
    //    tirXX   -   struct containing tire      %
    //                parameters for one tire     %
    //                where XX is FL/FR/RL/RR     %
    //    x       -   state vector                %
    //    throttle -  normalized throttle         %
    //  Output:                                   %
    //    T_MXX   -   Motor torque                %
    //                where XX is FL/FR/RL/RR     %
    //  ----------------------------------------- %
    //     %% Example of a simple slip controller
    //  Useful signals
    //  Motor commands
    //  Simple slip controller function
    //  Target slip
    //  Controller gain
    v_T = x[24] * 0.205;

    //  Transport velocity
    //  Absolute transport velocity + v_n
    //  Longitudinal slip
    //  Slip error (only negative)
    dv[0] = 0.0;
    dv[1] = 0.15 - (v_T - x[14]) / (fabs(v_T) + 0.5);
    d = throttle * veh_T_MMax;
    d1 = d + 100.0 * coder::internal::minimum(dv);

    //  Motor torque command
    if (d1 < 0.0) {
      d1 = 0.0;
    }

    //  Simple slip controller function
    //  Target slip
    //  Controller gain
    v_T = x[25] * 0.205;

    //  Transport velocity
    //  Absolute transport velocity + v_n
    //  Longitudinal slip
    //  Slip error (only negative)
    dv[0] = 0.0;
    dv[1] = 0.15 - (v_T - x[14]) / (fabs(v_T) + 0.5);
    T_MCmdFR = d + 100.0 * coder::internal::minimum(dv);

    //  Motor torque command
    if (T_MCmdFR < 0.0) {
      T_MCmdFR = 0.0;
    }

    //  Simple slip controller function
    //  Target slip
    //  Controller gain
    v_T = x[26] * 0.205;

    //  Transport velocity
    //  Absolute transport velocity + v_n
    //  Longitudinal slip
    //  Slip error (only negative)
    dv[0] = 0.0;
    dv[1] = 0.15 - (v_T - x[14]) / (fabs(v_T) + 0.5);
    T_MCmdRL = d + 100.0 * coder::internal::minimum(dv);

    //  Motor torque command
    if (T_MCmdRL < 0.0) {
      T_MCmdRL = 0.0;
    }

    //  Simple slip controller function
    //  Target slip
    //  Controller gain
    v_T = x[27] * 0.205;

    //  Transport velocity
    //  Absolute transport velocity + v_n
    //  Longitudinal slip
    //  Slip error (only negative)
    dv[0] = 0.0;
    dv[1] = 0.15 - (v_T - x[14]) / (fabs(v_T) + 0.5);
    v_T = d + 100.0 * coder::internal::minimum(dv);

    //  Motor torque command
    if (v_T < 0.0) {
      v_T = 0.0;
    }

    //  Power Limit
    d = fabs(omega_FL * veh_i_G);
    b_veh_T_MMax[0] = veh_T_MMax;
    if (!(d >= 2.2204460492503131E-16)) {
      d = 2.2204460492503131E-16;
    }

    b_veh_T_MMax[1] = 30800.0 / d;
    b_veh_T_MMax[2] = d1;
    if (rtIsNaN(d1)) {
      d1 = rtNaN;
    } else if (d1 < 0.0) {
      d1 = -1.0;
    } else {
      d1 = (d1 > 0.0);
    }

    T_MFL = T_MFL * d1 * coder::internal::b_minimum(b_veh_T_MMax);
    d = fabs(omega_FR * veh_i_G);
    b_veh_T_MMax[0] = veh_T_MMax;
    if (!(d >= 2.2204460492503131E-16)) {
      d = 2.2204460492503131E-16;
    }

    b_veh_T_MMax[1] = 30800.0 / d;
    b_veh_T_MMax[2] = T_MCmdFR;
    if (rtIsNaN(T_MCmdFR)) {
      d1 = rtNaN;
    } else if (T_MCmdFR < 0.0) {
      d1 = -1.0;
    } else {
      d1 = (T_MCmdFR > 0.0);
    }

    T_MFR = facOmegaMLimFR * d1 * coder::internal::b_minimum(b_veh_T_MMax);
    d = fabs(omega_RL * veh_i_G);
    b_veh_T_MMax[0] = veh_T_MMax;
    if (!(d >= 2.2204460492503131E-16)) {
      d = 2.2204460492503131E-16;
    }

    b_veh_T_MMax[1] = 30800.0 / d;
    b_veh_T_MMax[2] = T_MCmdRL;
    if (rtIsNaN(T_MCmdRL)) {
      d1 = rtNaN;
    } else if (T_MCmdRL < 0.0) {
      d1 = -1.0;
    } else {
      d1 = (T_MCmdRL > 0.0);
    }

    T_MRL = facOmegaMLimRL * d1 * coder::internal::b_minimum(b_veh_T_MMax);
    d = fabs(omega_RR * veh_i_G);
    b_veh_T_MMax[0] = veh_T_MMax;
    if (!(d >= 2.2204460492503131E-16)) {
      d = 2.2204460492503131E-16;
    }

    b_veh_T_MMax[1] = 30800.0 / d;
    b_veh_T_MMax[2] = v_T;
    if (rtIsNaN(v_T)) {
      d1 = rtNaN;
    } else if (v_T < 0.0) {
      d1 = -1.0;
    } else {
      d1 = (v_T > 0.0);
    }

    T_MRR = facOmegaMLimRR * d1 * coder::internal::b_minimum(b_veh_T_MMax);
    T_DFL = veh_i_G * 0.97 * T_MFL - omega_FL * 0.000954929658551372;
    T_DFR = veh_i_G * 0.97 * T_MFR - omega_FR * 0.000954929658551372;
    T_DRL = veh_i_G * 0.97 * T_MRL - omega_RL * 0.000954929658551372;
    T_DRR = veh_i_G * 0.97 * T_MRR - omega_RR * 0.000954929658551372;

    //  No single motor
    return T_MFL;
  }

  static double f_powertrain(double omega_FL, double omega_FR, double omega_RL,
    double omega_RR, double T_MCmdFL, double T_MCmdFR, double T_MCmdRL, double
    T_MCmdRR, double veh_T_MMax, double veh_i_G, double &T_MFR, double &T_MRL,
    double &T_MRR, double &T_DFL, double &T_DFR, double &T_DRL, double &T_DRR)
  {
    double b_veh_T_MMax[3];
    double T_MFL;
    double d;
    double facOmegaMLimFR;
    double facOmegaMLimRL;
    double facOmegaMLimRR;

    //    Vehicle Power- and Drivetrain         %%
    //  ----------------------------------------- %
    //  Version: V1.1 - 2026.10.02                %
    //  Compatible ADTM Release:                  %
    //  Author: Simon Frank                       %
    //  Modified by: Tim Braungardt               %
    //  ----------------------------------------- %
    //  Hub-Motors
    T_MFL = 0.1 * (2094.3951023931954 / veh_i_G - omega_FL);

    //  Limit each absolute value of val0 to limit
    if (fabs(T_MFL) > 1.0) {
      if (rtIsNaN(T_MFL)) {
        T_MFL = rtNaN;
      } else if (T_MFL < 0.0) {
        T_MFL = -1.0;
      } else {
        T_MFL = (T_MFL > 0.0);
      }
    }

    facOmegaMLimFR = 0.1 * (2094.3951023931954 / veh_i_G - omega_FR);

    //  Limit each absolute value of val0 to limit
    if (fabs(facOmegaMLimFR) > 1.0) {
      if (rtIsNaN(facOmegaMLimFR)) {
        facOmegaMLimFR = rtNaN;
      } else if (facOmegaMLimFR < 0.0) {
        facOmegaMLimFR = -1.0;
      } else {
        facOmegaMLimFR = (facOmegaMLimFR > 0.0);
      }
    }

    facOmegaMLimRL = 0.1 * (2094.3951023931954 / veh_i_G - omega_RL);

    //  Limit each absolute value of val0 to limit
    if (fabs(facOmegaMLimRL) > 1.0) {
      if (rtIsNaN(facOmegaMLimRL)) {
        facOmegaMLimRL = rtNaN;
      } else if (facOmegaMLimRL < 0.0) {
        facOmegaMLimRL = -1.0;
      } else {
        facOmegaMLimRL = (facOmegaMLimRL > 0.0);
      }
    }

    facOmegaMLimRR = 0.1 * (2094.3951023931954 / veh_i_G - omega_RR);

    //  Limit each absolute value of val0 to limit
    if (fabs(facOmegaMLimRR) > 1.0) {
      if (rtIsNaN(facOmegaMLimRR)) {
        facOmegaMLimRR = rtNaN;
      } else if (facOmegaMLimRR < 0.0) {
        facOmegaMLimRR = -1.0;
      } else {
        facOmegaMLimRR = (facOmegaMLimRR > 0.0);
      }
    }

    d = fabs(omega_FL * veh_i_G);
    b_veh_T_MMax[0] = veh_T_MMax;
    if (!(d >= 2.2204460492503131E-16)) {
      d = 2.2204460492503131E-16;
    }

    b_veh_T_MMax[1] = 30800.0 / d;
    b_veh_T_MMax[2] = fabs(T_MCmdFL);
    if (rtIsNaN(T_MCmdFL)) {
      d = rtNaN;
    } else if (T_MCmdFL < 0.0) {
      d = -1.0;
    } else {
      d = (T_MCmdFL > 0.0);
    }

    T_MFL = T_MFL * d * coder::internal::b_minimum(b_veh_T_MMax);
    d = fabs(omega_FR * veh_i_G);
    b_veh_T_MMax[0] = veh_T_MMax;
    if (!(d >= 2.2204460492503131E-16)) {
      d = 2.2204460492503131E-16;
    }

    b_veh_T_MMax[1] = 30800.0 / d;
    b_veh_T_MMax[2] = fabs(T_MCmdFR);
    if (rtIsNaN(T_MCmdFR)) {
      d = rtNaN;
    } else if (T_MCmdFR < 0.0) {
      d = -1.0;
    } else {
      d = (T_MCmdFR > 0.0);
    }

    T_MFR = facOmegaMLimFR * d * coder::internal::b_minimum(b_veh_T_MMax);
    d = fabs(omega_RL * veh_i_G);
    b_veh_T_MMax[0] = veh_T_MMax;
    if (!(d >= 2.2204460492503131E-16)) {
      d = 2.2204460492503131E-16;
    }

    b_veh_T_MMax[1] = 30800.0 / d;
    b_veh_T_MMax[2] = fabs(T_MCmdRL);
    if (rtIsNaN(T_MCmdRL)) {
      d = rtNaN;
    } else if (T_MCmdRL < 0.0) {
      d = -1.0;
    } else {
      d = (T_MCmdRL > 0.0);
    }

    T_MRL = facOmegaMLimRL * d * coder::internal::b_minimum(b_veh_T_MMax);
    d = fabs(omega_RR * veh_i_G);
    b_veh_T_MMax[0] = veh_T_MMax;
    if (!(d >= 2.2204460492503131E-16)) {
      d = 2.2204460492503131E-16;
    }

    b_veh_T_MMax[1] = 30800.0 / d;
    b_veh_T_MMax[2] = fabs(T_MCmdRR);
    if (rtIsNaN(T_MCmdRR)) {
      d = rtNaN;
    } else if (T_MCmdRR < 0.0) {
      d = -1.0;
    } else {
      d = (T_MCmdRR > 0.0);
    }

    T_MRR = facOmegaMLimRR * d * coder::internal::b_minimum(b_veh_T_MMax);
    T_DFL = veh_i_G * 0.97 * T_MFL - omega_FL * 0.000954929658551372;
    T_DFR = veh_i_G * 0.97 * T_MFR - omega_FR * 0.000954929658551372;
    T_DRL = veh_i_G * 0.97 * T_MRL - omega_RL * 0.000954929658551372;
    T_DRR = veh_i_G * 0.97 * T_MRR - omega_RR * 0.000954929658551372;

    //  No single motor
    return T_MFL;
  }

  static double f_roadContact(const double road_x[61], const double road_y[61],
    const double road_Z[3721], const double road_Mu[3721], const double r_0M0[3],
    const double v_0M0[3], const double omega_0U0[3], const double e_yR0[3],
    double v_0QQ[3], double A_0Q[9], double &r_st, double &dz, double &w, double
    r_MQ0[3], double &mu_R)
  {
    __m128d r;
    __m128d r1;
    double e_xQ[3];
    double e_xStar[3];
    double r_0PStar[3];
    double r_0R1[3];
    double r_0R2[3];
    double r_0R3[3];
    double r_0RStar1[3];
    double r_0RStar3[3];
    double b_gamma;
    double d;
    double dz_circ;
    double dz_rect;
    double r_stL;
    double r_stR;
    double w_rect;
    double y_Q_rect;
    double yqOnYGrid_tmp;
    boolean_T xqOnXGrid;
    boolean_T yqOnYGrid;

    //           Road Contact Model             %%
    //  ----------------------------------------- %
    //  Version: V1.7 - 2025.06.25                %
    //  Compatible ADTM Release: ADTM_1.4         %
    //  Author: Simon Frank simon.sf.frank@tum.de %
    //  Modified by:                              %
    //  Input:                                    %
    //    road    -   struct of road              %
    //                x, y, Z, and Mu data        %
    //    tir     -   struct containing tire      %
    //                parameters for one tire     %
    //    r_0M0   -   distance vector from origin %
    //                to wheel center M in origin %
    //                frame                       %
    //    v_0M0   -   velocity vector between     %
    //                origin and wheel center M   %
    //                in origin frame             %
    //    omega_0Ustar0 - angular velocity vector %
    //                of upright in relation to   %
    //                origin in origin frame      %
    //                adjusted for wheel bearings %
    //    e_yR0   -   wheel rotation axis in      %
    //                origin frame                %
    //    t       -   current simulation time     %
    //  Output:                                   %
    //    v_0QQ   -   velocity vector between     %
    //                origin and contact point Q  %
    //                in Q frame                  %
    //    A_0Q    -   rotation matrix from Q to   %
    //                origin frame                %
    //    gamma   -   inclination angle           %
    //    r_st    -   static tire radius          %
    //    dz      -   vertical tire deflection    %
    //    w       -   width of tire contact patch %
    //    r_MQ0   -   distance vector from wheel  %
    //                center M to contact point Q %
    //                in origin frame             %
    //    mu_R    -   local friction coefficient  %
    //  ----------------------------------------- %
    //  Road contact
    //  Global road normal
    //  Half tire width b/2
    //  First guess contact point
    e_xStar[0] = e_yR0[1] - 0.0 * e_yR0[2];
    e_xStar[1] = 0.0 * e_yR0[2] - e_yR0[0];
    e_xStar[2] = e_yR0[0] * 0.0 - 0.0 * e_yR0[1];

    //  Kind of forward
    dz_circ = sqrt((e_xStar[0] * e_xStar[0] + e_xStar[1] * e_xStar[1]) +
                   e_xStar[2] * e_xStar[2]);
    r = _mm_loadu_pd(&e_xStar[0]);
    _mm_storeu_pd(&e_xStar[0], _mm_div_pd(r, _mm_set1_pd(dz_circ)));
    e_xStar[2] /= dz_circ;

    //  Adjust length to 1
    //  Kind of up
    r_0PStar[0] = r_0M0[0] - 0.10974999999999999 * (e_xStar[1] * e_yR0[2] -
      e_yR0[1] * e_xStar[2]);
    r_0PStar[1] = r_0M0[1] - 0.10974999999999999 * (e_yR0[0] * e_xStar[2] -
      e_xStar[0] * e_yR0[2]);

    //  Wheel center -> contact point guess P*
    //  3D road
    //  Put P* on road surface
    r_0PStar[2] = f_roadHeight(road_x, road_y, road_Z, r_0PStar[0], r_0PStar[1]);

    //  Sample offsets
    //  Road sample points
    r = _mm_loadu_pd(&e_xStar[0]);
    r = _mm_mul_pd(_mm_set1_pd(0.0205), r);
    r1 = _mm_loadu_pd(&r_0PStar[0]);
    _mm_storeu_pd(&r_0RStar1[0], _mm_add_pd(r1, r));
    _mm_storeu_pd(&e_xStar[0], _mm_sub_pd(r1, r));
    r = _mm_mul_pd(_mm_set1_pd(0.05715), _mm_loadu_pd(&e_yR0[0]));
    _mm_storeu_pd(&r_0RStar3[0], _mm_add_pd(r1, r));
    _mm_storeu_pd(&r_0PStar[0], _mm_sub_pd(r1, r));

    //  Heights of sample points
    r_stL = f_roadHeight(road_x, road_y, road_Z, r_0RStar1[0], r_0RStar1[1]);
    r_stR = f_roadHeight(road_x, road_y, road_Z, r_0RStar3[0], r_0RStar3[1]);

    //  Sample points with height
    r_0R1[0] = r_0RStar1[0];
    r_0R1[1] = r_0RStar1[1];
    r_0R1[2] = r_stL;
    r_0R2[0] = e_xStar[0];
    r_0R2[1] = e_xStar[1];
    y_Q_rect = f_roadHeight(road_x, road_y, road_Z, e_xStar[0], e_xStar[1]);
    r_0R2[2] = y_Q_rect;
    r_0R3[0] = r_0RStar3[0];
    r_0R3[1] = r_0RStar3[1];
    r_0R3[2] = r_stR;
    r_0RStar3[0] = r_0PStar[0];
    r_0RStar3[1] = r_0PStar[1];
    w_rect = f_roadHeight(road_x, road_y, road_Z, r_0PStar[0], r_0PStar[1]);
    r_0RStar3[2] = w_rect;

    //  Road normal
    r = _mm_loadu_pd(&r_0R1[0]);
    r1 = _mm_loadu_pd(&r_0R2[0]);
    _mm_storeu_pd(&r_0RStar1[0], _mm_sub_pd(r, r1));
    r = _mm_loadu_pd(&r_0R3[0]);
    r1 = _mm_loadu_pd(&r_0RStar3[0]);
    _mm_storeu_pd(&e_xQ[0], _mm_sub_pd(r, r1));
    r_0RStar1[2] = r_stL - y_Q_rect;
    e_xQ[2] = r_stR - w_rect;
    e_xStar[0] = r_0RStar1[1] * e_xQ[2] - e_xQ[1] * r_0RStar1[2];
    e_xStar[1] = e_xQ[0] * r_0RStar1[2] - r_0RStar1[0] * e_xQ[2];
    e_xStar[2] = r_0RStar1[0] * e_xQ[1] - e_xQ[0] * r_0RStar1[1];
    dz_circ = sqrt((e_xStar[0] * e_xStar[0] + e_xStar[1] * e_xStar[1]) +
                   e_xStar[2] * e_xStar[2]);
    r = _mm_loadu_pd(&e_xStar[0]);
    _mm_storeu_pd(&e_xStar[0], _mm_div_pd(r, _mm_set1_pd(dz_circ)));
    e_xStar[2] /= dz_circ;

    //  Adjust length to 1
    //  Geometric contact point P
    //  Unit vectors of Q frame
    e_xQ[0] = e_yR0[1] * e_xStar[2] - e_xStar[1] * e_yR0[2];
    e_xQ[1] = e_xStar[0] * e_yR0[2] - e_yR0[0] * e_xStar[2];
    e_xQ[2] = e_yR0[0] * e_xStar[1] - e_xStar[0] * e_yR0[1];

    //  Forward on road surface
    dz_circ = sqrt((e_xQ[0] * e_xQ[0] + e_xQ[1] * e_xQ[1]) + e_xQ[2] * e_xQ[2]);

    //  Adjust length to 1
    //  Left on road surface
    //  Up on wheel rot. axis
    r = _mm_loadu_pd(&e_xQ[0]);
    r = _mm_div_pd(r, _mm_set1_pd(dz_circ));
    _mm_storeu_pd(&e_xQ[0], r);
    _mm_storeu_pd(&A_0Q[0], r);
    d = e_xQ[2] / dz_circ;
    A_0Q[2] = d;
    r_0RStar1[0] = e_xStar[1] * d - e_xQ[1] * e_xStar[2];
    r_0RStar1[1] = e_xQ[0] * e_xStar[2] - e_xStar[0] * d;
    r_0RStar1[2] = e_xStar[0] * e_xQ[1] - e_xQ[0] * e_xStar[1];
    r_0PStar[0] = e_xQ[1] * e_yR0[2] - e_yR0[1] * d;
    r_0PStar[1] = e_yR0[0] * d - e_xQ[0] * e_yR0[2];
    r_0PStar[2] = e_xQ[0] * e_yR0[1] - e_yR0[0] * e_xQ[1];

    //  Rot. matrix from Q to 0
    //  Inclination angle ("camber")
    //  Static tire radius
    A_0Q[3] = r_0RStar1[0];
    A_0Q[6] = e_xStar[0];
    A_0Q[4] = r_0RStar1[1];
    A_0Q[7] = e_xStar[1];
    A_0Q[5] = r_0RStar1[2];
    A_0Q[8] = e_xStar[2];
    b_gamma = asin((e_yR0[0] * e_xStar[0] + e_yR0[1] * e_xStar[1]) + e_yR0[2] *
                   e_xStar[2]);
    r_st = -((e_xStar[0] * (0.25 * (((r_0R1[0] + r_0R2[0]) + r_0R3[0]) +
                r_0RStar3[0]) - r_0M0[0]) + e_xStar[1] * (0.25 * (((r_0R1[1] +
      r_0R2[1]) + r_0R3[1]) + r_0RStar3[1]) - r_0M0[1])) + e_xStar[2] * (0.25 *
              (((r_stL + y_Q_rect) + r_stR) + w_rect) - r_0M0[2])) / ((e_xStar[0]
      * r_0PStar[0] + e_xStar[1] * r_0PStar[1]) + e_xStar[2] * r_0PStar[2]);
    if (r_st > 0.205) {
      r_st = 0.205;
    }

    //  Tire deflection
    //  Rectangular cross section
    y_Q_rect = tan(b_gamma);
    r_stL = r_st + 0.09525 * y_Q_rect;
    r_stR = r_st - 0.09525 * y_Q_rect;
    if ((r_stL < 0.205) && (r_stR < 0.205)) {
      //  Full contact
      dz_rect = 0.205 - r_st;
      dz_circ = cos(b_gamma);
      w_rect = 0.1905 / dz_circ;
      y_Q_rect = -0.03629025 * y_Q_rect / (12.0 * (0.205 - r_st) * dz_circ);
    } else if ((r_stL >= 0.205) && (r_stR < 0.205) && (b_gamma != 0.0)) {
      //  Left edge lifts off road
      dz_circ = tan(fabs(b_gamma));
      dz_rect = (0.205 - r_stR) * (0.205 - r_stR) / (0.381 * dz_circ);
      w_rect = (0.205 - r_stR) / dz_circ;
      if (rtIsNaN(b_gamma)) {
        dz_circ = rtNaN;
      } else if (b_gamma < 0.0) {
        dz_circ = -1.0;
      } else {
        dz_circ = (b_gamma > 0.0);
      }

      y_Q_rect = -dz_circ * (0.09525 / cos(b_gamma) - w_rect / 3.0);
    } else if ((r_stL < 0.205) && (r_stR >= 0.205) && (b_gamma != 0.0)) {
      //  Right edge lifts off road
      dz_circ = tan(fabs(b_gamma));
      dz_rect = (0.205 - r_stL) * (0.205 - r_stL) / (0.381 * dz_circ);
      w_rect = (0.205 - r_stL) / dz_circ;
      if (rtIsNaN(b_gamma)) {
        dz_circ = rtNaN;
      } else if (b_gamma < 0.0) {
        dz_circ = -1.0;
      } else {
        dz_circ = (b_gamma > 0.0);
      }

      y_Q_rect = -dz_circ * (0.09525 / cos(b_gamma) - w_rect / 3.0);
    } else {
      //  No contact
      dz_rect = 0.0;
      w_rect = 0.1905;
      y_Q_rect = 0.0;
      b_gamma = 0.0;
    }

    //  Circular cross section
    //  Radius of circular cross section
    dz_circ = 0.09525 - (r_st - 0.10974999999999999) * cos(b_gamma);

    //  Blended deflection for rounded cross section
    dz = 0.95 * dz_rect + 0.05 * dz_circ;
    w = 0.95 * w_rect + 0.05 * (2.0 * sqrt(0.0090725625 - (0.09525 - dz_circ) *
      (0.09525 - dz_circ)));
    r_stL = 0.95 * y_Q_rect + 0.05 * (-(r_st - 0.10974999999999999) * sin
      (b_gamma));

    //  Contact velocity
    //  Static contact point Q
    //  Upright angular velocity with wheel bearing taken into account
    d = (r_0M0[0] - r_st * r_0PStar[0]) + r_stL * r_0RStar1[0];
    r_0PStar[0] = d;
    r_MQ0[0] = d - r_0M0[0];
    d = (r_0M0[1] - r_st * r_0PStar[1]) + r_stL * r_0RStar1[1];
    r_MQ0[1] = d - r_0M0[1];
    r_MQ0[2] = ((r_0M0[2] - r_st * r_0PStar[2]) + r_stL * r_0RStar1[2]) - r_0M0
      [2];
    dz_circ = (e_yR0[0] * omega_0U0[0] + e_yR0[1] * omega_0U0[1]) + e_yR0[2] *
      omega_0U0[2];

    //  Contact point velocity
    _mm_storeu_pd(&r_0RStar1[0], _mm_sub_pd(_mm_loadu_pd(&omega_0U0[0]),
      _mm_mul_pd(_mm_set1_pd(dz_circ), _mm_loadu_pd(&e_yR0[0]))));
    r_0RStar1[2] = omega_0U0[2] - dz_circ * e_yR0[2];
    w_rect = v_0M0[0] + (r_0RStar1[1] * r_MQ0[2] - r_MQ0[1] * r_0RStar1[2]);
    r_stL = v_0M0[1] + (r_MQ0[0] * r_0RStar1[2] - r_0RStar1[0] * r_MQ0[2]);
    r_stR = v_0M0[2] + (r_0RStar1[0] * r_MQ0[1] - r_MQ0[0] * r_0RStar1[1]);
    for (int i = 0; i < 3; i++) {
      v_0QQ[i] = (A_0Q[3 * i] * w_rect + A_0Q[3 * i + 1] * r_stL) + A_0Q[3 * i +
        2] * r_stR;
    }

    if (dz == 0.0) {
      v_0QQ[2] = 0.0;
    }

    //  Local friction coefficient
    //  Mu from road file
    // F_ROADMU FS Lap's copy of the team ADTM's model\f_roadMu.m (V1.0, 2024.06.21), used only by FS Lap's code generation.
    //    The team's f_roadContact calls f_roadMu(road, x, y) with three arguments, but the original's error message uses its
    //    fourth, t. MATLAB only fails there if the car leaves the road; MATLAB Coder refuses to compile it at all. This copy
    //    differs only in that message. tools\adtm is first on the path in adtm_codegen, so it shadows the original there; the
    //    team's repository is not changed. Remove this file once the team's f_roadMu takes three arguments.
    //    Info: Z is in m while x/y are in 0.1m!
    w_rect = r_0PStar[0] * 10.0;

    //  Convert to 0.1m
    dz_rect = d * 10.0;

    //  Convert to 0.1m
    //  Edge coordinates
    r_stL = floor(w_rect - road_x[0]);
    r_stR = floor(dz_rect - road_y[0]);
    y_Q_rect = road_x[static_cast<int>(r_stL + 1.0) - 1];
    xqOnXGrid = (w_rect == y_Q_rect);
    yqOnYGrid_tmp = road_y[static_cast<int>(r_stR + 1.0) - 1];
    yqOnYGrid = (dz_rect == yqOnYGrid_tmp);

    //  Interpolation functions
    if (xqOnXGrid && (!yqOnYGrid)) {
      double b_road_Mu[61];
      for (int i = 0; i < 61; i++) {
        b_road_Mu[i] = road_Mu[(static_cast<int>(r_stL + 1.0) + 61 * i) - 1];
      }

      mu_R = f_LUT1D(road_y, b_road_Mu, dz_rect);
    } else if ((!xqOnXGrid) && yqOnYGrid) {
      mu_R = f_LUT1D(road_x, &road_Mu[61 * (static_cast<int>(r_stR + 1.0) - 1)],
                     w_rect);
    } else if (xqOnXGrid && yqOnYGrid) {
      mu_R = 1.0;
    } else {
      dz_circ = road_x[static_cast<int>((r_stL + 1.0) + 1.0) - 1];
      r_stR = road_y[static_cast<int>((r_stR + 1.0) + 1.0) - 1];
      r_stL = 1.0 / ((dz_circ - y_Q_rect) * (r_stR - yqOnYGrid_tmp));
      d = r_stL * (dz_circ - w_rect) + r_stL * (w_rect - y_Q_rect);
      mu_R = d * (r_stR - dz_rect) + d * (dz_rect - yqOnYGrid_tmp);
    }

    return b_gamma;
  }

  static double f_roadHeight(const double road_x[61], const double road_y[61],
    const double road_Z[3721], double x_0R, double y_0R)
  {
    double b_y;
    double xqOnXGrid_tmp;
    double y;
    double yq;
    double yqOnYGrid_tmp;
    double z;
    boolean_T xqOnXGrid;
    boolean_T yqOnYGrid;

    //           Local Road Height              %%
    //  ----------------------------------------- %
    //  Version: V1.1 - 2024.05.18                %
    //  Compatible ADTM Release: ADTM_1.4         %
    //  Author: Simon Frank simon.sf.frank@tum.de %
    //  Modified by:                              %
    //  Info: Z is in m while x/y are in 0.1m!    %
    //  Input:                                    %
    //    road    -   struct of road              %
    //                x, y, Z, and Mu data        %
    //    x_0R    -   x coordinate on road        %
    //    y_0R    -   y coordinate on road        %
    //    t       -   current simulation time     %
    //  Output:                                   %
    //    z       -   local road height           %
    //  ----------------------------------------- %
    z = x_0R * 10.0;

    //  Convert to 0.1m
    yq = y_0R * 10.0;

    //  Convert to 0.1m
    //  Edge coordinates
    y = floor(z - road_x[0]);
    b_y = floor(yq - road_y[0]);
    xqOnXGrid_tmp = road_x[static_cast<int>(y + 1.0) - 1];
    xqOnXGrid = (z == xqOnXGrid_tmp);
    yqOnYGrid_tmp = road_y[static_cast<int>(b_y + 1.0) - 1];
    yqOnYGrid = (yq == yqOnYGrid_tmp);

    //  Interpolation functions
    if (xqOnXGrid && (!yqOnYGrid)) {
      double b_road_Z[61];
      for (int road_Z_tmp = 0; road_Z_tmp < 61; road_Z_tmp++) {
        b_road_Z[road_Z_tmp] = road_Z[(static_cast<int>(y + 1.0) + 61 *
          road_Z_tmp) - 1];
      }

      z = f_LUT1D(road_y, b_road_Z, yq);
    } else if ((!xqOnXGrid) && yqOnYGrid) {
      z = f_LUT1D(road_x, &road_Z[61 * (static_cast<int>(b_y + 1.0) - 1)], z);
    } else if (xqOnXGrid && yqOnYGrid) {
      z = road_Z[(static_cast<int>(y + 1.0) + 61 * (static_cast<int>(b_y + 1.0)
        - 1)) - 1];
    } else {
      double a;
      double a_idx_0;
      double a_tmp;
      double b_a_tmp;
      int b_road_Z_tmp;
      int road_Z_tmp;
      a_tmp = road_x[static_cast<int>((y + 1.0) + 1.0) - 1];
      b_a_tmp = road_y[static_cast<int>((b_y + 1.0) + 1.0) - 1];
      a = 1.0 / ((a_tmp - xqOnXGrid_tmp) * (b_a_tmp - yqOnYGrid_tmp));
      a_idx_0 = a * (a_tmp - z);
      a_tmp = a * (z - xqOnXGrid_tmp);
      road_Z_tmp = 61 * (static_cast<int>(b_y + 1.0) - 1);
      b_road_Z_tmp = 61 * (static_cast<int>((b_y + 1.0) + 1.0) - 1);
      z = (a_idx_0 * road_Z[(static_cast<int>(y + 1.0) + road_Z_tmp) - 1] +
           a_tmp * road_Z[(static_cast<int>((y + 1.0) + 1.0) + road_Z_tmp) - 1])
        * (b_a_tmp - yq) + (a_idx_0 * road_Z[(static_cast<int>(y + 1.0) +
        b_road_Z_tmp) - 1] + a_tmp * road_Z[(static_cast<int>((y + 1.0) + 1.0) +
        b_road_Z_tmp) - 1]) * (yq - yqOnYGrid_tmp);
    }

    return z;
  }

  static double f_suspensionDWB(double sus_delta_0, double sus_gamma_0, const
    double sus_r_VWk[3], const double sus_r_VAk[3], const double sus_r_VBk[3],
    const double sus_r_VCk[3], const double sus_r_VDk[3], const double
    sus_r_VEk[3], const double sus_r_VFk[3], const double sus_r_VRk[3], const
    double sus_r_VQk[3], double phi, double u, double A_VW[9], double r_VWV[3],
    double dvdphi[3], double domdphi[3], double dvdu[3], double domdu[3])
  {
    static const signed char a[3] = { 1, 0, 0 };

    static const signed char b_iv[3] = { 0, 0, 1 };

    static const signed char b_iv1[3] = { 1, 0, 0 };

    __m128d r;
    __m128d r1;
    __m128d r2;
    __m128d r3;
    double A_beta[9];
    double A_phi_tmp[9];
    double b_e_ABV2[9];
    double dv[9];
    double e_ABV2[9];
    double e_CFW2[9];
    double b_sus_r_VCk[3];
    double e_ABV[3];
    double e_DEV[3];
    double r_ABV[3];
    double r_ACV[3];
    double r_CFk[3];
    double r_CQk[3];
    double r_DEV[3];
    double r_DFV[3];
    double r_DFk[3];
    double r_VCV[3];
    double r_VRV[3];
    double b;
    double b_r_ABV;
    double b_r_CFk;
    double b_r_DFk;
    double c_r_ABV;
    double d;
    double d1;
    double d2;
    double d3;
    double delta;
    double psi;
    int b_i;
    int i1;

    //       Double Wishbone Suspension         %%
    //  ----------------------------------------- %
    //  Version: V1.3 - 2024.12.26                %
    //  Compatible ADTM Release: ADTM_1.4         %
    //  Author: Simon Frank simon.sf.frank@tum.de %
    //  Modified by:                              %
    //  Info: Suspension points:                  %
    //        A: lower arm @ chassis rear         %
    //        B: lower arm @ chassis front        %
    //        C: lower arm @ upright              %
    //        D: upper arm @ chassis rear         %
    //        E: upper arm @ chassis front        %
    //        F: upper arm @ upright              %
    //        R: tie rod @ steering rack          %
    //        Q: tie rod @ upright                %
    //  Input:                                    %
    //    phi     -   rotation angle of lower     %
    //                wishbone                    %
    //    u       -   steering rack travel        %
    //    sus     -   struct containing           %
    //                suspension parameters       %
    //    t       -   current simulation time     %
    //  Output:                                   %
    //    A_VW    -   rotation matrix from wheel  %
    //                to vehicle frame            %
    //    r_VWV   -   distance vector from CoG    %
    //                to wheel center in vehicle  %
    //                frame                       %
    //    dvdphi  -   dv_U / dphi_dot             %
    //    domdphi -   domega_U / dphi_dot         %
    //    dvdu    -   dv_U / du_dot               %
    //    domdu   -   domega_U / du_dot           %
    //  ----------------------------------------- %
    //  Input check
    //  Upright position and orientation
    //  Rotation of lower wishbone with angle phi
    _mm_storeu_pd(&r_ABV[0], _mm_sub_pd(_mm_loadu_pd(&sus_r_VBk[0]),
      _mm_loadu_pd(&sus_r_VAk[0])));
    r_ABV[2] = sus_r_VBk[2] - sus_r_VAk[2];
    d = coder::b_norm(r_ABV);
    r = _mm_loadu_pd(&r_ABV[0]);
    _mm_storeu_pd(&e_ABV[0], _mm_div_pd(r, _mm_set1_pd(d)));
    e_ABV[2] = r_ABV[2] / d;

    //  Prepare skew symmetric cross product matrix from vector r
    coder::eye(A_phi_tmp);
    b = cos(phi);
    psi = sin(phi);

    //  Rotation of upper wishbone with angle psi
    r = _mm_loadu_pd(&e_ABV[0]);
    _mm_storeu_pd(&e_ABV2[0], _mm_mul_pd(r, _mm_set1_pd(e_ABV[0])));
    e_ABV2[2] = e_ABV[0] * e_ABV[2];
    r_CFk[0] = sus_r_VFk[0] - sus_r_VCk[0];
    r_DFk[0] = sus_r_VFk[0] - sus_r_VDk[0];
    r = _mm_loadu_pd(&e_ABV[0]);
    _mm_storeu_pd(&e_ABV2[3], _mm_mul_pd(r, _mm_set1_pd(e_ABV[1])));
    e_ABV2[5] = e_ABV[1] * e_ABV[2];
    r_CFk[1] = sus_r_VFk[1] - sus_r_VCk[1];
    r_DFk[1] = sus_r_VFk[1] - sus_r_VDk[1];
    r = _mm_loadu_pd(&e_ABV[0]);
    _mm_storeu_pd(&e_ABV2[6], _mm_mul_pd(r, _mm_set1_pd(e_ABV[2])));
    e_ABV2[8] = e_ABV[2] * e_ABV[2];
    r_CFk[2] = sus_r_VFk[2] - sus_r_VCk[2];
    r_DFk[2] = sus_r_VFk[2] - sus_r_VDk[2];
    dv[0] = 0.0 * psi;
    dv[3] = -e_ABV[2] * psi;
    dv[6] = e_ABV[1] * psi;
    dv[1] = e_ABV[2] * psi;
    dv[4] = 0.0 * psi;
    dv[7] = -e_ABV[0] * psi;
    dv[2] = -e_ABV[1] * psi;
    dv[5] = e_ABV[0] * psi;
    dv[8] = 0.0 * psi;
    r = _mm_loadu_pd(&A_phi_tmp[0]);
    r1 = _mm_loadu_pd(&e_ABV2[0]);
    r2 = _mm_loadu_pd(&dv[0]);
    r3 = _mm_set1_pd(b);
    _mm_storeu_pd(&e_ABV2[0], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(_mm_sub_pd(r,
      r1), r3)), r2));
    r = _mm_loadu_pd(&A_phi_tmp[2]);
    r1 = _mm_loadu_pd(&e_ABV2[2]);
    r2 = _mm_loadu_pd(&dv[2]);
    _mm_storeu_pd(&e_ABV2[2], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(_mm_sub_pd(r,
      r1), r3)), r2));
    r = _mm_loadu_pd(&A_phi_tmp[4]);
    r1 = _mm_loadu_pd(&e_ABV2[4]);
    r2 = _mm_loadu_pd(&dv[4]);
    _mm_storeu_pd(&e_ABV2[4], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(_mm_sub_pd(r,
      r1), r3)), r2));
    r = _mm_loadu_pd(&A_phi_tmp[6]);
    r1 = _mm_loadu_pd(&e_ABV2[6]);
    r2 = _mm_loadu_pd(&dv[6]);
    _mm_storeu_pd(&e_ABV2[6], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(_mm_sub_pd(r,
      r1), r3)), r2));
    e_ABV2[8] = (e_ABV2[8] + (A_phi_tmp[8] - e_ABV2[8]) * b) + dv[8];
    _mm_storeu_pd(&b_sus_r_VCk[0], _mm_sub_pd(_mm_loadu_pd(&sus_r_VCk[0]),
      _mm_loadu_pd(&sus_r_VAk[0])));
    d = b_sus_r_VCk[0];
    d1 = b_sus_r_VCk[1];
    d2 = sus_r_VCk[2] - sus_r_VAk[2];
    r = _mm_loadu_pd(&e_ABV2[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&e_ABV2[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&e_ABV2[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&r_ACV[0], r);
    r1 = _mm_loadu_pd(&sus_r_VAk[0]);
    r = _mm_add_pd(r1, r);
    _mm_storeu_pd(&r_VCV[0], r);
    r1 = _mm_loadu_pd(&sus_r_VDk[0]);
    r = _mm_sub_pd(r1, r);
    _mm_storeu_pd(&r_ABV[0], r);
    r = _mm_loadu_pd(&sus_r_VEk[0]);
    r = _mm_sub_pd(r, r1);
    _mm_storeu_pd(&r_DEV[0], r);
    d3 = (e_ABV2[2] * d + e_ABV2[5] * d1) + e_ABV2[8] * d2;
    r_ACV[2] = d3;
    d3 += sus_r_VAk[2];
    r_VCV[2] = d3;
    psi = sus_r_VDk[2];
    r_ABV[2] = psi - d3;
    r_DEV[2] = sus_r_VEk[2] - psi;
    d = coder::b_norm(r_DEV);
    r = _mm_loadu_pd(&r_DEV[0]);
    _mm_storeu_pd(&e_DEV[0], _mm_div_pd(r, _mm_set1_pd(d)));
    e_DEV[2] = r_DEV[2] / d;
    r = _mm_loadu_pd(&e_DEV[0]);
    _mm_storeu_pd(&e_ABV2[0], _mm_mul_pd(r, _mm_set1_pd(e_DEV[0])));
    e_ABV2[2] = e_DEV[0] * e_DEV[2];
    r = _mm_loadu_pd(&e_DEV[0]);
    _mm_storeu_pd(&e_ABV2[3], _mm_mul_pd(r, _mm_set1_pd(e_DEV[1])));
    e_ABV2[5] = e_DEV[1] * e_DEV[2];
    r = _mm_loadu_pd(&e_DEV[0]);
    _mm_storeu_pd(&e_ABV2[6], _mm_mul_pd(r, _mm_set1_pd(e_DEV[2])));
    e_ABV2[8] = e_DEV[2] * e_DEV[2];

    //  Prepare skew symmetric cross product matrix from vector r
    r = _mm_loadu_pd(&A_phi_tmp[0]);
    r1 = _mm_loadu_pd(&e_ABV2[0]);
    _mm_storeu_pd(&A_beta[0], _mm_sub_pd(r, r1));
    r = _mm_loadu_pd(&A_phi_tmp[2]);
    r1 = _mm_loadu_pd(&e_ABV2[2]);
    _mm_storeu_pd(&A_beta[2], _mm_sub_pd(r, r1));
    r = _mm_loadu_pd(&A_phi_tmp[4]);
    r1 = _mm_loadu_pd(&e_ABV2[4]);
    _mm_storeu_pd(&A_beta[4], _mm_sub_pd(r, r1));
    r = _mm_loadu_pd(&A_phi_tmp[6]);
    r1 = _mm_loadu_pd(&e_ABV2[6]);
    _mm_storeu_pd(&A_beta[6], _mm_sub_pd(r, r1));
    A_beta[8] = A_phi_tmp[8] - e_ABV2[8];
    b = 0.0;
    b_sus_r_VCk[0] = e_DEV[1] * r_DFk[2] - r_DFk[1] * e_DEV[2];
    b_sus_r_VCk[1] = r_DFk[0] * e_DEV[2] - e_DEV[0] * r_DFk[2];
    b_sus_r_VCk[2] = e_DEV[0] * r_DFk[1] - r_DFk[0] * e_DEV[1];
    b_r_ABV = 0.0;
    for (int i = 0; i < 3; i++) {
      b += ((r_ABV[0] * A_beta[3 * i] + r_ABV[1] * A_beta[3 * i + 1]) + r_ABV[2]
            * A_beta[3 * i + 2]) * r_DFk[i];
      b_r_ABV += r_ABV[i] * b_sus_r_VCk[i];
      r_DEV[i] = -r_ABV[i];
    }

    c_r_ABV = 0.0;
    b_r_DFk = 0.0;
    psi = 0.0;
    b_r_CFk = 0.0;
    d = r_DEV[0];
    d1 = r_DEV[1];
    d2 = r_DEV[2];
    for (int i = 0; i < 3; i++) {
      d3 = r_DFk[i];
      c_r_ABV += ((d * e_ABV2[3 * i] + d1 * e_ABV2[3 * i + 1]) + d2 * e_ABV2[3 *
                  i + 2]) * d3;
      b_r_DFk += d3 * d3;
      d3 = r_ABV[i];
      psi += d3 * d3;
      d3 = r_CFk[i];
      b_r_CFk += d3 * d3;
    }

    psi = c_r_ABV - 0.5 * ((b_r_DFk + psi) - b_r_CFk);

    //  Additional functions
    //  Solve "a*cos(ang) + b*sin(ang) = c" for angle ang
    if (b_r_ABV < 0.0) {
      psi = asin(-psi / sqrt(b * b + b_r_ABV * b_r_ABV)) - coder::b_atan2(-b,
        -b_r_ABV);
    } else {
      psi = asin(psi / sqrt(b * b + b_r_ABV * b_r_ABV)) - coder::b_atan2(b,
        b_r_ABV);
    }

    b = cos(psi);
    psi = sin(psi);

    //  Rotation of upright around X & Y axis
    dv[0] = 0.0 * psi;
    dv[3] = -e_DEV[2] * psi;
    dv[6] = e_DEV[1] * psi;
    dv[1] = e_DEV[2] * psi;
    dv[4] = 0.0 * psi;
    dv[7] = -e_DEV[0] * psi;
    dv[2] = -e_DEV[1] * psi;
    dv[5] = e_DEV[0] * psi;
    dv[8] = 0.0 * psi;
    r = _mm_loadu_pd(&A_beta[0]);
    r1 = _mm_loadu_pd(&e_ABV2[0]);
    r2 = _mm_loadu_pd(&dv[0]);
    r3 = _mm_set1_pd(b);
    _mm_storeu_pd(&e_ABV2[0], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(r, r3)), r2));
    r = _mm_loadu_pd(&A_beta[2]);
    r1 = _mm_loadu_pd(&e_ABV2[2]);
    r2 = _mm_loadu_pd(&dv[2]);
    _mm_storeu_pd(&e_ABV2[2], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(r, r3)), r2));
    r = _mm_loadu_pd(&A_beta[4]);
    r1 = _mm_loadu_pd(&e_ABV2[4]);
    r2 = _mm_loadu_pd(&dv[4]);
    _mm_storeu_pd(&e_ABV2[4], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(r, r3)), r2));
    r = _mm_loadu_pd(&A_beta[6]);
    r1 = _mm_loadu_pd(&e_ABV2[6]);
    r2 = _mm_loadu_pd(&dv[6]);
    _mm_storeu_pd(&e_ABV2[6], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(r, r3)), r2));
    e_ABV2[8] = (e_ABV2[8] + A_beta[8] * b) + dv[8];
    d = r_DFk[0];
    d1 = r_DFk[1];
    d2 = r_DFk[2];
    r = _mm_loadu_pd(&e_ABV2[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&e_ABV2[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&e_ABV2[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&r_DFV[0], r);
    r1 = _mm_loadu_pd(&sus_r_VDk[0]);
    r = _mm_add_pd(r1, r);
    r1 = _mm_loadu_pd(&r_VCV[0]);
    r = _mm_sub_pd(r, r1);
    _mm_storeu_pd(&domdu[0], r);
    d3 = (e_ABV2[2] * d + e_ABV2[5] * d1) + e_ABV2[8] * d2;
    r_DFV[2] = d3;
    domdu[2] = (sus_r_VDk[2] + d3) - r_VCV[2];

    //  Angle alpha around X axis
    //  Additional functions
    //  Solve "a*cos(ang) + b*sin(ang) = c" for angle ang
    if (domdu[2] < 0.0) {
      psi = asin(-r_CFk[1] / sqrt(domdu[1] * domdu[1] + domdu[2] * domdu[2])) -
        coder::b_atan2(-domdu[1], -domdu[2]);
    } else {
      psi = asin(r_CFk[1] / sqrt(domdu[1] * domdu[1] + domdu[2] * domdu[2])) -
        coder::b_atan2(domdu[1], domdu[2]);
    }

    //  Rotation matrix for angle ang about X axis
    b_r_ABV = sin(psi);
    b_r_DFk = cos(psi);
    e_ABV2[1] = 0.0;
    e_ABV2[4] = b_r_DFk;
    e_ABV2[7] = -b_r_ABV;
    e_ABV2[2] = 0.0;
    e_ABV2[5] = b_r_ABV;
    e_ABV2[8] = b_r_DFk;

    //  Angle beta around Y axis
    //  Additional functions
    //  Solve "a*cos(ang) + b*sin(ang) = c" for angle ang
    psi = asin(domdu[0] / 0.17135654902502986) - coder::b_atan2
      (-0.0089681088520000563, 0.17112171083);

    //  Rotation matrix for angle ang about Y axis
    b_r_CFk = sin(psi);
    psi = cos(psi);
    A_beta[0] = psi;
    A_beta[3] = 0.0;
    A_beta[6] = b_r_CFk;
    A_beta[2] = -b_r_CFk;
    A_beta[5] = 0.0;
    A_beta[8] = psi;

    //  Steering angle delta around kingpin axis
    r_VRV[0] = sus_r_VRk[0];
    r_VRV[1] = sus_r_VRk[1] + u;
    r_VRV[2] = sus_r_VRk[2];
    e_ABV2[0] = 1.0;
    A_beta[1] = 0.0;
    r_ABV[0] = r_VCV[0] - sus_r_VRk[0];
    e_ABV2[3] = 0.0;
    A_beta[4] = 1.0;
    r_ABV[1] = r_VCV[1] - r_VRV[1];
    e_ABV2[6] = 0.0;
    A_beta[7] = 0.0;
    r_ABV[2] = r_VCV[2] - sus_r_VRk[2];
    d = r_ABV[0];
    d1 = r_ABV[1];
    d2 = r_ABV[2];
    for (int i = 0; i < 3; i++) {
      r_DEV[i] = (d * e_ABV2[3 * i] + d1 * e_ABV2[3 * i + 1]) + d2 * e_ABV2[3 *
        i + 2];
    }

    d = coder::b_norm(r_CFk);
    d1 = r_DEV[0];
    d2 = r_DEV[1];
    d3 = r_DEV[2];
    for (int i = 0; i < 3; i++) {
      r_DEV[i] = (d1 * A_beta[3 * i] + d2 * A_beta[3 * i + 1]) + d3 * A_beta[3 *
        i + 2];
      psi = sus_r_VQk[i];
      r_DFk[i] = psi - sus_r_VRk[i];
      r_CQk[i] = psi - sus_r_VCk[i];
      r_CFk[i] /= d;
    }

    r = _mm_loadu_pd(&r_CFk[0]);
    _mm_storeu_pd(&e_CFW2[0], _mm_mul_pd(r, _mm_set1_pd(r_CFk[0])));
    e_CFW2[2] = r_CFk[0] * r_CFk[2];
    r = _mm_loadu_pd(&r_CFk[0]);
    _mm_storeu_pd(&e_CFW2[3], _mm_mul_pd(r, _mm_set1_pd(r_CFk[1])));
    e_CFW2[5] = r_CFk[1] * r_CFk[2];
    r = _mm_loadu_pd(&r_CFk[0]);
    _mm_storeu_pd(&e_CFW2[6], _mm_mul_pd(r, _mm_set1_pd(r_CFk[2])));
    e_CFW2[8] = r_CFk[2] * r_CFk[2];

    //  Prepare skew symmetric cross product matrix from vector r
    r = _mm_loadu_pd(&A_phi_tmp[0]);
    r1 = _mm_loadu_pd(&e_CFW2[0]);
    _mm_storeu_pd(&A_phi_tmp[0], _mm_sub_pd(r, r1));
    r = _mm_loadu_pd(&A_phi_tmp[2]);
    r1 = _mm_loadu_pd(&e_CFW2[2]);
    _mm_storeu_pd(&A_phi_tmp[2], _mm_sub_pd(r, r1));
    r = _mm_loadu_pd(&A_phi_tmp[4]);
    r1 = _mm_loadu_pd(&e_CFW2[4]);
    _mm_storeu_pd(&A_phi_tmp[4], _mm_sub_pd(r, r1));
    r = _mm_loadu_pd(&A_phi_tmp[6]);
    r1 = _mm_loadu_pd(&e_CFW2[6]);
    _mm_storeu_pd(&A_phi_tmp[6], _mm_sub_pd(r, r1));
    A_phi_tmp[8] -= e_CFW2[8];
    b = 0.0;
    b_sus_r_VCk[0] = r_CFk[1] * r_CQk[2] - r_CQk[1] * r_CFk[2];
    b_sus_r_VCk[1] = r_CQk[0] * r_CFk[2] - r_CFk[0] * r_CQk[2];
    b_sus_r_VCk[2] = r_CFk[0] * r_CQk[1] - r_CQk[0] * r_CFk[1];
    b_r_CFk = 0.0;
    b_r_ABV = 0.0;
    c_r_ABV = 0.0;
    psi = 0.0;
    b_r_DFk = 0.0;
    for (int i = 0; i < 3; i++) {
      b_i = 3 * i + 1;
      i1 = 3 * i + 2;
      d = r_CQk[i];
      b += ((r_DEV[0] * A_phi_tmp[3 * i] + r_DEV[1] * A_phi_tmp[b_i]) + r_DEV[2]
            * A_phi_tmp[i1]) * d;
      b_r_CFk += r_DEV[i] * b_sus_r_VCk[i];
      b_r_ABV += ((r_DEV[0] * e_CFW2[3 * i] + r_DEV[1] * e_CFW2[b_i]) + r_DEV[2]
                  * e_CFW2[i1]) * d;
      d1 = r_ABV[i];
      c_r_ABV += d1 * d1;
      psi += d * d;
      d = r_DFk[i];
      b_r_DFk += d * d;
    }

    psi = -(b_r_ABV + 0.5 * ((c_r_ABV + psi) - b_r_DFk));

    //  Additional functions
    //  Solve "a*cos(ang) + b*sin(ang) = c" for angle ang
    if (b_r_CFk < 0.0) {
      delta = asin(-psi / sqrt(b * b + b_r_CFk * b_r_CFk)) - coder::b_atan2(-b,
        -b_r_CFk);
    } else {
      delta = asin(psi / sqrt(b * b + b_r_CFk * b_r_CFk)) - coder::b_atan2(b,
        b_r_CFk);
    }

    b = cos(delta);
    psi = sin(delta);

    //  Upright orientation and position
    //  Rotation matrix for angle ang about Z axis
    b_r_CFk = sin(sus_delta_0);
    c_r_ABV = cos(sus_delta_0);

    //  Rotation matrix for angle ang about X axis
    b_r_ABV = sin(sus_gamma_0);
    b_r_DFk = cos(sus_gamma_0);
    dv[0] = 0.0 * psi;
    dv[3] = -r_CFk[2] * psi;
    dv[6] = r_CFk[1] * psi;
    dv[1] = r_CFk[2] * psi;
    dv[4] = 0.0 * psi;
    dv[7] = -r_CFk[0] * psi;
    dv[2] = -r_CFk[1] * psi;
    dv[5] = r_CFk[0] * psi;
    dv[8] = 0.0 * psi;
    for (int i = 0; i < 3; i++) {
      b_i = static_cast<int>(e_ABV2[i]);
      d = e_ABV2[i + 3];
      d1 = e_ABV2[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        b_e_ABV2[i + 3 * i1] = (static_cast<double>(b_i) * A_beta[3 * i1] + d *
          A_beta[3 * i1 + 1]) + d1 * A_beta[3 * i1 + 2];
      }
    }

    r = _mm_loadu_pd(&A_phi_tmp[0]);
    r1 = _mm_loadu_pd(&e_CFW2[0]);
    r2 = _mm_loadu_pd(&dv[0]);
    r3 = _mm_set1_pd(b);
    _mm_storeu_pd(&e_CFW2[0], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(r, r3)), r2));
    r = _mm_loadu_pd(&A_phi_tmp[2]);
    r1 = _mm_loadu_pd(&e_CFW2[2]);
    r2 = _mm_loadu_pd(&dv[2]);
    _mm_storeu_pd(&e_CFW2[2], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(r, r3)), r2));
    r = _mm_loadu_pd(&A_phi_tmp[4]);
    r1 = _mm_loadu_pd(&e_CFW2[4]);
    r2 = _mm_loadu_pd(&dv[4]);
    _mm_storeu_pd(&e_CFW2[4], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(r, r3)), r2));
    r = _mm_loadu_pd(&A_phi_tmp[6]);
    r1 = _mm_loadu_pd(&e_CFW2[6]);
    r2 = _mm_loadu_pd(&dv[6]);
    _mm_storeu_pd(&e_CFW2[6], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(r, r3)), r2));
    e_CFW2[8] = (e_CFW2[8] + A_phi_tmp[8] * b) + dv[8];
    A_phi_tmp[0] = c_r_ABV;
    A_phi_tmp[3] = -b_r_CFk;
    A_phi_tmp[6] = 0.0;
    A_phi_tmp[1] = b_r_CFk;
    A_phi_tmp[4] = c_r_ABV;
    A_phi_tmp[7] = 0.0;
    for (int i = 0; i < 3; i++) {
      d = b_e_ABV2[i];
      d1 = b_e_ABV2[i + 3];
      d2 = b_e_ABV2[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        A_beta[i + 3 * b_i] = (d * e_CFW2[3 * b_i] + d1 * e_CFW2[3 * b_i + 1]) +
          d2 * e_CFW2[3 * b_i + 2];
      }

      A_phi_tmp[3 * i + 2] = b_iv[i];
    }

    for (int i = 0; i < 3; i++) {
      d = A_beta[i];
      d1 = A_beta[i + 3];
      d2 = A_beta[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        b_e_ABV2[i + 3 * b_i] = (d * A_phi_tmp[3 * b_i] + d1 * A_phi_tmp[3 * b_i
          + 1]) + d2 * A_phi_tmp[3 * b_i + 2];
      }

      dv[3 * i] = b_iv1[i];
    }

    dv[1] = 0.0;
    dv[4] = b_r_DFk;
    dv[7] = -b_r_ABV;
    dv[2] = 0.0;
    dv[5] = b_r_ABV;
    dv[8] = b_r_DFk;

    //  X, Y, steering, toe, inclination angle
    delta += sus_delta_0;
    for (int i = 0; i < 3; i++) {
      d = b_e_ABV2[i];
      d1 = b_e_ABV2[i + 3];
      d2 = b_e_ABV2[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        A_VW[i + 3 * b_i] = (d * dv[3 * b_i] + d1 * dv[3 * b_i + 1]) + d2 * dv[3
          * b_i + 2];
      }

      b_sus_r_VCk[i] = sus_r_VWk[i] - sus_r_VCk[i];
    }

    //  Partial velocities
    //  Partial derivatives dpsi/dphi, dalpha/dphi, dbeta/dphi
    d = b_sus_r_VCk[0];
    d1 = b_sus_r_VCk[1];
    d2 = b_sus_r_VCk[2];
    r = _mm_loadu_pd(&A_VW[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&A_VW[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&A_VW[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&e_ABV2[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(0.0));
    r2 = _mm_loadu_pd(&e_ABV2[3]);
    r1 = _mm_add_pd(r2, r1);
    _mm_storeu_pd(&r_DEV[0], r1);
    _mm_storeu_pd(&r_CFk[0], r);
    r1 = _mm_loadu_pd(&r_VCV[0]);
    r = _mm_add_pd(r1, r);
    _mm_storeu_pd(&r_VWV[0], r);
    d3 = (A_VW[2] * d + A_VW[5] * d1) + A_VW[8] * d2;
    r_DEV[2] = e_ABV2[5] + e_ABV2[8] * 0.0;
    r_CFk[2] = d3;
    r_VWV[2] = r_VCV[2] + d3;
    r_DFk[0] = e_ABV[1] * r_ACV[2] - r_ACV[1] * e_ABV[2];
    r_DFk[1] = r_ACV[0] * e_ABV[2] - e_ABV[0] * r_ACV[2];
    r_DFk[2] = e_ABV[0] * r_ACV[1] - r_ACV[0] * e_ABV[1];
    A_phi_tmp[0] = e_DEV[1] * r_DFV[2] - r_DFV[1] * e_DEV[2];
    A_phi_tmp[1] = r_DFV[0] * e_DEV[2] - e_DEV[0] * r_DFV[2];
    A_phi_tmp[2] = e_DEV[0] * r_DFV[1] - r_DFV[0] * e_DEV[1];
    A_phi_tmp[3] = -(0.0 * domdu[2] - domdu[1] * 0.0);
    A_phi_tmp[4] = -(domdu[0] * 0.0 - domdu[2]);
    A_phi_tmp[5] = -(domdu[1] - domdu[0] * 0.0);
    A_phi_tmp[6] = -(r_DEV[1] * domdu[2] - domdu[1] * r_DEV[2]);
    A_phi_tmp[7] = -(domdu[0] * r_DEV[2] - r_DEV[0] * domdu[2]);
    A_phi_tmp[8] = -(r_DEV[0] * domdu[1] - domdu[0] * r_DEV[1]);
    coder::mldivide(A_phi_tmp, r_DFk, domdphi);

    //  Partial derivatives ddelta/dphi, ddelta/du
    d = coder::b_norm(domdu);
    b_r_CFk = domdphi[1];
    c_r_ABV = domdphi[2];
    d1 = r_CQk[0];
    d2 = r_CQk[1];
    d3 = r_CQk[2];
    for (int i = 0; i < 3; i++) {
      domdu[i] /= d;
      psi = (A_VW[i] * d1 + A_VW[i + 3] * d2) + A_VW[i + 6] * d3;
      r_ABV[i] = psi;
      r_VCV[i] = (r_VCV[i] + psi) - r_VRV[i];
      domdphi[i] = static_cast<double>(a[i]) * b_r_CFk + r_DEV[i] * c_r_ABV;
    }

    psi = (r_VCV[0] * (domdu[1] * r_ABV[2] - r_ABV[1] * domdu[2]) + r_VCV[1] *
           (r_ABV[0] * domdu[2] - domdu[0] * r_ABV[2])) + r_VCV[2] * (domdu[0] *
      r_ABV[1] - r_ABV[0] * domdu[1]);
    b = ((-r_VCV[0] * (r_DFk[0] + (domdphi[1] * r_ABV[2] - r_ABV[1] * domdphi[2]))
          + -r_VCV[1] * (r_DFk[1] + (r_ABV[0] * domdphi[2] - domdphi[0] * r_ABV
            [2]))) + -r_VCV[2] * (r_DFk[2] + (domdphi[0] * r_ABV[1] - r_ABV[0] *
           domdphi[1]))) / psi;
    psi = ((r_VCV[0] * 0.0 + r_VCV[1]) + r_VCV[2] * 0.0) / psi;

    //  Partial angular and partial velocities of upright
    r = _mm_loadu_pd(&domdu[0]);
    r1 = _mm_loadu_pd(&domdphi[0]);
    _mm_storeu_pd(&domdphi[0], _mm_add_pd(r1, _mm_mul_pd(r, _mm_set1_pd(b))));
    _mm_storeu_pd(&domdu[0], _mm_mul_pd(r, _mm_set1_pd(psi)));
    domdphi[2] += domdu[2] * b;
    d = domdu[2] * psi;
    domdu[2] = d;
    dvdphi[0] = r_DFk[0] + (domdphi[1] * r_CFk[2] - r_CFk[1] * domdphi[2]);
    dvdphi[1] = r_DFk[1] + (r_CFk[0] * domdphi[2] - domdphi[0] * r_CFk[2]);
    dvdphi[2] = r_DFk[2] + (domdphi[0] * r_CFk[1] - r_CFk[0] * domdphi[1]);
    dvdu[0] = domdu[1] * r_CFk[2] - r_CFk[1] * d;
    dvdu[1] = r_CFk[0] * d - domdu[0] * r_CFk[2];
    dvdu[2] = domdu[0] * r_CFk[1] - r_CFk[0] * domdu[1];
    return delta;
  }

  static double f_suspensionDWB(double sus_delta_0, double sus_gamma_0, const
    double sus_r_VWk[3], const double sus_r_VAk[3], const double sus_r_VBk[3],
    const double sus_r_VCk[3], const double sus_r_VDk[3], const double
    sus_r_VEk[3], const double sus_r_VFk[3], const double sus_r_VRk[3], const
    double sus_r_VQk[3], double phi, double A_VW[9], double r_VWV[3], double
    dvdphi[3], double domdphi[3])
  {
    static const signed char a[3] = { 1, 0, 0 };

    static const signed char b_iv[3] = { 0, 0, 1 };

    static const signed char b_iv1[3] = { 1, 0, 0 };

    __m128d r;
    __m128d r1;
    __m128d r2;
    __m128d r3;
    double A_beta[9];
    double A_phi_tmp[9];
    double b_e_ABV2[9];
    double dv[9];
    double e_ABV2[9];
    double e_CFW2[9];
    double b_sus_r_VCk[3];
    double e_ABV[3];
    double e_CFW[3];
    double e_DEV[3];
    double r_ABV[3];
    double r_ACV[3];
    double r_CFV[3];
    double r_CFk[3];
    double r_CQk[3];
    double r_DEV[3];
    double r_DFV[3];
    double r_DFk[3];
    double r_VCV[3];
    double b;
    double b_r_ABV;
    double b_r_CFk;
    double c_r_ABV;
    double d;
    double d1;
    double d2;
    double d3;
    double d_r_ABV;
    double delta;
    double psi;
    int b_i;
    int i1;

    //       Double Wishbone Suspension         %%
    //  ----------------------------------------- %
    //  Version: V1.3 - 2024.12.26                %
    //  Compatible ADTM Release: ADTM_1.4         %
    //  Author: Simon Frank simon.sf.frank@tum.de %
    //  Modified by:                              %
    //  Info: Suspension points:                  %
    //        A: lower arm @ chassis rear         %
    //        B: lower arm @ chassis front        %
    //        C: lower arm @ upright              %
    //        D: upper arm @ chassis rear         %
    //        E: upper arm @ chassis front        %
    //        F: upper arm @ upright              %
    //        R: tie rod @ steering rack          %
    //        Q: tie rod @ upright                %
    //  Input:                                    %
    //    phi     -   rotation angle of lower     %
    //                wishbone                    %
    //    u       -   steering rack travel        %
    //    sus     -   struct containing           %
    //                suspension parameters       %
    //    t       -   current simulation time     %
    //  Output:                                   %
    //    A_VW    -   rotation matrix from wheel  %
    //                to vehicle frame            %
    //    r_VWV   -   distance vector from CoG    %
    //                to wheel center in vehicle  %
    //                frame                       %
    //    dvdphi  -   dv_U / dphi_dot             %
    //    domdphi -   domega_U / dphi_dot         %
    //    dvdu    -   dv_U / du_dot               %
    //    domdu   -   domega_U / du_dot           %
    //  ----------------------------------------- %
    //  Input check
    //  Upright position and orientation
    //  Rotation of lower wishbone with angle phi
    _mm_storeu_pd(&r_ABV[0], _mm_sub_pd(_mm_loadu_pd(&sus_r_VBk[0]),
      _mm_loadu_pd(&sus_r_VAk[0])));
    r_ABV[2] = sus_r_VBk[2] - sus_r_VAk[2];
    d = coder::b_norm(r_ABV);
    r = _mm_loadu_pd(&r_ABV[0]);
    _mm_storeu_pd(&e_ABV[0], _mm_div_pd(r, _mm_set1_pd(d)));
    e_ABV[2] = r_ABV[2] / d;

    //  Prepare skew symmetric cross product matrix from vector r
    coder::eye(A_phi_tmp);
    b = cos(phi);
    psi = sin(phi);

    //  Rotation of upper wishbone with angle psi
    r = _mm_loadu_pd(&e_ABV[0]);
    _mm_storeu_pd(&e_ABV2[0], _mm_mul_pd(r, _mm_set1_pd(e_ABV[0])));
    e_ABV2[2] = e_ABV[0] * e_ABV[2];
    r_CFk[0] = sus_r_VFk[0] - sus_r_VCk[0];
    r_DFk[0] = sus_r_VFk[0] - sus_r_VDk[0];
    r = _mm_loadu_pd(&e_ABV[0]);
    _mm_storeu_pd(&e_ABV2[3], _mm_mul_pd(r, _mm_set1_pd(e_ABV[1])));
    e_ABV2[5] = e_ABV[1] * e_ABV[2];
    r_CFk[1] = sus_r_VFk[1] - sus_r_VCk[1];
    r_DFk[1] = sus_r_VFk[1] - sus_r_VDk[1];
    r = _mm_loadu_pd(&e_ABV[0]);
    _mm_storeu_pd(&e_ABV2[6], _mm_mul_pd(r, _mm_set1_pd(e_ABV[2])));
    e_ABV2[8] = e_ABV[2] * e_ABV[2];
    r_CFk[2] = sus_r_VFk[2] - sus_r_VCk[2];
    r_DFk[2] = sus_r_VFk[2] - sus_r_VDk[2];
    dv[0] = 0.0 * psi;
    dv[3] = -e_ABV[2] * psi;
    dv[6] = e_ABV[1] * psi;
    dv[1] = e_ABV[2] * psi;
    dv[4] = 0.0 * psi;
    dv[7] = -e_ABV[0] * psi;
    dv[2] = -e_ABV[1] * psi;
    dv[5] = e_ABV[0] * psi;
    dv[8] = 0.0 * psi;
    r = _mm_loadu_pd(&A_phi_tmp[0]);
    r1 = _mm_loadu_pd(&e_ABV2[0]);
    r2 = _mm_loadu_pd(&dv[0]);
    r3 = _mm_set1_pd(b);
    _mm_storeu_pd(&e_ABV2[0], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(_mm_sub_pd(r,
      r1), r3)), r2));
    r = _mm_loadu_pd(&A_phi_tmp[2]);
    r1 = _mm_loadu_pd(&e_ABV2[2]);
    r2 = _mm_loadu_pd(&dv[2]);
    _mm_storeu_pd(&e_ABV2[2], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(_mm_sub_pd(r,
      r1), r3)), r2));
    r = _mm_loadu_pd(&A_phi_tmp[4]);
    r1 = _mm_loadu_pd(&e_ABV2[4]);
    r2 = _mm_loadu_pd(&dv[4]);
    _mm_storeu_pd(&e_ABV2[4], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(_mm_sub_pd(r,
      r1), r3)), r2));
    r = _mm_loadu_pd(&A_phi_tmp[6]);
    r1 = _mm_loadu_pd(&e_ABV2[6]);
    r2 = _mm_loadu_pd(&dv[6]);
    _mm_storeu_pd(&e_ABV2[6], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(_mm_sub_pd(r,
      r1), r3)), r2));
    e_ABV2[8] = (e_ABV2[8] + (A_phi_tmp[8] - e_ABV2[8]) * b) + dv[8];
    _mm_storeu_pd(&b_sus_r_VCk[0], _mm_sub_pd(_mm_loadu_pd(&sus_r_VCk[0]),
      _mm_loadu_pd(&sus_r_VAk[0])));
    d = b_sus_r_VCk[0];
    d1 = b_sus_r_VCk[1];
    d2 = sus_r_VCk[2] - sus_r_VAk[2];
    r = _mm_loadu_pd(&e_ABV2[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&e_ABV2[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&e_ABV2[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&r_ACV[0], r);
    r1 = _mm_loadu_pd(&sus_r_VAk[0]);
    r = _mm_add_pd(r1, r);
    _mm_storeu_pd(&r_VCV[0], r);
    r1 = _mm_loadu_pd(&sus_r_VDk[0]);
    r = _mm_sub_pd(r1, r);
    _mm_storeu_pd(&r_ABV[0], r);
    r = _mm_loadu_pd(&sus_r_VEk[0]);
    r = _mm_sub_pd(r, r1);
    _mm_storeu_pd(&r_DEV[0], r);
    d3 = (e_ABV2[2] * d + e_ABV2[5] * d1) + e_ABV2[8] * d2;
    r_ACV[2] = d3;
    d3 += sus_r_VAk[2];
    r_VCV[2] = d3;
    psi = sus_r_VDk[2];
    r_ABV[2] = psi - d3;
    r_DEV[2] = sus_r_VEk[2] - psi;
    d = coder::b_norm(r_DEV);
    r = _mm_loadu_pd(&r_DEV[0]);
    _mm_storeu_pd(&e_DEV[0], _mm_div_pd(r, _mm_set1_pd(d)));
    e_DEV[2] = r_DEV[2] / d;
    r = _mm_loadu_pd(&e_DEV[0]);
    _mm_storeu_pd(&e_ABV2[0], _mm_mul_pd(r, _mm_set1_pd(e_DEV[0])));
    e_ABV2[2] = e_DEV[0] * e_DEV[2];
    r = _mm_loadu_pd(&e_DEV[0]);
    _mm_storeu_pd(&e_ABV2[3], _mm_mul_pd(r, _mm_set1_pd(e_DEV[1])));
    e_ABV2[5] = e_DEV[1] * e_DEV[2];
    r = _mm_loadu_pd(&e_DEV[0]);
    _mm_storeu_pd(&e_ABV2[6], _mm_mul_pd(r, _mm_set1_pd(e_DEV[2])));
    e_ABV2[8] = e_DEV[2] * e_DEV[2];

    //  Prepare skew symmetric cross product matrix from vector r
    r = _mm_loadu_pd(&A_phi_tmp[0]);
    r1 = _mm_loadu_pd(&e_ABV2[0]);
    _mm_storeu_pd(&A_beta[0], _mm_sub_pd(r, r1));
    r = _mm_loadu_pd(&A_phi_tmp[2]);
    r1 = _mm_loadu_pd(&e_ABV2[2]);
    _mm_storeu_pd(&A_beta[2], _mm_sub_pd(r, r1));
    r = _mm_loadu_pd(&A_phi_tmp[4]);
    r1 = _mm_loadu_pd(&e_ABV2[4]);
    _mm_storeu_pd(&A_beta[4], _mm_sub_pd(r, r1));
    r = _mm_loadu_pd(&A_phi_tmp[6]);
    r1 = _mm_loadu_pd(&e_ABV2[6]);
    _mm_storeu_pd(&A_beta[6], _mm_sub_pd(r, r1));
    A_beta[8] = A_phi_tmp[8] - e_ABV2[8];
    b_r_ABV = 0.0;
    b_sus_r_VCk[0] = e_DEV[1] * r_DFk[2] - r_DFk[1] * e_DEV[2];
    b_sus_r_VCk[1] = r_DFk[0] * e_DEV[2] - e_DEV[0] * r_DFk[2];
    b_sus_r_VCk[2] = e_DEV[0] * r_DFk[1] - r_DFk[0] * e_DEV[1];
    c_r_ABV = 0.0;
    for (int i = 0; i < 3; i++) {
      b_r_ABV += ((r_ABV[0] * A_beta[3 * i] + r_ABV[1] * A_beta[3 * i + 1]) +
                  r_ABV[2] * A_beta[3 * i + 2]) * r_DFk[i];
      c_r_ABV += r_ABV[i] * b_sus_r_VCk[i];
      r_DEV[i] = -r_ABV[i];
    }

    d_r_ABV = 0.0;
    b = 0.0;
    psi = 0.0;
    b_r_CFk = 0.0;
    d = r_DEV[0];
    d1 = r_DEV[1];
    d2 = r_DEV[2];
    for (int i = 0; i < 3; i++) {
      d3 = r_DFk[i];
      d_r_ABV += ((d * e_ABV2[3 * i] + d1 * e_ABV2[3 * i + 1]) + d2 * e_ABV2[3 *
                  i + 2]) * d3;
      b += d3 * d3;
      d3 = r_ABV[i];
      psi += d3 * d3;
      d3 = r_CFk[i];
      b_r_CFk += d3 * d3;
    }

    psi = d_r_ABV - 0.5 * ((b + psi) - b_r_CFk);

    //  Additional functions
    //  Solve "a*cos(ang) + b*sin(ang) = c" for angle ang
    if (c_r_ABV < 0.0) {
      psi = asin(-psi / sqrt(b_r_ABV * b_r_ABV + c_r_ABV * c_r_ABV)) - coder::
        b_atan2(-b_r_ABV, -c_r_ABV);
    } else {
      psi = asin(psi / sqrt(b_r_ABV * b_r_ABV + c_r_ABV * c_r_ABV)) - coder::
        b_atan2(b_r_ABV, c_r_ABV);
    }

    b = cos(psi);
    psi = sin(psi);

    //  Rotation of upright around X & Y axis
    dv[0] = 0.0 * psi;
    dv[3] = -e_DEV[2] * psi;
    dv[6] = e_DEV[1] * psi;
    dv[1] = e_DEV[2] * psi;
    dv[4] = 0.0 * psi;
    dv[7] = -e_DEV[0] * psi;
    dv[2] = -e_DEV[1] * psi;
    dv[5] = e_DEV[0] * psi;
    dv[8] = 0.0 * psi;
    r = _mm_loadu_pd(&A_beta[0]);
    r1 = _mm_loadu_pd(&e_ABV2[0]);
    r2 = _mm_loadu_pd(&dv[0]);
    r3 = _mm_set1_pd(b);
    _mm_storeu_pd(&e_ABV2[0], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(r, r3)), r2));
    r = _mm_loadu_pd(&A_beta[2]);
    r1 = _mm_loadu_pd(&e_ABV2[2]);
    r2 = _mm_loadu_pd(&dv[2]);
    _mm_storeu_pd(&e_ABV2[2], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(r, r3)), r2));
    r = _mm_loadu_pd(&A_beta[4]);
    r1 = _mm_loadu_pd(&e_ABV2[4]);
    r2 = _mm_loadu_pd(&dv[4]);
    _mm_storeu_pd(&e_ABV2[4], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(r, r3)), r2));
    r = _mm_loadu_pd(&A_beta[6]);
    r1 = _mm_loadu_pd(&e_ABV2[6]);
    r2 = _mm_loadu_pd(&dv[6]);
    _mm_storeu_pd(&e_ABV2[6], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(r, r3)), r2));
    e_ABV2[8] = (e_ABV2[8] + A_beta[8] * b) + dv[8];
    d = r_DFk[0];
    d1 = r_DFk[1];
    d2 = r_DFk[2];
    r = _mm_loadu_pd(&e_ABV2[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&e_ABV2[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&e_ABV2[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&r_DFV[0], r);
    r1 = _mm_loadu_pd(&sus_r_VDk[0]);
    r = _mm_add_pd(r1, r);
    r1 = _mm_loadu_pd(&r_VCV[0]);
    r = _mm_sub_pd(r, r1);
    _mm_storeu_pd(&r_CFV[0], r);
    d3 = (e_ABV2[2] * d + e_ABV2[5] * d1) + e_ABV2[8] * d2;
    r_DFV[2] = d3;
    r_CFV[2] = (sus_r_VDk[2] + d3) - r_VCV[2];

    //  Angle alpha around X axis
    //  Additional functions
    //  Solve "a*cos(ang) + b*sin(ang) = c" for angle ang
    if (r_CFV[2] < 0.0) {
      psi = asin(-r_CFk[1] / sqrt(r_CFV[1] * r_CFV[1] + r_CFV[2] * r_CFV[2])) -
        coder::b_atan2(-r_CFV[1], -r_CFV[2]);
    } else {
      psi = asin(r_CFk[1] / sqrt(r_CFV[1] * r_CFV[1] + r_CFV[2] * r_CFV[2])) -
        coder::b_atan2(r_CFV[1], r_CFV[2]);
    }

    //  Rotation matrix for angle ang about X axis
    c_r_ABV = sin(psi);
    b_r_ABV = cos(psi);
    e_ABV2[1] = 0.0;
    e_ABV2[4] = b_r_ABV;
    e_ABV2[7] = -c_r_ABV;
    e_ABV2[2] = 0.0;
    e_ABV2[5] = c_r_ABV;
    e_ABV2[8] = b_r_ABV;

    //  Angle beta around Y axis
    //  Additional functions
    //  Solve "a*cos(ang) + b*sin(ang) = c" for angle ang
    psi = asin(r_CFV[0] / 0.18793852415) - coder::b_atan2(0.0, 0.18793852415);

    //  Rotation matrix for angle ang about Y axis
    b_r_CFk = sin(psi);
    psi = cos(psi);
    A_beta[0] = psi;
    A_beta[3] = 0.0;
    A_beta[6] = b_r_CFk;
    A_beta[2] = -b_r_CFk;
    A_beta[5] = 0.0;
    A_beta[8] = psi;

    //  Steering angle delta around kingpin axis
    e_ABV2[0] = 1.0;
    A_beta[1] = 0.0;
    r_ABV[0] = r_VCV[0] - sus_r_VRk[0];
    e_ABV2[3] = 0.0;
    A_beta[4] = 1.0;
    r_ABV[1] = r_VCV[1] - sus_r_VRk[1];
    e_ABV2[6] = 0.0;
    A_beta[7] = 0.0;
    r_ABV[2] = r_VCV[2] - sus_r_VRk[2];
    d = r_ABV[0];
    d1 = r_ABV[1];
    d2 = r_ABV[2];
    for (int i = 0; i < 3; i++) {
      r_DEV[i] = (d * e_ABV2[3 * i] + d1 * e_ABV2[3 * i + 1]) + d2 * e_ABV2[3 *
        i + 2];
    }

    d = coder::b_norm(r_CFk);
    d1 = r_DEV[0];
    d2 = r_DEV[1];
    d3 = r_DEV[2];
    for (int i = 0; i < 3; i++) {
      r_DEV[i] = (d1 * A_beta[3 * i] + d2 * A_beta[3 * i + 1]) + d3 * A_beta[3 *
        i + 2];
      psi = sus_r_VQk[i];
      r_DFk[i] = psi - sus_r_VRk[i];
      r_CQk[i] = psi - sus_r_VCk[i];
      e_CFW[i] = r_CFk[i] / d;
    }

    r = _mm_loadu_pd(&e_CFW[0]);
    _mm_storeu_pd(&e_CFW2[0], _mm_mul_pd(r, _mm_set1_pd(e_CFW[0])));
    e_CFW2[2] = e_CFW[0] * e_CFW[2];
    r = _mm_loadu_pd(&e_CFW[0]);
    _mm_storeu_pd(&e_CFW2[3], _mm_mul_pd(r, _mm_set1_pd(e_CFW[1])));
    e_CFW2[5] = e_CFW[1] * e_CFW[2];
    r = _mm_loadu_pd(&e_CFW[0]);
    _mm_storeu_pd(&e_CFW2[6], _mm_mul_pd(r, _mm_set1_pd(e_CFW[2])));
    e_CFW2[8] = e_CFW[2] * e_CFW[2];

    //  Prepare skew symmetric cross product matrix from vector r
    r = _mm_loadu_pd(&A_phi_tmp[0]);
    r1 = _mm_loadu_pd(&e_CFW2[0]);
    _mm_storeu_pd(&A_phi_tmp[0], _mm_sub_pd(r, r1));
    r = _mm_loadu_pd(&A_phi_tmp[2]);
    r1 = _mm_loadu_pd(&e_CFW2[2]);
    _mm_storeu_pd(&A_phi_tmp[2], _mm_sub_pd(r, r1));
    r = _mm_loadu_pd(&A_phi_tmp[4]);
    r1 = _mm_loadu_pd(&e_CFW2[4]);
    _mm_storeu_pd(&A_phi_tmp[4], _mm_sub_pd(r, r1));
    r = _mm_loadu_pd(&A_phi_tmp[6]);
    r1 = _mm_loadu_pd(&e_CFW2[6]);
    _mm_storeu_pd(&A_phi_tmp[6], _mm_sub_pd(r, r1));
    A_phi_tmp[8] -= e_CFW2[8];
    b_r_ABV = 0.0;
    b_sus_r_VCk[0] = e_CFW[1] * r_CQk[2] - r_CQk[1] * e_CFW[2];
    b_sus_r_VCk[1] = r_CQk[0] * e_CFW[2] - e_CFW[0] * r_CQk[2];
    b_sus_r_VCk[2] = e_CFW[0] * r_CQk[1] - r_CQk[0] * e_CFW[1];
    b_r_CFk = 0.0;
    c_r_ABV = 0.0;
    d_r_ABV = 0.0;
    psi = 0.0;
    b = 0.0;
    for (int i = 0; i < 3; i++) {
      b_i = 3 * i + 1;
      i1 = 3 * i + 2;
      d = r_CQk[i];
      b_r_ABV += ((r_DEV[0] * A_phi_tmp[3 * i] + r_DEV[1] * A_phi_tmp[b_i]) +
                  r_DEV[2] * A_phi_tmp[i1]) * d;
      b_r_CFk += r_DEV[i] * b_sus_r_VCk[i];
      c_r_ABV += ((r_DEV[0] * e_CFW2[3 * i] + r_DEV[1] * e_CFW2[b_i]) + r_DEV[2]
                  * e_CFW2[i1]) * d;
      d1 = r_ABV[i];
      d_r_ABV += d1 * d1;
      psi += d * d;
      d = r_DFk[i];
      b += d * d;
    }

    psi = -(c_r_ABV + 0.5 * ((d_r_ABV + psi) - b));

    //  Additional functions
    //  Solve "a*cos(ang) + b*sin(ang) = c" for angle ang
    if (b_r_CFk < 0.0) {
      delta = asin(-psi / sqrt(b_r_ABV * b_r_ABV + b_r_CFk * b_r_CFk)) - coder::
        b_atan2(-b_r_ABV, -b_r_CFk);
    } else {
      delta = asin(psi / sqrt(b_r_ABV * b_r_ABV + b_r_CFk * b_r_CFk)) - coder::
        b_atan2(b_r_ABV, b_r_CFk);
    }

    b = cos(delta);
    psi = sin(delta);

    //  Upright orientation and position
    //  Rotation matrix for angle ang about Z axis
    b_r_CFk = sin(sus_delta_0);
    d_r_ABV = cos(sus_delta_0);

    //  Rotation matrix for angle ang about X axis
    c_r_ABV = sin(sus_gamma_0);
    b_r_ABV = cos(sus_gamma_0);
    dv[0] = 0.0 * psi;
    dv[3] = -e_CFW[2] * psi;
    dv[6] = e_CFW[1] * psi;
    dv[1] = e_CFW[2] * psi;
    dv[4] = 0.0 * psi;
    dv[7] = -e_CFW[0] * psi;
    dv[2] = -e_CFW[1] * psi;
    dv[5] = e_CFW[0] * psi;
    dv[8] = 0.0 * psi;
    for (int i = 0; i < 3; i++) {
      b_i = static_cast<int>(e_ABV2[i]);
      d = e_ABV2[i + 3];
      d1 = e_ABV2[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        b_e_ABV2[i + 3 * i1] = (static_cast<double>(b_i) * A_beta[3 * i1] + d *
          A_beta[3 * i1 + 1]) + d1 * A_beta[3 * i1 + 2];
      }
    }

    r = _mm_loadu_pd(&A_phi_tmp[0]);
    r1 = _mm_loadu_pd(&e_CFW2[0]);
    r2 = _mm_loadu_pd(&dv[0]);
    r3 = _mm_set1_pd(b);
    _mm_storeu_pd(&e_CFW2[0], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(r, r3)), r2));
    r = _mm_loadu_pd(&A_phi_tmp[2]);
    r1 = _mm_loadu_pd(&e_CFW2[2]);
    r2 = _mm_loadu_pd(&dv[2]);
    _mm_storeu_pd(&e_CFW2[2], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(r, r3)), r2));
    r = _mm_loadu_pd(&A_phi_tmp[4]);
    r1 = _mm_loadu_pd(&e_CFW2[4]);
    r2 = _mm_loadu_pd(&dv[4]);
    _mm_storeu_pd(&e_CFW2[4], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(r, r3)), r2));
    r = _mm_loadu_pd(&A_phi_tmp[6]);
    r1 = _mm_loadu_pd(&e_CFW2[6]);
    r2 = _mm_loadu_pd(&dv[6]);
    _mm_storeu_pd(&e_CFW2[6], _mm_add_pd(_mm_add_pd(r1, _mm_mul_pd(r, r3)), r2));
    e_CFW2[8] = (e_CFW2[8] + A_phi_tmp[8] * b) + dv[8];
    A_phi_tmp[0] = d_r_ABV;
    A_phi_tmp[3] = -b_r_CFk;
    A_phi_tmp[6] = 0.0;
    A_phi_tmp[1] = b_r_CFk;
    A_phi_tmp[4] = d_r_ABV;
    A_phi_tmp[7] = 0.0;
    for (int i = 0; i < 3; i++) {
      d = b_e_ABV2[i];
      d1 = b_e_ABV2[i + 3];
      d2 = b_e_ABV2[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        A_beta[i + 3 * b_i] = (d * e_CFW2[3 * b_i] + d1 * e_CFW2[3 * b_i + 1]) +
          d2 * e_CFW2[3 * b_i + 2];
      }

      A_phi_tmp[3 * i + 2] = b_iv[i];
    }

    for (int i = 0; i < 3; i++) {
      d = A_beta[i];
      d1 = A_beta[i + 3];
      d2 = A_beta[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        b_e_ABV2[i + 3 * b_i] = (d * A_phi_tmp[3 * b_i] + d1 * A_phi_tmp[3 * b_i
          + 1]) + d2 * A_phi_tmp[3 * b_i + 2];
      }

      dv[3 * i] = b_iv1[i];
    }

    dv[1] = 0.0;
    dv[4] = b_r_ABV;
    dv[7] = -c_r_ABV;
    dv[2] = 0.0;
    dv[5] = c_r_ABV;
    dv[8] = b_r_ABV;

    //  X, Y, steering, toe, inclination angle
    delta += sus_delta_0;
    for (int i = 0; i < 3; i++) {
      d = b_e_ABV2[i];
      d1 = b_e_ABV2[i + 3];
      d2 = b_e_ABV2[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        A_VW[i + 3 * b_i] = (d * dv[3 * b_i] + d1 * dv[3 * b_i + 1]) + d2 * dv[3
          * b_i + 2];
      }

      b_sus_r_VCk[i] = sus_r_VWk[i] - sus_r_VCk[i];
    }

    //  Partial velocities
    //  Partial derivatives dpsi/dphi, dalpha/dphi, dbeta/dphi
    d = b_sus_r_VCk[0];
    d1 = b_sus_r_VCk[1];
    d2 = b_sus_r_VCk[2];
    r = _mm_loadu_pd(&A_VW[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&A_VW[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&A_VW[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&e_ABV2[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(0.0));
    r2 = _mm_loadu_pd(&e_ABV2[3]);
    r1 = _mm_add_pd(r2, r1);
    _mm_storeu_pd(&r_DFk[0], r1);
    _mm_storeu_pd(&e_CFW[0], r);
    r1 = _mm_loadu_pd(&r_VCV[0]);
    r = _mm_add_pd(r1, r);
    _mm_storeu_pd(&r_VWV[0], r);
    d3 = (A_VW[2] * d + A_VW[5] * d1) + A_VW[8] * d2;
    r_DFk[2] = e_ABV2[5] + e_ABV2[8] * 0.0;
    e_CFW[2] = d3;
    r_VWV[2] = r_VCV[2] + d3;
    r_CFk[0] = e_ABV[1] * r_ACV[2] - r_ACV[1] * e_ABV[2];
    r_CFk[1] = r_ACV[0] * e_ABV[2] - e_ABV[0] * r_ACV[2];
    r_CFk[2] = e_ABV[0] * r_ACV[1] - r_ACV[0] * e_ABV[1];
    A_phi_tmp[0] = e_DEV[1] * r_DFV[2] - r_DFV[1] * e_DEV[2];
    A_phi_tmp[1] = r_DFV[0] * e_DEV[2] - e_DEV[0] * r_DFV[2];
    A_phi_tmp[2] = e_DEV[0] * r_DFV[1] - r_DFV[0] * e_DEV[1];
    A_phi_tmp[3] = -(0.0 * r_CFV[2] - r_CFV[1] * 0.0);
    A_phi_tmp[4] = -(r_CFV[0] * 0.0 - r_CFV[2]);
    A_phi_tmp[5] = -(r_CFV[1] - r_CFV[0] * 0.0);
    A_phi_tmp[6] = -(r_DFk[1] * r_CFV[2] - r_CFV[1] * r_DFk[2]);
    A_phi_tmp[7] = -(r_CFV[0] * r_DFk[2] - r_DFk[0] * r_CFV[2]);
    A_phi_tmp[8] = -(r_DFk[0] * r_CFV[1] - r_CFV[0] * r_DFk[1]);
    coder::mldivide(A_phi_tmp, r_CFk, domdphi);

    //  Partial derivatives ddelta/dphi, ddelta/du
    d = coder::b_norm(r_CFV);
    b_r_CFk = domdphi[1];
    d_r_ABV = domdphi[2];
    d1 = r_CQk[0];
    d2 = r_CQk[1];
    d3 = r_CQk[2];
    for (int i = 0; i < 3; i++) {
      r_DEV[i] = r_CFV[i] / d;
      psi = (A_VW[i] * d1 + A_VW[i + 3] * d2) + A_VW[i + 6] * d3;
      r_ABV[i] = psi;
      r_VCV[i] = (r_VCV[i] + psi) - sus_r_VRk[i];
      domdphi[i] = static_cast<double>(a[i]) * b_r_CFk + r_DFk[i] * d_r_ABV;
    }

    b = ((-r_VCV[0] * (r_CFk[0] + (domdphi[1] * r_ABV[2] - r_ABV[1] * domdphi[2]))
          + -r_VCV[1] * (r_CFk[1] + (r_ABV[0] * domdphi[2] - domdphi[0] * r_ABV
            [2]))) + -r_VCV[2] * (r_CFk[2] + (domdphi[0] * r_ABV[1] - r_ABV[0] *
           domdphi[1]))) / ((r_VCV[0] * (r_DEV[1] * r_ABV[2] - r_ABV[1] * r_DEV
      [2]) + r_VCV[1] * (r_ABV[0] * r_DEV[2] - r_DEV[0] * r_ABV[2])) + r_VCV[2] *
      (r_DEV[0] * r_ABV[1] - r_ABV[0] * r_DEV[1]));

    //  Partial angular and partial velocities of upright
    r = _mm_loadu_pd(&r_DEV[0]);
    r1 = _mm_loadu_pd(&domdphi[0]);
    _mm_storeu_pd(&domdphi[0], _mm_add_pd(r1, _mm_mul_pd(r, _mm_set1_pd(b))));
    domdphi[2] += r_DEV[2] * b;
    dvdphi[0] = r_CFk[0] + (domdphi[1] * e_CFW[2] - e_CFW[1] * domdphi[2]);
    dvdphi[1] = r_CFk[1] + (e_CFW[0] * domdphi[2] - domdphi[0] * e_CFW[2]);
    dvdphi[2] = r_CFk[2] + (domdphi[0] * e_CFW[1] - e_CFW[0] * domdphi[1]);
    return delta;
  }

  static double f_suspensionForces(const double r_VFLV[3], const double r_VFRV[3],
    const double r_VRLV[3], const double r_VRRV[3], const double r_VFLV_dot[3],
    const double r_VFRV_dot[3], const double r_VRLV_dot[3], const double
    r_VRRV_dot[3], double veh_z_cg, double veh_c_RollFA, double veh_c_RollRA,
    double veh_F_SFL0, double veh_F_SFR0, double veh_F_SRL0, double veh_F_SRR0,
    double &z_FR, double &z_RL, double &z_RR, double &z_FL_dot, double &z_FR_dot,
    double &z_RL_dot, double &z_RR_dot, double &F_CrSFL, double &F_CrSFR, double
    &F_CrSRL, double &F_CrSRR, double &F_HvSFA, double &F_HvSRA, double &F_RoSFA,
    double &F_RoSRA, double &F_SFL, double &F_SFR, double &F_SRL, double &F_SRR)
  {
    static const double b_dv[5] = { -246.10336341263329, -164.06890894175552,
      0.0, 164.06890894175552, 246.10336341263329 };

    static const double dv1[5] = { -0.11850000000000001, -0.059250000000000004,
      0.0, 0.059250000000000004, 0.11850000000000001 };

    static const double dv2[5] = { -253.1645569620253, -168.77637130801688, 0.0,
      168.77637130801688, 253.1645569620253 };

    double dv[5];
    double z_FL;

    //    Vehicle Suspension Forces Model       %%
    //  ----------------------------------------- %
    //  Version: V1.1 - 2026.10.02                %
    //  Compatible ADTM Release:                  %
    //  Author: Simon Frank                       %
    //  Modified by: Tim Braungardt               %
    //  ----------------------------------------- %
    //  passive
    z_FL = r_VFLV[2] + veh_z_cg;
    z_FR = r_VFRV[2] + veh_z_cg;
    z_RL = r_VRLV[2] + veh_z_cg;
    z_RR = r_VRRV[2] + veh_z_cg;
    z_FL_dot = r_VFLV_dot[2];
    z_FR_dot = r_VFRV_dot[2];
    z_RL_dot = r_VRLV_dot[2];
    z_RR_dot = r_VRRV_dot[2];

    //  Suspension corner spring & damper forces
    for (int i = 0; i < 5; i++) {
      dv[i] = 0.060950000000000004 * static_cast<double>(i) -
        0.12190000000000001;
    }

    F_CrSFL = z_FL * 106328.49718128536 + b_f_LUT1D(dv, b_dv, r_VFLV_dot[2]);
    for (int i = 0; i < 5; i++) {
      dv[i] = 0.060950000000000004 * static_cast<double>(i) -
        0.12190000000000001;
    }

    F_CrSFR = z_FR * 106328.49718128536 + b_f_LUT1D(dv, b_dv, r_VFRV_dot[2]);
    F_CrSRL = z_RL * 62668.019726183476 + b_f_LUT1D(dv1, dv2, r_VRLV_dot[2]);
    F_CrSRR = z_RR * 62668.019726183476 + b_f_LUT1D(dv1, dv2, r_VRRV_dot[2]);

    //  Suspension heave spring & damper forces
    F_HvSFA = 0.5 * (z_FL + z_FR) * 0.0;
    F_HvSRA = 0.5 * (z_RL + z_RR) * 0.0;

    //  Suspension roll spring & damper forces
    F_RoSFA = (z_FL - z_FR) * veh_c_RollFA;
    F_RoSRA = (z_RL - z_RR) * veh_c_RollRA;

    //  Pretension + spring & damper forces
    F_SFL = ((veh_F_SFL0 + F_CrSFL) + F_RoSFA) + 0.5 * F_HvSFA;
    F_SFR = ((veh_F_SFR0 + F_CrSFR) - F_RoSFA) + 0.5 * F_HvSFA;
    F_SRL = ((veh_F_SRL0 + F_CrSRL) + F_RoSRA) + 0.5 * F_HvSRA;
    F_SRR = ((veh_F_SRR0 + F_CrSRR) - F_RoSRA) + 0.5 * F_HvSRA;
    return z_FL;
  }

  static double f_tireTMeasy(const double v_0QQ[3], const double omega_0WW[3],
    double r_st, double dz, double w, double b_gamma, double mu_n, const double
    x_T[3], double F_T[3], double T_T[3], double s[4], double x_T_dot[3], double
    &F_yc, double &dF_x0, double &F_xM, double &dF_y0, double &F_yM)
  {
    double F_z;
    double F_zUnlim;
    double L;
    double R_B;
    double omega_c;
    double omega_n;
    double r_0;
    double r_dyn;
    double v_T;
    double y_tmp;

    //           TMeasy Tire Model              %%
    //  ----------------------------------------- %
    //  Version: V2.5 - 2025.06.09                %
    //  Compatible ADTM Release: ADTM_1.4         %
    //  Author: Simon Frank simon.sf.frank@tum.de %
    //  Modified by:                              %
    //  Input:                                    %
    //    tir     -   struct containing tire      %
    //                parameters for one tire     %
    //    t       -   current simulation time     %
    //    v_0QQ   -   velocity vector between     %
    //                origin and contact point Q  %
    //                in Q frame                  %
    //    omega_0WW - angular velocity vector     %
    //                of wheel in relation to     %
    //                origin in wheel frame       %
    //    r_st    -   static tire radius          %
    //    dz      -   vertical tire deflection    %
    //    w       -   width of tire contact patch %
    //    gamma   -   inclination angle           %
    //    mu_n    -   normalized coefficient of   %
    //                friction                    %
    //    x_T     -   tire deflection states      %
    //                x_e, y_e, psi_e             %
    //    dynamic -   Enable/disable first order  %
    //                dynamics (true/false)       %
    //  Output:                                   %
    //    F_T     -   tire force vector           %
    //    T_T     -   tire torque vector          %
    //    s       -   tire slip vector            %
    //    x_T_dot -   tire state derivatives      %
    //                x_e_dot, y_e_dot, psi_e_dot %
    //    r_dyn   -   dynamic tire rolling radius %
    //    F_yc    -   side force due to camber    %
    //    dF_x0   -   longitudinal slip stiffness %
    //    F_xM    -   maximum possible F_x        %
    //    dF_y0   -   lateral slip stiffness      %
    //    F_yM    -   maximum possible F_y        %
    //  ----------------------------------------- %
    //  Preparation
    //  Contact velocities
    //  Angular velocities
    //  Tire deflections
    r_0 = 1.0E-7 * (omega_0WW[1] * omega_0WW[1]) + 0.205;

    //  Unloaded radius
    r_dyn = r_0 * 2.0 / 3.0 + r_st / 3.0;

    //  Dynamic rolling radius
    v_T = omega_0WW[1] * r_dyn;

    //  Transport velocity
    y_tmp = fabs(v_T);

    //  Absolute transport velocity + v_n
    L = 2.0 * sqrt(r_0 * dz);

    //  Contact patch length
    R_B = 0.16666666666666666 * (L + w);

    //  Bore radius
    omega_c = omega_0WW[1] * sin(b_gamma);

    //  Camber rotation
    omega_n = omega_c + omega_0WW[2] * cos(b_gamma);

    //  Bore rotation
    //  Normal force
    F_z = dz * 98000.0 - v_0QQ[2] * 150.0;

    //  Normal force
    F_zUnlim = F_z;

    //  Normal force that stays unlimited
    if (F_z * mu_n <= 0.0) {
      //  No normal force or no friction
      F_T[0] = 0.0;
      T_T[0] = 0.0;
      F_T[1] = 0.0;
      T_T[1] = 0.0;
      F_T[2] = 0.0;
      T_T[2] = 0.0;
      s[0] = 0.0;
      s[1] = 0.0;
      s[2] = 0.0;
      s[3] = 0.0;
      F_yc = 0.0;
      dF_x0 = 0.0;
      F_xM = 0.0;
      dF_y0 = 0.0;
      F_yM = 0.0;
      x_T_dot[0] = -140000.0 * x_T[0] / 400.5;
      x_T_dot[1] = -65000.0 * x_T[1] / 300.5;
      x_T_dot[2] = -400.0 * x_T[2] / 1.5;
    } else {
      double F_M;
      double F_xS;
      double F_yS;
      double a;
      double dF_0;
      double s_N;
      double s_b;
      double s_c;
      double s_x;
      double s_xM;
      double s_xN;
      double s_xS;
      double s_x_hat;
      double s_y;
      double s_yM;
      double s_yM_tmp;
      double s_yN;
      double s_yS;
      double s_y_hat;
      double sinphi;
      double x_e_dot;
      if (F_z > 4000.0) {
        F_z = 4000.0;
      }

      //  Slip
      s_x = (v_T - v_0QQ[0]) / (y_tmp + 0.5);

      //  Longitudinal slip
      s_y = -v_0QQ[1] / (y_tmp + 0.5);

      //  Lateral slip
      s_c = -L / 2.0 * omega_c / (y_tmp + 0.5);

      //  Camber slip
      s_b = -R_B * omega_n / (y_tmp + 0.5);

      //  Bore slip
      s[0] = s_x;
      s[1] = s_y;
      s[2] = s_c;
      s[3] = s_b;

      //  Slip vector
      //  Force parameters
      if (rtIsNaN(s_x)) {
        s_xN = rtNaN;
      } else if (s_x < 0.0) {
        s_xN = -1.0;
      } else {
        s_xN = (s_x > 0.0);
      }

      if (rtIsNaN(v_T)) {
        s_xM = rtNaN;
      } else if (v_T < 0.0) {
        s_xM = -1.0;
      } else {
        s_xM = (v_T > 0.0);
      }

      if (s_xN == s_xM) {
        dF_x0 = F_z / 575.0 * (32496.0 - 2248.0 * F_z / 575.0);

        //  Parameter interpolation functions
        s_xN = F_z / 575.0 - 1.0;
        s_xM = (-0.0079999999999999932 * s_xN + 0.122) * mu_n;
        F_xM = F_z / 575.0 * (1428.0 - 69.0 * F_z / 575.0) * mu_n;

        //  Parameter interpolation functions
        s_xS = (-0.013359999999999983 * s_xN + 0.20373999999999998) * mu_n;
        F_xS = F_z / 575.0 * (1356.6 - 65.549999999999955 * F_z / 575.0) * mu_n;
      } else {
        dF_x0 = F_z / 575.0 * (32496.0 - 2248.0 * F_z / 575.0);

        //  Parameter interpolation functions
        s_xN = F_z / 575.0 - 1.0;
        s_xM = (-0.021999999999999992 * s_xN + 0.152) * mu_n;
        F_xM = F_z / 575.0 * (1499.0 - 67.0 * F_z / 575.0) * mu_n;

        //  Parameter interpolation functions
        s_xS = (-0.036740000000000023 * s_xN + 0.25384) * mu_n;
        F_xS = F_z / 575.0 * (1424.0499999999997 - 63.649999999999864 * F_z /
                              575.0) * mu_n;
      }

      //  Fix extrapolation errors
      if (F_xS > F_xM) {
        s_xS = s_xM;
        F_xS = F_xM;
      }

      if (s_xS < s_xM) {
        s_xS = s_xM;
        F_xS = F_xM;
      }

      dF_y0 = F_z / 575.0 * (29180.0 - 5590.0 * F_z / 575.0);

      //  Parameter interpolation functions
      s_yM_tmp = F_z / 575.0 - 1.0;
      s_yM = (-0.023999999999999994 * s_yM_tmp + 0.186) * mu_n;
      F_yM = F_z / 575.0 * (1629.0 - 137.0 * F_z / 575.0) * mu_n;

      //  Parameter interpolation functions
      s_yS = (-0.040080000000000005 * s_yM_tmp + 0.31062) * mu_n;
      F_yS = F_z / 575.0 * (1547.5499999999997 - 130.14999999999986 * F_z /
                            575.0) * mu_n;

      //  Fix extrapolation errors
      if (F_yS > F_yM) {
        s_yS = s_yM;
        F_yS = F_yM;
      }

      if (s_yS < s_yM) {
        s_yS = s_yM;
        F_yS = F_yM;
      }

      //  Normalized slip
      s_xN = F_xM / dF_x0;
      s_yN = s_xM + s_yM;
      dF_0 = F_yM / dF_y0;
      s_N = s_xN + dF_0;
      s_x_hat = s_xM / s_yN + s_xN / s_N;
      s_y_hat = s_yM / s_yN + dF_0 / s_N;
      s_xN = s_x / s_x_hat;
      s_yN = s_y / s_y_hat;
      x_e_dot = s_xN * s_xN + s_yN * s_yN;
      s_N = sqrt(x_e_dot);
      if (s_N > 0.0) {
        s_xN = fabs(s_xN) / s_N;
        sinphi = fabs(s_yN) / s_N;
      } else {
        s_xN = 0.0;
        sinphi = 1.0;
      }

      //  Horizontal forces
      //  Parameters for combined force calculation
      a = dF_x0 * s_x_hat * s_xN;
      s_yN = dF_y0 * s_y_hat * sinphi;
      dF_0 = sqrt(a * a + s_yN * s_yN);

      //  Slip stiffness
      a = F_xM * s_xN;
      s_yN = F_yM * sinphi;
      F_M = sqrt(a * a + s_yN * s_yN);

      //  Max. force
      a = s_xM / s_x_hat * s_xN;
      s_yN = s_yM / s_y_hat * sinphi;
      s_xM = sqrt(a * a + s_yN * s_yN);

      //  Slip for max. force
      a = F_xS * s_xN;
      s_yN = F_yS * sinphi;
      s_x = sqrt(a * a + s_yN * s_yN);

      //  Sliding force
      a = s_xS / s_x_hat * s_xN;
      s_yN = s_yS / s_y_hat * sinphi;
      s_N = sqrt(a * a + s_yN * s_yN);

      //  Slip for sliding force
      s_xN = 2.0 * F_M / s_xM;
      if (dF_0 < s_xN) {
        dF_0 = s_xN;
      }

      //  Generalized slip
      s_yM = sqrt(x_e_dot + s_b * s_b);

      //  Global tire force derivative
      if (s_yM > s_N) {
        //  Full sliding
        s_xM = s_x / s_yM;

        //  Adhesion
      } else if (s_yM < s_xM) {
        s_yN = s_yM / s_xM;
        s_xM = dF_0 / (s_yN * ((s_yN + dF_0 * s_xM / F_M) - 2.0) + 1.0);

        //  Transition from adhesion to sliding
      } else {
        a = F_M / s_xM;
        a = a * a / (dF_0 * s_xM);
        s_xN = s_N - s_xM;
        s_yN = F_M - s_x;
        dF_0 = s_xM + s_yN / (a * s_xN);

        //  Case: s_M <= s <= s_S
        if (dF_0 < s_N) {
          //  Case: s_M <= s < s_star
          if (s_yM < dF_0) {
            s_yN = s_yM - s_xM;
            s_xN = F_M - a * (s_yN * s_yN);

            //  Case: s_star <= s <= s_S
          } else {
            s_yN = s_N - s_yM;
            s_xN = s_x + a * (dF_0 - s_xM) / (s_N - dF_0) * (s_yN * s_yN);
          }
        } else {
          s_xN = (s_yM - s_xM) / s_xN;
          s_xN = F_M - s_yN * (s_xN * s_xN) * (3.0 - 2.0 * s_xN);
        }

        s_xM = s_xN / s_yM;
      }

      //  Steady state camber force
      F_yc = 0.33333333333333331 * s_xM * s_c * 0.2;

      //  Tire deflection derivatives
      s_yN = (y_tmp + 0.5) * s_x_hat;
      s_xN = (y_tmp + 0.5) * s_y_hat;
      x_e_dot = (-s_yN * 140000.0 * x_T[0] - s_xM * (v_0QQ[0] - v_T)) / (s_yN *
        400.0 + s_xM);
      s_x = (-s_xN * 65000.0 * x_T[1] - s_xM * (v_0QQ[1] + L / 6.0 * omega_c *
              0.2)) / (s_xN * 300.0 + s_xM);

      //  Dynamic tire forces in x and y
      s_yM = 65000.0 * x_T[1] + 300.0 * s_x;

      //  Tire force vector
      F_T[0] = 140000.0 * x_T[0] + 400.0 * x_e_dot;
      F_T[1] = s_yM;
      F_T[2] = F_zUnlim;

      //  Torques
      //  Overturning torque T_x
      //  Rolling resistance torque T_y
      //  Aligning torque T_z
      //  Parameter interpolation functions
      dF_0 = (-0.036000000000000032 * s_yM_tmp + 0.279) * mu_n;

      //  Parameter interpolation functions
      s_N = (-0.045599999999999974 * s_yM_tmp + 0.3534) * mu_n;
      s_yN = fabs(s_y);

      //  Normalized pneumatic trail
      if (s_yN < s_N) {
        s_xN = dF_0 + s_N;
        a = s_yN / (dF_0 * s_N);
        s_xN *= s_xN;
        s_yN = (0.049999999999999989 * s_yM_tmp + 0.3) * (a * a * (-(2.0 * (dF_0
          * dF_0) + s_xN) + s_yN * (2.0 * s_xN / s_N + s_yN * (-(2.0 * dF_0 +
          s_N) / s_N))) + 1.0) * sinphi;
      } else {
        s_yN = 0.0;
      }

      //  Self-aligning torque
      //  Torsional deflection derivative
      s_xN = R_B * R_B * s_xM;
      s_xN = (-(y_tmp + 0.5) * 400.0 * x_T[2] - s_xN * omega_n) / ((y_tmp + 0.5)
        + s_xN);

      //  Dynamic bore torque
      //  T_z
      //  Tire torque vector
      T_T[0] = x_T[1] * F_z;
      T_T[1] = -F_z * 0.02 * r_0 * v_T / (y_tmp + 0.5);
      T_T[2] = -s_yN * L * (s_yM - F_yc) + (400.0 * x_T[2] + s_xN);

      //  Tire deflection derivatives
      x_T_dot[0] = x_e_dot;
      x_T_dot[1] = s_x;
      x_T_dot[2] = s_xN;
    }

    return r_dyn;
  }

  static void f_vehicleModel(double inputs_delta_D, double inputs_p_BFA, double
    inputs_p_BRA, double inputs_throttle, double inputs_mu_N, const double x[42],
    const b_struct_T &veh, const struct_T &road, double x_dot[42])
  {
    static const signed char b[3] = { 0, 1, 0 };

    __m128d r;
    __m128d r1;
    __m128d r2;
    __m128d r3;
    __m128d r4;
    __m128d r5;
    double K[196];
    double f_q_tmp[196];
    double g_q_tmp[196];
    double h_q_tmp[196];
    double i_q_tmp[196];
    double m_y_tmp[196];
    double n_y_tmp[196];
    double b_q_tmp[42];
    double b_y_tmp[42];
    double c_q_tmp[42];
    double c_y_tmp[42];
    double d_q_tmp[42];
    double d_y_tmp[42];
    double domUFLdz[42];
    double domUFRdz[42];
    double domURLdz[42];
    double domURRdz[42];
    double domWFLdz[42];
    double domWFRdz[42];
    double domWRLdz[42];
    double domWRRdz[42];
    double dvFLdz[42];
    double dvFRdz[42];
    double dvRLdz[42];
    double dvRRdz[42];
    double e_q_tmp[42];
    double e_y_tmp[42];
    double f_y_tmp[42];
    double g_y_tmp[42];
    double h_y_tmp[42];
    double i_y_tmp[42];
    double j_y_tmp[42];
    double k_y_tmp[42];
    double q_tmp[42];
    double y_tmp[42];
    double b_dv3[14];
    double dv[14];
    double dv1[14];
    double dv2[14];
    double l_y_tmp[14];
    double A_0V[9];
    double A_VQFR[9];
    double A_VQRL[9];
    double A_VQRR[9];
    double A_VRL[9];
    double A_y_tmp[9];
    double A_z_tmp[9];
    double I_UFLV[9];
    double I_UFLV_tmp[9];
    double I_UFRV[9];
    double I_UFRV_tmp[9];
    double I_URLV[9];
    double I_URLV_tmp[9];
    double I_URRV[9];
    double I_URRV_tmp[9];
    double I_WFLV[9];
    double I_WFRV[9];
    double I_WRLV[9];
    double I_WRRV[9];
    double F_ARAV[3];
    double T_C_tmp[3];
    double T_TFL[3];
    double T_TFR[3];
    double T_TRL[3];
    double T_TRR[3];
    double T_WFL_tmp[3];
    double T_WFR_tmp[3];
    double T_WRL_tmp[3];
    double T_WRR_tmp[3];
    double b_A_0V[3];
    double b_I_UFRV_tmp[3];
    double b_T_C_tmp[3];
    double b_a[3];
    double c_A_0V[3];
    double c_T_C_tmp[3];
    double d_A_0V[3];
    double d_T_C_tmp[3];
    double domdphi_FL[3];
    double domdphi_FR[3];
    double domdphi_RR[3];
    double domdu_FL[3];
    double dvdphi_FL[3];
    double dvdphi_FR[3];
    double dvdphi_RR[3];
    double dvdu_FL[3];
    double e_yFLV[3];
    double e_yFRV[3];
    double e_yRLV[3];
    double e_yRRV[3];
    double g_V[3];
    double omega_0UFLV[3];
    double omega_0UFRV[3];
    double omega_0URLV[3];
    double omega_0URRV[3];
    double omega_0VV[3];
    double omega_0WFLV[3];
    double omega_0WFLV_tmp[3];
    double omega_0WFRV[3];
    double omega_0WFRV_tmp[3];
    double omega_0WRLV[3];
    double omega_0WRLV_tmp[3];
    double omega_0WRRV[3];
    double omega_0WRRV_tmp[3];
    double omega_VUFLV[3];
    double omega_VUFRV[3];
    double omega_VURLV[3];
    double omega_VURRV[3];
    double r_0V0[3];
    double r_MQFL0[3];
    double r_MQFR0[3];
    double r_MQRL0[3];
    double r_MQRR0[3];
    double r_VFLV[3];
    double r_VFLV_dot[3];
    double r_VFRV[3];
    double r_VFRV_dot[3];
    double r_VRLV[3];
    double r_VRLV_dot[3];
    double r_VRRV[3];
    double r_VRRV_dot[3];
    double v_0FAV[3];
    double v_0RAV[3];
    double x_TFL_dot[3];
    double x_TFR_dot[3];
    double x_TRL_dot[3];
    double x_TRR_dot[3];
    double A_x_tmp_tmp;
    double T_BFL;
    double T_BFR;
    double T_BRL;
    double T_BRR;
    double a;
    double b_b_tmp;
    double b_tmp;
    double c_b_tmp;
    double d;
    double d1;
    double d10;
    double d11;
    double d12;
    double d13;
    double d2;
    double d3;
    double d4;
    double d5;
    double d6;
    double d7;
    double d8;
    double d9;
    double dz_FL;
    double dz_FR;
    double dz_RL;
    double dz_RR;
    double gamma_FL;
    double gamma_FR;
    double gamma_RL;
    double gamma_RR;
    double mu_RFL;
    double mu_RFR;
    double mu_RRL;
    double mu_RRR;
    double r_stFL;
    double r_stFR;
    double r_stRL;
    double r_stRR;
    double u;
    double v_0FAV_tmp;
    double v_0FLV_idx_0;
    double v_0FLV_idx_1;
    double v_0FLV_idx_2;
    double v_0FRV_idx_0;
    double v_0FRV_idx_1;
    double v_0FRV_idx_2;
    double v_0RLV_idx_0;
    double v_0RLV_idx_1;
    double v_0RLV_idx_2;
    double v_0RRV_idx_0;
    double v_0RRV_idx_1;
    double v_0RRV_idx_2;
    double w_FL;
    double w_FR;
    double w_RL;
    double w_RR;
    int b_i;
    int dvRRdz_tmp;
    int e_yFLV_tmp;
    int i;
    int i1;
    int i2;

    //       Vehicle Model Equations            %%
    //  ----------------------------------------- %
    //  Version: V1.19 - 2026.01.05               %
    //  Compatible ADTM Release: ADTM_1.4         %
    //  Author: Simon Frank simon.sf.frank@tum.de %
    //  Modified by: Tim Braungardt  (V1.19)      %
    //  Input:                                    %
    //    inputs  -   struct of model input       %
    //                values at time t            %
    //    t       -   current simulation time     %
    //    x       -   state vector                %
    //    veh     -   struct containing vehicle   %
    //                parameters                  %
    //    tirXX   -   struct containing tire      %
    //                parameters for one tire     %
    //                where XX is FL/FR/RL/RR     %
    //    env     -   struct containing           %
    //                environment parameters      %
    //    road    -   struct of road              %
    //                x, y, Z, and Mu data        %
    //    opts    -   struct of additional model  %
    //                options                     %
    //  Output:                                   %
    //    x_dot   -   state derivative vector     %
    //    out     -   model output vector         %
    //  ----------------------------------------- %
    //  Model states
    //  Generalized coordinates
    //  Vehicle position
    //  Euler angles
    //  Lower wishbone angles
    //  Wheel rotation angles
    //  Generalized velocities
    //  Vehicle velocities
    //  Vehicle angular velocities
    //  Lower wishbone angular velocities
    //  Wheel rotation angular velocities
    //  Tire model internal states
    //  Check states
    //  Model Input
    //  Default motor torque commands
    //  Rotations
    //  Direction vectors
    //  Chassis to earth
    //  Rotation matrix for angle ang about Z axis
    T_BFL = sin(x[5]);
    u = cos(x[5]);

    //  Rotation matrix for angle ang about Y axis
    T_BFR = sin(x[4]);
    T_BRL = cos(x[4]);

    //  Rotation matrix for angle ang about X axis
    T_BRR = sin(x[3]);
    A_x_tmp_tmp = cos(x[3]);
    A_z_tmp[0] = u;
    A_z_tmp[3] = -T_BFL;
    A_z_tmp[6] = 0.0;
    A_z_tmp[1] = T_BFL;
    A_z_tmp[4] = u;
    A_z_tmp[7] = 0.0;
    A_y_tmp[0] = T_BRL;
    A_y_tmp[3] = 0.0;
    A_y_tmp[6] = T_BFR;
    A_z_tmp[2] = 0.0;
    A_y_tmp[1] = 0.0;
    A_z_tmp[5] = 0.0;
    A_y_tmp[4] = 1.0;
    A_z_tmp[8] = 1.0;
    A_y_tmp[7] = 0.0;
    A_y_tmp[2] = -T_BFR;
    A_y_tmp[5] = 0.0;
    A_y_tmp[8] = T_BRL;
    for (i = 0; i < 3; i++) {
      d = A_z_tmp[i];
      d1 = A_z_tmp[i + 3];
      i1 = static_cast<int>(A_z_tmp[i + 6]);
      for (i2 = 0; i2 < 3; i2++) {
        A_VQRR[i + 3 * i2] = (d * A_y_tmp[3 * i2] + d1 * A_y_tmp[3 * i2 + 1]) +
          static_cast<double>(i1) * A_y_tmp[3 * i2 + 2];
      }
    }

    A_z_tmp[0] = 1.0;
    A_z_tmp[3] = 0.0;
    A_z_tmp[6] = 0.0;
    A_z_tmp[1] = 0.0;
    A_z_tmp[4] = A_x_tmp_tmp;
    A_z_tmp[7] = -T_BRR;
    A_z_tmp[2] = 0.0;
    A_z_tmp[5] = T_BRR;
    A_z_tmp[8] = A_x_tmp_tmp;
    for (i = 0; i < 3; i++) {
      d = A_VQRR[i];
      d1 = A_VQRR[i + 3];
      d2 = A_VQRR[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        A_0V[i + 3 * i1] = (d * A_z_tmp[3 * i1] + d1 * A_z_tmp[3 * i1 + 1]) + d2
          * A_z_tmp[3 * i1 + 2];
      }
    }

    //  Velocities to coordinates
    for (i = 0; i < 196; i++) {
      K[i] = iv[i];
    }

    for (i = 0; i < 3; i++) {
      d = A_0V[i];
      A_y_tmp[3 * i] = d;
      K[14 * i] = d;
      d = A_0V[i + 3];
      A_y_tmp[3 * i + 1] = d;
      K[14 * i + 1] = d;
      d = A_0V[i + 6];
      A_y_tmp[3 * i + 2] = d;
      K[14 * i + 2] = d;
    }

    K[45] = 1.0;
    K[59] = 0.0;
    K[73] = -T_BFR;
    K[46] = 0.0;
    K[60] = A_x_tmp_tmp;
    K[74] = T_BRR * T_BRL;
    K[47] = 0.0;
    K[61] = -T_BRR;
    K[75] = A_x_tmp_tmp * T_BRL;
    r = _mm_loadu_pd(&A_y_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(9.80665));
    r = _mm_mul_pd(r, _mm_set1_pd(0.0));
    r1 = _mm_loadu_pd(&A_y_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(9.80665));
    r1 = _mm_mul_pd(r1, _mm_set1_pd(0.0));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&A_y_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(9.80665));
    r = _mm_sub_pd(r, r1);
    _mm_storeu_pd(&g_V[0], r);
    g_V[2] = (A_y_tmp[2] * 9.80665 * 0.0 + A_y_tmp[5] * 9.80665 * 0.0) -
      A_y_tmp[8] * 9.80665;

    //  Suspension kinematics
    //  Front axle
    u = 0.014584380239693683 * inputs_delta_D;

    //  Limit each absolute value of val0 to limit
    if (fabs(u) > 0.028) {
      if (rtIsNaN(u)) {
        T_BRR = rtNaN;
      } else if (u < 0.0) {
        T_BRR = -1.0;
      } else {
        T_BRR = (u > 0.0);
      }

      u = 0.028 * T_BRR;
    }

    //  Front steering rack travel
    //  Double-Wishbone
    f_suspensionDWB(0.0034906585039886592, 0.017453292519943295, dv3, dv4, dv5,
                    dv6, dv7, dv8, dv9, dv10, dv11, x[6], u, A_VQRR, r_VFLV,
                    dvdphi_FL, domdphi_FL, dvdu_FL, domdu_FL);
    f_suspensionDWB(-0.0034906585039886592, -0.017453292519943295, dv12, dv13,
                    dv14, dv15, dv16, dv17, dv18, dv19, dv20, x[7], u, A_VQRL,
                    r_VFRV, dvdphi_FR, domdphi_FR, dvdu_FL, domdu_FL);

    //  Rear axle
    //  Double-Wishbone
    f_suspensionDWB(-0.0087266462599716477, 0.017453292519943295, dv21, dv22,
                    dv23, dv24, dv25, dv26, dv27, dv28, dv29, x[8], A_VRL,
                    r_VRLV, dvdu_FL, domdu_FL);
    f_suspensionDWB(0.0087266462599716477, -0.017453292519943295, dv30, dv31,
                    dv32, dv33, dv34, dv35, dv36, dv37, dv38, x[9], A_VQFR,
                    r_VRRV, dvdphi_RR, domdphi_RR);

    //  Wheel rot. axis vehicle frame
    //  Wheel rot. axis origin frame
    //  Upright inertia tensor vehicle frame
    for (i = 0; i < 3; i++) {
      d = 0.0;
      d1 = 0.0;
      d2 = 0.0;
      d3 = 0.0;
      for (i1 = 0; i1 < 3; i1++) {
        i2 = i + 3 * i1;
        d4 = A_VQRR[i2];
        e_yFLV_tmp = b[i1];
        d += d4 * static_cast<double>(e_yFLV_tmp);
        d1 += A_VQRL[i2] * static_cast<double>(e_yFLV_tmp);
        d2 += A_VRL[i2] * static_cast<double>(e_yFLV_tmp);
        d3 += A_VQFR[i2] * static_cast<double>(e_yFLV_tmp);
        I_UFLV_tmp[i1 + 3 * i] = d4;
        A_z_tmp[i2] = (A_VQRR[i] * dv39[3 * i1] + A_VQRR[i + 3] * dv39[3 * i1 +
                       1]) + A_VQRR[i + 6] * dv39[3 * i1 + 2];
      }

      e_yRRV[i] = d3;
      e_yRLV[i] = d2;
      e_yFRV[i] = d1;
      e_yFLV[i] = d;
    }

    for (i = 0; i < 3; i++) {
      d = A_z_tmp[i];
      d1 = A_z_tmp[i + 3];
      d2 = A_z_tmp[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        e_yFLV_tmp = i + 3 * i1;
        I_UFLV[e_yFLV_tmp] = (d * I_UFLV_tmp[3 * i1] + d1 * I_UFLV_tmp[3 * i1 +
                              1]) + d2 * I_UFLV_tmp[3 * i1 + 2];
        I_UFRV_tmp[i1 + 3 * i] = A_VQRL[e_yFLV_tmp];
      }

      d = A_VQRL[i];
      d1 = A_VQRL[i + 3];
      d2 = A_VQRL[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        A_z_tmp[i + 3 * i1] = (d * dv39[3 * i1] + d1 * dv39[3 * i1 + 1]) + d2 *
          dv39[3 * i1 + 2];
      }
    }

    for (i = 0; i < 3; i++) {
      d = A_z_tmp[i];
      d1 = A_z_tmp[i + 3];
      d2 = A_z_tmp[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        e_yFLV_tmp = i + 3 * i1;
        I_UFRV[e_yFLV_tmp] = (d * I_UFRV_tmp[3 * i1] + d1 * I_UFRV_tmp[3 * i1 +
                              1]) + d2 * I_UFRV_tmp[3 * i1 + 2];
        I_URLV_tmp[i1 + 3 * i] = A_VRL[e_yFLV_tmp];
      }

      d = A_VRL[i];
      d1 = A_VRL[i + 3];
      d2 = A_VRL[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        A_z_tmp[i + 3 * i1] = (d * dv39[3 * i1] + d1 * dv39[3 * i1 + 1]) + d2 *
          dv39[3 * i1 + 2];
      }
    }

    for (i = 0; i < 3; i++) {
      d = A_z_tmp[i];
      d1 = A_z_tmp[i + 3];
      d2 = A_z_tmp[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        e_yFLV_tmp = i + 3 * i1;
        I_URLV[e_yFLV_tmp] = (d * I_URLV_tmp[3 * i1] + d1 * I_URLV_tmp[3 * i1 +
                              1]) + d2 * I_URLV_tmp[3 * i1 + 2];
        I_URRV_tmp[i1 + 3 * i] = A_VQFR[e_yFLV_tmp];
      }

      d = A_VQFR[i];
      d1 = A_VQFR[i + 3];
      d2 = A_VQFR[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        A_z_tmp[i + 3 * i1] = (d * dv39[3 * i1] + d1 * dv39[3 * i1 + 1]) + d2 *
          dv39[3 * i1 + 2];
      }
    }

    //  Wheel inertia tensor vehicle frame
    //  Positions
    r_0V0[0] = x[0];
    r_0V0[1] = x[1];
    r_0V0[2] = x[2];

    //  Velocities
    //  Vehicle velocity vehicle frame
    //  Vel. @ CoG
    omega_0VV[0] = x[17];
    omega_0VV[1] = x[18];
    omega_0VV[2] = x[19];

    //  Angular vel. @ CoG
    v_0FAV_tmp = x[14] + (x[18] * -veh.z_cg - 0.0 * x[19]);
    v_0FAV[0] = v_0FAV_tmp;
    T_BFL = x[17] * -veh.z_cg;
    v_0FAV[1] = x[15] + (veh.l_FA * x[19] - T_BFL);
    v_0FAV[2] = x[16] + (x[17] * 0.0 - veh.l_FA * x[18]);

    //  Vel. @ front axle
    //  Norm of vel. @ front axle
    v_0RAV[0] = v_0FAV_tmp;
    v_0RAV[1] = x[15] + (-veh.l_RA * x[19] - T_BFL);
    v_0RAV[2] = x[16] + (x[17] * 0.0 - -veh.l_RA * x[18]);

    //  Vel. @ rear axle
    //  Norm of vel. @ rear axle
    //  Wheel center velocity vehicle frame
    //  Wheel center total velocity vehicle frame
    //  Wheel center velocities origin frame
    //  Upright angular velocity vehicle frame
    //  Upright total angular velocity vehicle frame
    //  Upright angular velocity origin frame
    //  Wheel angular velocity vehicle frame
    //  Wheel angular velocity wheel frame
    //  Partial velocities
    //  Prepare skew symmetric cross product matrix from vector r
    dvFLdz[9] = 0.0;
    dvFLdz[10] = -r_VFLV[2];
    dvFLdz[11] = r_VFLV[1];
    dvFLdz[12] = r_VFLV[2];
    dvFLdz[13] = 0.0;
    dvFLdz[14] = -r_VFLV[0];
    dvFLdz[15] = -r_VFLV[1];
    dvFLdz[16] = r_VFLV[0];
    dvFLdz[17] = 0.0;
    for (b_i = 0; b_i < 3; b_i++) {
      d = A_z_tmp[b_i];
      d1 = A_z_tmp[b_i + 3];
      d2 = A_z_tmp[b_i + 6];
      for (i = 0; i < 3; i++) {
        I_URRV[b_i + 3 * i] = (d * I_URRV_tmp[3 * i] + d1 * I_URRV_tmp[3 * i + 1])
          + d2 * I_URRV_tmp[3 * i + 2];
      }

      d = A_VQRR[b_i];
      d1 = A_VQRR[b_i + 3];
      d2 = A_VQRR[b_i + 6];
      for (i = 0; i < 3; i++) {
        A_z_tmp[b_i + 3 * i] = (d * dv40[3 * i] + d1 * dv40[3 * i + 1]) + d2 *
          dv40[3 * i + 2];
      }

      d = A_z_tmp[b_i];
      d1 = A_z_tmp[b_i + 3];
      d2 = A_z_tmp[b_i + 6];
      for (i = 0; i < 3; i++) {
        I_WFLV[b_i + 3 * i] = (d * I_UFLV_tmp[3 * i] + d1 * I_UFLV_tmp[3 * i + 1])
          + d2 * I_UFLV_tmp[3 * i + 2];
      }

      d = A_VQRL[b_i];
      d1 = A_VQRL[b_i + 3];
      d2 = A_VQRL[b_i + 6];
      for (i = 0; i < 3; i++) {
        A_z_tmp[b_i + 3 * i] = (d * dv40[3 * i] + d1 * dv40[3 * i + 1]) + d2 *
          dv40[3 * i + 2];
      }

      d = A_z_tmp[b_i];
      d1 = A_z_tmp[b_i + 3];
      d2 = A_z_tmp[b_i + 6];
      for (i = 0; i < 3; i++) {
        I_WFRV[b_i + 3 * i] = (d * I_UFRV_tmp[3 * i] + d1 * I_UFRV_tmp[3 * i + 1])
          + d2 * I_UFRV_tmp[3 * i + 2];
      }

      d = A_VRL[b_i];
      d1 = A_VRL[b_i + 3];
      d2 = A_VRL[b_i + 6];
      for (i = 0; i < 3; i++) {
        A_z_tmp[b_i + 3 * i] = (d * dv40[3 * i] + d1 * dv40[3 * i + 1]) + d2 *
          dv40[3 * i + 2];
      }

      d = A_z_tmp[b_i];
      d1 = A_z_tmp[b_i + 3];
      d2 = A_z_tmp[b_i + 6];
      for (i = 0; i < 3; i++) {
        I_WRLV[b_i + 3 * i] = (d * I_URLV_tmp[3 * i] + d1 * I_URLV_tmp[3 * i + 1])
          + d2 * I_URLV_tmp[3 * i + 2];
      }

      d = A_VQFR[b_i];
      d1 = A_VQFR[b_i + 3];
      d2 = A_VQFR[b_i + 6];
      for (i = 0; i < 3; i++) {
        A_z_tmp[b_i + 3 * i] = (d * dv40[3 * i] + d1 * dv40[3 * i + 1]) + d2 *
          dv40[3 * i + 2];
      }

      d = A_z_tmp[b_i];
      d1 = A_z_tmp[b_i + 3];
      d2 = A_z_tmp[b_i + 6];
      for (i = 0; i < 3; i++) {
        I_WRRV[b_i + 3 * i] = (d * I_URRV_tmp[3 * i] + d1 * I_URRV_tmp[3 * i + 1])
          + d2 * I_URRV_tmp[3 * i + 2];
      }

      d = dvdphi_FL[b_i];
      r_VFLV_dot[b_i] = d * x[20];
      r_VFRV_dot[b_i] = dvdphi_FR[b_i] * x[21];
      r_VRLV_dot[b_i] = dvdu_FL[b_i] * x[22];
      r_VRRV_dot[b_i] = dvdphi_RR[b_i] * x[23];
      d1 = domdphi_FL[b_i] * x[20];
      omega_VUFLV[b_i] = d1;
      d2 = domdphi_FR[b_i] * x[21];
      omega_VUFRV[b_i] = d2;
      d3 = domdu_FL[b_i] * x[22];
      omega_VURLV[b_i] = d3;
      d4 = domdphi_RR[b_i] * x[23];
      omega_VURRV[b_i] = d4;
      d5 = omega_0VV[b_i];
      d1 += d5;
      omega_0UFLV[b_i] = d1;
      d2 += d5;
      omega_0UFRV[b_i] = d2;
      d3 += d5;
      omega_0URLV[b_i] = d3;
      d4 += d5;
      omega_0URRV[b_i] = d4;
      d5 = e_yFLV[b_i] * x[24];
      omega_0WFLV_tmp[b_i] = d5;
      omega_0WFLV[b_i] = d1 + d5;
      d1 = e_yFRV[b_i] * x[25];
      omega_0WFRV_tmp[b_i] = d1;
      omega_0WFRV[b_i] = d2 + d1;
      d1 = e_yRLV[b_i] * x[26];
      omega_0WRLV_tmp[b_i] = d1;
      omega_0WRLV[b_i] = d3 + d1;
      d1 = e_yRRV[b_i] * x[27];
      omega_0WRRV_tmp[b_i] = d1;
      omega_0WRRV[b_i] = d4 + d1;
      dvFLdz[3 * b_i] = iv1[3 * b_i];
      e_yFLV_tmp = 3 * b_i + 1;
      dvFLdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
      e_yFLV_tmp = 3 * b_i + 2;
      dvFLdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
      dvFLdz[b_i + 18] = d;
    }

    v_0FLV_idx_0 = (x[14] + (r_VFLV[2] * x[18] - r_VFLV[1] * x[19])) +
      r_VFLV_dot[0];
    v_0FLV_idx_1 = (x[15] + (r_VFLV[0] * x[19] - r_VFLV[2] * x[17])) +
      r_VFLV_dot[1];
    v_0FLV_idx_2 = (x[16] + (r_VFLV[1] * x[17] - r_VFLV[0] * x[18])) +
      r_VFLV_dot[2];
    v_0FRV_idx_0 = (x[14] + (r_VFRV[2] * x[18] - r_VFRV[1] * x[19])) +
      r_VFRV_dot[0];
    v_0FRV_idx_1 = (x[15] + (r_VFRV[0] * x[19] - r_VFRV[2] * x[17])) +
      r_VFRV_dot[1];
    v_0FRV_idx_2 = (x[16] + (r_VFRV[1] * x[17] - r_VFRV[0] * x[18])) +
      r_VFRV_dot[2];
    v_0RLV_idx_0 = (x[14] + (r_VRLV[2] * x[18] - r_VRLV[1] * x[19])) +
      r_VRLV_dot[0];
    v_0RLV_idx_1 = (x[15] + (r_VRLV[0] * x[19] - r_VRLV[2] * x[17])) +
      r_VRLV_dot[1];
    v_0RLV_idx_2 = (x[16] + (r_VRLV[1] * x[17] - r_VRLV[0] * x[18])) +
      r_VRLV_dot[2];
    v_0RRV_idx_0 = (x[14] + (r_VRRV[2] * x[18] - r_VRRV[1] * x[19])) +
      r_VRRV_dot[0];
    v_0RRV_idx_1 = (x[15] + (r_VRRV[0] * x[19] - r_VRRV[2] * x[17])) +
      r_VRRV_dot[1];
    v_0RRV_idx_2 = (x[16] + (r_VRRV[1] * x[17] - r_VRRV[0] * x[18])) +
      r_VRRV_dot[2];
    for (i = 0; i < 7; i++) {
      e_yFLV_tmp = 3 * (i + 7);
      dvFLdz[e_yFLV_tmp] = 0.0;
      dvFLdz[e_yFLV_tmp + 1] = 0.0;
      dvFLdz[e_yFLV_tmp + 2] = 0.0;
    }

    //  Prepare skew symmetric cross product matrix from vector r
    dvFRdz[9] = 0.0;
    dvFRdz[10] = -r_VFRV[2];
    dvFRdz[11] = r_VFRV[1];
    dvFRdz[12] = r_VFRV[2];
    dvFRdz[13] = 0.0;
    dvFRdz[14] = -r_VFRV[0];
    dvFRdz[15] = -r_VFRV[1];
    dvFRdz[16] = r_VFRV[0];
    dvFRdz[17] = 0.0;
    for (i = 0; i < 3; i++) {
      dvFRdz[3 * i] = iv1[3 * i];
      e_yFLV_tmp = 3 * i + 1;
      dvFRdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
      e_yFLV_tmp = 3 * i + 2;
      dvFRdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
      dvFRdz[i + 18] = 0.0;
      dvFRdz[i + 21] = dvdphi_FR[i];
    }

    for (i = 0; i < 6; i++) {
      e_yFLV_tmp = 3 * (i + 8);
      dvFRdz[e_yFLV_tmp] = 0.0;
      dvFRdz[e_yFLV_tmp + 1] = 0.0;
      dvFRdz[e_yFLV_tmp + 2] = 0.0;
    }

    //  Prepare skew symmetric cross product matrix from vector r
    for (i = 0; i < 3; i++) {
      dvRLdz[3 * i] = iv1[3 * i];
      e_yFLV_tmp = 3 * i + 1;
      dvRLdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
      e_yFLV_tmp = 3 * i + 2;
      dvRLdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
    }

    dvRLdz[9] = 0.0;
    dvRLdz[10] = -r_VRLV[2];
    dvRLdz[11] = r_VRLV[1];
    dvRLdz[12] = r_VRLV[2];
    dvRLdz[13] = 0.0;
    dvRLdz[14] = -r_VRLV[0];
    dvRLdz[15] = -r_VRLV[1];
    dvRLdz[16] = r_VRLV[0];
    dvRLdz[17] = 0.0;
    for (i = 0; i < 2; i++) {
      e_yFLV_tmp = 3 * (i + 6);
      dvRLdz[e_yFLV_tmp] = 0.0;
      dvRLdz[e_yFLV_tmp + 1] = 0.0;
      dvRLdz[e_yFLV_tmp + 2] = 0.0;
    }

    dvRLdz[24] = dvdu_FL[0];
    dvRLdz[25] = dvdu_FL[1];
    dvRLdz[26] = dvdu_FL[2];
    for (i = 0; i < 5; i++) {
      e_yFLV_tmp = 3 * (i + 9);
      dvRLdz[e_yFLV_tmp] = 0.0;
      dvRLdz[e_yFLV_tmp + 1] = 0.0;
      dvRLdz[e_yFLV_tmp + 2] = 0.0;
    }

    //  Prepare skew symmetric cross product matrix from vector r
    dvRRdz[9] = 0.0;
    dvRRdz[10] = -r_VRRV[2];
    dvRRdz[11] = r_VRRV[1];
    dvRRdz[12] = r_VRRV[2];
    dvRRdz[13] = 0.0;
    dvRRdz[14] = -r_VRRV[0];
    dvRRdz[15] = -r_VRRV[1];
    dvRRdz[16] = r_VRRV[0];
    dvRRdz[17] = 0.0;
    for (i = 0; i < 3; i++) {
      dvRRdz[3 * i] = iv1[3 * i];
      e_yFLV_tmp = 3 * (i + 6);
      dvRRdz[e_yFLV_tmp] = 0.0;
      dvRRdz_tmp = 3 * i + 1;
      dvRRdz[dvRRdz_tmp] = iv1[dvRRdz_tmp];
      dvRRdz[e_yFLV_tmp + 1] = 0.0;
      dvRRdz_tmp = 3 * i + 2;
      dvRRdz[dvRRdz_tmp] = iv1[dvRRdz_tmp];
      dvRRdz[e_yFLV_tmp + 2] = 0.0;
      dvRRdz[i + 27] = dvdphi_RR[i];
    }

    for (i = 0; i < 4; i++) {
      e_yFLV_tmp = 3 * (i + 10);
      dvRRdz[e_yFLV_tmp] = 0.0;
      dvRRdz[e_yFLV_tmp + 1] = 0.0;
      dvRRdz[e_yFLV_tmp + 2] = 0.0;
    }

    //  Partial angular velocities
    //  Wheels
    coder::eye(A_VQRR);
    r = _mm_loadu_pd(&e_yFLV[0]);
    r1 = _mm_set1_pd(0.0);
    r2 = _mm_loadu_pd(&e_yFRV[0]);
    for (i = 0; i < 3; i++) {
      r3 = _mm_loadu_pd(&A_VQRR[3 * i]);
      _mm_storeu_pd(&domWFLdz[3 * i], r1);
      i1 = 3 * (i + 3);
      _mm_storeu_pd(&domWFLdz[i1], _mm_sub_pd(r3, _mm_add_pd(r1, _mm_mul_pd(r,
        _mm_set1_pd(e_yFLV[i])))));
      e_yFLV_tmp = 3 * i + 2;
      domWFLdz[e_yFLV_tmp] = 0.0;
      u = A_VQRR[e_yFLV_tmp];
      domWFLdz[i1 + 2] = u - e_yFLV[2] * e_yFLV[i];
      domWFLdz[i + 18] = domdphi_FL[i];
      dvRRdz_tmp = 3 * (i + 7);
      domWFLdz[dvRRdz_tmp] = 0.0;
      domWFLdz[dvRRdz_tmp + 1] = 0.0;
      domWFLdz[dvRRdz_tmp + 2] = 0.0;
      domWFLdz[i + 30] = e_yFLV[i];
      i2 = 3 * (i + 11);
      _mm_storeu_pd(&domWFLdz[i2], r1);
      r3 = _mm_loadu_pd(&A_VQRR[3 * i]);
      _mm_storeu_pd(&domWFRdz[3 * i], r1);
      _mm_storeu_pd(&domWFRdz[i1], _mm_sub_pd(r3, _mm_add_pd(r1, _mm_mul_pd(r2,
        _mm_set1_pd(e_yFRV[i])))));
      domWFLdz[i2 + 2] = 0.0;
      domWFRdz[e_yFLV_tmp] = 0.0;
      domWFRdz[i1 + 2] = u - e_yFRV[2] * e_yFRV[i];
      domWFRdz[i + 18] = 0.0;
      domWFRdz[i + 21] = domdphi_FR[i];
      e_yFLV_tmp = 3 * (i + 8);
      domWFRdz[e_yFLV_tmp] = 0.0;
      domWFRdz[e_yFLV_tmp + 1] = 0.0;
      domWFRdz[e_yFLV_tmp + 2] = 0.0;
      domWFRdz[i + 33] = e_yFRV[i];
    }

    r = _mm_loadu_pd(&e_yRLV[0]);
    r1 = _mm_loadu_pd(&A_VQRR[0]);
    r2 = _mm_set1_pd(0.0);
    _mm_storeu_pd(&domWRLdz[0], r2);
    _mm_storeu_pd(&domWRLdz[9], _mm_sub_pd(r1, _mm_add_pd(r2, _mm_mul_pd(r,
      _mm_set1_pd(e_yRLV[0])))));
    domWRLdz[2] = 0.0;
    domWRLdz[11] = A_VQRR[2] - e_yRLV[0] * e_yRLV[2];
    r = _mm_loadu_pd(&e_yRLV[0]);
    r1 = _mm_loadu_pd(&A_VQRR[3]);
    _mm_storeu_pd(&domWRLdz[3], r2);
    _mm_storeu_pd(&domWRLdz[12], _mm_sub_pd(r1, _mm_add_pd(r2, _mm_mul_pd(r,
      _mm_set1_pd(e_yRLV[1])))));
    domWRLdz[5] = 0.0;
    domWRLdz[14] = A_VQRR[5] - e_yRLV[1] * e_yRLV[2];
    r = _mm_loadu_pd(&e_yRLV[0]);
    r1 = _mm_loadu_pd(&A_VQRR[6]);
    _mm_storeu_pd(&domWRLdz[6], r2);
    _mm_storeu_pd(&domWRLdz[15], _mm_sub_pd(r1, _mm_add_pd(r2, _mm_mul_pd(r,
      _mm_set1_pd(e_yRLV[2])))));
    domWRLdz[8] = 0.0;
    domWRLdz[17] = A_VQRR[8] - e_yRLV[2] * e_yRLV[2];
    for (i = 0; i < 2; i++) {
      e_yFLV_tmp = 3 * (i + 12);
      domWFRdz[e_yFLV_tmp] = 0.0;
      domWFRdz[e_yFLV_tmp + 1] = 0.0;
      domWFRdz[e_yFLV_tmp + 2] = 0.0;
      e_yFLV_tmp = 3 * (i + 6);
      domWRLdz[e_yFLV_tmp] = 0.0;
      domWRLdz[e_yFLV_tmp + 1] = 0.0;
      domWRLdz[e_yFLV_tmp + 2] = 0.0;
    }

    //  Uprights
    r = _mm_loadu_pd(&e_yRRV[0]);
    for (i = 0; i < 3; i++) {
      domWRLdz[i + 24] = domdu_FL[i];
      i1 = 3 * (i + 9);
      _mm_storeu_pd(&domWRLdz[i1], r2);
      r1 = _mm_loadu_pd(&A_VQRR[3 * i]);
      _mm_storeu_pd(&domWRRdz[3 * i], r2);
      i2 = 3 * (i + 3);
      _mm_storeu_pd(&domWRRdz[i2], _mm_sub_pd(r1, _mm_add_pd(r2, _mm_mul_pd(r,
        _mm_set1_pd(e_yRRV[i])))));
      e_yFLV_tmp = 3 * (i + 6);
      _mm_storeu_pd(&domWRRdz[e_yFLV_tmp], r2);
      domWRLdz[i1 + 2] = 0.0;
      b_i = 3 * i + 2;
      domWRRdz[b_i] = 0.0;
      domWRRdz[i2 + 2] = A_VQRR[b_i] - e_yRRV[2] * e_yRRV[i];
      domWRRdz[e_yFLV_tmp + 2] = 0.0;
      domWRLdz[i + 36] = e_yRLV[i];
      domWRLdz[i + 39] = 0.0;
      domWRRdz[i + 27] = domdphi_RR[i];
      dvRRdz_tmp = 3 * (i + 10);
      domWRRdz[dvRRdz_tmp] = 0.0;
      domUFLdz[3 * i] = 0.0;
      domUFLdz[i2] = iv1[3 * i];
      domWRRdz[dvRRdz_tmp + 1] = 0.0;
      e_yFLV_tmp = 3 * i + 1;
      domUFLdz[e_yFLV_tmp] = 0.0;
      domUFLdz[i2 + 1] = iv1[e_yFLV_tmp];
      domWRRdz[dvRRdz_tmp + 2] = 0.0;
      domUFLdz[b_i] = 0.0;
      domUFLdz[i2 + 2] = iv1[b_i];
      domWRRdz[i + 39] = e_yRRV[i];
      domUFLdz[i + 18] = domdphi_FL[i];
    }

    for (i = 0; i < 7; i++) {
      e_yFLV_tmp = 3 * (i + 7);
      domUFLdz[e_yFLV_tmp] = 0.0;
      domUFLdz[e_yFLV_tmp + 1] = 0.0;
      domUFLdz[e_yFLV_tmp + 2] = 0.0;
    }

    for (i = 0; i < 3; i++) {
      domUFRdz[3 * i] = 0.0;
      e_yFLV_tmp = 3 * (i + 3);
      domUFRdz[e_yFLV_tmp] = iv1[3 * i];
      dvRRdz_tmp = 3 * i + 1;
      domUFRdz[dvRRdz_tmp] = 0.0;
      domUFRdz[e_yFLV_tmp + 1] = iv1[dvRRdz_tmp];
      dvRRdz_tmp = 3 * i + 2;
      domUFRdz[dvRRdz_tmp] = 0.0;
      domUFRdz[e_yFLV_tmp + 2] = iv1[dvRRdz_tmp];
      domUFRdz[i + 18] = 0.0;
      domUFRdz[i + 21] = domdphi_FR[i];
    }

    for (i = 0; i < 6; i++) {
      e_yFLV_tmp = 3 * (i + 8);
      domUFRdz[e_yFLV_tmp] = 0.0;
      domUFRdz[e_yFLV_tmp + 1] = 0.0;
      domUFRdz[e_yFLV_tmp + 2] = 0.0;
    }

    for (i = 0; i < 3; i++) {
      domURLdz[3 * i] = 0.0;
      e_yFLV_tmp = 3 * (i + 3);
      domURLdz[e_yFLV_tmp] = iv1[3 * i];
      dvRRdz_tmp = 3 * i + 1;
      domURLdz[dvRRdz_tmp] = 0.0;
      domURLdz[e_yFLV_tmp + 1] = iv1[dvRRdz_tmp];
      dvRRdz_tmp = 3 * i + 2;
      domURLdz[dvRRdz_tmp] = 0.0;
      domURLdz[e_yFLV_tmp + 2] = iv1[dvRRdz_tmp];
    }

    for (i = 0; i < 2; i++) {
      e_yFLV_tmp = 3 * (i + 6);
      domURLdz[e_yFLV_tmp] = 0.0;
      domURLdz[e_yFLV_tmp + 1] = 0.0;
      domURLdz[e_yFLV_tmp + 2] = 0.0;
    }

    domURLdz[24] = domdu_FL[0];
    domURLdz[25] = domdu_FL[1];
    domURLdz[26] = domdu_FL[2];
    for (i = 0; i < 5; i++) {
      e_yFLV_tmp = 3 * (i + 9);
      domURLdz[e_yFLV_tmp] = 0.0;
      domURLdz[e_yFLV_tmp + 1] = 0.0;
      domURLdz[e_yFLV_tmp + 2] = 0.0;
    }

    for (i = 0; i < 3; i++) {
      domURRdz[3 * i] = 0.0;
      dvRRdz_tmp = 3 * (i + 3);
      domURRdz[dvRRdz_tmp] = iv1[3 * i];
      e_yFLV_tmp = 3 * (i + 6);
      domURRdz[e_yFLV_tmp] = 0.0;
      b_i = 3 * i + 1;
      domURRdz[b_i] = 0.0;
      domURRdz[dvRRdz_tmp + 1] = iv1[b_i];
      domURRdz[e_yFLV_tmp + 1] = 0.0;
      b_i = 3 * i + 2;
      domURRdz[b_i] = 0.0;
      domURRdz[dvRRdz_tmp + 2] = iv1[b_i];
      domURRdz[e_yFLV_tmp + 2] = 0.0;
      domURRdz[i + 27] = domdphi_RR[i];
    }

    for (i = 0; i < 4; i++) {
      dvRRdz_tmp = 3 * (i + 10);
      domURRdz[dvRRdz_tmp] = 0.0;
      domURRdz[dvRRdz_tmp + 1] = 0.0;
      domURRdz[dvRRdz_tmp + 2] = 0.0;
    }

    //  Tire road contact
    d = r_VFLV[0];
    d1 = r_VFLV[1];
    d2 = r_VFLV[2];
    d3 = omega_0UFLV[0];
    d4 = omega_0UFLV[1];
    d5 = omega_0UFLV[2];
    d6 = e_yFLV[0];
    d7 = e_yFLV[1];
    d8 = e_yFLV[2];
    r = _mm_loadu_pd(&A_0V[0]);
    r1 = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_mul_pd(r, _mm_set1_pd(v_0FLV_idx_0));
    r3 = _mm_mul_pd(r, _mm_set1_pd(d3));
    r = _mm_mul_pd(r, _mm_set1_pd(d6));
    r4 = _mm_loadu_pd(&A_0V[3]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d1));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0FLV_idx_1));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d4));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d7));
    r = _mm_add_pd(r, r4);
    r4 = _mm_loadu_pd(&A_0V[6]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d2));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0FLV_idx_2));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d5));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d8));
    r = _mm_add_pd(r, r4);
    _mm_storeu_pd(&d_A_0V[0], r);
    _mm_storeu_pd(&c_A_0V[0], r3);
    _mm_storeu_pd(&b_A_0V[0], r2);
    r = _mm_loadu_pd(&r_0V0[0]);
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&omega_0VV[0], r);
    d9 = A_0V[2];
    d10 = d9 * d;
    d11 = d9 * v_0FLV_idx_0;
    d12 = d9 * d3;
    d13 = d9 * d6;
    d9 = A_0V[5];
    d10 += d9 * d1;
    d11 += d9 * v_0FLV_idx_1;
    d12 += d9 * d4;
    d13 += d9 * d7;
    d9 = A_0V[8];
    d10 += d9 * d2;
    d11 += d9 * v_0FLV_idx_2;
    d12 += d9 * d5;
    d13 += d9 * d8;
    d_A_0V[2] = d13;
    c_A_0V[2] = d12;
    b_A_0V[2] = d11;
    omega_0VV[2] = r_0V0[2] + d10;
    gamma_FL = f_roadContact(road.x, road.y, road.Z, road.Mu, omega_0VV, b_A_0V,
      c_A_0V, d_A_0V, dvdu_FL, A_VQRL, r_stFL, dz_FL, w_FL, r_MQFL0, mu_RFL);
    d = r_VFRV[0];
    d1 = r_VFRV[1];
    d2 = r_VFRV[2];
    d3 = omega_0UFRV[0];
    d4 = omega_0UFRV[1];
    d5 = omega_0UFRV[2];
    d6 = e_yFRV[0];
    d7 = e_yFRV[1];
    d8 = e_yFRV[2];
    r = _mm_loadu_pd(&A_0V[0]);
    r1 = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_mul_pd(r, _mm_set1_pd(v_0FRV_idx_0));
    r3 = _mm_mul_pd(r, _mm_set1_pd(d3));
    r = _mm_mul_pd(r, _mm_set1_pd(d6));
    r4 = _mm_loadu_pd(&A_0V[3]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d1));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0FRV_idx_1));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d4));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d7));
    r = _mm_add_pd(r, r4);
    r4 = _mm_loadu_pd(&A_0V[6]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d2));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0FRV_idx_2));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d5));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d8));
    r = _mm_add_pd(r, r4);
    _mm_storeu_pd(&d_A_0V[0], r);
    _mm_storeu_pd(&c_A_0V[0], r3);
    _mm_storeu_pd(&b_A_0V[0], r2);
    r = _mm_loadu_pd(&r_0V0[0]);
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&omega_0VV[0], r);
    d9 = A_0V[2];
    d10 = d9 * d;
    d11 = d9 * v_0FRV_idx_0;
    d12 = d9 * d3;
    d13 = d9 * d6;
    d9 = A_0V[5];
    d10 += d9 * d1;
    d11 += d9 * v_0FRV_idx_1;
    d12 += d9 * d4;
    d13 += d9 * d7;
    d9 = A_0V[8];
    d10 += d9 * d2;
    d11 += d9 * v_0FRV_idx_2;
    d12 += d9 * d5;
    d13 += d9 * d8;
    d_A_0V[2] = d13;
    c_A_0V[2] = d12;
    b_A_0V[2] = d11;
    omega_0VV[2] = r_0V0[2] + d10;
    gamma_FR = f_roadContact(road.x, road.y, road.Z, road.Mu, omega_0VV, b_A_0V,
      c_A_0V, d_A_0V, domdu_FL, A_VRL, r_stFR, dz_FR, w_FR, r_MQFR0, mu_RFR);
    d = r_VRLV[0];
    d1 = r_VRLV[1];
    d2 = r_VRLV[2];
    d3 = omega_0URLV[0];
    d4 = omega_0URLV[1];
    d5 = omega_0URLV[2];
    d6 = e_yRLV[0];
    d7 = e_yRLV[1];
    d8 = e_yRLV[2];
    r = _mm_loadu_pd(&A_0V[0]);
    r1 = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_mul_pd(r, _mm_set1_pd(v_0RLV_idx_0));
    r3 = _mm_mul_pd(r, _mm_set1_pd(d3));
    r = _mm_mul_pd(r, _mm_set1_pd(d6));
    r4 = _mm_loadu_pd(&A_0V[3]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d1));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0RLV_idx_1));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d4));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d7));
    r = _mm_add_pd(r, r4);
    r4 = _mm_loadu_pd(&A_0V[6]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d2));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0RLV_idx_2));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d5));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d8));
    r = _mm_add_pd(r, r4);
    _mm_storeu_pd(&d_A_0V[0], r);
    _mm_storeu_pd(&c_A_0V[0], r3);
    _mm_storeu_pd(&b_A_0V[0], r2);
    r = _mm_loadu_pd(&r_0V0[0]);
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&omega_0VV[0], r);
    d9 = A_0V[2];
    d10 = d9 * d;
    d11 = d9 * v_0RLV_idx_0;
    d12 = d9 * d3;
    d13 = d9 * d6;
    d9 = A_0V[5];
    d10 += d9 * d1;
    d11 += d9 * v_0RLV_idx_1;
    d12 += d9 * d4;
    d13 += d9 * d7;
    d9 = A_0V[8];
    d10 += d9 * d2;
    d11 += d9 * v_0RLV_idx_2;
    d12 += d9 * d5;
    d13 += d9 * d8;
    d_A_0V[2] = d13;
    c_A_0V[2] = d12;
    b_A_0V[2] = d11;
    omega_0VV[2] = r_0V0[2] + d10;
    gamma_RL = f_roadContact(road.x, road.y, road.Z, road.Mu, omega_0VV, b_A_0V,
      c_A_0V, d_A_0V, dvdphi_RR, A_VQRR, r_stRL, dz_RL, w_RL, r_MQRL0, mu_RRL);
    d = r_VRRV[0];
    d1 = r_VRRV[1];
    d2 = r_VRRV[2];
    d3 = omega_0URRV[0];
    d4 = omega_0URRV[1];
    d5 = omega_0URRV[2];
    d6 = e_yRRV[0];
    d7 = e_yRRV[1];
    d8 = e_yRRV[2];
    r = _mm_loadu_pd(&A_0V[0]);
    r1 = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_mul_pd(r, _mm_set1_pd(v_0RRV_idx_0));
    r3 = _mm_mul_pd(r, _mm_set1_pd(d3));
    r = _mm_mul_pd(r, _mm_set1_pd(d6));
    r4 = _mm_loadu_pd(&A_0V[3]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d1));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0RRV_idx_1));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d4));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d7));
    r = _mm_add_pd(r, r4);
    r4 = _mm_loadu_pd(&A_0V[6]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d2));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0RRV_idx_2));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d5));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d8));
    r = _mm_add_pd(r, r4);
    _mm_storeu_pd(&d_A_0V[0], r);
    _mm_storeu_pd(&c_A_0V[0], r3);
    _mm_storeu_pd(&b_A_0V[0], r2);
    r = _mm_loadu_pd(&r_0V0[0]);
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&omega_0VV[0], r);
    d9 = A_0V[2];
    d10 = d9 * d;
    d11 = d9 * v_0RRV_idx_0;
    d12 = d9 * d3;
    d13 = d9 * d6;
    d9 = A_0V[5];
    d10 += d9 * d1;
    d11 += d9 * v_0RRV_idx_1;
    d12 += d9 * d4;
    d13 += d9 * d7;
    d9 = A_0V[8];
    d10 += d9 * d2;
    d11 += d9 * v_0RRV_idx_2;
    d12 += d9 * d5;
    d13 += d9 * d8;
    d_A_0V[2] = d13;
    c_A_0V[2] = d12;
    b_A_0V[2] = d11;
    omega_0VV[2] = r_0V0[2] + d10;
    gamma_RR = f_roadContact(road.x, road.y, road.Z, road.Mu, omega_0VV, b_A_0V,
      c_A_0V, d_A_0V, domdphi_RR, A_z_tmp, r_stRR, dz_RR, w_RR, r_MQRR0, mu_RRR);
    for (i = 0; i < 3; i++) {
      d = A_y_tmp[i];
      d1 = A_y_tmp[i + 3];
      d2 = A_y_tmp[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        i2 = 3 * i1 + 1;
        e_yFLV_tmp = 3 * i1 + 2;
        dvRRdz_tmp = i + 3 * i1;
        A_VQFR[dvRRdz_tmp] = (d * A_VRL[3 * i1] + d1 * A_VRL[i2]) + d2 *
          A_VRL[e_yFLV_tmp];
        A_0V[dvRRdz_tmp] = (d * A_VQRL[3 * i1] + d1 * A_VQRL[i2]) + d2 *
          A_VQRL[e_yFLV_tmp];
      }
    }

    for (i = 0; i < 3; i++) {
      d = A_y_tmp[i];
      d1 = A_y_tmp[i + 3];
      d2 = A_y_tmp[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        A_VQRL[i + 3 * i1] = (d * A_VQRR[3 * i1] + d1 * A_VQRR[3 * i1 + 1]) + d2
          * A_VQRR[3 * i1 + 2];
      }
    }

    //  Tire forces
    //  TMeasy
    for (i = 0; i < 3; i++) {
      d = A_y_tmp[i];
      d1 = A_y_tmp[i + 3];
      d2 = A_y_tmp[i + 6];
      d3 = 0.0;
      for (i1 = 0; i1 < 3; i1++) {
        e_yFLV_tmp = i + 3 * i1;
        A_VQRR[e_yFLV_tmp] = (d * A_z_tmp[3 * i1] + d1 * A_z_tmp[3 * i1 + 1]) +
          d2 * A_z_tmp[3 * i1 + 2];
        d3 += I_UFLV_tmp[e_yFLV_tmp] * omega_0WFLV[i1];
      }

      d_A_0V[i] = d3;
    }

    double out_aero[8];
    double s_FL[4];
    f_tireTMeasy(dvdu_FL, d_A_0V, r_stFL, dz_FL, w_FL, gamma_FL, 0.7 *
                 inputs_mu_N * mu_RFL, &x[28], dvdphi_FR, T_TFL, s_FL, x_TFL_dot,
                 A_x_tmp_tmp, T_BFL, T_BFR, T_BRL, T_BRR);
    d = omega_0WFRV[0];
    d1 = omega_0WFRV[1];
    d2 = omega_0WFRV[2];
    r = _mm_loadu_pd(&I_UFRV_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&I_UFRV_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&I_UFRV_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&b_I_UFRV_tmp[0], r);
    b_I_UFRV_tmp[2] = (I_UFRV_tmp[2] * d + I_UFRV_tmp[5] * d1) + I_UFRV_tmp[8] *
      d2;
    f_tireTMeasy(domdu_FL, b_I_UFRV_tmp, r_stFR, dz_FR, w_FR, gamma_FR, 0.7 *
                 inputs_mu_N * mu_RFR, &x[31], domdphi_FR, T_TFR, s_FL,
                 x_TFR_dot, A_x_tmp_tmp, T_BFL, T_BFR, T_BRL, T_BRR);
    d = omega_0WRLV[0];
    d1 = omega_0WRLV[1];
    d2 = omega_0WRLV[2];
    r = _mm_loadu_pd(&I_URLV_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&I_URLV_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&I_URLV_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&omega_0VV[0], r);
    omega_0VV[2] = (I_URLV_tmp[2] * d + I_URLV_tmp[5] * d1) + I_URLV_tmp[8] * d2;
    f_tireTMeasy(dvdphi_RR, omega_0VV, r_stRL, dz_RL, w_RL, gamma_RL, 0.7 *
                 inputs_mu_N * mu_RRL, &x[34], dvdphi_FL, T_TRL, s_FL, x_TRL_dot,
                 A_x_tmp_tmp, T_BFL, T_BFR, T_BRL, T_BRR);
    d = omega_0WRRV[0];
    d1 = omega_0WRRV[1];
    d2 = omega_0WRRV[2];
    r = _mm_loadu_pd(&I_URRV_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&I_URRV_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&I_URRV_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&omega_0VV[0], r);
    omega_0VV[2] = (I_URRV_tmp[2] * d + I_URRV_tmp[5] * d1) + I_URRV_tmp[8] * d2;
    f_tireTMeasy(domdphi_RR, omega_0VV, r_stRR, dz_RR, w_RR, gamma_RR, 0.7 *
                 inputs_mu_N * mu_RRR, &x[37], domdphi_FL, T_TRR, s_FL,
                 x_TRR_dot, T_BFL, T_BFR, T_BRL, T_BRR, A_x_tmp_tmp);

    //  Suspension forces
    f_suspensionForces(r_VFLV, r_VFRV, r_VRLV, r_VRRV, r_VFLV_dot, r_VFRV_dot,
                       r_VRLV_dot, r_VRRV_dot, veh.z_cg, veh.c_RollFA,
                       veh.c_RollRA, veh.F_SFL0, veh.F_SFR0, veh.F_SRL0,
                       veh.F_SRR0, A_x_tmp_tmp, T_BFL, T_BFR, T_BRL, T_BRR,
                       gamma_FL, r_stFL, dz_FL, w_FL, mu_RFL, gamma_FR, r_stFR,
                       dz_FR, w_FR, mu_RFR, gamma_RR, r_stRR, dz_RR, w_RR);

    //  Aerodynamic Forces
    d = v_0FAV_tmp * v_0FAV_tmp;
    f_aerodynamics(sqrt((d + v_0FAV[1] * v_0FAV[1]) + v_0FAV[2] * v_0FAV[2]),
                   v_0FAV, sqrt((d + v_0RAV[1] * v_0RAV[1]) + v_0RAV[2] *
      v_0RAV[2]), v_0RAV, dv41, veh.C_LFA, veh.C_LRA, veh.C_D, dvdu_FL, domdu_FL,
                   dvdphi_RR, domdphi_RR, omega_0VV, F_ARAV, out_aero);

    //  Applied forces & torques
    //  Wheel rot. friction damping coef.
    //  Powertrain
    f_powertrain(x[24], x[25], x[26], x[27], x, inputs_throttle, veh.T_MMax,
                 veh.i_G, T_BFL, T_BFR, T_BRL, A_x_tmp_tmp, gamma_FL, r_stFL,
                 dz_FL);

    //  Brake torques
    T_BFL = -39.111507258094768 * x[24];
    u = inputs_p_BFA * 2.62119;

    //  Limit each absolute value of val0 to limit
    if (fabs(T_BFL) > u) {
      if (rtIsNaN(T_BFL)) {
        T_BRR = rtNaN;
      } else if (T_BFL < 0.0) {
        T_BRR = -1.0;
      } else {
        T_BRR = (T_BFL > 0.0);
      }

      T_BFL = u * T_BRR;
    }

    T_BFR = -39.111507258094768 * x[25];

    //  Limit each absolute value of val0 to limit
    if (fabs(T_BFR) > u) {
      if (rtIsNaN(T_BFR)) {
        T_BRR = rtNaN;
      } else if (T_BFR < 0.0) {
        T_BRR = -1.0;
      } else {
        T_BRR = (T_BFR > 0.0);
      }

      T_BFR = u * T_BRR;
    }

    T_BRL = -39.111507258094768 * x[26];
    u = inputs_p_BRA * 2.4887362499999997;

    //  Limit each absolute value of val0 to limit
    if (fabs(T_BRL) > u) {
      if (rtIsNaN(T_BRL)) {
        T_BRR = rtNaN;
      } else if (T_BRL < 0.0) {
        T_BRR = -1.0;
      } else {
        T_BRR = (T_BRL > 0.0);
      }

      T_BRL = u * T_BRR;
    }

    T_BRR = -39.111507258094768 * x[27];

    //  Limit each absolute value of val0 to limit
    if (fabs(T_BRR) > u) {
      if (rtIsNaN(T_BRR)) {
        T_BRR = rtNaN;
      } else if (T_BRR < 0.0) {
        T_BRR = -1.0;
      } else {
        T_BRR = (T_BRR > 0.0);
      }

      T_BRR *= u;
    }

    //  Applied torques
    //  Wheels
    b_tmp = T_BFL + A_x_tmp_tmp;

    //  Wheel FL
    b_b_tmp = T_BFR + gamma_FL;

    //  Wheel FR
    c_b_tmp = T_BRL + r_stFL;

    //  Wheel RL
    v_0FAV_tmp = T_BRR + dz_FL;

    //  Wheel RR
    //  Hub-Motors
    //  Uprights
    //  Upright FL
    //  Upright FR
    //  Upright RL
    //  Upright RR
    //  Chassis
    //  Applied forces
    a = ((gamma_RR + r_stRR) + dz_RR) + w_RR;

    //  Chassis
    //  FL
    //  FR
    //  RL
    //  RR
    //  Applied force vector
    for (b_i = 0; b_i < 3; b_i++) {
      T_WRR_tmp[b_i] = (A_VQRR[b_i] * domdphi_FL[0] + A_VQRR[b_i + 3] *
                        domdphi_FL[1]) + A_VQRR[b_i + 6] * domdphi_FL[2];
      T_WRL_tmp[b_i] = (A_VQRL[b_i] * dvdphi_FL[0] + A_VQRL[b_i + 3] *
                        dvdphi_FL[1]) + A_VQRL[b_i + 6] * dvdphi_FL[2];
      T_WFR_tmp[b_i] = (A_VQFR[b_i] * domdphi_FR[0] + A_VQFR[b_i + 3] *
                        domdphi_FR[1]) + A_VQFR[b_i + 6] * domdphi_FR[2];
      T_WFL_tmp[b_i] = (A_0V[b_i] * dvdphi_FR[0] + A_0V[b_i + 3] * dvdphi_FR[1])
        + A_0V[b_i + 6] * dvdphi_FR[2];
      i = static_cast<int>(dv41[b_i]);
      T_C_tmp[b_i] = gamma_RR * static_cast<double>(i);
      b_T_C_tmp[b_i] = r_stRR * static_cast<double>(i);
      c_T_C_tmp[b_i] = dz_RR * static_cast<double>(i);
      d_T_C_tmp[b_i] = w_RR * static_cast<double>(i);
      for (i = 0; i < 14; i++) {
        e_yFLV_tmp = b_i + 3 * i;
        dvRRdz_tmp = i + 14 * b_i;
        y_tmp[dvRRdz_tmp] = dvFRdz[e_yFLV_tmp];
        b_y_tmp[dvRRdz_tmp] = dvRLdz[e_yFLV_tmp];
        c_y_tmp[dvRRdz_tmp] = dvRRdz[e_yFLV_tmp];
        d_y_tmp[dvRRdz_tmp] = domUFLdz[e_yFLV_tmp];
        e_y_tmp[dvRRdz_tmp] = domUFRdz[e_yFLV_tmp];
        f_y_tmp[dvRRdz_tmp] = domURLdz[e_yFLV_tmp];
        g_y_tmp[dvRRdz_tmp] = domURRdz[e_yFLV_tmp];
        h_y_tmp[dvRRdz_tmp] = domWFLdz[e_yFLV_tmp];
        i_y_tmp[dvRRdz_tmp] = domWFRdz[e_yFLV_tmp];
        j_y_tmp[dvRRdz_tmp] = domWRLdz[e_yFLV_tmp];
        k_y_tmp[dvRRdz_tmp] = domWRRdz[e_yFLV_tmp];
      }
    }

    //       % Chassis force
    //       % Chassis torque
    //     % Force FL
    //     % Force FR
    //     % Force RL
    //     % Force RR
    //  % Upright torque FL
    //  % Upright torque FR
    //  % Upright torque RL
    //  % Upright torque RR
    //  % Wheel torque FL
    //  % Wheel torque FR
    //  % Wheel torque RL
    //  Wheel torque RR
    //  Generalized force vector
    for (i = 0; i < 42; i++) {
      q_tmp[i] = static_cast<double>(iv2[i]) * veh.m_C;
    }

    for (i = 0; i < 14; i++) {
      b_q_tmp[i] = dvFLdz[3 * i] * 11.6;
      b_q_tmp[i + 14] = dvFLdz[3 * i + 1] * 11.6;
      b_q_tmp[i + 28] = dvFLdz[3 * i + 2] * 11.6;
    }

    r = _mm_set1_pd(11.6);
    for (i = 0; i <= 40; i += 2) {
      r1 = _mm_loadu_pd(&y_tmp[i]);
      _mm_storeu_pd(&c_q_tmp[i], _mm_mul_pd(r1, r));
      r1 = _mm_loadu_pd(&b_y_tmp[i]);
      _mm_storeu_pd(&d_q_tmp[i], _mm_mul_pd(r1, r));
      r1 = _mm_loadu_pd(&c_y_tmp[i]);
      _mm_storeu_pd(&e_q_tmp[i], _mm_mul_pd(r1, r));
    }

    //  % Chassis force
    //  % Chassis torque
    //  % Force FL
    //  % Force FR
    //  % Force RL
    //  % Force RR
    //  % Upright torque FL
    //  % Upright torque FR
    //  % Upright torque RL
    //  % Upright torque RR
    //  % Wheel torque FL
    //  % Wheel torque FR
    //  % Wheel torque RL
    //  Wheel torque RR
    //  Generalized mass matrix
    //  % Chassis
    //  % FL
    //  % FR
    //  % RL
    //  RR
    //  Solve for state derivatives
    //  Generalized coordinates derivatives
    //  Generalized velocities derivatives
    d = r_MQFL0[0];
    d1 = r_MQFL0[1];
    d2 = r_MQFL0[2];
    r = _mm_loadu_pd(&A_y_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&A_y_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&A_y_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&b_a[0], r);
    b_a[2] = (A_y_tmp[2] * d + A_y_tmp[5] * d1) + A_y_tmp[8] * d2;
    d = r_MQFR0[0];
    d1 = r_MQFR0[1];
    d2 = r_MQFR0[2];
    d3 = r_MQRL0[0];
    d4 = r_MQRL0[1];
    d5 = r_MQRL0[2];
    d6 = r_MQRR0[0];
    d7 = r_MQRR0[1];
    d8 = r_MQRR0[2];
    d9 = omega_0WRRV[0];
    d10 = omega_0WRRV[1];
    d11 = omega_0WRRV[2];
    d12 = omega_0WRLV[0];
    d13 = omega_0WRLV[1];
    u = omega_0WRLV[2];
    A_x_tmp_tmp = omega_0WFRV[0];
    T_BFL = omega_0WFRV[1];
    T_BFR = omega_0WFRV[2];
    T_BRL = omega_0WFLV[0];
    T_BRR = omega_0WFLV[1];
    gamma_FR = omega_0WFLV[2];
    r_stFR = omega_0URRV[0];
    dz_FR = omega_0URRV[1];
    w_FR = omega_0URRV[2];
    mu_RFR = omega_0URLV[0];
    gamma_RR = omega_0URLV[1];
    r_stRR = omega_0URLV[2];
    dz_RR = omega_0UFRV[0];
    w_RR = omega_0UFRV[1];
    gamma_FL = omega_0UFRV[2];
    r_stFL = omega_0UFLV[0];
    dz_FL = omega_0UFLV[1];
    w_FL = omega_0UFLV[2];
    mu_RFL = x[17];
    mu_RRR = x[18];
    gamma_RL = x[19];
    for (i = 0; i < 3; i++) {
      r_stRL = A_y_tmp[i];
      dz_RL = r_stRL * d;
      w_RL = r_stRL * d3;
      mu_RRL = r_stRL * d6;
      r_stRL = A_y_tmp[i + 3];
      dz_RL += r_stRL * d1;
      w_RL += r_stRL * d4;
      mu_RRL += r_stRL * d7;
      r_stRL = A_y_tmp[i + 6];
      dz_RL += r_stRL * d2;
      w_RL += r_stRL * d5;
      mu_RRL += r_stRL * d8;
      r_0V0[i] = (I_WRRV[i] * d9 + I_WRRV[i + 3] * d10) + I_WRRV[i + 6] * d11;
      domdphi_FL[i] = (I_WRLV[i] * d12 + I_WRLV[i + 3] * d13) + I_WRLV[i + 6] *
        u;
      dvdphi_FL[i] = (I_WFRV[i] * A_x_tmp_tmp + I_WFRV[i + 3] * T_BFL) +
        I_WFRV[i + 6] * T_BFR;
      domdphi_FR[i] = (I_WFLV[i] * T_BRL + I_WFLV[i + 3] * T_BRR) + I_WFLV[i + 6]
        * gamma_FR;
      dvdphi_FR[i] = (I_URRV[i] * r_stFR + I_URRV[i + 3] * dz_FR) + I_URRV[i + 6]
        * w_FR;
      domdphi_RR[i] = (I_URLV[i] * mu_RFR + I_URLV[i + 3] * gamma_RR) + I_URLV[i
        + 6] * r_stRR;
      dvdphi_RR[i] = (I_UFRV[i] * dz_RR + I_UFRV[i + 3] * w_RR) + I_UFRV[i + 6] *
        gamma_FL;
      domdu_FL[i] = (I_UFLV[i] * r_stFL + I_UFLV[i + 3] * dz_FL) + I_UFLV[i + 6]
        * w_FL;
      dvdu_FL[i] = (static_cast<double>(iv3[i]) * mu_RFL + static_cast<double>
                    (iv3[i + 3]) * mu_RRR) + static_cast<double>(iv3[i + 6]) *
        gamma_RL;
      v_0FAV[i] = mu_RRL;
      v_0RAV[i] = w_RL;
      r_MQFL0[i] = dz_RL;
    }

    memcpy(&dv[0], &x[14], 14U * sizeof(double));
    coder::mldivide(K, dv);
    r = _mm_loadu_pd(&omega_0VV[0]);
    r1 = _mm_loadu_pd(&F_ARAV[0]);
    _mm_storeu_pd(&r_MQRR0[0], _mm_add_pd(_mm_add_pd(r, r1), _mm_mul_pd
      (_mm_set1_pd(a), _mm_loadu_pd(&dv41[0]))));
    r_MQRR0[2] = (omega_0VV[2] + F_ARAV[2]) + a;
    r_MQFR0[0] = r_VFLV[1] * T_C_tmp[2] - T_C_tmp[1] * r_VFLV[2];
    r_MQFR0[1] = T_C_tmp[0] * r_VFLV[2] - r_VFLV[0] * T_C_tmp[2];
    r_MQFR0[2] = r_VFLV[0] * T_C_tmp[1] - T_C_tmp[0] * r_VFLV[1];
    b_I_UFRV_tmp[0] = r_VFRV[1] * b_T_C_tmp[2] - b_T_C_tmp[1] * r_VFRV[2];
    b_I_UFRV_tmp[1] = b_T_C_tmp[0] * r_VFRV[2] - r_VFRV[0] * b_T_C_tmp[2];
    b_I_UFRV_tmp[2] = r_VFRV[0] * b_T_C_tmp[1] - b_T_C_tmp[0] * r_VFRV[1];
    d_A_0V[0] = r_VRLV[1] * c_T_C_tmp[2] - c_T_C_tmp[1] * r_VRLV[2];
    d_A_0V[1] = c_T_C_tmp[0] * r_VRLV[2] - r_VRLV[0] * c_T_C_tmp[2];
    d_A_0V[2] = r_VRLV[0] * c_T_C_tmp[1] - c_T_C_tmp[0] * r_VRLV[1];
    c_A_0V[0] = r_VRRV[1] * d_T_C_tmp[2] - d_T_C_tmp[1] * r_VRRV[2];
    c_A_0V[1] = d_T_C_tmp[0] * r_VRRV[2] - r_VRRV[0] * d_T_C_tmp[2];
    c_A_0V[2] = r_VRRV[0] * d_T_C_tmp[1] - d_T_C_tmp[0] * r_VRRV[1];
    b_A_0V[0] = 0.0 * omega_0VV[2] - omega_0VV[1] * -veh.z_cg;
    b_A_0V[1] = omega_0VV[0] * -veh.z_cg - veh.l_FA * omega_0VV[2];
    b_A_0V[2] = veh.l_FA * omega_0VV[1] - omega_0VV[0] * 0.0;
    omega_0VV[0] = 0.0 * F_ARAV[2] - F_ARAV[1] * -veh.z_cg;
    omega_0VV[1] = F_ARAV[0] * -veh.z_cg - -veh.l_RA * F_ARAV[2];
    omega_0VV[2] = -veh.l_RA * F_ARAV[1] - F_ARAV[0] * 0.0;
    r = _mm_loadu_pd(&r_MQFR0[0]);
    r1 = _mm_loadu_pd(&b_I_UFRV_tmp[0]);
    r2 = _mm_loadu_pd(&d_A_0V[0]);
    r3 = _mm_loadu_pd(&c_A_0V[0]);
    r4 = _mm_loadu_pd(&b_A_0V[0]);
    r5 = _mm_loadu_pd(&omega_0VV[0]);
    _mm_storeu_pd(&r_MQFR0[0], _mm_add_pd(_mm_add_pd(_mm_add_pd(_mm_add_pd
      (_mm_add_pd(r, r1), r2), r3), r4), r5));
    r_MQFR0[2] = ((((r_MQFR0[2] + b_I_UFRV_tmp[2]) + d_A_0V[2]) + c_A_0V[2]) +
                  b_A_0V[2]) + omega_0VV[2];
    r = _mm_loadu_pd(&T_WFL_tmp[0]);
    r1 = _mm_loadu_pd(&T_C_tmp[0]);
    _mm_storeu_pd(&T_C_tmp[0], _mm_sub_pd(r, r1));
    r = _mm_loadu_pd(&T_WFR_tmp[0]);
    r1 = _mm_loadu_pd(&b_T_C_tmp[0]);
    _mm_storeu_pd(&b_T_C_tmp[0], _mm_sub_pd(r, r1));
    T_C_tmp[2] = T_WFL_tmp[2] - T_C_tmp[2];
    b_T_C_tmp[2] = T_WFR_tmp[2] - b_T_C_tmp[2];
    r = _mm_loadu_pd(&T_WRL_tmp[0]);
    r1 = _mm_loadu_pd(&c_T_C_tmp[0]);
    _mm_storeu_pd(&c_T_C_tmp[0], _mm_sub_pd(r, r1));
    r = _mm_loadu_pd(&T_WRR_tmp[0]);
    r1 = _mm_loadu_pd(&d_T_C_tmp[0]);
    _mm_storeu_pd(&d_T_C_tmp[0], _mm_sub_pd(r, r1));
    c_T_C_tmp[2] = T_WRL_tmp[2] - c_T_C_tmp[2];
    d_T_C_tmp[2] = T_WRR_tmp[2] - d_T_C_tmp[2];
    r = _mm_loadu_pd(&e_yFLV[0]);
    r1 = _mm_set1_pd(-1.0);
    _mm_storeu_pd(&b_A_0V[0], _mm_mul_pd(_mm_mul_pd(r, r1), _mm_set1_pd(b_tmp)));
    r = _mm_loadu_pd(&e_yFRV[0]);
    _mm_storeu_pd(&omega_0VV[0], _mm_mul_pd(_mm_mul_pd(r, r1), _mm_set1_pd
      (b_b_tmp)));
    d = r_MQFR0[0];
    d1 = r_MQFR0[1];
    d2 = r_MQFR0[2];
    d3 = r_MQRR0[0];
    d4 = r_MQRR0[1];
    d5 = r_MQRR0[2];
    d6 = b_T_C_tmp[0];
    d7 = b_T_C_tmp[1];
    d8 = b_T_C_tmp[2];
    d9 = T_C_tmp[0];
    d10 = T_C_tmp[1];
    d11 = T_C_tmp[2];
    d12 = c_T_C_tmp[0];
    d13 = c_T_C_tmp[1];
    u = c_T_C_tmp[2];
    A_x_tmp_tmp = d_T_C_tmp[0];
    T_BFL = d_T_C_tmp[1];
    T_BFR = d_T_C_tmp[2];
    T_BRL = b_A_0V[0];
    T_BRR = b_A_0V[1];
    gamma_FR = -e_yFLV[2] * b_tmp;
    r_stFR = omega_0VV[0];
    dz_FR = omega_0VV[1];
    w_FR = -e_yFRV[2] * b_b_tmp;
    for (i = 0; i < 14; i++) {
      dv2[i] = (((((((static_cast<double>(iv2[i]) * d3 + static_cast<double>
                      (iv2[i + 14]) * d4) + static_cast<double>(iv2[i + 28]) *
                     d5) + ((static_cast<double>(iv4[i]) * d + static_cast<
        double>(iv4[i + 14]) * d1) + static_cast<double>(iv4[i + 28]) * d2)) +
                   ((dvFLdz[3 * i] * d9 + dvFLdz[3 * i + 1] * d10) + dvFLdz[3 *
                    i + 2] * d11)) + ((y_tmp[i] * d6 + y_tmp[i + 14] * d7) +
        y_tmp[i + 28] * d8)) + ((b_y_tmp[i] * d12 + b_y_tmp[i + 14] * d13) +
                  b_y_tmp[i + 28] * u)) + ((c_y_tmp[i] * A_x_tmp_tmp + c_y_tmp[i
                  + 14] * T_BFL) + c_y_tmp[i + 28] * T_BFR)) + ((d_y_tmp[i] *
        T_BRL + d_y_tmp[i + 14] * T_BRR) + d_y_tmp[i + 28] * gamma_FR);
      l_y_tmp[i] = (e_y_tmp[i] * r_stFR + e_y_tmp[i + 14] * dz_FR) + e_y_tmp[i +
        28] * w_FR;
    }

    r = _mm_loadu_pd(&e_yRLV[0]);
    _mm_storeu_pd(&c_A_0V[0], _mm_mul_pd(_mm_mul_pd(r, r1), _mm_set1_pd(c_b_tmp)));
    r = _mm_loadu_pd(&e_yRRV[0]);
    _mm_storeu_pd(&b_A_0V[0], _mm_mul_pd(_mm_mul_pd(r, r1), _mm_set1_pd
      (v_0FAV_tmp)));
    d = c_A_0V[0];
    d1 = c_A_0V[1];
    d2 = -e_yRLV[2] * c_b_tmp;
    d3 = b_A_0V[0];
    d4 = b_A_0V[1];
    d5 = -e_yRRV[2] * v_0FAV_tmp;
    for (i = 0; i <= 12; i += 2) {
      r = _mm_loadu_pd(&dv2[i]);
      r2 = _mm_loadu_pd(&l_y_tmp[i]);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&f_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
      r3 = _mm_loadu_pd(&f_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&f_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&dv1[i], r);
      r = _mm_loadu_pd(&g_y_tmp[i]);
      r = _mm_mul_pd(r, _mm_set1_pd(d3));
      r2 = _mm_loadu_pd(&g_y_tmp[i + 14]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d4));
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&g_y_tmp[i + 28]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d5));
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&l_y_tmp[i], r);
    }

    omega_0VV[0] = b_a[1] * T_WFL_tmp[2] - T_WFL_tmp[1] * b_a[2];
    omega_0VV[1] = T_WFL_tmp[0] * b_a[2] - b_a[0] * T_WFL_tmp[2];
    omega_0VV[2] = b_a[0] * T_WFL_tmp[1] - T_WFL_tmp[0] * b_a[1];
    b_I_UFRV_tmp[0] = r_MQFL0[1] * T_WFR_tmp[2] - T_WFR_tmp[1] * r_MQFL0[2];
    b_I_UFRV_tmp[1] = T_WFR_tmp[0] * r_MQFL0[2] - r_MQFL0[0] * T_WFR_tmp[2];
    b_I_UFRV_tmp[2] = r_MQFL0[0] * T_WFR_tmp[1] - T_WFR_tmp[0] * r_MQFL0[1];
    d = T_TFL[0];
    d1 = T_TFL[1];
    d2 = T_TFL[2];
    d3 = T_TFR[0];
    d4 = T_TFR[1];
    d5 = T_TFR[2];
    r = _mm_loadu_pd(&e_yFLV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(b_tmp));
    r2 = _mm_loadu_pd(&omega_0VV[0]);
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_0V[0]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
    r3 = _mm_loadu_pd(&A_0V[3]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
    r2 = _mm_add_pd(r2, r3);
    r3 = _mm_loadu_pd(&A_0V[6]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
    r2 = _mm_add_pd(r2, r3);
    r = _mm_add_pd(r, r2);
    _mm_storeu_pd(&b_A_0V[0], r);
    r = _mm_loadu_pd(&e_yFRV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(b_b_tmp));
    r2 = _mm_loadu_pd(&b_I_UFRV_tmp[0]);
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_VQFR[0]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d3));
    r3 = _mm_loadu_pd(&A_VQFR[3]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d4));
    r2 = _mm_add_pd(r2, r3);
    r3 = _mm_loadu_pd(&A_VQFR[6]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d5));
    r2 = _mm_add_pd(r2, r3);
    r = _mm_add_pd(r, r2);
    _mm_storeu_pd(&omega_0VV[0], r);
    b_A_0V[2] = (e_yFLV[2] * b_tmp + omega_0VV[2]) + ((A_0V[2] * d + A_0V[5] *
      d1) + A_0V[8] * d2);
    omega_0VV[2] = (e_yFRV[2] * b_b_tmp + b_I_UFRV_tmp[2]) + ((A_VQFR[2] * d3 +
      A_VQFR[5] * d4) + A_VQFR[8] * d5);
    d = b_A_0V[0];
    d1 = b_A_0V[1];
    d2 = b_A_0V[2];
    d3 = omega_0VV[0];
    d4 = omega_0VV[1];
    d5 = omega_0VV[2];
    for (i = 0; i <= 12; i += 2) {
      r = _mm_loadu_pd(&dv1[i]);
      r2 = _mm_loadu_pd(&l_y_tmp[i]);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&h_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
      r3 = _mm_loadu_pd(&h_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&h_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&dv2[i], r);
      r = _mm_loadu_pd(&i_y_tmp[i]);
      r = _mm_mul_pd(r, _mm_set1_pd(d3));
      r2 = _mm_loadu_pd(&i_y_tmp[i + 14]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d4));
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&i_y_tmp[i + 28]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d5));
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&l_y_tmp[i], r);
    }

    b_A_0V[0] = v_0RAV[1] * T_WRL_tmp[2] - T_WRL_tmp[1] * v_0RAV[2];
    b_A_0V[1] = T_WRL_tmp[0] * v_0RAV[2] - v_0RAV[0] * T_WRL_tmp[2];
    b_A_0V[2] = v_0RAV[0] * T_WRL_tmp[1] - T_WRL_tmp[0] * v_0RAV[1];
    omega_0VV[0] = v_0FAV[1] * T_WRR_tmp[2] - T_WRR_tmp[1] * v_0FAV[2];
    omega_0VV[1] = T_WRR_tmp[0] * v_0FAV[2] - v_0FAV[0] * T_WRR_tmp[2];
    omega_0VV[2] = v_0FAV[0] * T_WRR_tmp[1] - T_WRR_tmp[0] * v_0FAV[1];
    d = T_TRL[0];
    d1 = T_TRL[1];
    d2 = T_TRL[2];
    d3 = T_TRR[0];
    d4 = T_TRR[1];
    d5 = T_TRR[2];
    r = _mm_loadu_pd(&e_yRLV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(c_b_tmp));
    r2 = _mm_loadu_pd(&b_A_0V[0]);
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_VQRL[0]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
    r3 = _mm_loadu_pd(&A_VQRL[3]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
    r2 = _mm_add_pd(r2, r3);
    r3 = _mm_loadu_pd(&A_VQRL[6]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
    r2 = _mm_add_pd(r2, r3);
    r = _mm_add_pd(r, r2);
    _mm_storeu_pd(&c_A_0V[0], r);
    r = _mm_loadu_pd(&e_yRRV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(v_0FAV_tmp));
    r2 = _mm_loadu_pd(&omega_0VV[0]);
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_VQRR[0]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d3));
    r3 = _mm_loadu_pd(&A_VQRR[3]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d4));
    r2 = _mm_add_pd(r2, r3);
    r3 = _mm_loadu_pd(&A_VQRR[6]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d5));
    r2 = _mm_add_pd(r2, r3);
    r = _mm_add_pd(r, r2);
    _mm_storeu_pd(&b_A_0V[0], r);
    c_A_0V[2] = (e_yRLV[2] * c_b_tmp + b_A_0V[2]) + ((A_VQRL[2] * d + A_VQRL[5] *
      d1) + A_VQRL[8] * d2);
    b_A_0V[2] = (e_yRRV[2] * v_0FAV_tmp + omega_0VV[2]) + ((A_VQRR[2] * d3 +
      A_VQRR[5] * d4) + A_VQRR[8] * d5);
    d = c_A_0V[0];
    d1 = c_A_0V[1];
    d2 = c_A_0V[2];
    d3 = b_A_0V[0];
    d4 = b_A_0V[1];
    d5 = b_A_0V[2];
    d6 = g_V[0] - (x[16] * x[18] - x[15] * x[19]);
    d7 = g_V[1] - (x[14] * x[19] - x[16] * x[17]);
    d8 = g_V[2] - (x[15] * x[17] - x[14] * x[18]);
    for (i = 0; i <= 12; i += 2) {
      r = _mm_loadu_pd(&dv2[i]);
      r2 = _mm_loadu_pd(&l_y_tmp[i]);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&j_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
      r3 = _mm_loadu_pd(&j_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&j_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&k_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d3));
      r3 = _mm_loadu_pd(&k_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d4));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&k_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d5));
      r2 = _mm_add_pd(r2, r3);
      _mm_storeu_pd(&l_y_tmp[i], r2);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&q_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d6));
      r3 = _mm_loadu_pd(&q_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d7));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&q_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d8));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&dv2[i], r);
    }

    d = -(dvdu_FL[2] * x[18] - dvdu_FL[1] * x[19]);
    d1 = -(dvdu_FL[0] * x[19] - dvdu_FL[2] * x[17]);
    d2 = -(dvdu_FL[1] * x[17] - dvdu_FL[0] * x[18]);
    for (i = 0; i < 14; i++) {
      dv1[i] = (static_cast<double>(iv4[i]) * d + static_cast<double>(iv4[i + 14])
                * d1) + static_cast<double>(iv4[i + 28]) * d2;
    }

    c_A_0V[0] = r_VFLV_dot[2] * x[18] - r_VFLV_dot[1] * x[19];
    c_A_0V[1] = r_VFLV_dot[0] * x[19] - r_VFLV_dot[2] * x[17];
    c_A_0V[2] = r_VFLV_dot[1] * x[17] - r_VFLV_dot[0] * x[18];
    d_A_0V[0] = x[18] * v_0FLV_idx_2 - v_0FLV_idx_1 * x[19];
    d_A_0V[1] = v_0FLV_idx_0 * x[19] - x[17] * v_0FLV_idx_2;
    d_A_0V[2] = x[17] * v_0FLV_idx_1 - v_0FLV_idx_0 * x[18];
    r = _mm_loadu_pd(&c_A_0V[0]);
    r2 = _mm_loadu_pd(&d_A_0V[0]);
    r3 = _mm_loadu_pd(&g_V[0]);
    _mm_storeu_pd(&c_A_0V[0], _mm_sub_pd(r3, _mm_add_pd(r, r2)));
    c_A_0V[2] = g_V[2] - (c_A_0V[2] + d_A_0V[2]);
    d_A_0V[0] = r_VFRV_dot[2] * x[18] - r_VFRV_dot[1] * x[19];
    d_A_0V[1] = r_VFRV_dot[0] * x[19] - r_VFRV_dot[2] * x[17];
    d_A_0V[2] = r_VFRV_dot[1] * x[17] - r_VFRV_dot[0] * x[18];
    b_A_0V[0] = x[18] * v_0FRV_idx_2 - v_0FRV_idx_1 * x[19];
    b_A_0V[1] = v_0FRV_idx_0 * x[19] - x[17] * v_0FRV_idx_2;
    b_A_0V[2] = x[17] * v_0FRV_idx_1 - v_0FRV_idx_0 * x[18];
    r = _mm_loadu_pd(&d_A_0V[0]);
    r2 = _mm_loadu_pd(&b_A_0V[0]);
    r3 = _mm_loadu_pd(&g_V[0]);
    _mm_storeu_pd(&d_A_0V[0], _mm_sub_pd(r3, _mm_add_pd(r, r2)));
    d_A_0V[2] = g_V[2] - (d_A_0V[2] + b_A_0V[2]);
    d = d_A_0V[0];
    d1 = d_A_0V[1];
    d2 = d_A_0V[2];
    d3 = c_A_0V[0];
    d4 = c_A_0V[1];
    d5 = c_A_0V[2];
    for (i = 0; i <= 12; i += 2) {
      r = _mm_loadu_pd(&c_q_tmp[i]);
      r = _mm_mul_pd(r, _mm_set1_pd(d));
      r2 = _mm_loadu_pd(&c_q_tmp[i + 14]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&c_q_tmp[i + 28]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&l_y_tmp[i], r);
      r = _mm_loadu_pd(&dv2[i]);
      r2 = _mm_loadu_pd(&dv1[i]);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&b_q_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d3));
      r3 = _mm_loadu_pd(&b_q_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d4));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&b_q_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d5));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&b_dv3[i], r);
    }

    c_A_0V[0] = r_VRLV_dot[2] * x[18] - r_VRLV_dot[1] * x[19];
    c_A_0V[1] = r_VRLV_dot[0] * x[19] - r_VRLV_dot[2] * x[17];
    c_A_0V[2] = r_VRLV_dot[1] * x[17] - r_VRLV_dot[0] * x[18];
    d_A_0V[0] = x[18] * v_0RLV_idx_2 - v_0RLV_idx_1 * x[19];
    d_A_0V[1] = v_0RLV_idx_0 * x[19] - x[17] * v_0RLV_idx_2;
    d_A_0V[2] = x[17] * v_0RLV_idx_1 - v_0RLV_idx_0 * x[18];
    r = _mm_loadu_pd(&c_A_0V[0]);
    r2 = _mm_loadu_pd(&d_A_0V[0]);
    r3 = _mm_loadu_pd(&g_V[0]);
    _mm_storeu_pd(&c_A_0V[0], _mm_sub_pd(r3, _mm_add_pd(r, r2)));
    c_A_0V[2] = g_V[2] - (c_A_0V[2] + d_A_0V[2]);
    d_A_0V[0] = r_VRRV_dot[2] * x[18] - r_VRRV_dot[1] * x[19];
    d_A_0V[1] = r_VRRV_dot[0] * x[19] - r_VRRV_dot[2] * x[17];
    d_A_0V[2] = r_VRRV_dot[1] * x[17] - r_VRRV_dot[0] * x[18];
    b_A_0V[0] = x[18] * v_0RRV_idx_2 - v_0RRV_idx_1 * x[19];
    b_A_0V[1] = v_0RRV_idx_0 * x[19] - x[17] * v_0RRV_idx_2;
    b_A_0V[2] = x[17] * v_0RRV_idx_1 - v_0RRV_idx_0 * x[18];
    r = _mm_loadu_pd(&d_A_0V[0]);
    r2 = _mm_loadu_pd(&b_A_0V[0]);
    r3 = _mm_loadu_pd(&g_V[0]);
    _mm_storeu_pd(&g_V[0], _mm_sub_pd(r3, _mm_add_pd(r, r2)));
    g_V[2] -= d_A_0V[2] + b_A_0V[2];
    d = c_A_0V[0];
    d1 = c_A_0V[1];
    d2 = c_A_0V[2];
    d3 = -(omega_0UFRV[1] * dvdphi_RR[2] - dvdphi_RR[1] * omega_0UFRV[2]);
    d4 = -(dvdphi_RR[0] * omega_0UFRV[2] - omega_0UFRV[0] * dvdphi_RR[2]);
    d5 = -(omega_0UFRV[0] * dvdphi_RR[1] - dvdphi_RR[0] * omega_0UFRV[1]);
    d6 = g_V[0];
    d7 = g_V[1];
    d8 = g_V[2];
    d9 = -(omega_0UFLV[1] * domdu_FL[2] - domdu_FL[1] * omega_0UFLV[2]);
    d10 = -(domdu_FL[0] * omega_0UFLV[2] - omega_0UFLV[0] * domdu_FL[2]);
    d11 = -(omega_0UFLV[0] * domdu_FL[1] - domdu_FL[0] * omega_0UFLV[1]);
    d12 = -(omega_0URLV[1] * domdphi_RR[2] - domdphi_RR[1] * omega_0URLV[2]);
    d13 = -(domdphi_RR[0] * omega_0URLV[2] - omega_0URLV[0] * domdphi_RR[2]);
    u = -(omega_0URLV[0] * domdphi_RR[1] - domdphi_RR[0] * omega_0URLV[1]);
    for (i = 0; i <= 12; i += 2) {
      r = _mm_loadu_pd(&b_dv3[i]);
      r2 = _mm_loadu_pd(&l_y_tmp[i]);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&d_q_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
      r3 = _mm_loadu_pd(&d_q_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&d_q_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&e_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d3));
      r3 = _mm_loadu_pd(&e_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d4));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&e_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d5));
      r2 = _mm_add_pd(r2, r3);
      _mm_storeu_pd(&l_y_tmp[i], r2);
      r3 = _mm_loadu_pd(&e_q_tmp[i]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d6));
      r4 = _mm_loadu_pd(&e_q_tmp[i + 14]);
      r4 = _mm_mul_pd(r4, _mm_set1_pd(d7));
      r3 = _mm_add_pd(r3, r4);
      r4 = _mm_loadu_pd(&e_q_tmp[i + 28]);
      r4 = _mm_mul_pd(r4, _mm_set1_pd(d8));
      r3 = _mm_add_pd(r3, r4);
      r = _mm_add_pd(r, r3);
      r3 = _mm_loadu_pd(&d_y_tmp[i]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d9));
      r4 = _mm_loadu_pd(&d_y_tmp[i + 14]);
      r4 = _mm_mul_pd(r4, _mm_set1_pd(d10));
      r3 = _mm_add_pd(r3, r4);
      r4 = _mm_loadu_pd(&d_y_tmp[i + 28]);
      r4 = _mm_mul_pd(r4, _mm_set1_pd(d11));
      r3 = _mm_add_pd(r3, r4);
      r = _mm_add_pd(r, r3);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&f_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d12));
      r3 = _mm_loadu_pd(&f_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d13));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&f_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(u));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&dv1[i], r);
    }

    d = -(omega_0URRV[1] * dvdphi_FR[2] - dvdphi_FR[1] * omega_0URRV[2]);
    d1 = -(dvdphi_FR[0] * omega_0URRV[2] - omega_0URRV[0] * dvdphi_FR[2]);
    d2 = -(omega_0URRV[0] * dvdphi_FR[1] - dvdphi_FR[0] * omega_0URRV[1]);
    for (i = 0; i <= 12; i += 2) {
      r = _mm_loadu_pd(&g_y_tmp[i]);
      r = _mm_mul_pd(r, _mm_set1_pd(d));
      r2 = _mm_loadu_pd(&g_y_tmp[i + 14]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&g_y_tmp[i + 28]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&l_y_tmp[i], r);
    }

    c_A_0V[0] = omega_0WFLV[2] * x[18] - omega_0WFLV[1] * x[19];
    c_A_0V[1] = omega_0WFLV[0] * x[19] - omega_0WFLV[2] * x[17];
    c_A_0V[2] = omega_0WFLV[1] * x[17] - omega_0WFLV[0] * x[18];
    b_I_UFRV_tmp[0] = omega_VUFLV[1] * omega_0WFLV_tmp[2] - omega_0WFLV_tmp[1] *
      omega_VUFLV[2];
    b_I_UFRV_tmp[1] = omega_0WFLV_tmp[0] * omega_VUFLV[2] - omega_VUFLV[0] *
      omega_0WFLV_tmp[2];
    b_I_UFRV_tmp[2] = omega_VUFLV[0] * omega_0WFLV_tmp[1] - omega_0WFLV_tmp[0] *
      omega_VUFLV[1];
    r = _mm_loadu_pd(&I_WFLV[0]);
    _mm_storeu_pd(&A_z_tmp[0], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WFLV[2]);
    _mm_storeu_pd(&A_z_tmp[2], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WFLV[4]);
    _mm_storeu_pd(&A_z_tmp[4], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WFLV[6]);
    _mm_storeu_pd(&A_z_tmp[6], _mm_mul_pd(r, r1));
    A_z_tmp[8] = -I_WFLV[8];
    r = _mm_loadu_pd(&c_A_0V[0]);
    r2 = _mm_loadu_pd(&b_I_UFRV_tmp[0]);
    _mm_storeu_pd(&c_A_0V[0], _mm_add_pd(r, r2));
    c_A_0V[2] += b_I_UFRV_tmp[2];
    d_A_0V[0] = omega_0WFLV[1] * domdphi_FR[2] - domdphi_FR[1] * omega_0WFLV[2];
    d_A_0V[1] = domdphi_FR[0] * omega_0WFLV[2] - omega_0WFLV[0] * domdphi_FR[2];
    d_A_0V[2] = omega_0WFLV[0] * domdphi_FR[1] - domdphi_FR[0] * omega_0WFLV[1];
    d = c_A_0V[0];
    d1 = c_A_0V[1];
    d2 = c_A_0V[2];
    r = _mm_loadu_pd(&A_z_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_loadu_pd(&A_z_tmp[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_z_tmp[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&d_A_0V[0]);
    r = _mm_sub_pd(r, r2);
    _mm_storeu_pd(&b_A_0V[0], r);
    b_A_0V[2] = ((A_z_tmp[2] * d + A_z_tmp[5] * d1) + A_z_tmp[8] * d2) - d_A_0V
      [2];
    c_A_0V[0] = omega_0WFRV[2] * x[18] - omega_0WFRV[1] * x[19];
    c_A_0V[1] = omega_0WFRV[0] * x[19] - omega_0WFRV[2] * x[17];
    c_A_0V[2] = omega_0WFRV[1] * x[17] - omega_0WFRV[0] * x[18];
    omega_0VV[0] = omega_VUFRV[1] * omega_0WFRV_tmp[2] - omega_0WFRV_tmp[1] *
      omega_VUFRV[2];
    omega_0VV[1] = omega_0WFRV_tmp[0] * omega_VUFRV[2] - omega_VUFRV[0] *
      omega_0WFRV_tmp[2];
    omega_0VV[2] = omega_VUFRV[0] * omega_0WFRV_tmp[1] - omega_0WFRV_tmp[0] *
      omega_VUFRV[1];
    r = _mm_loadu_pd(&I_WFRV[0]);
    _mm_storeu_pd(&A_z_tmp[0], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WFRV[2]);
    _mm_storeu_pd(&A_z_tmp[2], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WFRV[4]);
    _mm_storeu_pd(&A_z_tmp[4], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WFRV[6]);
    _mm_storeu_pd(&A_z_tmp[6], _mm_mul_pd(r, r1));
    A_z_tmp[8] = -I_WFRV[8];
    r = _mm_loadu_pd(&c_A_0V[0]);
    r2 = _mm_loadu_pd(&omega_0VV[0]);
    _mm_storeu_pd(&c_A_0V[0], _mm_add_pd(r, r2));
    c_A_0V[2] += omega_0VV[2];
    d_A_0V[0] = omega_0WFRV[1] * dvdphi_FL[2] - dvdphi_FL[1] * omega_0WFRV[2];
    d_A_0V[1] = dvdphi_FL[0] * omega_0WFRV[2] - omega_0WFRV[0] * dvdphi_FL[2];
    d_A_0V[2] = omega_0WFRV[0] * dvdphi_FL[1] - dvdphi_FL[0] * omega_0WFRV[1];
    d = c_A_0V[0];
    d1 = c_A_0V[1];
    d2 = c_A_0V[2];
    r = _mm_loadu_pd(&A_z_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_loadu_pd(&A_z_tmp[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_z_tmp[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&d_A_0V[0]);
    r = _mm_sub_pd(r, r2);
    _mm_storeu_pd(&b_I_UFRV_tmp[0], r);
    b_I_UFRV_tmp[2] = ((A_z_tmp[2] * d + A_z_tmp[5] * d1) + A_z_tmp[8] * d2) -
      d_A_0V[2];
    d = b_A_0V[0];
    d1 = b_A_0V[1];
    d2 = b_A_0V[2];
    d3 = b_I_UFRV_tmp[0];
    d4 = b_I_UFRV_tmp[1];
    d5 = b_I_UFRV_tmp[2];
    for (i = 0; i <= 12; i += 2) {
      r = _mm_loadu_pd(&dv1[i]);
      r2 = _mm_loadu_pd(&l_y_tmp[i]);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&h_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
      r3 = _mm_loadu_pd(&h_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&h_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&dv2[i], r);
      r = _mm_loadu_pd(&i_y_tmp[i]);
      r = _mm_mul_pd(r, _mm_set1_pd(d3));
      r2 = _mm_loadu_pd(&i_y_tmp[i + 14]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d4));
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&i_y_tmp[i + 28]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d5));
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&l_y_tmp[i], r);
    }

    c_A_0V[0] = omega_0WRLV[2] * x[18] - omega_0WRLV[1] * x[19];
    c_A_0V[1] = omega_0WRLV[0] * x[19] - omega_0WRLV[2] * x[17];
    c_A_0V[2] = omega_0WRLV[1] * x[17] - omega_0WRLV[0] * x[18];
    b_I_UFRV_tmp[0] = omega_VURLV[1] * omega_0WRLV_tmp[2] - omega_0WRLV_tmp[1] *
      omega_VURLV[2];
    b_I_UFRV_tmp[1] = omega_0WRLV_tmp[0] * omega_VURLV[2] - omega_VURLV[0] *
      omega_0WRLV_tmp[2];
    b_I_UFRV_tmp[2] = omega_VURLV[0] * omega_0WRLV_tmp[1] - omega_0WRLV_tmp[0] *
      omega_VURLV[1];
    r = _mm_loadu_pd(&I_WRLV[0]);
    _mm_storeu_pd(&A_z_tmp[0], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRLV[2]);
    _mm_storeu_pd(&A_z_tmp[2], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRLV[4]);
    _mm_storeu_pd(&A_z_tmp[4], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRLV[6]);
    _mm_storeu_pd(&A_z_tmp[6], _mm_mul_pd(r, r1));
    A_z_tmp[8] = -I_WRLV[8];
    r = _mm_loadu_pd(&c_A_0V[0]);
    r2 = _mm_loadu_pd(&b_I_UFRV_tmp[0]);
    _mm_storeu_pd(&c_A_0V[0], _mm_add_pd(r, r2));
    c_A_0V[2] += b_I_UFRV_tmp[2];
    d_A_0V[0] = omega_0WRLV[1] * domdphi_FL[2] - domdphi_FL[1] * omega_0WRLV[2];
    d_A_0V[1] = domdphi_FL[0] * omega_0WRLV[2] - omega_0WRLV[0] * domdphi_FL[2];
    d_A_0V[2] = omega_0WRLV[0] * domdphi_FL[1] - domdphi_FL[0] * omega_0WRLV[1];
    d = c_A_0V[0];
    d1 = c_A_0V[1];
    d2 = c_A_0V[2];
    r = _mm_loadu_pd(&A_z_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_loadu_pd(&A_z_tmp[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_z_tmp[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&d_A_0V[0]);
    r = _mm_sub_pd(r, r2);
    _mm_storeu_pd(&omega_0VV[0], r);
    omega_0VV[2] = ((A_z_tmp[2] * d + A_z_tmp[5] * d1) + A_z_tmp[8] * d2) -
      d_A_0V[2];
    c_A_0V[0] = omega_0WRRV[2] * x[18] - omega_0WRRV[1] * x[19];
    c_A_0V[1] = omega_0WRRV[0] * x[19] - omega_0WRRV[2] * x[17];
    c_A_0V[2] = omega_0WRRV[1] * x[17] - omega_0WRRV[0] * x[18];
    b_I_UFRV_tmp[0] = omega_VURRV[1] * omega_0WRRV_tmp[2] - omega_0WRRV_tmp[1] *
      omega_VURRV[2];
    b_I_UFRV_tmp[1] = omega_0WRRV_tmp[0] * omega_VURRV[2] - omega_VURRV[0] *
      omega_0WRRV_tmp[2];
    b_I_UFRV_tmp[2] = omega_VURRV[0] * omega_0WRRV_tmp[1] - omega_0WRRV_tmp[0] *
      omega_VURRV[1];
    r = _mm_loadu_pd(&I_WRRV[0]);
    _mm_storeu_pd(&A_z_tmp[0], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRRV[2]);
    _mm_storeu_pd(&A_z_tmp[2], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRRV[4]);
    _mm_storeu_pd(&A_z_tmp[4], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRRV[6]);
    _mm_storeu_pd(&A_z_tmp[6], _mm_mul_pd(r, r1));
    A_z_tmp[8] = -I_WRRV[8];
    r = _mm_loadu_pd(&c_A_0V[0]);
    r1 = _mm_loadu_pd(&b_I_UFRV_tmp[0]);
    _mm_storeu_pd(&c_A_0V[0], _mm_add_pd(r, r1));
    c_A_0V[2] += b_I_UFRV_tmp[2];
    b_I_UFRV_tmp[0] = omega_0WRRV[1] * r_0V0[2] - r_0V0[1] * omega_0WRRV[2];
    b_I_UFRV_tmp[1] = r_0V0[0] * omega_0WRRV[2] - omega_0WRRV[0] * r_0V0[2];
    b_I_UFRV_tmp[2] = omega_0WRRV[0] * r_0V0[1] - r_0V0[0] * omega_0WRRV[1];
    d = c_A_0V[0];
    d1 = c_A_0V[1];
    d2 = c_A_0V[2];
    r = _mm_loadu_pd(&A_z_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&A_z_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&A_z_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&b_I_UFRV_tmp[0]);
    r = _mm_sub_pd(r, r1);
    _mm_storeu_pd(&d_A_0V[0], r);
    d_A_0V[2] = ((A_z_tmp[2] * d + A_z_tmp[5] * d1) + A_z_tmp[8] * d2) -
      b_I_UFRV_tmp[2];
    for (b_i = 0; b_i < 14; b_i++) {
      d = j_y_tmp[b_i + 14];
      d1 = j_y_tmp[b_i + 28];
      d2 = (dv2[b_i] + l_y_tmp[b_i]) + ((j_y_tmp[b_i] * omega_0VV[0] + d *
        omega_0VV[1]) + d1 * omega_0VV[2]);
      d3 = 0.0;
      d4 = d_y_tmp[b_i];
      d5 = d_y_tmp[b_i + 14];
      d6 = d_y_tmp[b_i + 28];
      for (i = 0; i < 3; i++) {
        e_yFLV_tmp = b_i + 14 * i;
        d3 += k_y_tmp[e_yFLV_tmp] * d_A_0V[i];
        y_tmp[e_yFLV_tmp] = (d4 * I_UFLV[3 * i] + d5 * I_UFLV[3 * i + 1]) + d6 *
          I_UFLV[3 * i + 2];
      }

      l_y_tmp[b_i] = d3;
      d2 += d3;
      dv1[b_i] = d2;
      d2 = y_tmp[b_i];
      d3 = y_tmp[b_i + 14];
      d4 = y_tmp[b_i + 28];
      d5 = b_q_tmp[b_i];
      d6 = b_q_tmp[b_i + 14];
      d7 = b_q_tmp[b_i + 28];
      for (i = 0; i < 14; i++) {
        i1 = 3 * i + 1;
        i2 = 3 * i + 2;
        e_yFLV_tmp = b_i + 14 * i;
        m_y_tmp[e_yFLV_tmp] = (d2 * domUFLdz[3 * i] + d3 * domUFLdz[i1]) + d4 *
          domUFLdz[i2];
        K[e_yFLV_tmp] = (d5 * dvFLdz[3 * i] + d6 * dvFLdz[i1]) + d7 * dvFLdz[i2];
      }

      d2 = h_y_tmp[b_i];
      d3 = h_y_tmp[b_i + 14];
      d4 = h_y_tmp[b_i + 28];
      for (i = 0; i < 3; i++) {
        y_tmp[b_i + 14 * i] = (d2 * I_WFLV[3 * i] + d3 * I_WFLV[3 * i + 1]) + d4
          * I_WFLV[3 * i + 2];
      }

      d2 = q_tmp[b_i];
      d3 = q_tmp[b_i + 14];
      d4 = q_tmp[b_i + 28];
      d5 = y_tmp[b_i];
      d6 = y_tmp[b_i + 14];
      d7 = y_tmp[b_i + 28];
      for (i = 0; i < 14; i++) {
        i1 = 3 * i + 1;
        i2 = 3 * i + 2;
        dvRRdz_tmp = b_i + 14 * i;
        f_q_tmp[dvRRdz_tmp] = ((d2 * static_cast<double>(iv5[3 * i]) + d3 *
          static_cast<double>(iv5[i1])) + d4 * static_cast<double>(iv5[i2])) +
          static_cast<double>(iv6[dvRRdz_tmp]);
        g_q_tmp[dvRRdz_tmp] = (K[dvRRdz_tmp] + m_y_tmp[dvRRdz_tmp]) + ((d5 *
          domWFLdz[3 * i] + d6 * domWFLdz[i1]) + d7 * domWFLdz[i2]);
      }

      d2 = e_y_tmp[b_i];
      d3 = e_y_tmp[b_i + 14];
      d4 = e_y_tmp[b_i + 28];
      for (i = 0; i < 3; i++) {
        y_tmp[b_i + 14 * i] = (d2 * I_UFRV[3 * i] + d3 * I_UFRV[3 * i + 1]) + d4
          * I_UFRV[3 * i + 2];
      }

      d2 = y_tmp[b_i];
      d3 = y_tmp[b_i + 14];
      d4 = y_tmp[b_i + 28];
      d5 = c_q_tmp[b_i];
      d6 = c_q_tmp[b_i + 14];
      d7 = c_q_tmp[b_i + 28];
      for (i = 0; i < 14; i++) {
        i1 = 3 * i + 1;
        i2 = 3 * i + 2;
        e_yFLV_tmp = b_i + 14 * i;
        m_y_tmp[e_yFLV_tmp] = (d2 * domUFRdz[3 * i] + d3 * domUFRdz[i1]) + d4 *
          domUFRdz[i2];
        K[e_yFLV_tmp] = (d5 * dvFRdz[3 * i] + d6 * dvFRdz[i1]) + d7 * dvFRdz[i2];
      }

      d2 = f_y_tmp[b_i];
      d3 = f_y_tmp[b_i + 14];
      d4 = f_y_tmp[b_i + 28];
      d5 = i_y_tmp[b_i];
      d6 = i_y_tmp[b_i + 14];
      d7 = i_y_tmp[b_i + 28];
      for (i = 0; i < 3; i++) {
        i1 = 3 * i + 1;
        i2 = 3 * i + 2;
        e_yFLV_tmp = b_i + 14 * i;
        b_y_tmp[e_yFLV_tmp] = (d2 * I_URLV[3 * i] + d3 * I_URLV[i1]) + d4 *
          I_URLV[i2];
        y_tmp[e_yFLV_tmp] = (d5 * I_WFRV[3 * i] + d6 * I_WFRV[i1]) + d7 *
          I_WFRV[i2];
      }

      d2 = b_y_tmp[b_i];
      d3 = b_y_tmp[b_i + 14];
      d4 = b_y_tmp[b_i + 28];
      d5 = d_q_tmp[b_i];
      d6 = d_q_tmp[b_i + 14];
      d7 = d_q_tmp[b_i + 28];
      for (i = 0; i < 14; i++) {
        i1 = 3 * i + 1;
        i2 = 3 * i + 2;
        e_yFLV_tmp = b_i + 14 * i;
        n_y_tmp[e_yFLV_tmp] = (d2 * domURLdz[3 * i] + d3 * domURLdz[i1]) + d4 *
          domURLdz[i2];
        h_q_tmp[e_yFLV_tmp] = (d5 * dvRLdz[3 * i] + d6 * dvRLdz[i1]) + d7 *
          dvRLdz[i2];
      }

      for (i = 0; i < 3; i++) {
        b_y_tmp[b_i + 14 * i] = (j_y_tmp[b_i] * I_WRLV[3 * i] + d * I_WRLV[3 * i
          + 1]) + d1 * I_WRLV[3 * i + 2];
      }

      d = y_tmp[b_i];
      d1 = y_tmp[b_i + 14];
      d2 = y_tmp[b_i + 28];
      d3 = b_y_tmp[b_i];
      d4 = b_y_tmp[b_i + 14];
      d5 = b_y_tmp[b_i + 28];
      for (i = 0; i < 14; i++) {
        i1 = 3 * i + 1;
        i2 = 3 * i + 2;
        dvRRdz_tmp = b_i + 14 * i;
        i_q_tmp[dvRRdz_tmp] = (f_q_tmp[dvRRdz_tmp] + g_q_tmp[dvRRdz_tmp]) +
          ((K[dvRRdz_tmp] + m_y_tmp[dvRRdz_tmp]) + ((d * domWFRdz[3 * i] + d1 *
             domWFRdz[i1]) + d2 * domWFRdz[i2]));
        K[dvRRdz_tmp] = (h_q_tmp[dvRRdz_tmp] + n_y_tmp[dvRRdz_tmp]) + ((d3 *
          domWRLdz[3 * i] + d4 * domWRLdz[i1]) + d5 * domWRLdz[i2]);
      }

      d = g_y_tmp[b_i];
      d1 = g_y_tmp[b_i + 14];
      d2 = g_y_tmp[b_i + 28];
      for (i = 0; i < 3; i++) {
        y_tmp[b_i + 14 * i] = (d * I_URRV[3 * i] + d1 * I_URRV[3 * i + 1]) + d2 *
          I_URRV[3 * i + 2];
      }

      d = y_tmp[b_i];
      d1 = y_tmp[b_i + 14];
      d2 = y_tmp[b_i + 28];
      d3 = e_q_tmp[b_i];
      d4 = e_q_tmp[b_i + 14];
      d5 = e_q_tmp[b_i + 28];
      for (i = 0; i < 14; i++) {
        i1 = 3 * i + 1;
        i2 = 3 * i + 2;
        e_yFLV_tmp = b_i + 14 * i;
        m_y_tmp[e_yFLV_tmp] = (d * domURRdz[3 * i] + d1 * domURRdz[i1]) + d2 *
          domURRdz[i2];
        f_q_tmp[e_yFLV_tmp] = (d3 * dvRRdz[3 * i] + d4 * dvRRdz[i1]) + d5 *
          dvRRdz[i2];
      }

      d = k_y_tmp[b_i];
      d1 = k_y_tmp[b_i + 14];
      d2 = k_y_tmp[b_i + 28];
      for (i = 0; i < 3; i++) {
        y_tmp[b_i + 14 * i] = (d * I_WRRV[3 * i] + d1 * I_WRRV[3 * i + 1]) + d2 *
          I_WRRV[3 * i + 2];
      }

      d = y_tmp[b_i];
      d1 = y_tmp[b_i + 14];
      d2 = y_tmp[b_i + 28];
      for (i = 0; i < 14; i++) {
        dvRRdz_tmp = b_i + 14 * i;
        g_q_tmp[dvRRdz_tmp] = (i_q_tmp[dvRRdz_tmp] + K[dvRRdz_tmp]) +
          ((f_q_tmp[dvRRdz_tmp] + m_y_tmp[dvRRdz_tmp]) + ((d * domWRRdz[3 * i] +
             d1 * domWRRdz[3 * i + 1]) + d2 * domWRRdz[3 * i + 2]));
      }

      x_dot[b_i] = dv[b_i];
    }

    coder::mldivide(g_q_tmp, dv1);
    memcpy(&x_dot[14], &dv1[0], 14U * sizeof(double));
    x_dot[28] = x_TFL_dot[0];
    x_dot[31] = x_TFR_dot[0];
    x_dot[34] = x_TRL_dot[0];
    x_dot[37] = x_TRR_dot[0];
    x_dot[29] = x_TFL_dot[1];
    x_dot[32] = x_TFR_dot[1];
    x_dot[35] = x_TRL_dot[1];
    x_dot[38] = x_TRR_dot[1];
    x_dot[30] = x_TFL_dot[2];
    x_dot[33] = x_TFR_dot[2];
    x_dot[36] = x_TRL_dot[2];
    x_dot[39] = x_TRR_dot[2];
    x_dot[40] = 0.0;
    x_dot[41] = 0.0;

    //  State derivatives
    //  Additional output
    //  Inertial acceleration
    //  Combined aerodynamic forces
    //  Steering Torque
    //  --- Tire utilization (per wheel + global)
    //  Output vector
  }

  static void f_vehicleModel(double inputs_delta_D, double inputs_p_BFA, double
    inputs_p_BRA, double inputs_mu_N, double inputs_T_MCmdFL, double
    inputs_T_MCmdFR, double inputs_T_MCmdRL, double inputs_T_MCmdRR, const
    double x[42], const b_struct_T &veh, const struct_T &road, double x_dot[42])
  {
    static const signed char b[3] = { 0, 1, 0 };

    __m128d r;
    __m128d r1;
    __m128d r2;
    __m128d r3;
    __m128d r4;
    __m128d r5;
    double K[196];
    double f_q_tmp[196];
    double g_q_tmp[196];
    double h_q_tmp[196];
    double i_q_tmp[196];
    double m_y_tmp[196];
    double n_y_tmp[196];
    double b_q_tmp[42];
    double b_y_tmp[42];
    double c_q_tmp[42];
    double c_y_tmp[42];
    double d_q_tmp[42];
    double d_y_tmp[42];
    double domUFLdz[42];
    double domUFRdz[42];
    double domURLdz[42];
    double domURRdz[42];
    double domWFLdz[42];
    double domWFRdz[42];
    double domWRLdz[42];
    double domWRRdz[42];
    double dvFLdz[42];
    double dvFRdz[42];
    double dvRLdz[42];
    double dvRRdz[42];
    double e_q_tmp[42];
    double e_y_tmp[42];
    double f_y_tmp[42];
    double g_y_tmp[42];
    double h_y_tmp[42];
    double i_y_tmp[42];
    double j_y_tmp[42];
    double k_y_tmp[42];
    double q_tmp[42];
    double y_tmp[42];
    double b_dv3[14];
    double dv[14];
    double dv1[14];
    double dv2[14];
    double l_y_tmp[14];
    double A_0V[9];
    double A_VQFR[9];
    double A_VQRL[9];
    double A_VQRR[9];
    double A_VRL[9];
    double A_y_tmp[9];
    double A_z_tmp[9];
    double I_UFLV[9];
    double I_UFLV_tmp[9];
    double I_UFRV[9];
    double I_UFRV_tmp[9];
    double I_URLV[9];
    double I_URLV_tmp[9];
    double I_URRV[9];
    double I_URRV_tmp[9];
    double I_WFLV[9];
    double I_WFRV[9];
    double I_WRLV[9];
    double I_WRRV[9];
    double F_ARAV[3];
    double T_C_tmp[3];
    double T_TFL[3];
    double T_TFR[3];
    double T_TRL[3];
    double T_TRR[3];
    double T_WFL_tmp[3];
    double T_WFR_tmp[3];
    double T_WRL_tmp[3];
    double T_WRR_tmp[3];
    double b_A_0V[3];
    double b_I_UFRV_tmp[3];
    double b_T_C_tmp[3];
    double b_a[3];
    double c_A_0V[3];
    double c_T_C_tmp[3];
    double d_A_0V[3];
    double d_T_C_tmp[3];
    double domdphi_FL[3];
    double domdphi_FR[3];
    double domdphi_RR[3];
    double domdu_FL[3];
    double dvdphi_FL[3];
    double dvdphi_FR[3];
    double dvdphi_RR[3];
    double dvdu_FL[3];
    double e_yFLV[3];
    double e_yFRV[3];
    double e_yRLV[3];
    double e_yRRV[3];
    double g_V[3];
    double omega_0UFLV[3];
    double omega_0UFRV[3];
    double omega_0URLV[3];
    double omega_0URRV[3];
    double omega_0VV[3];
    double omega_0WFLV[3];
    double omega_0WFLV_tmp[3];
    double omega_0WFRV[3];
    double omega_0WFRV_tmp[3];
    double omega_0WRLV[3];
    double omega_0WRLV_tmp[3];
    double omega_0WRRV[3];
    double omega_0WRRV_tmp[3];
    double omega_VUFLV[3];
    double omega_VUFRV[3];
    double omega_VURLV[3];
    double omega_VURRV[3];
    double r_0V0[3];
    double r_MQFL0[3];
    double r_MQFR0[3];
    double r_MQRL0[3];
    double r_MQRR0[3];
    double r_VFLV[3];
    double r_VFLV_dot[3];
    double r_VFRV[3];
    double r_VFRV_dot[3];
    double r_VRLV[3];
    double r_VRLV_dot[3];
    double r_VRRV[3];
    double r_VRRV_dot[3];
    double v_0FAV[3];
    double v_0RAV[3];
    double x_TFL_dot[3];
    double x_TFR_dot[3];
    double x_TRL_dot[3];
    double x_TRR_dot[3];
    double A_x_tmp_tmp;
    double T_BFL;
    double T_BFR;
    double T_BRL;
    double T_BRR;
    double a;
    double b_b_tmp;
    double b_tmp;
    double c_b_tmp;
    double d;
    double d1;
    double d10;
    double d11;
    double d12;
    double d13;
    double d2;
    double d3;
    double d4;
    double d5;
    double d6;
    double d7;
    double d8;
    double d9;
    double dz_FL;
    double dz_FR;
    double dz_RL;
    double dz_RR;
    double gamma_FL;
    double gamma_FR;
    double gamma_RL;
    double gamma_RR;
    double mu_RFL;
    double mu_RFR;
    double mu_RRL;
    double mu_RRR;
    double r_stFL;
    double r_stFR;
    double r_stRL;
    double r_stRR;
    double u;
    double v_0FAV_tmp;
    double v_0FLV_idx_0;
    double v_0FLV_idx_1;
    double v_0FLV_idx_2;
    double v_0FRV_idx_0;
    double v_0FRV_idx_1;
    double v_0FRV_idx_2;
    double v_0RLV_idx_0;
    double v_0RLV_idx_1;
    double v_0RLV_idx_2;
    double v_0RRV_idx_0;
    double v_0RRV_idx_1;
    double v_0RRV_idx_2;
    double w_FL;
    double w_FR;
    double w_RL;
    double w_RR;
    int b_i;
    int dvRRdz_tmp;
    int e_yFLV_tmp;
    int i;
    int i1;
    int i2;

    //       Vehicle Model Equations            %%
    //  ----------------------------------------- %
    //  Version: V1.19 - 2026.01.05               %
    //  Compatible ADTM Release: ADTM_1.4         %
    //  Author: Simon Frank simon.sf.frank@tum.de %
    //  Modified by: Tim Braungardt  (V1.19)      %
    //  Input:                                    %
    //    inputs  -   struct of model input       %
    //                values at time t            %
    //    t       -   current simulation time     %
    //    x       -   state vector                %
    //    veh     -   struct containing vehicle   %
    //                parameters                  %
    //    tirXX   -   struct containing tire      %
    //                parameters for one tire     %
    //                where XX is FL/FR/RL/RR     %
    //    env     -   struct containing           %
    //                environment parameters      %
    //    road    -   struct of road              %
    //                x, y, Z, and Mu data        %
    //    opts    -   struct of additional model  %
    //                options                     %
    //  Output:                                   %
    //    x_dot   -   state derivative vector     %
    //    out     -   model output vector         %
    //  ----------------------------------------- %
    //  Model states
    //  Generalized coordinates
    //  Vehicle position
    //  Euler angles
    //  Lower wishbone angles
    //  Wheel rotation angles
    //  Generalized velocities
    //  Vehicle velocities
    //  Vehicle angular velocities
    //  Lower wishbone angular velocities
    //  Wheel rotation angular velocities
    //  Tire model internal states
    //  Check states
    //  Model Input
    //  Use code when NOT running in Simulink
    //  Default motor torque commands
    //  Motor torque commands from Simulink
    //  Rotations
    //  Direction vectors
    //  Chassis to earth
    //  Rotation matrix for angle ang about Z axis
    T_BFL = sin(x[5]);
    u = cos(x[5]);

    //  Rotation matrix for angle ang about Y axis
    T_BFR = sin(x[4]);
    T_BRL = cos(x[4]);

    //  Rotation matrix for angle ang about X axis
    T_BRR = sin(x[3]);
    A_x_tmp_tmp = cos(x[3]);
    A_z_tmp[0] = u;
    A_z_tmp[3] = -T_BFL;
    A_z_tmp[6] = 0.0;
    A_z_tmp[1] = T_BFL;
    A_z_tmp[4] = u;
    A_z_tmp[7] = 0.0;
    A_y_tmp[0] = T_BRL;
    A_y_tmp[3] = 0.0;
    A_y_tmp[6] = T_BFR;
    A_z_tmp[2] = 0.0;
    A_y_tmp[1] = 0.0;
    A_z_tmp[5] = 0.0;
    A_y_tmp[4] = 1.0;
    A_z_tmp[8] = 1.0;
    A_y_tmp[7] = 0.0;
    A_y_tmp[2] = -T_BFR;
    A_y_tmp[5] = 0.0;
    A_y_tmp[8] = T_BRL;
    for (i = 0; i < 3; i++) {
      d = A_z_tmp[i];
      d1 = A_z_tmp[i + 3];
      i1 = static_cast<int>(A_z_tmp[i + 6]);
      for (i2 = 0; i2 < 3; i2++) {
        A_VQRR[i + 3 * i2] = (d * A_y_tmp[3 * i2] + d1 * A_y_tmp[3 * i2 + 1]) +
          static_cast<double>(i1) * A_y_tmp[3 * i2 + 2];
      }
    }

    A_z_tmp[0] = 1.0;
    A_z_tmp[3] = 0.0;
    A_z_tmp[6] = 0.0;
    A_z_tmp[1] = 0.0;
    A_z_tmp[4] = A_x_tmp_tmp;
    A_z_tmp[7] = -T_BRR;
    A_z_tmp[2] = 0.0;
    A_z_tmp[5] = T_BRR;
    A_z_tmp[8] = A_x_tmp_tmp;
    for (i = 0; i < 3; i++) {
      d = A_VQRR[i];
      d1 = A_VQRR[i + 3];
      d2 = A_VQRR[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        A_0V[i + 3 * i1] = (d * A_z_tmp[3 * i1] + d1 * A_z_tmp[3 * i1 + 1]) + d2
          * A_z_tmp[3 * i1 + 2];
      }
    }

    //  Velocities to coordinates
    for (i = 0; i < 196; i++) {
      K[i] = iv[i];
    }

    for (i = 0; i < 3; i++) {
      d = A_0V[i];
      A_y_tmp[3 * i] = d;
      K[14 * i] = d;
      d = A_0V[i + 3];
      A_y_tmp[3 * i + 1] = d;
      K[14 * i + 1] = d;
      d = A_0V[i + 6];
      A_y_tmp[3 * i + 2] = d;
      K[14 * i + 2] = d;
    }

    K[45] = 1.0;
    K[59] = 0.0;
    K[73] = -T_BFR;
    K[46] = 0.0;
    K[60] = A_x_tmp_tmp;
    K[74] = T_BRR * T_BRL;
    K[47] = 0.0;
    K[61] = -T_BRR;
    K[75] = A_x_tmp_tmp * T_BRL;
    r = _mm_loadu_pd(&A_y_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(9.80665));
    r = _mm_mul_pd(r, _mm_set1_pd(0.0));
    r1 = _mm_loadu_pd(&A_y_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(9.80665));
    r1 = _mm_mul_pd(r1, _mm_set1_pd(0.0));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&A_y_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(9.80665));
    r = _mm_sub_pd(r, r1);
    _mm_storeu_pd(&g_V[0], r);
    g_V[2] = (A_y_tmp[2] * 9.80665 * 0.0 + A_y_tmp[5] * 9.80665 * 0.0) -
      A_y_tmp[8] * 9.80665;

    //  Suspension kinematics
    //  Front axle
    u = 0.014584380239693683 * inputs_delta_D;

    //  Limit each absolute value of val0 to limit
    if (fabs(u) > 0.028) {
      if (rtIsNaN(u)) {
        T_BRR = rtNaN;
      } else if (u < 0.0) {
        T_BRR = -1.0;
      } else {
        T_BRR = (u > 0.0);
      }

      u = 0.028 * T_BRR;
    }

    //  Front steering rack travel
    //  Double-Wishbone
    f_suspensionDWB(0.0034906585039886592, 0.017453292519943295, dv3, dv4, dv5,
                    dv6, dv7, dv8, dv9, dv10, dv11, x[6], u, A_VQRR, r_VFLV,
                    dvdphi_FL, domdphi_FL, dvdu_FL, domdu_FL);
    f_suspensionDWB(-0.0034906585039886592, -0.017453292519943295, dv12, dv13,
                    dv14, dv15, dv16, dv17, dv18, dv19, dv20, x[7], u, A_VQRL,
                    r_VFRV, dvdphi_FR, domdphi_FR, dvdu_FL, domdu_FL);

    //  Rear axle
    //  Double-Wishbone
    f_suspensionDWB(-0.0087266462599716477, 0.017453292519943295, dv21, dv22,
                    dv23, dv24, dv25, dv26, dv27, dv28, dv29, x[8], A_VRL,
                    r_VRLV, dvdu_FL, domdu_FL);
    f_suspensionDWB(0.0087266462599716477, -0.017453292519943295, dv30, dv31,
                    dv32, dv33, dv34, dv35, dv36, dv37, dv38, x[9], A_VQFR,
                    r_VRRV, dvdphi_RR, domdphi_RR);

    //  Wheel rot. axis vehicle frame
    //  Wheel rot. axis origin frame
    //  Upright inertia tensor vehicle frame
    for (i = 0; i < 3; i++) {
      d = 0.0;
      d1 = 0.0;
      d2 = 0.0;
      d3 = 0.0;
      for (i1 = 0; i1 < 3; i1++) {
        i2 = i + 3 * i1;
        d4 = A_VQRR[i2];
        e_yFLV_tmp = b[i1];
        d += d4 * static_cast<double>(e_yFLV_tmp);
        d1 += A_VQRL[i2] * static_cast<double>(e_yFLV_tmp);
        d2 += A_VRL[i2] * static_cast<double>(e_yFLV_tmp);
        d3 += A_VQFR[i2] * static_cast<double>(e_yFLV_tmp);
        I_UFLV_tmp[i1 + 3 * i] = d4;
        A_z_tmp[i2] = (A_VQRR[i] * dv39[3 * i1] + A_VQRR[i + 3] * dv39[3 * i1 +
                       1]) + A_VQRR[i + 6] * dv39[3 * i1 + 2];
      }

      e_yRRV[i] = d3;
      e_yRLV[i] = d2;
      e_yFRV[i] = d1;
      e_yFLV[i] = d;
    }

    for (i = 0; i < 3; i++) {
      d = A_z_tmp[i];
      d1 = A_z_tmp[i + 3];
      d2 = A_z_tmp[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        e_yFLV_tmp = i + 3 * i1;
        I_UFLV[e_yFLV_tmp] = (d * I_UFLV_tmp[3 * i1] + d1 * I_UFLV_tmp[3 * i1 +
                              1]) + d2 * I_UFLV_tmp[3 * i1 + 2];
        I_UFRV_tmp[i1 + 3 * i] = A_VQRL[e_yFLV_tmp];
      }

      d = A_VQRL[i];
      d1 = A_VQRL[i + 3];
      d2 = A_VQRL[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        A_z_tmp[i + 3 * i1] = (d * dv39[3 * i1] + d1 * dv39[3 * i1 + 1]) + d2 *
          dv39[3 * i1 + 2];
      }
    }

    for (i = 0; i < 3; i++) {
      d = A_z_tmp[i];
      d1 = A_z_tmp[i + 3];
      d2 = A_z_tmp[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        e_yFLV_tmp = i + 3 * i1;
        I_UFRV[e_yFLV_tmp] = (d * I_UFRV_tmp[3 * i1] + d1 * I_UFRV_tmp[3 * i1 +
                              1]) + d2 * I_UFRV_tmp[3 * i1 + 2];
        I_URLV_tmp[i1 + 3 * i] = A_VRL[e_yFLV_tmp];
      }

      d = A_VRL[i];
      d1 = A_VRL[i + 3];
      d2 = A_VRL[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        A_z_tmp[i + 3 * i1] = (d * dv39[3 * i1] + d1 * dv39[3 * i1 + 1]) + d2 *
          dv39[3 * i1 + 2];
      }
    }

    for (i = 0; i < 3; i++) {
      d = A_z_tmp[i];
      d1 = A_z_tmp[i + 3];
      d2 = A_z_tmp[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        e_yFLV_tmp = i + 3 * i1;
        I_URLV[e_yFLV_tmp] = (d * I_URLV_tmp[3 * i1] + d1 * I_URLV_tmp[3 * i1 +
                              1]) + d2 * I_URLV_tmp[3 * i1 + 2];
        I_URRV_tmp[i1 + 3 * i] = A_VQFR[e_yFLV_tmp];
      }

      d = A_VQFR[i];
      d1 = A_VQFR[i + 3];
      d2 = A_VQFR[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        A_z_tmp[i + 3 * i1] = (d * dv39[3 * i1] + d1 * dv39[3 * i1 + 1]) + d2 *
          dv39[3 * i1 + 2];
      }
    }

    //  Wheel inertia tensor vehicle frame
    //  Positions
    r_0V0[0] = x[0];
    r_0V0[1] = x[1];
    r_0V0[2] = x[2];

    //  Velocities
    //  Vehicle velocity vehicle frame
    //  Vel. @ CoG
    omega_0VV[0] = x[17];
    omega_0VV[1] = x[18];
    omega_0VV[2] = x[19];

    //  Angular vel. @ CoG
    v_0FAV_tmp = x[14] + (x[18] * -veh.z_cg - 0.0 * x[19]);
    v_0FAV[0] = v_0FAV_tmp;
    T_BFL = x[17] * -veh.z_cg;
    v_0FAV[1] = x[15] + (veh.l_FA * x[19] - T_BFL);
    v_0FAV[2] = x[16] + (x[17] * 0.0 - veh.l_FA * x[18]);

    //  Vel. @ front axle
    //  Norm of vel. @ front axle
    v_0RAV[0] = v_0FAV_tmp;
    v_0RAV[1] = x[15] + (-veh.l_RA * x[19] - T_BFL);
    v_0RAV[2] = x[16] + (x[17] * 0.0 - -veh.l_RA * x[18]);

    //  Vel. @ rear axle
    //  Norm of vel. @ rear axle
    //  Wheel center velocity vehicle frame
    //  Wheel center total velocity vehicle frame
    //  Wheel center velocities origin frame
    //  Upright angular velocity vehicle frame
    //  Upright total angular velocity vehicle frame
    //  Upright angular velocity origin frame
    //  Wheel angular velocity vehicle frame
    //  Wheel angular velocity wheel frame
    //  Partial velocities
    //  Prepare skew symmetric cross product matrix from vector r
    dvFLdz[9] = 0.0;
    dvFLdz[10] = -r_VFLV[2];
    dvFLdz[11] = r_VFLV[1];
    dvFLdz[12] = r_VFLV[2];
    dvFLdz[13] = 0.0;
    dvFLdz[14] = -r_VFLV[0];
    dvFLdz[15] = -r_VFLV[1];
    dvFLdz[16] = r_VFLV[0];
    dvFLdz[17] = 0.0;
    for (b_i = 0; b_i < 3; b_i++) {
      d = A_z_tmp[b_i];
      d1 = A_z_tmp[b_i + 3];
      d2 = A_z_tmp[b_i + 6];
      for (i = 0; i < 3; i++) {
        I_URRV[b_i + 3 * i] = (d * I_URRV_tmp[3 * i] + d1 * I_URRV_tmp[3 * i + 1])
          + d2 * I_URRV_tmp[3 * i + 2];
      }

      d = A_VQRR[b_i];
      d1 = A_VQRR[b_i + 3];
      d2 = A_VQRR[b_i + 6];
      for (i = 0; i < 3; i++) {
        A_z_tmp[b_i + 3 * i] = (d * dv40[3 * i] + d1 * dv40[3 * i + 1]) + d2 *
          dv40[3 * i + 2];
      }

      d = A_z_tmp[b_i];
      d1 = A_z_tmp[b_i + 3];
      d2 = A_z_tmp[b_i + 6];
      for (i = 0; i < 3; i++) {
        I_WFLV[b_i + 3 * i] = (d * I_UFLV_tmp[3 * i] + d1 * I_UFLV_tmp[3 * i + 1])
          + d2 * I_UFLV_tmp[3 * i + 2];
      }

      d = A_VQRL[b_i];
      d1 = A_VQRL[b_i + 3];
      d2 = A_VQRL[b_i + 6];
      for (i = 0; i < 3; i++) {
        A_z_tmp[b_i + 3 * i] = (d * dv40[3 * i] + d1 * dv40[3 * i + 1]) + d2 *
          dv40[3 * i + 2];
      }

      d = A_z_tmp[b_i];
      d1 = A_z_tmp[b_i + 3];
      d2 = A_z_tmp[b_i + 6];
      for (i = 0; i < 3; i++) {
        I_WFRV[b_i + 3 * i] = (d * I_UFRV_tmp[3 * i] + d1 * I_UFRV_tmp[3 * i + 1])
          + d2 * I_UFRV_tmp[3 * i + 2];
      }

      d = A_VRL[b_i];
      d1 = A_VRL[b_i + 3];
      d2 = A_VRL[b_i + 6];
      for (i = 0; i < 3; i++) {
        A_z_tmp[b_i + 3 * i] = (d * dv40[3 * i] + d1 * dv40[3 * i + 1]) + d2 *
          dv40[3 * i + 2];
      }

      d = A_z_tmp[b_i];
      d1 = A_z_tmp[b_i + 3];
      d2 = A_z_tmp[b_i + 6];
      for (i = 0; i < 3; i++) {
        I_WRLV[b_i + 3 * i] = (d * I_URLV_tmp[3 * i] + d1 * I_URLV_tmp[3 * i + 1])
          + d2 * I_URLV_tmp[3 * i + 2];
      }

      d = A_VQFR[b_i];
      d1 = A_VQFR[b_i + 3];
      d2 = A_VQFR[b_i + 6];
      for (i = 0; i < 3; i++) {
        A_z_tmp[b_i + 3 * i] = (d * dv40[3 * i] + d1 * dv40[3 * i + 1]) + d2 *
          dv40[3 * i + 2];
      }

      d = A_z_tmp[b_i];
      d1 = A_z_tmp[b_i + 3];
      d2 = A_z_tmp[b_i + 6];
      for (i = 0; i < 3; i++) {
        I_WRRV[b_i + 3 * i] = (d * I_URRV_tmp[3 * i] + d1 * I_URRV_tmp[3 * i + 1])
          + d2 * I_URRV_tmp[3 * i + 2];
      }

      d = dvdphi_FL[b_i];
      r_VFLV_dot[b_i] = d * x[20];
      r_VFRV_dot[b_i] = dvdphi_FR[b_i] * x[21];
      r_VRLV_dot[b_i] = dvdu_FL[b_i] * x[22];
      r_VRRV_dot[b_i] = dvdphi_RR[b_i] * x[23];
      d1 = domdphi_FL[b_i] * x[20];
      omega_VUFLV[b_i] = d1;
      d2 = domdphi_FR[b_i] * x[21];
      omega_VUFRV[b_i] = d2;
      d3 = domdu_FL[b_i] * x[22];
      omega_VURLV[b_i] = d3;
      d4 = domdphi_RR[b_i] * x[23];
      omega_VURRV[b_i] = d4;
      d5 = omega_0VV[b_i];
      d1 += d5;
      omega_0UFLV[b_i] = d1;
      d2 += d5;
      omega_0UFRV[b_i] = d2;
      d3 += d5;
      omega_0URLV[b_i] = d3;
      d4 += d5;
      omega_0URRV[b_i] = d4;
      d5 = e_yFLV[b_i] * x[24];
      omega_0WFLV_tmp[b_i] = d5;
      omega_0WFLV[b_i] = d1 + d5;
      d1 = e_yFRV[b_i] * x[25];
      omega_0WFRV_tmp[b_i] = d1;
      omega_0WFRV[b_i] = d2 + d1;
      d1 = e_yRLV[b_i] * x[26];
      omega_0WRLV_tmp[b_i] = d1;
      omega_0WRLV[b_i] = d3 + d1;
      d1 = e_yRRV[b_i] * x[27];
      omega_0WRRV_tmp[b_i] = d1;
      omega_0WRRV[b_i] = d4 + d1;
      dvFLdz[3 * b_i] = iv1[3 * b_i];
      e_yFLV_tmp = 3 * b_i + 1;
      dvFLdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
      e_yFLV_tmp = 3 * b_i + 2;
      dvFLdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
      dvFLdz[b_i + 18] = d;
    }

    v_0FLV_idx_0 = (x[14] + (r_VFLV[2] * x[18] - r_VFLV[1] * x[19])) +
      r_VFLV_dot[0];
    v_0FLV_idx_1 = (x[15] + (r_VFLV[0] * x[19] - r_VFLV[2] * x[17])) +
      r_VFLV_dot[1];
    v_0FLV_idx_2 = (x[16] + (r_VFLV[1] * x[17] - r_VFLV[0] * x[18])) +
      r_VFLV_dot[2];
    v_0FRV_idx_0 = (x[14] + (r_VFRV[2] * x[18] - r_VFRV[1] * x[19])) +
      r_VFRV_dot[0];
    v_0FRV_idx_1 = (x[15] + (r_VFRV[0] * x[19] - r_VFRV[2] * x[17])) +
      r_VFRV_dot[1];
    v_0FRV_idx_2 = (x[16] + (r_VFRV[1] * x[17] - r_VFRV[0] * x[18])) +
      r_VFRV_dot[2];
    v_0RLV_idx_0 = (x[14] + (r_VRLV[2] * x[18] - r_VRLV[1] * x[19])) +
      r_VRLV_dot[0];
    v_0RLV_idx_1 = (x[15] + (r_VRLV[0] * x[19] - r_VRLV[2] * x[17])) +
      r_VRLV_dot[1];
    v_0RLV_idx_2 = (x[16] + (r_VRLV[1] * x[17] - r_VRLV[0] * x[18])) +
      r_VRLV_dot[2];
    v_0RRV_idx_0 = (x[14] + (r_VRRV[2] * x[18] - r_VRRV[1] * x[19])) +
      r_VRRV_dot[0];
    v_0RRV_idx_1 = (x[15] + (r_VRRV[0] * x[19] - r_VRRV[2] * x[17])) +
      r_VRRV_dot[1];
    v_0RRV_idx_2 = (x[16] + (r_VRRV[1] * x[17] - r_VRRV[0] * x[18])) +
      r_VRRV_dot[2];
    for (i = 0; i < 7; i++) {
      e_yFLV_tmp = 3 * (i + 7);
      dvFLdz[e_yFLV_tmp] = 0.0;
      dvFLdz[e_yFLV_tmp + 1] = 0.0;
      dvFLdz[e_yFLV_tmp + 2] = 0.0;
    }

    //  Prepare skew symmetric cross product matrix from vector r
    dvFRdz[9] = 0.0;
    dvFRdz[10] = -r_VFRV[2];
    dvFRdz[11] = r_VFRV[1];
    dvFRdz[12] = r_VFRV[2];
    dvFRdz[13] = 0.0;
    dvFRdz[14] = -r_VFRV[0];
    dvFRdz[15] = -r_VFRV[1];
    dvFRdz[16] = r_VFRV[0];
    dvFRdz[17] = 0.0;
    for (i = 0; i < 3; i++) {
      dvFRdz[3 * i] = iv1[3 * i];
      e_yFLV_tmp = 3 * i + 1;
      dvFRdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
      e_yFLV_tmp = 3 * i + 2;
      dvFRdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
      dvFRdz[i + 18] = 0.0;
      dvFRdz[i + 21] = dvdphi_FR[i];
    }

    for (i = 0; i < 6; i++) {
      e_yFLV_tmp = 3 * (i + 8);
      dvFRdz[e_yFLV_tmp] = 0.0;
      dvFRdz[e_yFLV_tmp + 1] = 0.0;
      dvFRdz[e_yFLV_tmp + 2] = 0.0;
    }

    //  Prepare skew symmetric cross product matrix from vector r
    for (i = 0; i < 3; i++) {
      dvRLdz[3 * i] = iv1[3 * i];
      e_yFLV_tmp = 3 * i + 1;
      dvRLdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
      e_yFLV_tmp = 3 * i + 2;
      dvRLdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
    }

    dvRLdz[9] = 0.0;
    dvRLdz[10] = -r_VRLV[2];
    dvRLdz[11] = r_VRLV[1];
    dvRLdz[12] = r_VRLV[2];
    dvRLdz[13] = 0.0;
    dvRLdz[14] = -r_VRLV[0];
    dvRLdz[15] = -r_VRLV[1];
    dvRLdz[16] = r_VRLV[0];
    dvRLdz[17] = 0.0;
    for (i = 0; i < 2; i++) {
      e_yFLV_tmp = 3 * (i + 6);
      dvRLdz[e_yFLV_tmp] = 0.0;
      dvRLdz[e_yFLV_tmp + 1] = 0.0;
      dvRLdz[e_yFLV_tmp + 2] = 0.0;
    }

    dvRLdz[24] = dvdu_FL[0];
    dvRLdz[25] = dvdu_FL[1];
    dvRLdz[26] = dvdu_FL[2];
    for (i = 0; i < 5; i++) {
      e_yFLV_tmp = 3 * (i + 9);
      dvRLdz[e_yFLV_tmp] = 0.0;
      dvRLdz[e_yFLV_tmp + 1] = 0.0;
      dvRLdz[e_yFLV_tmp + 2] = 0.0;
    }

    //  Prepare skew symmetric cross product matrix from vector r
    dvRRdz[9] = 0.0;
    dvRRdz[10] = -r_VRRV[2];
    dvRRdz[11] = r_VRRV[1];
    dvRRdz[12] = r_VRRV[2];
    dvRRdz[13] = 0.0;
    dvRRdz[14] = -r_VRRV[0];
    dvRRdz[15] = -r_VRRV[1];
    dvRRdz[16] = r_VRRV[0];
    dvRRdz[17] = 0.0;
    for (i = 0; i < 3; i++) {
      dvRRdz[3 * i] = iv1[3 * i];
      e_yFLV_tmp = 3 * (i + 6);
      dvRRdz[e_yFLV_tmp] = 0.0;
      dvRRdz_tmp = 3 * i + 1;
      dvRRdz[dvRRdz_tmp] = iv1[dvRRdz_tmp];
      dvRRdz[e_yFLV_tmp + 1] = 0.0;
      dvRRdz_tmp = 3 * i + 2;
      dvRRdz[dvRRdz_tmp] = iv1[dvRRdz_tmp];
      dvRRdz[e_yFLV_tmp + 2] = 0.0;
      dvRRdz[i + 27] = dvdphi_RR[i];
    }

    for (i = 0; i < 4; i++) {
      e_yFLV_tmp = 3 * (i + 10);
      dvRRdz[e_yFLV_tmp] = 0.0;
      dvRRdz[e_yFLV_tmp + 1] = 0.0;
      dvRRdz[e_yFLV_tmp + 2] = 0.0;
    }

    //  Partial angular velocities
    //  Wheels
    coder::eye(A_VQRR);
    r = _mm_loadu_pd(&e_yFLV[0]);
    r1 = _mm_set1_pd(0.0);
    r2 = _mm_loadu_pd(&e_yFRV[0]);
    for (i = 0; i < 3; i++) {
      r3 = _mm_loadu_pd(&A_VQRR[3 * i]);
      _mm_storeu_pd(&domWFLdz[3 * i], r1);
      i1 = 3 * (i + 3);
      _mm_storeu_pd(&domWFLdz[i1], _mm_sub_pd(r3, _mm_add_pd(r1, _mm_mul_pd(r,
        _mm_set1_pd(e_yFLV[i])))));
      e_yFLV_tmp = 3 * i + 2;
      domWFLdz[e_yFLV_tmp] = 0.0;
      u = A_VQRR[e_yFLV_tmp];
      domWFLdz[i1 + 2] = u - e_yFLV[2] * e_yFLV[i];
      domWFLdz[i + 18] = domdphi_FL[i];
      dvRRdz_tmp = 3 * (i + 7);
      domWFLdz[dvRRdz_tmp] = 0.0;
      domWFLdz[dvRRdz_tmp + 1] = 0.0;
      domWFLdz[dvRRdz_tmp + 2] = 0.0;
      domWFLdz[i + 30] = e_yFLV[i];
      i2 = 3 * (i + 11);
      _mm_storeu_pd(&domWFLdz[i2], r1);
      r3 = _mm_loadu_pd(&A_VQRR[3 * i]);
      _mm_storeu_pd(&domWFRdz[3 * i], r1);
      _mm_storeu_pd(&domWFRdz[i1], _mm_sub_pd(r3, _mm_add_pd(r1, _mm_mul_pd(r2,
        _mm_set1_pd(e_yFRV[i])))));
      domWFLdz[i2 + 2] = 0.0;
      domWFRdz[e_yFLV_tmp] = 0.0;
      domWFRdz[i1 + 2] = u - e_yFRV[2] * e_yFRV[i];
      domWFRdz[i + 18] = 0.0;
      domWFRdz[i + 21] = domdphi_FR[i];
      e_yFLV_tmp = 3 * (i + 8);
      domWFRdz[e_yFLV_tmp] = 0.0;
      domWFRdz[e_yFLV_tmp + 1] = 0.0;
      domWFRdz[e_yFLV_tmp + 2] = 0.0;
      domWFRdz[i + 33] = e_yFRV[i];
    }

    r = _mm_loadu_pd(&e_yRLV[0]);
    r1 = _mm_loadu_pd(&A_VQRR[0]);
    r2 = _mm_set1_pd(0.0);
    _mm_storeu_pd(&domWRLdz[0], r2);
    _mm_storeu_pd(&domWRLdz[9], _mm_sub_pd(r1, _mm_add_pd(r2, _mm_mul_pd(r,
      _mm_set1_pd(e_yRLV[0])))));
    domWRLdz[2] = 0.0;
    domWRLdz[11] = A_VQRR[2] - e_yRLV[0] * e_yRLV[2];
    r = _mm_loadu_pd(&e_yRLV[0]);
    r1 = _mm_loadu_pd(&A_VQRR[3]);
    _mm_storeu_pd(&domWRLdz[3], r2);
    _mm_storeu_pd(&domWRLdz[12], _mm_sub_pd(r1, _mm_add_pd(r2, _mm_mul_pd(r,
      _mm_set1_pd(e_yRLV[1])))));
    domWRLdz[5] = 0.0;
    domWRLdz[14] = A_VQRR[5] - e_yRLV[1] * e_yRLV[2];
    r = _mm_loadu_pd(&e_yRLV[0]);
    r1 = _mm_loadu_pd(&A_VQRR[6]);
    _mm_storeu_pd(&domWRLdz[6], r2);
    _mm_storeu_pd(&domWRLdz[15], _mm_sub_pd(r1, _mm_add_pd(r2, _mm_mul_pd(r,
      _mm_set1_pd(e_yRLV[2])))));
    domWRLdz[8] = 0.0;
    domWRLdz[17] = A_VQRR[8] - e_yRLV[2] * e_yRLV[2];
    for (i = 0; i < 2; i++) {
      e_yFLV_tmp = 3 * (i + 12);
      domWFRdz[e_yFLV_tmp] = 0.0;
      domWFRdz[e_yFLV_tmp + 1] = 0.0;
      domWFRdz[e_yFLV_tmp + 2] = 0.0;
      e_yFLV_tmp = 3 * (i + 6);
      domWRLdz[e_yFLV_tmp] = 0.0;
      domWRLdz[e_yFLV_tmp + 1] = 0.0;
      domWRLdz[e_yFLV_tmp + 2] = 0.0;
    }

    //  Uprights
    r = _mm_loadu_pd(&e_yRRV[0]);
    for (i = 0; i < 3; i++) {
      domWRLdz[i + 24] = domdu_FL[i];
      i1 = 3 * (i + 9);
      _mm_storeu_pd(&domWRLdz[i1], r2);
      r1 = _mm_loadu_pd(&A_VQRR[3 * i]);
      _mm_storeu_pd(&domWRRdz[3 * i], r2);
      i2 = 3 * (i + 3);
      _mm_storeu_pd(&domWRRdz[i2], _mm_sub_pd(r1, _mm_add_pd(r2, _mm_mul_pd(r,
        _mm_set1_pd(e_yRRV[i])))));
      e_yFLV_tmp = 3 * (i + 6);
      _mm_storeu_pd(&domWRRdz[e_yFLV_tmp], r2);
      domWRLdz[i1 + 2] = 0.0;
      b_i = 3 * i + 2;
      domWRRdz[b_i] = 0.0;
      domWRRdz[i2 + 2] = A_VQRR[b_i] - e_yRRV[2] * e_yRRV[i];
      domWRRdz[e_yFLV_tmp + 2] = 0.0;
      domWRLdz[i + 36] = e_yRLV[i];
      domWRLdz[i + 39] = 0.0;
      domWRRdz[i + 27] = domdphi_RR[i];
      dvRRdz_tmp = 3 * (i + 10);
      domWRRdz[dvRRdz_tmp] = 0.0;
      domUFLdz[3 * i] = 0.0;
      domUFLdz[i2] = iv1[3 * i];
      domWRRdz[dvRRdz_tmp + 1] = 0.0;
      e_yFLV_tmp = 3 * i + 1;
      domUFLdz[e_yFLV_tmp] = 0.0;
      domUFLdz[i2 + 1] = iv1[e_yFLV_tmp];
      domWRRdz[dvRRdz_tmp + 2] = 0.0;
      domUFLdz[b_i] = 0.0;
      domUFLdz[i2 + 2] = iv1[b_i];
      domWRRdz[i + 39] = e_yRRV[i];
      domUFLdz[i + 18] = domdphi_FL[i];
    }

    for (i = 0; i < 7; i++) {
      e_yFLV_tmp = 3 * (i + 7);
      domUFLdz[e_yFLV_tmp] = 0.0;
      domUFLdz[e_yFLV_tmp + 1] = 0.0;
      domUFLdz[e_yFLV_tmp + 2] = 0.0;
    }

    for (i = 0; i < 3; i++) {
      domUFRdz[3 * i] = 0.0;
      e_yFLV_tmp = 3 * (i + 3);
      domUFRdz[e_yFLV_tmp] = iv1[3 * i];
      dvRRdz_tmp = 3 * i + 1;
      domUFRdz[dvRRdz_tmp] = 0.0;
      domUFRdz[e_yFLV_tmp + 1] = iv1[dvRRdz_tmp];
      dvRRdz_tmp = 3 * i + 2;
      domUFRdz[dvRRdz_tmp] = 0.0;
      domUFRdz[e_yFLV_tmp + 2] = iv1[dvRRdz_tmp];
      domUFRdz[i + 18] = 0.0;
      domUFRdz[i + 21] = domdphi_FR[i];
    }

    for (i = 0; i < 6; i++) {
      e_yFLV_tmp = 3 * (i + 8);
      domUFRdz[e_yFLV_tmp] = 0.0;
      domUFRdz[e_yFLV_tmp + 1] = 0.0;
      domUFRdz[e_yFLV_tmp + 2] = 0.0;
    }

    for (i = 0; i < 3; i++) {
      domURLdz[3 * i] = 0.0;
      e_yFLV_tmp = 3 * (i + 3);
      domURLdz[e_yFLV_tmp] = iv1[3 * i];
      dvRRdz_tmp = 3 * i + 1;
      domURLdz[dvRRdz_tmp] = 0.0;
      domURLdz[e_yFLV_tmp + 1] = iv1[dvRRdz_tmp];
      dvRRdz_tmp = 3 * i + 2;
      domURLdz[dvRRdz_tmp] = 0.0;
      domURLdz[e_yFLV_tmp + 2] = iv1[dvRRdz_tmp];
    }

    for (i = 0; i < 2; i++) {
      e_yFLV_tmp = 3 * (i + 6);
      domURLdz[e_yFLV_tmp] = 0.0;
      domURLdz[e_yFLV_tmp + 1] = 0.0;
      domURLdz[e_yFLV_tmp + 2] = 0.0;
    }

    domURLdz[24] = domdu_FL[0];
    domURLdz[25] = domdu_FL[1];
    domURLdz[26] = domdu_FL[2];
    for (i = 0; i < 5; i++) {
      e_yFLV_tmp = 3 * (i + 9);
      domURLdz[e_yFLV_tmp] = 0.0;
      domURLdz[e_yFLV_tmp + 1] = 0.0;
      domURLdz[e_yFLV_tmp + 2] = 0.0;
    }

    for (i = 0; i < 3; i++) {
      domURRdz[3 * i] = 0.0;
      dvRRdz_tmp = 3 * (i + 3);
      domURRdz[dvRRdz_tmp] = iv1[3 * i];
      e_yFLV_tmp = 3 * (i + 6);
      domURRdz[e_yFLV_tmp] = 0.0;
      b_i = 3 * i + 1;
      domURRdz[b_i] = 0.0;
      domURRdz[dvRRdz_tmp + 1] = iv1[b_i];
      domURRdz[e_yFLV_tmp + 1] = 0.0;
      b_i = 3 * i + 2;
      domURRdz[b_i] = 0.0;
      domURRdz[dvRRdz_tmp + 2] = iv1[b_i];
      domURRdz[e_yFLV_tmp + 2] = 0.0;
      domURRdz[i + 27] = domdphi_RR[i];
    }

    for (i = 0; i < 4; i++) {
      dvRRdz_tmp = 3 * (i + 10);
      domURRdz[dvRRdz_tmp] = 0.0;
      domURRdz[dvRRdz_tmp + 1] = 0.0;
      domURRdz[dvRRdz_tmp + 2] = 0.0;
    }

    //  Tire road contact
    d = r_VFLV[0];
    d1 = r_VFLV[1];
    d2 = r_VFLV[2];
    d3 = omega_0UFLV[0];
    d4 = omega_0UFLV[1];
    d5 = omega_0UFLV[2];
    d6 = e_yFLV[0];
    d7 = e_yFLV[1];
    d8 = e_yFLV[2];
    r = _mm_loadu_pd(&A_0V[0]);
    r1 = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_mul_pd(r, _mm_set1_pd(v_0FLV_idx_0));
    r3 = _mm_mul_pd(r, _mm_set1_pd(d3));
    r = _mm_mul_pd(r, _mm_set1_pd(d6));
    r4 = _mm_loadu_pd(&A_0V[3]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d1));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0FLV_idx_1));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d4));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d7));
    r = _mm_add_pd(r, r4);
    r4 = _mm_loadu_pd(&A_0V[6]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d2));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0FLV_idx_2));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d5));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d8));
    r = _mm_add_pd(r, r4);
    _mm_storeu_pd(&d_A_0V[0], r);
    _mm_storeu_pd(&c_A_0V[0], r3);
    _mm_storeu_pd(&b_A_0V[0], r2);
    r = _mm_loadu_pd(&r_0V0[0]);
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&omega_0VV[0], r);
    d9 = A_0V[2];
    d10 = d9 * d;
    d11 = d9 * v_0FLV_idx_0;
    d12 = d9 * d3;
    d13 = d9 * d6;
    d9 = A_0V[5];
    d10 += d9 * d1;
    d11 += d9 * v_0FLV_idx_1;
    d12 += d9 * d4;
    d13 += d9 * d7;
    d9 = A_0V[8];
    d10 += d9 * d2;
    d11 += d9 * v_0FLV_idx_2;
    d12 += d9 * d5;
    d13 += d9 * d8;
    d_A_0V[2] = d13;
    c_A_0V[2] = d12;
    b_A_0V[2] = d11;
    omega_0VV[2] = r_0V0[2] + d10;
    gamma_FL = f_roadContact(road.x, road.y, road.Z, road.Mu, omega_0VV, b_A_0V,
      c_A_0V, d_A_0V, dvdu_FL, A_VQRL, r_stFL, dz_FL, w_FL, r_MQFL0, mu_RFL);
    d = r_VFRV[0];
    d1 = r_VFRV[1];
    d2 = r_VFRV[2];
    d3 = omega_0UFRV[0];
    d4 = omega_0UFRV[1];
    d5 = omega_0UFRV[2];
    d6 = e_yFRV[0];
    d7 = e_yFRV[1];
    d8 = e_yFRV[2];
    r = _mm_loadu_pd(&A_0V[0]);
    r1 = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_mul_pd(r, _mm_set1_pd(v_0FRV_idx_0));
    r3 = _mm_mul_pd(r, _mm_set1_pd(d3));
    r = _mm_mul_pd(r, _mm_set1_pd(d6));
    r4 = _mm_loadu_pd(&A_0V[3]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d1));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0FRV_idx_1));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d4));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d7));
    r = _mm_add_pd(r, r4);
    r4 = _mm_loadu_pd(&A_0V[6]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d2));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0FRV_idx_2));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d5));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d8));
    r = _mm_add_pd(r, r4);
    _mm_storeu_pd(&d_A_0V[0], r);
    _mm_storeu_pd(&c_A_0V[0], r3);
    _mm_storeu_pd(&b_A_0V[0], r2);
    r = _mm_loadu_pd(&r_0V0[0]);
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&omega_0VV[0], r);
    d9 = A_0V[2];
    d10 = d9 * d;
    d11 = d9 * v_0FRV_idx_0;
    d12 = d9 * d3;
    d13 = d9 * d6;
    d9 = A_0V[5];
    d10 += d9 * d1;
    d11 += d9 * v_0FRV_idx_1;
    d12 += d9 * d4;
    d13 += d9 * d7;
    d9 = A_0V[8];
    d10 += d9 * d2;
    d11 += d9 * v_0FRV_idx_2;
    d12 += d9 * d5;
    d13 += d9 * d8;
    d_A_0V[2] = d13;
    c_A_0V[2] = d12;
    b_A_0V[2] = d11;
    omega_0VV[2] = r_0V0[2] + d10;
    gamma_FR = f_roadContact(road.x, road.y, road.Z, road.Mu, omega_0VV, b_A_0V,
      c_A_0V, d_A_0V, domdu_FL, A_VRL, r_stFR, dz_FR, w_FR, r_MQFR0, mu_RFR);
    d = r_VRLV[0];
    d1 = r_VRLV[1];
    d2 = r_VRLV[2];
    d3 = omega_0URLV[0];
    d4 = omega_0URLV[1];
    d5 = omega_0URLV[2];
    d6 = e_yRLV[0];
    d7 = e_yRLV[1];
    d8 = e_yRLV[2];
    r = _mm_loadu_pd(&A_0V[0]);
    r1 = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_mul_pd(r, _mm_set1_pd(v_0RLV_idx_0));
    r3 = _mm_mul_pd(r, _mm_set1_pd(d3));
    r = _mm_mul_pd(r, _mm_set1_pd(d6));
    r4 = _mm_loadu_pd(&A_0V[3]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d1));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0RLV_idx_1));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d4));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d7));
    r = _mm_add_pd(r, r4);
    r4 = _mm_loadu_pd(&A_0V[6]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d2));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0RLV_idx_2));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d5));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d8));
    r = _mm_add_pd(r, r4);
    _mm_storeu_pd(&d_A_0V[0], r);
    _mm_storeu_pd(&c_A_0V[0], r3);
    _mm_storeu_pd(&b_A_0V[0], r2);
    r = _mm_loadu_pd(&r_0V0[0]);
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&omega_0VV[0], r);
    d9 = A_0V[2];
    d10 = d9 * d;
    d11 = d9 * v_0RLV_idx_0;
    d12 = d9 * d3;
    d13 = d9 * d6;
    d9 = A_0V[5];
    d10 += d9 * d1;
    d11 += d9 * v_0RLV_idx_1;
    d12 += d9 * d4;
    d13 += d9 * d7;
    d9 = A_0V[8];
    d10 += d9 * d2;
    d11 += d9 * v_0RLV_idx_2;
    d12 += d9 * d5;
    d13 += d9 * d8;
    d_A_0V[2] = d13;
    c_A_0V[2] = d12;
    b_A_0V[2] = d11;
    omega_0VV[2] = r_0V0[2] + d10;
    gamma_RL = f_roadContact(road.x, road.y, road.Z, road.Mu, omega_0VV, b_A_0V,
      c_A_0V, d_A_0V, dvdphi_RR, A_VQRR, r_stRL, dz_RL, w_RL, r_MQRL0, mu_RRL);
    d = r_VRRV[0];
    d1 = r_VRRV[1];
    d2 = r_VRRV[2];
    d3 = omega_0URRV[0];
    d4 = omega_0URRV[1];
    d5 = omega_0URRV[2];
    d6 = e_yRRV[0];
    d7 = e_yRRV[1];
    d8 = e_yRRV[2];
    r = _mm_loadu_pd(&A_0V[0]);
    r1 = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_mul_pd(r, _mm_set1_pd(v_0RRV_idx_0));
    r3 = _mm_mul_pd(r, _mm_set1_pd(d3));
    r = _mm_mul_pd(r, _mm_set1_pd(d6));
    r4 = _mm_loadu_pd(&A_0V[3]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d1));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0RRV_idx_1));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d4));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d7));
    r = _mm_add_pd(r, r4);
    r4 = _mm_loadu_pd(&A_0V[6]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d2));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0RRV_idx_2));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d5));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d8));
    r = _mm_add_pd(r, r4);
    _mm_storeu_pd(&d_A_0V[0], r);
    _mm_storeu_pd(&c_A_0V[0], r3);
    _mm_storeu_pd(&b_A_0V[0], r2);
    r = _mm_loadu_pd(&r_0V0[0]);
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&omega_0VV[0], r);
    d9 = A_0V[2];
    d10 = d9 * d;
    d11 = d9 * v_0RRV_idx_0;
    d12 = d9 * d3;
    d13 = d9 * d6;
    d9 = A_0V[5];
    d10 += d9 * d1;
    d11 += d9 * v_0RRV_idx_1;
    d12 += d9 * d4;
    d13 += d9 * d7;
    d9 = A_0V[8];
    d10 += d9 * d2;
    d11 += d9 * v_0RRV_idx_2;
    d12 += d9 * d5;
    d13 += d9 * d8;
    d_A_0V[2] = d13;
    c_A_0V[2] = d12;
    b_A_0V[2] = d11;
    omega_0VV[2] = r_0V0[2] + d10;
    gamma_RR = f_roadContact(road.x, road.y, road.Z, road.Mu, omega_0VV, b_A_0V,
      c_A_0V, d_A_0V, domdphi_RR, A_z_tmp, r_stRR, dz_RR, w_RR, r_MQRR0, mu_RRR);
    for (i = 0; i < 3; i++) {
      d = A_y_tmp[i];
      d1 = A_y_tmp[i + 3];
      d2 = A_y_tmp[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        i2 = 3 * i1 + 1;
        e_yFLV_tmp = 3 * i1 + 2;
        dvRRdz_tmp = i + 3 * i1;
        A_VQFR[dvRRdz_tmp] = (d * A_VRL[3 * i1] + d1 * A_VRL[i2]) + d2 *
          A_VRL[e_yFLV_tmp];
        A_0V[dvRRdz_tmp] = (d * A_VQRL[3 * i1] + d1 * A_VQRL[i2]) + d2 *
          A_VQRL[e_yFLV_tmp];
      }
    }

    for (i = 0; i < 3; i++) {
      d = A_y_tmp[i];
      d1 = A_y_tmp[i + 3];
      d2 = A_y_tmp[i + 6];
      for (i1 = 0; i1 < 3; i1++) {
        A_VQRL[i + 3 * i1] = (d * A_VQRR[3 * i1] + d1 * A_VQRR[3 * i1 + 1]) + d2
          * A_VQRR[3 * i1 + 2];
      }
    }

    //  Tire forces
    //  TMeasy
    for (i = 0; i < 3; i++) {
      d = A_y_tmp[i];
      d1 = A_y_tmp[i + 3];
      d2 = A_y_tmp[i + 6];
      d3 = 0.0;
      for (i1 = 0; i1 < 3; i1++) {
        e_yFLV_tmp = i + 3 * i1;
        A_VQRR[e_yFLV_tmp] = (d * A_z_tmp[3 * i1] + d1 * A_z_tmp[3 * i1 + 1]) +
          d2 * A_z_tmp[3 * i1 + 2];
        d3 += I_UFLV_tmp[e_yFLV_tmp] * omega_0WFLV[i1];
      }

      d_A_0V[i] = d3;
    }

    double out_aero[8];
    double s_FL[4];
    f_tireTMeasy(dvdu_FL, d_A_0V, r_stFL, dz_FL, w_FL, gamma_FL, 0.7 *
                 inputs_mu_N * mu_RFL, &x[28], dvdphi_FR, T_TFL, s_FL, x_TFL_dot,
                 A_x_tmp_tmp, T_BFL, T_BFR, T_BRL, T_BRR);
    d = omega_0WFRV[0];
    d1 = omega_0WFRV[1];
    d2 = omega_0WFRV[2];
    r = _mm_loadu_pd(&I_UFRV_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&I_UFRV_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&I_UFRV_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&b_I_UFRV_tmp[0], r);
    b_I_UFRV_tmp[2] = (I_UFRV_tmp[2] * d + I_UFRV_tmp[5] * d1) + I_UFRV_tmp[8] *
      d2;
    f_tireTMeasy(domdu_FL, b_I_UFRV_tmp, r_stFR, dz_FR, w_FR, gamma_FR, 0.7 *
                 inputs_mu_N * mu_RFR, &x[31], domdphi_FR, T_TFR, s_FL,
                 x_TFR_dot, A_x_tmp_tmp, T_BFL, T_BFR, T_BRL, T_BRR);
    d = omega_0WRLV[0];
    d1 = omega_0WRLV[1];
    d2 = omega_0WRLV[2];
    r = _mm_loadu_pd(&I_URLV_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&I_URLV_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&I_URLV_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&omega_0VV[0], r);
    omega_0VV[2] = (I_URLV_tmp[2] * d + I_URLV_tmp[5] * d1) + I_URLV_tmp[8] * d2;
    f_tireTMeasy(dvdphi_RR, omega_0VV, r_stRL, dz_RL, w_RL, gamma_RL, 0.7 *
                 inputs_mu_N * mu_RRL, &x[34], dvdphi_FL, T_TRL, s_FL, x_TRL_dot,
                 A_x_tmp_tmp, T_BFL, T_BFR, T_BRL, T_BRR);
    d = omega_0WRRV[0];
    d1 = omega_0WRRV[1];
    d2 = omega_0WRRV[2];
    r = _mm_loadu_pd(&I_URRV_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&I_URRV_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&I_URRV_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&omega_0VV[0], r);
    omega_0VV[2] = (I_URRV_tmp[2] * d + I_URRV_tmp[5] * d1) + I_URRV_tmp[8] * d2;
    f_tireTMeasy(domdphi_RR, omega_0VV, r_stRR, dz_RR, w_RR, gamma_RR, 0.7 *
                 inputs_mu_N * mu_RRR, &x[37], domdphi_FL, T_TRR, s_FL,
                 x_TRR_dot, T_BFL, T_BFR, T_BRL, T_BRR, A_x_tmp_tmp);

    //  Suspension forces
    f_suspensionForces(r_VFLV, r_VFRV, r_VRLV, r_VRRV, r_VFLV_dot, r_VFRV_dot,
                       r_VRLV_dot, r_VRRV_dot, veh.z_cg, veh.c_RollFA,
                       veh.c_RollRA, veh.F_SFL0, veh.F_SFR0, veh.F_SRL0,
                       veh.F_SRR0, A_x_tmp_tmp, T_BFL, T_BFR, T_BRL, T_BRR,
                       gamma_FL, r_stFL, dz_FL, w_FL, mu_RFL, gamma_FR, r_stFR,
                       dz_FR, w_FR, mu_RFR, gamma_RR, r_stRR, dz_RR, w_RR);

    //  Aerodynamic Forces
    d = v_0FAV_tmp * v_0FAV_tmp;
    f_aerodynamics(sqrt((d + v_0FAV[1] * v_0FAV[1]) + v_0FAV[2] * v_0FAV[2]),
                   v_0FAV, sqrt((d + v_0RAV[1] * v_0RAV[1]) + v_0RAV[2] *
      v_0RAV[2]), v_0RAV, dv41, veh.C_LFA, veh.C_LRA, veh.C_D, dvdu_FL, domdu_FL,
                   dvdphi_RR, domdphi_RR, omega_0VV, F_ARAV, out_aero);

    //  Applied forces & torques
    //  Wheel rot. friction damping coef.
    //  Powertrain
    f_powertrain(x[24], x[25], x[26], x[27], inputs_T_MCmdFL, inputs_T_MCmdFR,
                 inputs_T_MCmdRL, inputs_T_MCmdRR, veh.T_MMax, veh.i_G, T_BFL,
                 T_BFR, T_BRL, A_x_tmp_tmp, gamma_FL, r_stFL, dz_FL);

    //  Brake torques
    T_BFL = -39.111507258094768 * x[24];
    u = inputs_p_BFA * 2.62119;

    //  Limit each absolute value of val0 to limit
    if (fabs(T_BFL) > u) {
      if (rtIsNaN(T_BFL)) {
        T_BRR = rtNaN;
      } else if (T_BFL < 0.0) {
        T_BRR = -1.0;
      } else {
        T_BRR = (T_BFL > 0.0);
      }

      T_BFL = u * T_BRR;
    }

    T_BFR = -39.111507258094768 * x[25];

    //  Limit each absolute value of val0 to limit
    if (fabs(T_BFR) > u) {
      if (rtIsNaN(T_BFR)) {
        T_BRR = rtNaN;
      } else if (T_BFR < 0.0) {
        T_BRR = -1.0;
      } else {
        T_BRR = (T_BFR > 0.0);
      }

      T_BFR = u * T_BRR;
    }

    T_BRL = -39.111507258094768 * x[26];
    u = inputs_p_BRA * 2.4887362499999997;

    //  Limit each absolute value of val0 to limit
    if (fabs(T_BRL) > u) {
      if (rtIsNaN(T_BRL)) {
        T_BRR = rtNaN;
      } else if (T_BRL < 0.0) {
        T_BRR = -1.0;
      } else {
        T_BRR = (T_BRL > 0.0);
      }

      T_BRL = u * T_BRR;
    }

    T_BRR = -39.111507258094768 * x[27];

    //  Limit each absolute value of val0 to limit
    if (fabs(T_BRR) > u) {
      if (rtIsNaN(T_BRR)) {
        T_BRR = rtNaN;
      } else if (T_BRR < 0.0) {
        T_BRR = -1.0;
      } else {
        T_BRR = (T_BRR > 0.0);
      }

      T_BRR *= u;
    }

    //  Applied torques
    //  Wheels
    b_tmp = T_BFL + A_x_tmp_tmp;

    //  Wheel FL
    b_b_tmp = T_BFR + gamma_FL;

    //  Wheel FR
    c_b_tmp = T_BRL + r_stFL;

    //  Wheel RL
    v_0FAV_tmp = T_BRR + dz_FL;

    //  Wheel RR
    //  Hub-Motors
    //  Uprights
    //  Upright FL
    //  Upright FR
    //  Upright RL
    //  Upright RR
    //  Chassis
    //  Applied forces
    a = ((gamma_RR + r_stRR) + dz_RR) + w_RR;

    //  Chassis
    //  FL
    //  FR
    //  RL
    //  RR
    //  Applied force vector
    for (b_i = 0; b_i < 3; b_i++) {
      T_WRR_tmp[b_i] = (A_VQRR[b_i] * domdphi_FL[0] + A_VQRR[b_i + 3] *
                        domdphi_FL[1]) + A_VQRR[b_i + 6] * domdphi_FL[2];
      T_WRL_tmp[b_i] = (A_VQRL[b_i] * dvdphi_FL[0] + A_VQRL[b_i + 3] *
                        dvdphi_FL[1]) + A_VQRL[b_i + 6] * dvdphi_FL[2];
      T_WFR_tmp[b_i] = (A_VQFR[b_i] * domdphi_FR[0] + A_VQFR[b_i + 3] *
                        domdphi_FR[1]) + A_VQFR[b_i + 6] * domdphi_FR[2];
      T_WFL_tmp[b_i] = (A_0V[b_i] * dvdphi_FR[0] + A_0V[b_i + 3] * dvdphi_FR[1])
        + A_0V[b_i + 6] * dvdphi_FR[2];
      i = static_cast<int>(dv41[b_i]);
      T_C_tmp[b_i] = gamma_RR * static_cast<double>(i);
      b_T_C_tmp[b_i] = r_stRR * static_cast<double>(i);
      c_T_C_tmp[b_i] = dz_RR * static_cast<double>(i);
      d_T_C_tmp[b_i] = w_RR * static_cast<double>(i);
      for (i = 0; i < 14; i++) {
        e_yFLV_tmp = b_i + 3 * i;
        dvRRdz_tmp = i + 14 * b_i;
        y_tmp[dvRRdz_tmp] = dvFRdz[e_yFLV_tmp];
        b_y_tmp[dvRRdz_tmp] = dvRLdz[e_yFLV_tmp];
        c_y_tmp[dvRRdz_tmp] = dvRRdz[e_yFLV_tmp];
        d_y_tmp[dvRRdz_tmp] = domUFLdz[e_yFLV_tmp];
        e_y_tmp[dvRRdz_tmp] = domUFRdz[e_yFLV_tmp];
        f_y_tmp[dvRRdz_tmp] = domURLdz[e_yFLV_tmp];
        g_y_tmp[dvRRdz_tmp] = domURRdz[e_yFLV_tmp];
        h_y_tmp[dvRRdz_tmp] = domWFLdz[e_yFLV_tmp];
        i_y_tmp[dvRRdz_tmp] = domWFRdz[e_yFLV_tmp];
        j_y_tmp[dvRRdz_tmp] = domWRLdz[e_yFLV_tmp];
        k_y_tmp[dvRRdz_tmp] = domWRRdz[e_yFLV_tmp];
      }
    }

    //       % Chassis force
    //       % Chassis torque
    //     % Force FL
    //     % Force FR
    //     % Force RL
    //     % Force RR
    //  % Upright torque FL
    //  % Upright torque FR
    //  % Upright torque RL
    //  % Upright torque RR
    //  % Wheel torque FL
    //  % Wheel torque FR
    //  % Wheel torque RL
    //  Wheel torque RR
    //  Generalized force vector
    for (i = 0; i < 42; i++) {
      q_tmp[i] = static_cast<double>(iv2[i]) * veh.m_C;
    }

    for (i = 0; i < 14; i++) {
      b_q_tmp[i] = dvFLdz[3 * i] * 11.6;
      b_q_tmp[i + 14] = dvFLdz[3 * i + 1] * 11.6;
      b_q_tmp[i + 28] = dvFLdz[3 * i + 2] * 11.6;
    }

    r = _mm_set1_pd(11.6);
    for (i = 0; i <= 40; i += 2) {
      r1 = _mm_loadu_pd(&y_tmp[i]);
      _mm_storeu_pd(&c_q_tmp[i], _mm_mul_pd(r1, r));
      r1 = _mm_loadu_pd(&b_y_tmp[i]);
      _mm_storeu_pd(&d_q_tmp[i], _mm_mul_pd(r1, r));
      r1 = _mm_loadu_pd(&c_y_tmp[i]);
      _mm_storeu_pd(&e_q_tmp[i], _mm_mul_pd(r1, r));
    }

    //  % Chassis force
    //  % Chassis torque
    //  % Force FL
    //  % Force FR
    //  % Force RL
    //  % Force RR
    //  % Upright torque FL
    //  % Upright torque FR
    //  % Upright torque RL
    //  % Upright torque RR
    //  % Wheel torque FL
    //  % Wheel torque FR
    //  % Wheel torque RL
    //  Wheel torque RR
    //  Generalized mass matrix
    //  % Chassis
    //  % FL
    //  % FR
    //  % RL
    //  RR
    //  Solve for state derivatives
    //  Generalized coordinates derivatives
    //  Generalized velocities derivatives
    d = r_MQFL0[0];
    d1 = r_MQFL0[1];
    d2 = r_MQFL0[2];
    r = _mm_loadu_pd(&A_y_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&A_y_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&A_y_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&b_a[0], r);
    b_a[2] = (A_y_tmp[2] * d + A_y_tmp[5] * d1) + A_y_tmp[8] * d2;
    d = r_MQFR0[0];
    d1 = r_MQFR0[1];
    d2 = r_MQFR0[2];
    d3 = r_MQRL0[0];
    d4 = r_MQRL0[1];
    d5 = r_MQRL0[2];
    d6 = r_MQRR0[0];
    d7 = r_MQRR0[1];
    d8 = r_MQRR0[2];
    d9 = omega_0WRRV[0];
    d10 = omega_0WRRV[1];
    d11 = omega_0WRRV[2];
    d12 = omega_0WRLV[0];
    d13 = omega_0WRLV[1];
    u = omega_0WRLV[2];
    A_x_tmp_tmp = omega_0WFRV[0];
    T_BFL = omega_0WFRV[1];
    T_BFR = omega_0WFRV[2];
    T_BRL = omega_0WFLV[0];
    T_BRR = omega_0WFLV[1];
    gamma_FR = omega_0WFLV[2];
    r_stFR = omega_0URRV[0];
    dz_FR = omega_0URRV[1];
    w_FR = omega_0URRV[2];
    mu_RFR = omega_0URLV[0];
    gamma_RR = omega_0URLV[1];
    r_stRR = omega_0URLV[2];
    dz_RR = omega_0UFRV[0];
    w_RR = omega_0UFRV[1];
    gamma_FL = omega_0UFRV[2];
    r_stFL = omega_0UFLV[0];
    dz_FL = omega_0UFLV[1];
    w_FL = omega_0UFLV[2];
    mu_RFL = x[17];
    mu_RRR = x[18];
    gamma_RL = x[19];
    for (i = 0; i < 3; i++) {
      r_stRL = A_y_tmp[i];
      dz_RL = r_stRL * d;
      w_RL = r_stRL * d3;
      mu_RRL = r_stRL * d6;
      r_stRL = A_y_tmp[i + 3];
      dz_RL += r_stRL * d1;
      w_RL += r_stRL * d4;
      mu_RRL += r_stRL * d7;
      r_stRL = A_y_tmp[i + 6];
      dz_RL += r_stRL * d2;
      w_RL += r_stRL * d5;
      mu_RRL += r_stRL * d8;
      r_0V0[i] = (I_WRRV[i] * d9 + I_WRRV[i + 3] * d10) + I_WRRV[i + 6] * d11;
      domdphi_FL[i] = (I_WRLV[i] * d12 + I_WRLV[i + 3] * d13) + I_WRLV[i + 6] *
        u;
      dvdphi_FL[i] = (I_WFRV[i] * A_x_tmp_tmp + I_WFRV[i + 3] * T_BFL) +
        I_WFRV[i + 6] * T_BFR;
      domdphi_FR[i] = (I_WFLV[i] * T_BRL + I_WFLV[i + 3] * T_BRR) + I_WFLV[i + 6]
        * gamma_FR;
      dvdphi_FR[i] = (I_URRV[i] * r_stFR + I_URRV[i + 3] * dz_FR) + I_URRV[i + 6]
        * w_FR;
      domdphi_RR[i] = (I_URLV[i] * mu_RFR + I_URLV[i + 3] * gamma_RR) + I_URLV[i
        + 6] * r_stRR;
      dvdphi_RR[i] = (I_UFRV[i] * dz_RR + I_UFRV[i + 3] * w_RR) + I_UFRV[i + 6] *
        gamma_FL;
      domdu_FL[i] = (I_UFLV[i] * r_stFL + I_UFLV[i + 3] * dz_FL) + I_UFLV[i + 6]
        * w_FL;
      dvdu_FL[i] = (static_cast<double>(iv3[i]) * mu_RFL + static_cast<double>
                    (iv3[i + 3]) * mu_RRR) + static_cast<double>(iv3[i + 6]) *
        gamma_RL;
      v_0FAV[i] = mu_RRL;
      v_0RAV[i] = w_RL;
      r_MQFL0[i] = dz_RL;
    }

    memcpy(&dv[0], &x[14], 14U * sizeof(double));
    coder::mldivide(K, dv);
    r = _mm_loadu_pd(&omega_0VV[0]);
    r1 = _mm_loadu_pd(&F_ARAV[0]);
    _mm_storeu_pd(&r_MQRR0[0], _mm_add_pd(_mm_add_pd(r, r1), _mm_mul_pd
      (_mm_set1_pd(a), _mm_loadu_pd(&dv41[0]))));
    r_MQRR0[2] = (omega_0VV[2] + F_ARAV[2]) + a;
    r_MQFR0[0] = r_VFLV[1] * T_C_tmp[2] - T_C_tmp[1] * r_VFLV[2];
    r_MQFR0[1] = T_C_tmp[0] * r_VFLV[2] - r_VFLV[0] * T_C_tmp[2];
    r_MQFR0[2] = r_VFLV[0] * T_C_tmp[1] - T_C_tmp[0] * r_VFLV[1];
    b_I_UFRV_tmp[0] = r_VFRV[1] * b_T_C_tmp[2] - b_T_C_tmp[1] * r_VFRV[2];
    b_I_UFRV_tmp[1] = b_T_C_tmp[0] * r_VFRV[2] - r_VFRV[0] * b_T_C_tmp[2];
    b_I_UFRV_tmp[2] = r_VFRV[0] * b_T_C_tmp[1] - b_T_C_tmp[0] * r_VFRV[1];
    d_A_0V[0] = r_VRLV[1] * c_T_C_tmp[2] - c_T_C_tmp[1] * r_VRLV[2];
    d_A_0V[1] = c_T_C_tmp[0] * r_VRLV[2] - r_VRLV[0] * c_T_C_tmp[2];
    d_A_0V[2] = r_VRLV[0] * c_T_C_tmp[1] - c_T_C_tmp[0] * r_VRLV[1];
    c_A_0V[0] = r_VRRV[1] * d_T_C_tmp[2] - d_T_C_tmp[1] * r_VRRV[2];
    c_A_0V[1] = d_T_C_tmp[0] * r_VRRV[2] - r_VRRV[0] * d_T_C_tmp[2];
    c_A_0V[2] = r_VRRV[0] * d_T_C_tmp[1] - d_T_C_tmp[0] * r_VRRV[1];
    b_A_0V[0] = 0.0 * omega_0VV[2] - omega_0VV[1] * -veh.z_cg;
    b_A_0V[1] = omega_0VV[0] * -veh.z_cg - veh.l_FA * omega_0VV[2];
    b_A_0V[2] = veh.l_FA * omega_0VV[1] - omega_0VV[0] * 0.0;
    omega_0VV[0] = 0.0 * F_ARAV[2] - F_ARAV[1] * -veh.z_cg;
    omega_0VV[1] = F_ARAV[0] * -veh.z_cg - -veh.l_RA * F_ARAV[2];
    omega_0VV[2] = -veh.l_RA * F_ARAV[1] - F_ARAV[0] * 0.0;
    r = _mm_loadu_pd(&r_MQFR0[0]);
    r1 = _mm_loadu_pd(&b_I_UFRV_tmp[0]);
    r2 = _mm_loadu_pd(&d_A_0V[0]);
    r3 = _mm_loadu_pd(&c_A_0V[0]);
    r4 = _mm_loadu_pd(&b_A_0V[0]);
    r5 = _mm_loadu_pd(&omega_0VV[0]);
    _mm_storeu_pd(&r_MQFR0[0], _mm_add_pd(_mm_add_pd(_mm_add_pd(_mm_add_pd
      (_mm_add_pd(r, r1), r2), r3), r4), r5));
    r_MQFR0[2] = ((((r_MQFR0[2] + b_I_UFRV_tmp[2]) + d_A_0V[2]) + c_A_0V[2]) +
                  b_A_0V[2]) + omega_0VV[2];
    r = _mm_loadu_pd(&T_WFL_tmp[0]);
    r1 = _mm_loadu_pd(&T_C_tmp[0]);
    _mm_storeu_pd(&T_C_tmp[0], _mm_sub_pd(r, r1));
    r = _mm_loadu_pd(&T_WFR_tmp[0]);
    r1 = _mm_loadu_pd(&b_T_C_tmp[0]);
    _mm_storeu_pd(&b_T_C_tmp[0], _mm_sub_pd(r, r1));
    T_C_tmp[2] = T_WFL_tmp[2] - T_C_tmp[2];
    b_T_C_tmp[2] = T_WFR_tmp[2] - b_T_C_tmp[2];
    r = _mm_loadu_pd(&T_WRL_tmp[0]);
    r1 = _mm_loadu_pd(&c_T_C_tmp[0]);
    _mm_storeu_pd(&c_T_C_tmp[0], _mm_sub_pd(r, r1));
    r = _mm_loadu_pd(&T_WRR_tmp[0]);
    r1 = _mm_loadu_pd(&d_T_C_tmp[0]);
    _mm_storeu_pd(&d_T_C_tmp[0], _mm_sub_pd(r, r1));
    c_T_C_tmp[2] = T_WRL_tmp[2] - c_T_C_tmp[2];
    d_T_C_tmp[2] = T_WRR_tmp[2] - d_T_C_tmp[2];
    r = _mm_loadu_pd(&e_yFLV[0]);
    r1 = _mm_set1_pd(-1.0);
    _mm_storeu_pd(&b_A_0V[0], _mm_mul_pd(_mm_mul_pd(r, r1), _mm_set1_pd(b_tmp)));
    r = _mm_loadu_pd(&e_yFRV[0]);
    _mm_storeu_pd(&omega_0VV[0], _mm_mul_pd(_mm_mul_pd(r, r1), _mm_set1_pd
      (b_b_tmp)));
    d = r_MQFR0[0];
    d1 = r_MQFR0[1];
    d2 = r_MQFR0[2];
    d3 = r_MQRR0[0];
    d4 = r_MQRR0[1];
    d5 = r_MQRR0[2];
    d6 = b_T_C_tmp[0];
    d7 = b_T_C_tmp[1];
    d8 = b_T_C_tmp[2];
    d9 = T_C_tmp[0];
    d10 = T_C_tmp[1];
    d11 = T_C_tmp[2];
    d12 = c_T_C_tmp[0];
    d13 = c_T_C_tmp[1];
    u = c_T_C_tmp[2];
    A_x_tmp_tmp = d_T_C_tmp[0];
    T_BFL = d_T_C_tmp[1];
    T_BFR = d_T_C_tmp[2];
    T_BRL = b_A_0V[0];
    T_BRR = b_A_0V[1];
    gamma_FR = -e_yFLV[2] * b_tmp;
    r_stFR = omega_0VV[0];
    dz_FR = omega_0VV[1];
    w_FR = -e_yFRV[2] * b_b_tmp;
    for (i = 0; i < 14; i++) {
      dv2[i] = (((((((static_cast<double>(iv2[i]) * d3 + static_cast<double>
                      (iv2[i + 14]) * d4) + static_cast<double>(iv2[i + 28]) *
                     d5) + ((static_cast<double>(iv4[i]) * d + static_cast<
        double>(iv4[i + 14]) * d1) + static_cast<double>(iv4[i + 28]) * d2)) +
                   ((dvFLdz[3 * i] * d9 + dvFLdz[3 * i + 1] * d10) + dvFLdz[3 *
                    i + 2] * d11)) + ((y_tmp[i] * d6 + y_tmp[i + 14] * d7) +
        y_tmp[i + 28] * d8)) + ((b_y_tmp[i] * d12 + b_y_tmp[i + 14] * d13) +
                  b_y_tmp[i + 28] * u)) + ((c_y_tmp[i] * A_x_tmp_tmp + c_y_tmp[i
                  + 14] * T_BFL) + c_y_tmp[i + 28] * T_BFR)) + ((d_y_tmp[i] *
        T_BRL + d_y_tmp[i + 14] * T_BRR) + d_y_tmp[i + 28] * gamma_FR);
      l_y_tmp[i] = (e_y_tmp[i] * r_stFR + e_y_tmp[i + 14] * dz_FR) + e_y_tmp[i +
        28] * w_FR;
    }

    r = _mm_loadu_pd(&e_yRLV[0]);
    _mm_storeu_pd(&c_A_0V[0], _mm_mul_pd(_mm_mul_pd(r, r1), _mm_set1_pd(c_b_tmp)));
    r = _mm_loadu_pd(&e_yRRV[0]);
    _mm_storeu_pd(&b_A_0V[0], _mm_mul_pd(_mm_mul_pd(r, r1), _mm_set1_pd
      (v_0FAV_tmp)));
    d = c_A_0V[0];
    d1 = c_A_0V[1];
    d2 = -e_yRLV[2] * c_b_tmp;
    d3 = b_A_0V[0];
    d4 = b_A_0V[1];
    d5 = -e_yRRV[2] * v_0FAV_tmp;
    for (i = 0; i <= 12; i += 2) {
      r = _mm_loadu_pd(&dv2[i]);
      r2 = _mm_loadu_pd(&l_y_tmp[i]);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&f_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
      r3 = _mm_loadu_pd(&f_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&f_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&dv1[i], r);
      r = _mm_loadu_pd(&g_y_tmp[i]);
      r = _mm_mul_pd(r, _mm_set1_pd(d3));
      r2 = _mm_loadu_pd(&g_y_tmp[i + 14]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d4));
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&g_y_tmp[i + 28]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d5));
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&l_y_tmp[i], r);
    }

    omega_0VV[0] = b_a[1] * T_WFL_tmp[2] - T_WFL_tmp[1] * b_a[2];
    omega_0VV[1] = T_WFL_tmp[0] * b_a[2] - b_a[0] * T_WFL_tmp[2];
    omega_0VV[2] = b_a[0] * T_WFL_tmp[1] - T_WFL_tmp[0] * b_a[1];
    b_I_UFRV_tmp[0] = r_MQFL0[1] * T_WFR_tmp[2] - T_WFR_tmp[1] * r_MQFL0[2];
    b_I_UFRV_tmp[1] = T_WFR_tmp[0] * r_MQFL0[2] - r_MQFL0[0] * T_WFR_tmp[2];
    b_I_UFRV_tmp[2] = r_MQFL0[0] * T_WFR_tmp[1] - T_WFR_tmp[0] * r_MQFL0[1];
    d = T_TFL[0];
    d1 = T_TFL[1];
    d2 = T_TFL[2];
    d3 = T_TFR[0];
    d4 = T_TFR[1];
    d5 = T_TFR[2];
    r = _mm_loadu_pd(&e_yFLV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(b_tmp));
    r2 = _mm_loadu_pd(&omega_0VV[0]);
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_0V[0]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
    r3 = _mm_loadu_pd(&A_0V[3]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
    r2 = _mm_add_pd(r2, r3);
    r3 = _mm_loadu_pd(&A_0V[6]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
    r2 = _mm_add_pd(r2, r3);
    r = _mm_add_pd(r, r2);
    _mm_storeu_pd(&b_A_0V[0], r);
    r = _mm_loadu_pd(&e_yFRV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(b_b_tmp));
    r2 = _mm_loadu_pd(&b_I_UFRV_tmp[0]);
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_VQFR[0]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d3));
    r3 = _mm_loadu_pd(&A_VQFR[3]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d4));
    r2 = _mm_add_pd(r2, r3);
    r3 = _mm_loadu_pd(&A_VQFR[6]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d5));
    r2 = _mm_add_pd(r2, r3);
    r = _mm_add_pd(r, r2);
    _mm_storeu_pd(&omega_0VV[0], r);
    b_A_0V[2] = (e_yFLV[2] * b_tmp + omega_0VV[2]) + ((A_0V[2] * d + A_0V[5] *
      d1) + A_0V[8] * d2);
    omega_0VV[2] = (e_yFRV[2] * b_b_tmp + b_I_UFRV_tmp[2]) + ((A_VQFR[2] * d3 +
      A_VQFR[5] * d4) + A_VQFR[8] * d5);
    d = b_A_0V[0];
    d1 = b_A_0V[1];
    d2 = b_A_0V[2];
    d3 = omega_0VV[0];
    d4 = omega_0VV[1];
    d5 = omega_0VV[2];
    for (i = 0; i <= 12; i += 2) {
      r = _mm_loadu_pd(&dv1[i]);
      r2 = _mm_loadu_pd(&l_y_tmp[i]);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&h_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
      r3 = _mm_loadu_pd(&h_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&h_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&dv2[i], r);
      r = _mm_loadu_pd(&i_y_tmp[i]);
      r = _mm_mul_pd(r, _mm_set1_pd(d3));
      r2 = _mm_loadu_pd(&i_y_tmp[i + 14]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d4));
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&i_y_tmp[i + 28]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d5));
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&l_y_tmp[i], r);
    }

    b_A_0V[0] = v_0RAV[1] * T_WRL_tmp[2] - T_WRL_tmp[1] * v_0RAV[2];
    b_A_0V[1] = T_WRL_tmp[0] * v_0RAV[2] - v_0RAV[0] * T_WRL_tmp[2];
    b_A_0V[2] = v_0RAV[0] * T_WRL_tmp[1] - T_WRL_tmp[0] * v_0RAV[1];
    omega_0VV[0] = v_0FAV[1] * T_WRR_tmp[2] - T_WRR_tmp[1] * v_0FAV[2];
    omega_0VV[1] = T_WRR_tmp[0] * v_0FAV[2] - v_0FAV[0] * T_WRR_tmp[2];
    omega_0VV[2] = v_0FAV[0] * T_WRR_tmp[1] - T_WRR_tmp[0] * v_0FAV[1];
    d = T_TRL[0];
    d1 = T_TRL[1];
    d2 = T_TRL[2];
    d3 = T_TRR[0];
    d4 = T_TRR[1];
    d5 = T_TRR[2];
    r = _mm_loadu_pd(&e_yRLV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(c_b_tmp));
    r2 = _mm_loadu_pd(&b_A_0V[0]);
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_VQRL[0]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
    r3 = _mm_loadu_pd(&A_VQRL[3]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
    r2 = _mm_add_pd(r2, r3);
    r3 = _mm_loadu_pd(&A_VQRL[6]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
    r2 = _mm_add_pd(r2, r3);
    r = _mm_add_pd(r, r2);
    _mm_storeu_pd(&c_A_0V[0], r);
    r = _mm_loadu_pd(&e_yRRV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(v_0FAV_tmp));
    r2 = _mm_loadu_pd(&omega_0VV[0]);
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_VQRR[0]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d3));
    r3 = _mm_loadu_pd(&A_VQRR[3]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d4));
    r2 = _mm_add_pd(r2, r3);
    r3 = _mm_loadu_pd(&A_VQRR[6]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d5));
    r2 = _mm_add_pd(r2, r3);
    r = _mm_add_pd(r, r2);
    _mm_storeu_pd(&b_A_0V[0], r);
    c_A_0V[2] = (e_yRLV[2] * c_b_tmp + b_A_0V[2]) + ((A_VQRL[2] * d + A_VQRL[5] *
      d1) + A_VQRL[8] * d2);
    b_A_0V[2] = (e_yRRV[2] * v_0FAV_tmp + omega_0VV[2]) + ((A_VQRR[2] * d3 +
      A_VQRR[5] * d4) + A_VQRR[8] * d5);
    d = c_A_0V[0];
    d1 = c_A_0V[1];
    d2 = c_A_0V[2];
    d3 = b_A_0V[0];
    d4 = b_A_0V[1];
    d5 = b_A_0V[2];
    d6 = g_V[0] - (x[16] * x[18] - x[15] * x[19]);
    d7 = g_V[1] - (x[14] * x[19] - x[16] * x[17]);
    d8 = g_V[2] - (x[15] * x[17] - x[14] * x[18]);
    for (i = 0; i <= 12; i += 2) {
      r = _mm_loadu_pd(&dv2[i]);
      r2 = _mm_loadu_pd(&l_y_tmp[i]);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&j_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
      r3 = _mm_loadu_pd(&j_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&j_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&k_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d3));
      r3 = _mm_loadu_pd(&k_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d4));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&k_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d5));
      r2 = _mm_add_pd(r2, r3);
      _mm_storeu_pd(&l_y_tmp[i], r2);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&q_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d6));
      r3 = _mm_loadu_pd(&q_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d7));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&q_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d8));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&dv2[i], r);
    }

    d = -(dvdu_FL[2] * x[18] - dvdu_FL[1] * x[19]);
    d1 = -(dvdu_FL[0] * x[19] - dvdu_FL[2] * x[17]);
    d2 = -(dvdu_FL[1] * x[17] - dvdu_FL[0] * x[18]);
    for (i = 0; i < 14; i++) {
      dv1[i] = (static_cast<double>(iv4[i]) * d + static_cast<double>(iv4[i + 14])
                * d1) + static_cast<double>(iv4[i + 28]) * d2;
    }

    c_A_0V[0] = r_VFLV_dot[2] * x[18] - r_VFLV_dot[1] * x[19];
    c_A_0V[1] = r_VFLV_dot[0] * x[19] - r_VFLV_dot[2] * x[17];
    c_A_0V[2] = r_VFLV_dot[1] * x[17] - r_VFLV_dot[0] * x[18];
    d_A_0V[0] = x[18] * v_0FLV_idx_2 - v_0FLV_idx_1 * x[19];
    d_A_0V[1] = v_0FLV_idx_0 * x[19] - x[17] * v_0FLV_idx_2;
    d_A_0V[2] = x[17] * v_0FLV_idx_1 - v_0FLV_idx_0 * x[18];
    r = _mm_loadu_pd(&c_A_0V[0]);
    r2 = _mm_loadu_pd(&d_A_0V[0]);
    r3 = _mm_loadu_pd(&g_V[0]);
    _mm_storeu_pd(&c_A_0V[0], _mm_sub_pd(r3, _mm_add_pd(r, r2)));
    c_A_0V[2] = g_V[2] - (c_A_0V[2] + d_A_0V[2]);
    d_A_0V[0] = r_VFRV_dot[2] * x[18] - r_VFRV_dot[1] * x[19];
    d_A_0V[1] = r_VFRV_dot[0] * x[19] - r_VFRV_dot[2] * x[17];
    d_A_0V[2] = r_VFRV_dot[1] * x[17] - r_VFRV_dot[0] * x[18];
    b_A_0V[0] = x[18] * v_0FRV_idx_2 - v_0FRV_idx_1 * x[19];
    b_A_0V[1] = v_0FRV_idx_0 * x[19] - x[17] * v_0FRV_idx_2;
    b_A_0V[2] = x[17] * v_0FRV_idx_1 - v_0FRV_idx_0 * x[18];
    r = _mm_loadu_pd(&d_A_0V[0]);
    r2 = _mm_loadu_pd(&b_A_0V[0]);
    r3 = _mm_loadu_pd(&g_V[0]);
    _mm_storeu_pd(&d_A_0V[0], _mm_sub_pd(r3, _mm_add_pd(r, r2)));
    d_A_0V[2] = g_V[2] - (d_A_0V[2] + b_A_0V[2]);
    d = d_A_0V[0];
    d1 = d_A_0V[1];
    d2 = d_A_0V[2];
    d3 = c_A_0V[0];
    d4 = c_A_0V[1];
    d5 = c_A_0V[2];
    for (i = 0; i <= 12; i += 2) {
      r = _mm_loadu_pd(&c_q_tmp[i]);
      r = _mm_mul_pd(r, _mm_set1_pd(d));
      r2 = _mm_loadu_pd(&c_q_tmp[i + 14]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&c_q_tmp[i + 28]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&l_y_tmp[i], r);
      r = _mm_loadu_pd(&dv2[i]);
      r2 = _mm_loadu_pd(&dv1[i]);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&b_q_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d3));
      r3 = _mm_loadu_pd(&b_q_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d4));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&b_q_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d5));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&b_dv3[i], r);
    }

    c_A_0V[0] = r_VRLV_dot[2] * x[18] - r_VRLV_dot[1] * x[19];
    c_A_0V[1] = r_VRLV_dot[0] * x[19] - r_VRLV_dot[2] * x[17];
    c_A_0V[2] = r_VRLV_dot[1] * x[17] - r_VRLV_dot[0] * x[18];
    d_A_0V[0] = x[18] * v_0RLV_idx_2 - v_0RLV_idx_1 * x[19];
    d_A_0V[1] = v_0RLV_idx_0 * x[19] - x[17] * v_0RLV_idx_2;
    d_A_0V[2] = x[17] * v_0RLV_idx_1 - v_0RLV_idx_0 * x[18];
    r = _mm_loadu_pd(&c_A_0V[0]);
    r2 = _mm_loadu_pd(&d_A_0V[0]);
    r3 = _mm_loadu_pd(&g_V[0]);
    _mm_storeu_pd(&c_A_0V[0], _mm_sub_pd(r3, _mm_add_pd(r, r2)));
    c_A_0V[2] = g_V[2] - (c_A_0V[2] + d_A_0V[2]);
    d_A_0V[0] = r_VRRV_dot[2] * x[18] - r_VRRV_dot[1] * x[19];
    d_A_0V[1] = r_VRRV_dot[0] * x[19] - r_VRRV_dot[2] * x[17];
    d_A_0V[2] = r_VRRV_dot[1] * x[17] - r_VRRV_dot[0] * x[18];
    b_A_0V[0] = x[18] * v_0RRV_idx_2 - v_0RRV_idx_1 * x[19];
    b_A_0V[1] = v_0RRV_idx_0 * x[19] - x[17] * v_0RRV_idx_2;
    b_A_0V[2] = x[17] * v_0RRV_idx_1 - v_0RRV_idx_0 * x[18];
    r = _mm_loadu_pd(&d_A_0V[0]);
    r2 = _mm_loadu_pd(&b_A_0V[0]);
    r3 = _mm_loadu_pd(&g_V[0]);
    _mm_storeu_pd(&g_V[0], _mm_sub_pd(r3, _mm_add_pd(r, r2)));
    g_V[2] -= d_A_0V[2] + b_A_0V[2];
    d = c_A_0V[0];
    d1 = c_A_0V[1];
    d2 = c_A_0V[2];
    d3 = -(omega_0UFRV[1] * dvdphi_RR[2] - dvdphi_RR[1] * omega_0UFRV[2]);
    d4 = -(dvdphi_RR[0] * omega_0UFRV[2] - omega_0UFRV[0] * dvdphi_RR[2]);
    d5 = -(omega_0UFRV[0] * dvdphi_RR[1] - dvdphi_RR[0] * omega_0UFRV[1]);
    d6 = g_V[0];
    d7 = g_V[1];
    d8 = g_V[2];
    d9 = -(omega_0UFLV[1] * domdu_FL[2] - domdu_FL[1] * omega_0UFLV[2]);
    d10 = -(domdu_FL[0] * omega_0UFLV[2] - omega_0UFLV[0] * domdu_FL[2]);
    d11 = -(omega_0UFLV[0] * domdu_FL[1] - domdu_FL[0] * omega_0UFLV[1]);
    d12 = -(omega_0URLV[1] * domdphi_RR[2] - domdphi_RR[1] * omega_0URLV[2]);
    d13 = -(domdphi_RR[0] * omega_0URLV[2] - omega_0URLV[0] * domdphi_RR[2]);
    u = -(omega_0URLV[0] * domdphi_RR[1] - domdphi_RR[0] * omega_0URLV[1]);
    for (i = 0; i <= 12; i += 2) {
      r = _mm_loadu_pd(&b_dv3[i]);
      r2 = _mm_loadu_pd(&l_y_tmp[i]);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&d_q_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
      r3 = _mm_loadu_pd(&d_q_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&d_q_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&e_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d3));
      r3 = _mm_loadu_pd(&e_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d4));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&e_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d5));
      r2 = _mm_add_pd(r2, r3);
      _mm_storeu_pd(&l_y_tmp[i], r2);
      r3 = _mm_loadu_pd(&e_q_tmp[i]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d6));
      r4 = _mm_loadu_pd(&e_q_tmp[i + 14]);
      r4 = _mm_mul_pd(r4, _mm_set1_pd(d7));
      r3 = _mm_add_pd(r3, r4);
      r4 = _mm_loadu_pd(&e_q_tmp[i + 28]);
      r4 = _mm_mul_pd(r4, _mm_set1_pd(d8));
      r3 = _mm_add_pd(r3, r4);
      r = _mm_add_pd(r, r3);
      r3 = _mm_loadu_pd(&d_y_tmp[i]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d9));
      r4 = _mm_loadu_pd(&d_y_tmp[i + 14]);
      r4 = _mm_mul_pd(r4, _mm_set1_pd(d10));
      r3 = _mm_add_pd(r3, r4);
      r4 = _mm_loadu_pd(&d_y_tmp[i + 28]);
      r4 = _mm_mul_pd(r4, _mm_set1_pd(d11));
      r3 = _mm_add_pd(r3, r4);
      r = _mm_add_pd(r, r3);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&f_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d12));
      r3 = _mm_loadu_pd(&f_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d13));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&f_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(u));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&dv1[i], r);
    }

    d = -(omega_0URRV[1] * dvdphi_FR[2] - dvdphi_FR[1] * omega_0URRV[2]);
    d1 = -(dvdphi_FR[0] * omega_0URRV[2] - omega_0URRV[0] * dvdphi_FR[2]);
    d2 = -(omega_0URRV[0] * dvdphi_FR[1] - dvdphi_FR[0] * omega_0URRV[1]);
    for (i = 0; i <= 12; i += 2) {
      r = _mm_loadu_pd(&g_y_tmp[i]);
      r = _mm_mul_pd(r, _mm_set1_pd(d));
      r2 = _mm_loadu_pd(&g_y_tmp[i + 14]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&g_y_tmp[i + 28]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&l_y_tmp[i], r);
    }

    c_A_0V[0] = omega_0WFLV[2] * x[18] - omega_0WFLV[1] * x[19];
    c_A_0V[1] = omega_0WFLV[0] * x[19] - omega_0WFLV[2] * x[17];
    c_A_0V[2] = omega_0WFLV[1] * x[17] - omega_0WFLV[0] * x[18];
    b_I_UFRV_tmp[0] = omega_VUFLV[1] * omega_0WFLV_tmp[2] - omega_0WFLV_tmp[1] *
      omega_VUFLV[2];
    b_I_UFRV_tmp[1] = omega_0WFLV_tmp[0] * omega_VUFLV[2] - omega_VUFLV[0] *
      omega_0WFLV_tmp[2];
    b_I_UFRV_tmp[2] = omega_VUFLV[0] * omega_0WFLV_tmp[1] - omega_0WFLV_tmp[0] *
      omega_VUFLV[1];
    r = _mm_loadu_pd(&I_WFLV[0]);
    _mm_storeu_pd(&A_z_tmp[0], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WFLV[2]);
    _mm_storeu_pd(&A_z_tmp[2], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WFLV[4]);
    _mm_storeu_pd(&A_z_tmp[4], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WFLV[6]);
    _mm_storeu_pd(&A_z_tmp[6], _mm_mul_pd(r, r1));
    A_z_tmp[8] = -I_WFLV[8];
    r = _mm_loadu_pd(&c_A_0V[0]);
    r2 = _mm_loadu_pd(&b_I_UFRV_tmp[0]);
    _mm_storeu_pd(&c_A_0V[0], _mm_add_pd(r, r2));
    c_A_0V[2] += b_I_UFRV_tmp[2];
    d_A_0V[0] = omega_0WFLV[1] * domdphi_FR[2] - domdphi_FR[1] * omega_0WFLV[2];
    d_A_0V[1] = domdphi_FR[0] * omega_0WFLV[2] - omega_0WFLV[0] * domdphi_FR[2];
    d_A_0V[2] = omega_0WFLV[0] * domdphi_FR[1] - domdphi_FR[0] * omega_0WFLV[1];
    d = c_A_0V[0];
    d1 = c_A_0V[1];
    d2 = c_A_0V[2];
    r = _mm_loadu_pd(&A_z_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_loadu_pd(&A_z_tmp[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_z_tmp[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&d_A_0V[0]);
    r = _mm_sub_pd(r, r2);
    _mm_storeu_pd(&b_A_0V[0], r);
    b_A_0V[2] = ((A_z_tmp[2] * d + A_z_tmp[5] * d1) + A_z_tmp[8] * d2) - d_A_0V
      [2];
    c_A_0V[0] = omega_0WFRV[2] * x[18] - omega_0WFRV[1] * x[19];
    c_A_0V[1] = omega_0WFRV[0] * x[19] - omega_0WFRV[2] * x[17];
    c_A_0V[2] = omega_0WFRV[1] * x[17] - omega_0WFRV[0] * x[18];
    omega_0VV[0] = omega_VUFRV[1] * omega_0WFRV_tmp[2] - omega_0WFRV_tmp[1] *
      omega_VUFRV[2];
    omega_0VV[1] = omega_0WFRV_tmp[0] * omega_VUFRV[2] - omega_VUFRV[0] *
      omega_0WFRV_tmp[2];
    omega_0VV[2] = omega_VUFRV[0] * omega_0WFRV_tmp[1] - omega_0WFRV_tmp[0] *
      omega_VUFRV[1];
    r = _mm_loadu_pd(&I_WFRV[0]);
    _mm_storeu_pd(&A_z_tmp[0], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WFRV[2]);
    _mm_storeu_pd(&A_z_tmp[2], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WFRV[4]);
    _mm_storeu_pd(&A_z_tmp[4], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WFRV[6]);
    _mm_storeu_pd(&A_z_tmp[6], _mm_mul_pd(r, r1));
    A_z_tmp[8] = -I_WFRV[8];
    r = _mm_loadu_pd(&c_A_0V[0]);
    r2 = _mm_loadu_pd(&omega_0VV[0]);
    _mm_storeu_pd(&c_A_0V[0], _mm_add_pd(r, r2));
    c_A_0V[2] += omega_0VV[2];
    d_A_0V[0] = omega_0WFRV[1] * dvdphi_FL[2] - dvdphi_FL[1] * omega_0WFRV[2];
    d_A_0V[1] = dvdphi_FL[0] * omega_0WFRV[2] - omega_0WFRV[0] * dvdphi_FL[2];
    d_A_0V[2] = omega_0WFRV[0] * dvdphi_FL[1] - dvdphi_FL[0] * omega_0WFRV[1];
    d = c_A_0V[0];
    d1 = c_A_0V[1];
    d2 = c_A_0V[2];
    r = _mm_loadu_pd(&A_z_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_loadu_pd(&A_z_tmp[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_z_tmp[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&d_A_0V[0]);
    r = _mm_sub_pd(r, r2);
    _mm_storeu_pd(&b_I_UFRV_tmp[0], r);
    b_I_UFRV_tmp[2] = ((A_z_tmp[2] * d + A_z_tmp[5] * d1) + A_z_tmp[8] * d2) -
      d_A_0V[2];
    d = b_A_0V[0];
    d1 = b_A_0V[1];
    d2 = b_A_0V[2];
    d3 = b_I_UFRV_tmp[0];
    d4 = b_I_UFRV_tmp[1];
    d5 = b_I_UFRV_tmp[2];
    for (i = 0; i <= 12; i += 2) {
      r = _mm_loadu_pd(&dv1[i]);
      r2 = _mm_loadu_pd(&l_y_tmp[i]);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&h_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
      r3 = _mm_loadu_pd(&h_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&h_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&dv2[i], r);
      r = _mm_loadu_pd(&i_y_tmp[i]);
      r = _mm_mul_pd(r, _mm_set1_pd(d3));
      r2 = _mm_loadu_pd(&i_y_tmp[i + 14]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d4));
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&i_y_tmp[i + 28]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d5));
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&l_y_tmp[i], r);
    }

    c_A_0V[0] = omega_0WRLV[2] * x[18] - omega_0WRLV[1] * x[19];
    c_A_0V[1] = omega_0WRLV[0] * x[19] - omega_0WRLV[2] * x[17];
    c_A_0V[2] = omega_0WRLV[1] * x[17] - omega_0WRLV[0] * x[18];
    b_I_UFRV_tmp[0] = omega_VURLV[1] * omega_0WRLV_tmp[2] - omega_0WRLV_tmp[1] *
      omega_VURLV[2];
    b_I_UFRV_tmp[1] = omega_0WRLV_tmp[0] * omega_VURLV[2] - omega_VURLV[0] *
      omega_0WRLV_tmp[2];
    b_I_UFRV_tmp[2] = omega_VURLV[0] * omega_0WRLV_tmp[1] - omega_0WRLV_tmp[0] *
      omega_VURLV[1];
    r = _mm_loadu_pd(&I_WRLV[0]);
    _mm_storeu_pd(&A_z_tmp[0], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRLV[2]);
    _mm_storeu_pd(&A_z_tmp[2], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRLV[4]);
    _mm_storeu_pd(&A_z_tmp[4], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRLV[6]);
    _mm_storeu_pd(&A_z_tmp[6], _mm_mul_pd(r, r1));
    A_z_tmp[8] = -I_WRLV[8];
    r = _mm_loadu_pd(&c_A_0V[0]);
    r2 = _mm_loadu_pd(&b_I_UFRV_tmp[0]);
    _mm_storeu_pd(&c_A_0V[0], _mm_add_pd(r, r2));
    c_A_0V[2] += b_I_UFRV_tmp[2];
    d_A_0V[0] = omega_0WRLV[1] * domdphi_FL[2] - domdphi_FL[1] * omega_0WRLV[2];
    d_A_0V[1] = domdphi_FL[0] * omega_0WRLV[2] - omega_0WRLV[0] * domdphi_FL[2];
    d_A_0V[2] = omega_0WRLV[0] * domdphi_FL[1] - domdphi_FL[0] * omega_0WRLV[1];
    d = c_A_0V[0];
    d1 = c_A_0V[1];
    d2 = c_A_0V[2];
    r = _mm_loadu_pd(&A_z_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_loadu_pd(&A_z_tmp[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_z_tmp[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&d_A_0V[0]);
    r = _mm_sub_pd(r, r2);
    _mm_storeu_pd(&omega_0VV[0], r);
    omega_0VV[2] = ((A_z_tmp[2] * d + A_z_tmp[5] * d1) + A_z_tmp[8] * d2) -
      d_A_0V[2];
    c_A_0V[0] = omega_0WRRV[2] * x[18] - omega_0WRRV[1] * x[19];
    c_A_0V[1] = omega_0WRRV[0] * x[19] - omega_0WRRV[2] * x[17];
    c_A_0V[2] = omega_0WRRV[1] * x[17] - omega_0WRRV[0] * x[18];
    b_I_UFRV_tmp[0] = omega_VURRV[1] * omega_0WRRV_tmp[2] - omega_0WRRV_tmp[1] *
      omega_VURRV[2];
    b_I_UFRV_tmp[1] = omega_0WRRV_tmp[0] * omega_VURRV[2] - omega_VURRV[0] *
      omega_0WRRV_tmp[2];
    b_I_UFRV_tmp[2] = omega_VURRV[0] * omega_0WRRV_tmp[1] - omega_0WRRV_tmp[0] *
      omega_VURRV[1];
    r = _mm_loadu_pd(&I_WRRV[0]);
    _mm_storeu_pd(&A_z_tmp[0], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRRV[2]);
    _mm_storeu_pd(&A_z_tmp[2], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRRV[4]);
    _mm_storeu_pd(&A_z_tmp[4], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRRV[6]);
    _mm_storeu_pd(&A_z_tmp[6], _mm_mul_pd(r, r1));
    A_z_tmp[8] = -I_WRRV[8];
    r = _mm_loadu_pd(&c_A_0V[0]);
    r1 = _mm_loadu_pd(&b_I_UFRV_tmp[0]);
    _mm_storeu_pd(&c_A_0V[0], _mm_add_pd(r, r1));
    c_A_0V[2] += b_I_UFRV_tmp[2];
    b_I_UFRV_tmp[0] = omega_0WRRV[1] * r_0V0[2] - r_0V0[1] * omega_0WRRV[2];
    b_I_UFRV_tmp[1] = r_0V0[0] * omega_0WRRV[2] - omega_0WRRV[0] * r_0V0[2];
    b_I_UFRV_tmp[2] = omega_0WRRV[0] * r_0V0[1] - r_0V0[0] * omega_0WRRV[1];
    d = c_A_0V[0];
    d1 = c_A_0V[1];
    d2 = c_A_0V[2];
    r = _mm_loadu_pd(&A_z_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&A_z_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&A_z_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&b_I_UFRV_tmp[0]);
    r = _mm_sub_pd(r, r1);
    _mm_storeu_pd(&d_A_0V[0], r);
    d_A_0V[2] = ((A_z_tmp[2] * d + A_z_tmp[5] * d1) + A_z_tmp[8] * d2) -
      b_I_UFRV_tmp[2];
    for (b_i = 0; b_i < 14; b_i++) {
      d = j_y_tmp[b_i + 14];
      d1 = j_y_tmp[b_i + 28];
      d2 = (dv2[b_i] + l_y_tmp[b_i]) + ((j_y_tmp[b_i] * omega_0VV[0] + d *
        omega_0VV[1]) + d1 * omega_0VV[2]);
      d3 = 0.0;
      d4 = d_y_tmp[b_i];
      d5 = d_y_tmp[b_i + 14];
      d6 = d_y_tmp[b_i + 28];
      for (i = 0; i < 3; i++) {
        e_yFLV_tmp = b_i + 14 * i;
        d3 += k_y_tmp[e_yFLV_tmp] * d_A_0V[i];
        y_tmp[e_yFLV_tmp] = (d4 * I_UFLV[3 * i] + d5 * I_UFLV[3 * i + 1]) + d6 *
          I_UFLV[3 * i + 2];
      }

      l_y_tmp[b_i] = d3;
      d2 += d3;
      dv1[b_i] = d2;
      d2 = y_tmp[b_i];
      d3 = y_tmp[b_i + 14];
      d4 = y_tmp[b_i + 28];
      d5 = b_q_tmp[b_i];
      d6 = b_q_tmp[b_i + 14];
      d7 = b_q_tmp[b_i + 28];
      for (i = 0; i < 14; i++) {
        i1 = 3 * i + 1;
        i2 = 3 * i + 2;
        e_yFLV_tmp = b_i + 14 * i;
        m_y_tmp[e_yFLV_tmp] = (d2 * domUFLdz[3 * i] + d3 * domUFLdz[i1]) + d4 *
          domUFLdz[i2];
        K[e_yFLV_tmp] = (d5 * dvFLdz[3 * i] + d6 * dvFLdz[i1]) + d7 * dvFLdz[i2];
      }

      d2 = h_y_tmp[b_i];
      d3 = h_y_tmp[b_i + 14];
      d4 = h_y_tmp[b_i + 28];
      for (i = 0; i < 3; i++) {
        y_tmp[b_i + 14 * i] = (d2 * I_WFLV[3 * i] + d3 * I_WFLV[3 * i + 1]) + d4
          * I_WFLV[3 * i + 2];
      }

      d2 = q_tmp[b_i];
      d3 = q_tmp[b_i + 14];
      d4 = q_tmp[b_i + 28];
      d5 = y_tmp[b_i];
      d6 = y_tmp[b_i + 14];
      d7 = y_tmp[b_i + 28];
      for (i = 0; i < 14; i++) {
        i1 = 3 * i + 1;
        i2 = 3 * i + 2;
        dvRRdz_tmp = b_i + 14 * i;
        f_q_tmp[dvRRdz_tmp] = ((d2 * static_cast<double>(iv5[3 * i]) + d3 *
          static_cast<double>(iv5[i1])) + d4 * static_cast<double>(iv5[i2])) +
          static_cast<double>(iv6[dvRRdz_tmp]);
        g_q_tmp[dvRRdz_tmp] = (K[dvRRdz_tmp] + m_y_tmp[dvRRdz_tmp]) + ((d5 *
          domWFLdz[3 * i] + d6 * domWFLdz[i1]) + d7 * domWFLdz[i2]);
      }

      d2 = e_y_tmp[b_i];
      d3 = e_y_tmp[b_i + 14];
      d4 = e_y_tmp[b_i + 28];
      for (i = 0; i < 3; i++) {
        y_tmp[b_i + 14 * i] = (d2 * I_UFRV[3 * i] + d3 * I_UFRV[3 * i + 1]) + d4
          * I_UFRV[3 * i + 2];
      }

      d2 = y_tmp[b_i];
      d3 = y_tmp[b_i + 14];
      d4 = y_tmp[b_i + 28];
      d5 = c_q_tmp[b_i];
      d6 = c_q_tmp[b_i + 14];
      d7 = c_q_tmp[b_i + 28];
      for (i = 0; i < 14; i++) {
        i1 = 3 * i + 1;
        i2 = 3 * i + 2;
        e_yFLV_tmp = b_i + 14 * i;
        m_y_tmp[e_yFLV_tmp] = (d2 * domUFRdz[3 * i] + d3 * domUFRdz[i1]) + d4 *
          domUFRdz[i2];
        K[e_yFLV_tmp] = (d5 * dvFRdz[3 * i] + d6 * dvFRdz[i1]) + d7 * dvFRdz[i2];
      }

      d2 = f_y_tmp[b_i];
      d3 = f_y_tmp[b_i + 14];
      d4 = f_y_tmp[b_i + 28];
      d5 = i_y_tmp[b_i];
      d6 = i_y_tmp[b_i + 14];
      d7 = i_y_tmp[b_i + 28];
      for (i = 0; i < 3; i++) {
        i1 = 3 * i + 1;
        i2 = 3 * i + 2;
        e_yFLV_tmp = b_i + 14 * i;
        b_y_tmp[e_yFLV_tmp] = (d2 * I_URLV[3 * i] + d3 * I_URLV[i1]) + d4 *
          I_URLV[i2];
        y_tmp[e_yFLV_tmp] = (d5 * I_WFRV[3 * i] + d6 * I_WFRV[i1]) + d7 *
          I_WFRV[i2];
      }

      d2 = b_y_tmp[b_i];
      d3 = b_y_tmp[b_i + 14];
      d4 = b_y_tmp[b_i + 28];
      d5 = d_q_tmp[b_i];
      d6 = d_q_tmp[b_i + 14];
      d7 = d_q_tmp[b_i + 28];
      for (i = 0; i < 14; i++) {
        i1 = 3 * i + 1;
        i2 = 3 * i + 2;
        e_yFLV_tmp = b_i + 14 * i;
        n_y_tmp[e_yFLV_tmp] = (d2 * domURLdz[3 * i] + d3 * domURLdz[i1]) + d4 *
          domURLdz[i2];
        h_q_tmp[e_yFLV_tmp] = (d5 * dvRLdz[3 * i] + d6 * dvRLdz[i1]) + d7 *
          dvRLdz[i2];
      }

      for (i = 0; i < 3; i++) {
        b_y_tmp[b_i + 14 * i] = (j_y_tmp[b_i] * I_WRLV[3 * i] + d * I_WRLV[3 * i
          + 1]) + d1 * I_WRLV[3 * i + 2];
      }

      d = y_tmp[b_i];
      d1 = y_tmp[b_i + 14];
      d2 = y_tmp[b_i + 28];
      d3 = b_y_tmp[b_i];
      d4 = b_y_tmp[b_i + 14];
      d5 = b_y_tmp[b_i + 28];
      for (i = 0; i < 14; i++) {
        i1 = 3 * i + 1;
        i2 = 3 * i + 2;
        dvRRdz_tmp = b_i + 14 * i;
        i_q_tmp[dvRRdz_tmp] = (f_q_tmp[dvRRdz_tmp] + g_q_tmp[dvRRdz_tmp]) +
          ((K[dvRRdz_tmp] + m_y_tmp[dvRRdz_tmp]) + ((d * domWFRdz[3 * i] + d1 *
             domWFRdz[i1]) + d2 * domWFRdz[i2]));
        K[dvRRdz_tmp] = (h_q_tmp[dvRRdz_tmp] + n_y_tmp[dvRRdz_tmp]) + ((d3 *
          domWRLdz[3 * i] + d4 * domWRLdz[i1]) + d5 * domWRLdz[i2]);
      }

      d = g_y_tmp[b_i];
      d1 = g_y_tmp[b_i + 14];
      d2 = g_y_tmp[b_i + 28];
      for (i = 0; i < 3; i++) {
        y_tmp[b_i + 14 * i] = (d * I_URRV[3 * i] + d1 * I_URRV[3 * i + 1]) + d2 *
          I_URRV[3 * i + 2];
      }

      d = y_tmp[b_i];
      d1 = y_tmp[b_i + 14];
      d2 = y_tmp[b_i + 28];
      d3 = e_q_tmp[b_i];
      d4 = e_q_tmp[b_i + 14];
      d5 = e_q_tmp[b_i + 28];
      for (i = 0; i < 14; i++) {
        i1 = 3 * i + 1;
        i2 = 3 * i + 2;
        e_yFLV_tmp = b_i + 14 * i;
        m_y_tmp[e_yFLV_tmp] = (d * domURRdz[3 * i] + d1 * domURRdz[i1]) + d2 *
          domURRdz[i2];
        f_q_tmp[e_yFLV_tmp] = (d3 * dvRRdz[3 * i] + d4 * dvRRdz[i1]) + d5 *
          dvRRdz[i2];
      }

      d = k_y_tmp[b_i];
      d1 = k_y_tmp[b_i + 14];
      d2 = k_y_tmp[b_i + 28];
      for (i = 0; i < 3; i++) {
        y_tmp[b_i + 14 * i] = (d * I_WRRV[3 * i] + d1 * I_WRRV[3 * i + 1]) + d2 *
          I_WRRV[3 * i + 2];
      }

      d = y_tmp[b_i];
      d1 = y_tmp[b_i + 14];
      d2 = y_tmp[b_i + 28];
      for (i = 0; i < 14; i++) {
        dvRRdz_tmp = b_i + 14 * i;
        g_q_tmp[dvRRdz_tmp] = (i_q_tmp[dvRRdz_tmp] + K[dvRRdz_tmp]) +
          ((f_q_tmp[dvRRdz_tmp] + m_y_tmp[dvRRdz_tmp]) + ((d * domWRRdz[3 * i] +
             d1 * domWRRdz[3 * i + 1]) + d2 * domWRRdz[3 * i + 2]));
      }

      x_dot[b_i] = dv[b_i];
    }

    coder::mldivide(g_q_tmp, dv1);
    memcpy(&x_dot[14], &dv1[0], 14U * sizeof(double));
    x_dot[28] = x_TFL_dot[0];
    x_dot[31] = x_TFR_dot[0];
    x_dot[34] = x_TRL_dot[0];
    x_dot[37] = x_TRR_dot[0];
    x_dot[29] = x_TFL_dot[1];
    x_dot[32] = x_TFR_dot[1];
    x_dot[35] = x_TRL_dot[1];
    x_dot[38] = x_TRR_dot[1];
    x_dot[30] = x_TFL_dot[2];
    x_dot[33] = x_TFR_dot[2];
    x_dot[36] = x_TRL_dot[2];
    x_dot[39] = x_TRR_dot[2];
    x_dot[40] = 0.0;
    x_dot[41] = 0.0;

    //  State derivatives
    //  Additional output
    //  Inertial acceleration
    //  Combined aerodynamic forces
    //  Steering Torque
    //  --- Tire utilization (per wheel + global)
    //  Output vector
  }

  void adtm_constants(double c[9])
  {
    static const double dv[9] = { 1.9198621771937625, 2.62119,
      2.4887362499999997, 0.205, 21.0, 12.421, 42.0, 110.0, 1.53 };

    if (!isInitialized_adtm_step) {
      adtm_step_initialize();
    }

    memcpy(&c[0], &dv[0], 9U * sizeof(double));

    // ADTM_CONSTANTS [steering wheel angle at full lock (rad), front and rear brake torque per bar (Nm), tyre radius (m),
    //    motor torque (Nm), gear ratio, number of states, number of outputs, wheelbase (m)]
  }

  void adtm_init(const double car[9], const double rz[3721], const double r0[2],
                 double x[42])
  {
    double road_x[61];
    double road_y[61];
    double A_y_tmp[9];
    double dv[9];
    double dv1[3];
    double y[3];
    double A_x_tmp;
    double ang;
    double b_ang;
    double l_FA;
    double l_RA;
    double m_FA;
    if (!isInitialized_adtm_step) {
      adtm_step_initialize();
    }

    // ADTM_INIT The ADTM standing still at the origin, settled on its springs (f_calcInitState), heading along +x, on the
    //    road around the origin given by rz, r0 (adtm_road): its height, pitch and roll follow that road.
    // ADTM_VEHICLE FS Lap's setup applied to the team's vehicle at run time: the values adtm_step and adtm_init then set.
    //    car: [C_D, C_LFA, C_LRA with A = 1 m^2 (negative = drag / downforce), mass shift (kg), shift of the mass share on
    //    the front axle, CoG height shift (m), motor torque factor, shift of the front share of the roll stiffness, gear
    //    ratio factor]. With [C_D C_LFA C_LRA 0 0 0 1 0 1] it is the vehicle file's car exactly; the shifts move the team's
    //    values, so FS Lap's stock setup always drives the team's ADTM.
    //    v: [m, m_C, l_FA, l_RA, F_SFL0 = F_SFR0, F_SRL0 = F_SRR0, z_cg, T_MMax, c_RollFA, c_RollRA]
    //    The callers set the fields themselves: a vehicle struct returned from here is no longer a constant to MATLAB Coder,
    //    and the model picks its branches (motor, driveline, suspension) by constant fields.
    //  the unsprung masses stay
    l_FA = 0.765 - 1.53 * car[4];
    l_RA = 1.53 * car[4] + 0.765;

    //  roll stiffness per axle (N m/rad) as f_suspensionForces has it: (corner spring / 2 + roll spring) * track^2. The
    //  shift moves stiffness from one axle's roll spring to the other's, so the total and the ride rates stay: a change of
    //  the split only, as on FS Lap's own model. The team's car has no roll springs, so one of them becomes negative (that
    //  axle softer in roll than its corner springs alone); the axle stays stiff in roll for a front share between 0 and 1.
    //  as f_vehicleParameters: the spring pretension carries the chassis
    memset(&x[0], 0, 42U * sizeof(double));

    // ADTM_ROAD The ADTM's 3D road (f_roadHeight, f_roadMu) as a window of 61 x 61 heights on its 0.1 m grid around the car.
    //    rz(i, j): the height (m) at x = (r0(1) + i - 1) / 10, y = (r0(2) + j - 1) / 10 (m), in the frame of the ADTM's x_V, y_V;
    //    r0: the window's corner in tenths of a metre (whole numbers). FS Lap moves the window with the car (HoMoLapAdtm.cpp);
    //    a flat track gives zeros. The road's friction factor is 1 everywhere: FS Lap's grip stays in mu_N.
    A_x_tmp = r0[0];
    m_FA = r0[1];
    for (int i = 0; i < 61; i++) {
      road_x[i] = A_x_tmp + static_cast<double>(i);
      road_y[i] = m_FA + static_cast<double>(i);
    }

    double z_AFA;
    double z_ARA;
    double z_FL;
    double z_FR;
    double z_RFL;
    double z_RFR;
    double z_RL;
    double z_RR;
    double z_RRL;
    double z_RRR;

    //       Calculation of initial states      %%
    //  ----------------------------------------- %
    //  Version: V1.5 - 2025.06.25                %
    //  Compatible ADTM Release: ADTM_1.4         %
    //  Author: Simon Frank simon.sf.frank@tum.de %
    //  Modified by:                              %
    //  Input:                                    %
    //    veh     -   struct containing vehicle   %
    //                parameters                  %
    //    tirXX   -   struct containing tire      %
    //                parameters for one tire     %
    //                where XX is FL/FR/RL/RR     %
    //    env     -   struct containing           %
    //                environment parameters      %
    //    road    -   struct of road              %
    //                x, y, and Z data            %
    //    opts    -   struct of additional model  %
    //                options                     %
    //  Output:                                   %
    //    x_V0    -   value of x_V at t=0         %
    //    y_V0    -   value of y_V at t=0         %
    //    z_V0    -   value of z_V at t=0         %
    //    phi_0   -   value of phi at t=0         %
    //    theta_0 -   value of theta at t=0       %
    //    phi_XX0 -   value of phi_XX at t=0      %
    //  ----------------------------------------- %
    //  Initial states @ t=0
    //  Mass on each axle
    m_FA = (car[3] + 240.0) * l_RA / 1.53;

    //  Road height under each wheel
    //  3D road
    z_RFL = f_roadHeight(road_x, road_y, rz, l_FA, 0.62);
    z_RFR = f_roadHeight(road_x, road_y, rz, l_FA, -0.62);
    z_RRL = f_roadHeight(road_x, road_y, rz, -l_RA, 0.6);
    z_RRR = f_roadHeight(road_x, road_y, rz, -l_RA, -0.6);

    //  Lift on each axle
    l_FA = 0.60204237944132111 * car[1] * 0.0;

    //  Lift front
    l_RA = 0.60204237944132111 * car[2] * 0.0;

    //  Lift rear
    //  Wheel travel from aerodynamic lift
    z_AFA = l_FA / 212656.99436257072;
    z_ARA = l_RA / 125336.03945236695;

    //  Approx. initial wishbone angles from aerodynamic lift
    x[6] = asin(-z_AFA / 0.62);
    x[7] = asin(-z_AFA / -0.62);
    x[8] = asin(-z_ARA / 0.6);
    x[9] = asin(-z_ARA / -0.6);

    //  Force on each axle center
    //  Wheel center heights
    l_FA = 0.5 * (l_FA - 9.80665 * m_FA) / 98000.0 + 0.205;
    z_FL = l_FA + z_RFL;
    z_FR = l_FA + z_RFR;
    l_FA = 0.5 * (l_RA - 9.80665 * ((car[3] + 240.0) - m_FA)) / 98000.0 + 0.205;
    z_RL = l_FA + z_RRL;
    z_RR = l_FA + z_RRR;

    //  Chassis angles
    ang = (asin((z_FL - z_FR) / 1.24) + asin((z_RL - z_RR) / 1.2)) / 2.0;
    b_ang = (asin((z_RL - z_FL) / 1.53) + asin((z_RR - z_FR) / 1.53)) / 2.0 +
      asin((z_ARA - z_AFA) / 1.53);

    //  Rotation matrix for angle ang about Y axis
    l_FA = sin(b_ang);
    l_RA = cos(b_ang);

    //  Rotation matrix for angle ang about X axis
    A_x_tmp = sin(ang);
    m_FA = cos(ang);
    A_y_tmp[0] = l_RA;
    A_y_tmp[3] = 0.0;
    A_y_tmp[6] = l_FA;
    A_y_tmp[2] = -l_FA;
    A_y_tmp[5] = 0.0;
    A_y_tmp[8] = l_RA;
    A_y_tmp[1] = 0.0;
    dv[0] = 1.0;
    A_y_tmp[4] = 1.0;
    dv[3] = 0.0;
    A_y_tmp[7] = 0.0;
    dv[6] = 0.0;
    dv[1] = 0.0;
    dv[4] = m_FA;
    dv[7] = -A_x_tmp;
    dv[2] = 0.0;
    dv[5] = A_x_tmp;
    dv[8] = m_FA;
    dv1[0] = 0.0;
    dv1[1] = 0.0;
    A_x_tmp = 0.25 * (((z_RFL + z_RFR) + z_RRL) + z_RRR);
    dv1[2] = (((car[5] + 0.09) + (z_AFA + z_ARA) / 2.0) + 0.25 * (((z_FL + z_FR)
                + z_RL) + z_RR)) - A_x_tmp;
    for (int i = 0; i < 3; i++) {
      int i1;
      m_FA = 0.0;
      l_FA = A_y_tmp[i];
      i1 = static_cast<int>(A_y_tmp[i + 3]);
      l_RA = A_y_tmp[i + 6];
      for (int i2 = 0; i2 < 3; i2++) {
        m_FA += ((l_FA * dv[3 * i2] + static_cast<double>(i1) * dv[3 * i2 + 1])
                 + l_RA * dv[3 * i2 + 2]) * dv1[i2];
      }

      y[i] = m_FA;
    }

    //  CG position
    x[0] = y[0];
    x[1] = y[1];
    x[2] = y[2] + A_x_tmp;
    x[3] = ang;
    x[4] = b_ang;
  }

  void adtm_step(double x[42], const double u[6], const double car[9], double,
                 double dt, const double rz[3721], const double r0[2], double
                 out[110])
  {
    static const signed char b[3] = { 0, 1, 0 };

    __m128d r;
    __m128d r1;
    __m128d r2;
    __m128d r3;
    __m128d r4;
    __m128d r5;
    b_struct_T expl_temp;
    struct_T b_expl_temp;
    double K[196];
    double M[196];
    double c_q_tmp[196];
    double d_q_tmp[196];
    double n_y_tmp[196];
    double o_y_tmp[196];
    double p_y_tmp[196];
    double q_y_tmp[196];
    double b_q_tmp[42];
    double b_y_tmp[42];
    double c_y_tmp[42];
    double d_y_tmp[42];
    double domUFLdz[42];
    double domUFRdz[42];
    double domURLdz[42];
    double domURRdz[42];
    double domWFLdz[42];
    double domWFRdz[42];
    double domWRLdz[42];
    double domWRRdz[42];
    double dvFLdz[42];
    double dvFRdz[42];
    double dvRLdz[42];
    double dvRRdz[42];
    double e_y_tmp[42];
    double f_y_tmp[42];
    double g_y_tmp[42];
    double h_y_tmp[42];
    double i_y_tmp[42];
    double j_y_tmp[42];
    double k_y_tmp[42];
    double m_y_tmp[42];
    double q_tmp[42];
    double y_tmp[42];
    double b_f[14];
    double c_f[14];
    double dv[14];
    double f[14];
    double l_y_tmp[14];
    double v[10];
    double A_0V[9];
    double A_VQFR[9];
    double A_VQRL[9];
    double A_VQRR[9];
    double A_VRL[9];
    double A_y_tmp[9];
    double A_z_tmp[9];
    double I_UFLV[9];
    double I_UFLV_tmp[9];
    double I_UFRV[9];
    double I_UFRV_tmp[9];
    double I_URLV[9];
    double I_URLV_tmp[9];
    double I_URRV[9];
    double I_URRV_tmp[9];
    double I_WRLV[9];
    double I_WRRV[9];
    double b_I_WFLV[9];
    double b_I_WFRV[9];
    double out_aero[8];
    double s_FL[4];
    double s_FR[4];
    double s_RL[4];
    double s_RR[4];
    double F_ARAV[3];
    double F_FL[3];
    double F_FR[3];
    double F_TFL[3];
    double F_TFR[3];
    double F_TRL[3];
    double F_TRR[3];
    double I_WFLV[3];
    double I_WFRV[3];
    double T_TRL[3];
    double T_TRR[3];
    double T_WFL[3];
    double T_WFR[3];
    double b_A_0V[3];
    double b_r_0V0[3];
    double b_r_VFLV[3];
    double c_A_0V[3];
    double d_A_0V[3];
    double domdphi_FL[3];
    double domdphi_FR[3];
    double domdphi_RR[3];
    double domdu_FL[3];
    double domdu_FR[3];
    double dvdphi_FL[3];
    double dvdphi_FR[3];
    double dvdphi_RR[3];
    double dvdu_FL[3];
    double dvdu_FR[3];
    double e_yFLV[3];
    double e_yFRV[3];
    double e_yRLV[3];
    double e_yRRV[3];
    double g_V[3];
    double omega_0UFLV[3];
    double omega_0UFRV[3];
    double omega_0URLV[3];
    double omega_0URRV[3];
    double omega_0VV[3];
    double omega_0WFLV[3];
    double omega_0WFLV_tmp[3];
    double omega_0WFRV[3];
    double omega_0WFRV_tmp[3];
    double omega_0WRLV[3];
    double omega_0WRLV_tmp[3];
    double omega_0WRRV[3];
    double omega_0WRRV_tmp[3];
    double omega_VUFLV[3];
    double omega_VUFRV[3];
    double omega_VURLV[3];
    double omega_VURRV[3];
    double r_0V0[3];
    double r_MQFR0[3];
    double r_MQRL0[3];
    double r_MQRR0[3];
    double r_VFLV[3];
    double r_VFLV_dot[3];
    double r_VFRV[3];
    double r_VFRV_dot[3];
    double r_VRLV[3];
    double r_VRLV_dot[3];
    double r_VRRV[3];
    double r_VRRV_dot[3];
    double v_0FAV[3];
    double v_0RAV[3];
    double x_TFL_dot[3];
    double x_TFR_dot[3];
    double x_TRL_dot[3];
    double x_TRR_dot[3];
    double A_x_tmp_tmp;
    double F_SFL;
    double F_SFR;
    double F_SRL;
    double F_SRR;
    double F_xMFL;
    double F_xMFR;
    double F_xMRL;
    double F_xMRR;
    double F_yMFL;
    double F_yMFR;
    double F_yMRL;
    double F_yMRR;
    double F_ycFL;
    double F_ycFR;
    double F_ycRL;
    double F_ycRR;
    double T_BFL;
    double T_BFR;
    double T_BRL;
    double T_BRR;
    double T_DFL;
    double T_DFR;
    double T_DRL;
    double T_DRR;
    double b_A_x_tmp_tmp;
    double b_b_tmp;
    double b_domdu_FR;
    double b_tmp;
    double b_u;
    double d;
    double d1;
    double d10;
    double d11;
    double d12;
    double d13;
    double d14;
    double d15;
    double d16;
    double d17;
    double d18;
    double d19;
    double d2;
    double d20;
    double d21;
    double d3;
    double d4;
    double d5;
    double d6;
    double d7;
    double d8;
    double d9;
    double dF_x0FL;
    double dF_x0FR;
    double dF_x0RL;
    double dF_x0RR;
    double dF_y0FL;
    double dF_y0FR;
    double dF_y0RL;
    double dF_y0RR;
    double delta_FL;
    double delta_FR;
    double delta_RL;
    double delta_RR;
    double dz_RL;
    double dz_RR;
    double gamma_FL;
    double gamma_FR;
    double gamma_RL;
    double gamma_RR;
    double mu_RRL;
    double mu_RRR;
    double r_dynFL;
    double r_dynFR;
    double r_dynRL;
    double r_dynRR;
    double r_stFL;
    double r_stFR;
    double r_stRL;
    double r_stRR;
    double uFR;
    double uRL;
    double uRR;
    double v_0FAV_tmp;
    double v_0FLV_idx_0;
    double v_0FLV_idx_1;
    double v_0FLV_idx_2;
    double v_0FRV_idx_0;
    double v_0FRV_idx_1;
    double v_0FRV_idx_2;
    double v_0RLV_idx_0;
    double v_0RLV_idx_1;
    double v_0RLV_idx_2;
    double v_0RRV_idx_0;
    double v_0RRV_idx_1;
    double v_0RRV_idx_2;
    double v_0VV_idx_0;
    double v_0VV_idx_1;
    double v_0VV_idx_2;
    double veh_i_G;
    double w_RL;
    double w_RR;
    double z_FL;
    double z_FL_dot;
    double z_FR;
    double z_FR_dot;
    double z_RL;
    double z_RL_dot;
    double z_RR;
    double z_RR_dot;
    int b_i;
    int dvRRdz_tmp;
    int e_yFLV_tmp;
    int i;
    int i1;
    int i2;
    if (!isInitialized_adtm_step) {
      adtm_step_initialize();
    }

    // ADTM_STEP One Runge-Kutta step of the team's ADTM (f_vehicleModel), as its own f_solveODE does it.
    //    x: the 42 states, u: [delta_D (rad at the steering wheel), p_BFA, p_BRA (bar), throttle, gear, mu_N],
    //    car: FS Lap's setup, see adtm_vehicle (9 values), t: time (s), dt: step (s).
    //    rz, r0: the road's heights around the car, see adtm_road.
    adtm_vehicle(car, v);
    veh_i_G = 12.421 * car[8];

    // ADTM_ROAD The ADTM's 3D road (f_roadHeight, f_roadMu) as a window of 61 x 61 heights on its 0.1 m grid around the car.
    //    rz(i, j): the height (m) at x = (r0(1) + i - 1) / 10, y = (r0(2) + j - 1) / 10 (m), in the frame of the ADTM's x_V, y_V;
    //    r0: the window's corner in tenths of a metre (whole numbers). FS Lap moves the window with the car (HoMoLapAdtm.cpp);
    //    a flat track gives zeros. The road's friction factor is 1 everywhere: FS Lap's grip stays in mu_N.
    d = r0[0];
    d1 = r0[1];
    for (i = 0; i < 61; i++) {
      b_expl_temp.x[i] = d + static_cast<double>(i);
      b_expl_temp.y[i] = d1 + static_cast<double>(i);
    }

    for (i = 0; i < 3721; i++) {
      b_expl_temp.Mu[i] = 1.0;
    }

    //  the ground around the car (FS Lap's terrain; zeros on a flat track)
    //       Vehicle Model Equations            %%
    //  ----------------------------------------- %
    //  Version: V1.19 - 2026.01.05               %
    //  Compatible ADTM Release: ADTM_1.4         %
    //  Author: Simon Frank simon.sf.frank@tum.de %
    //  Modified by: Tim Braungardt  (V1.19)      %
    //  Input:                                    %
    //    inputs  -   struct of model input       %
    //                values at time t            %
    //    t       -   current simulation time     %
    //    x       -   state vector                %
    //    veh     -   struct containing vehicle   %
    //                parameters                  %
    //    tirXX   -   struct containing tire      %
    //                parameters for one tire     %
    //                where XX is FL/FR/RL/RR     %
    //    env     -   struct containing           %
    //                environment parameters      %
    //    road    -   struct of road              %
    //                x, y, Z, and Mu data        %
    //    opts    -   struct of additional model  %
    //                options                     %
    //  Output:                                   %
    //    x_dot   -   state derivative vector     %
    //    out     -   model output vector         %
    //  ----------------------------------------- %
    //  Model states
    //  Generalized coordinates
    //  Vehicle position
    //  Euler angles
    //  Lower wishbone angles
    //  Wheel rotation angles
    //  Generalized velocities
    //  Vehicle velocities
    //  Vehicle angular velocities
    //  Lower wishbone angular velocities
    //  Wheel rotation angular velocities
    //  Tire model internal states
    //  Check states
    //  Model Input
    //  Default motor torque commands
    //  Rotations
    //  Direction vectors
    //  Chassis to earth
    //  Rotation matrix for angle ang about Z axis
    b_u = sin(x[5]);
    uFR = cos(x[5]);

    //  Rotation matrix for angle ang about Y axis
    uRL = sin(x[4]);
    uRR = cos(x[4]);

    //  Rotation matrix for angle ang about X axis
    A_x_tmp_tmp = sin(x[3]);
    b_A_x_tmp_tmp = cos(x[3]);
    A_z_tmp[0] = uFR;
    A_z_tmp[3] = -b_u;
    A_z_tmp[6] = 0.0;
    A_z_tmp[1] = b_u;
    A_z_tmp[4] = uFR;
    A_z_tmp[7] = 0.0;
    A_y_tmp[0] = uRR;
    A_y_tmp[3] = 0.0;
    A_y_tmp[6] = uRL;
    A_z_tmp[2] = 0.0;
    A_y_tmp[1] = 0.0;
    A_z_tmp[5] = 0.0;
    A_y_tmp[4] = 1.0;
    A_z_tmp[8] = 1.0;
    A_y_tmp[7] = 0.0;
    A_y_tmp[2] = -uRL;
    A_y_tmp[5] = 0.0;
    A_y_tmp[8] = uRR;
    for (i = 0; i < 3; i++) {
      d = A_z_tmp[i];
      d1 = A_z_tmp[i + 3];
      b_i = static_cast<int>(A_z_tmp[i + 6]);
      for (i1 = 0; i1 < 3; i1++) {
        A_VQRR[i + 3 * i1] = (d * A_y_tmp[3 * i1] + d1 * A_y_tmp[3 * i1 + 1]) +
          static_cast<double>(b_i) * A_y_tmp[3 * i1 + 2];
      }
    }

    A_z_tmp[0] = 1.0;
    A_z_tmp[3] = 0.0;
    A_z_tmp[6] = 0.0;
    A_z_tmp[1] = 0.0;
    A_z_tmp[4] = b_A_x_tmp_tmp;
    A_z_tmp[7] = -A_x_tmp_tmp;
    A_z_tmp[2] = 0.0;
    A_z_tmp[5] = A_x_tmp_tmp;
    A_z_tmp[8] = b_A_x_tmp_tmp;
    for (i = 0; i < 3; i++) {
      d = A_VQRR[i];
      d1 = A_VQRR[i + 3];
      d2 = A_VQRR[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        A_0V[i + 3 * b_i] = (d * A_z_tmp[3 * b_i] + d1 * A_z_tmp[3 * b_i + 1]) +
          d2 * A_z_tmp[3 * b_i + 2];
      }
    }

    //  Velocities to coordinates
    for (i = 0; i < 196; i++) {
      K[i] = iv[i];
    }

    for (i = 0; i < 3; i++) {
      d = A_0V[i];
      A_y_tmp[3 * i] = d;
      K[14 * i] = d;
      d = A_0V[i + 3];
      A_y_tmp[3 * i + 1] = d;
      K[14 * i + 1] = d;
      d = A_0V[i + 6];
      A_y_tmp[3 * i + 2] = d;
      K[14 * i + 2] = d;
    }

    K[45] = 1.0;
    K[59] = 0.0;
    K[73] = -sin(x[4]);
    K[46] = 0.0;
    K[60] = b_A_x_tmp_tmp;
    K[74] = A_x_tmp_tmp * uRR;
    K[47] = 0.0;
    K[61] = -A_x_tmp_tmp;
    K[75] = b_A_x_tmp_tmp * uRR;
    r = _mm_loadu_pd(&A_y_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(9.80665));
    r = _mm_mul_pd(r, _mm_set1_pd(0.0));
    r1 = _mm_loadu_pd(&A_y_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(9.80665));
    r1 = _mm_mul_pd(r1, _mm_set1_pd(0.0));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&A_y_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(9.80665));
    r = _mm_sub_pd(r, r1);
    _mm_storeu_pd(&g_V[0], r);
    g_V[2] = (A_y_tmp[2] * 9.80665 * 0.0 + A_y_tmp[5] * 9.80665 * 0.0) -
      A_y_tmp[8] * 9.80665;

    //  Suspension kinematics
    //  Front axle
    b_u = 0.014584380239693683 * u[0];

    //  Limit each absolute value of val0 to limit
    if (fabs(b_u) > 0.028) {
      if (rtIsNaN(b_u)) {
        d = rtNaN;
      } else if (b_u < 0.0) {
        d = -1.0;
      } else {
        d = (b_u > 0.0);
      }

      b_u = 0.028 * d;
    }

    //  Front steering rack travel
    //  Double-Wishbone
    delta_FL = f_suspensionDWB(0.0034906585039886592, 0.017453292519943295, dv3,
      dv4, dv5, dv6, dv7, dv8, dv9, dv10, dv11, x[6], b_u, A_VQRR, r_VFLV,
      dvdphi_FL, domdphi_FL, dvdu_FL, domdu_FL);
    delta_FR = f_suspensionDWB(-0.0034906585039886592, -0.017453292519943295,
      dv12, dv13, dv14, dv15, dv16, dv17, dv18, dv19, dv20, x[7], b_u, A_VQRL,
      r_VFRV, dvdphi_FR, domdphi_FR, dvdu_FR, domdu_FR);

    //  Rear axle
    //  Double-Wishbone
    delta_RL = f_suspensionDWB(-0.0087266462599716477, 0.017453292519943295,
      dv21, dv22, dv23, dv24, dv25, dv26, dv27, dv28, dv29, x[8], A_VRL, r_VRLV,
      I_WFRV, I_WFLV);
    delta_RR = f_suspensionDWB(0.0087266462599716477, -0.017453292519943295,
      dv30, dv31, dv32, dv33, dv34, dv35, dv36, dv37, dv38, x[9], A_VQFR, r_VRRV,
      dvdphi_RR, domdphi_RR);

    //  Wheel rot. axis vehicle frame
    //  Wheel rot. axis origin frame
    //  Upright inertia tensor vehicle frame
    for (i = 0; i < 3; i++) {
      d = 0.0;
      d1 = 0.0;
      d2 = 0.0;
      d3 = 0.0;
      for (b_i = 0; b_i < 3; b_i++) {
        i1 = i + 3 * b_i;
        d4 = A_VQRR[i1];
        e_yFLV_tmp = b[b_i];
        d += d4 * static_cast<double>(e_yFLV_tmp);
        d1 += A_VQRL[i1] * static_cast<double>(e_yFLV_tmp);
        d2 += A_VRL[i1] * static_cast<double>(e_yFLV_tmp);
        d3 += A_VQFR[i1] * static_cast<double>(e_yFLV_tmp);
        I_UFLV_tmp[b_i + 3 * i] = d4;
        A_z_tmp[i1] = (A_VQRR[i] * dv39[3 * b_i] + A_VQRR[i + 3] * dv39[3 * b_i
                       + 1]) + A_VQRR[i + 6] * dv39[3 * b_i + 2];
      }

      e_yRRV[i] = d3;
      e_yRLV[i] = d2;
      e_yFRV[i] = d1;
      e_yFLV[i] = d;
    }

    for (i = 0; i < 3; i++) {
      d = A_z_tmp[i];
      d1 = A_z_tmp[i + 3];
      d2 = A_z_tmp[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        e_yFLV_tmp = i + 3 * b_i;
        I_UFLV[e_yFLV_tmp] = (d * I_UFLV_tmp[3 * b_i] + d1 * I_UFLV_tmp[3 * b_i
                              + 1]) + d2 * I_UFLV_tmp[3 * b_i + 2];
        I_UFRV_tmp[b_i + 3 * i] = A_VQRL[e_yFLV_tmp];
      }

      d = A_VQRL[i];
      d1 = A_VQRL[i + 3];
      d2 = A_VQRL[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        A_z_tmp[i + 3 * b_i] = (d * dv39[3 * b_i] + d1 * dv39[3 * b_i + 1]) + d2
          * dv39[3 * b_i + 2];
      }
    }

    for (i = 0; i < 3; i++) {
      d = A_z_tmp[i];
      d1 = A_z_tmp[i + 3];
      d2 = A_z_tmp[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        e_yFLV_tmp = i + 3 * b_i;
        I_UFRV[e_yFLV_tmp] = (d * I_UFRV_tmp[3 * b_i] + d1 * I_UFRV_tmp[3 * b_i
                              + 1]) + d2 * I_UFRV_tmp[3 * b_i + 2];
        I_URLV_tmp[b_i + 3 * i] = A_VRL[e_yFLV_tmp];
      }

      d = A_VRL[i];
      d1 = A_VRL[i + 3];
      d2 = A_VRL[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        A_z_tmp[i + 3 * b_i] = (d * dv39[3 * b_i] + d1 * dv39[3 * b_i + 1]) + d2
          * dv39[3 * b_i + 2];
      }
    }

    for (i = 0; i < 3; i++) {
      d = A_z_tmp[i];
      d1 = A_z_tmp[i + 3];
      d2 = A_z_tmp[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        e_yFLV_tmp = i + 3 * b_i;
        I_URLV[e_yFLV_tmp] = (d * I_URLV_tmp[3 * b_i] + d1 * I_URLV_tmp[3 * b_i
                              + 1]) + d2 * I_URLV_tmp[3 * b_i + 2];
        I_URRV_tmp[b_i + 3 * i] = A_VQFR[e_yFLV_tmp];
      }

      d = A_VQFR[i];
      d1 = A_VQFR[i + 3];
      d2 = A_VQFR[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        A_z_tmp[i + 3 * b_i] = (d * dv39[3 * b_i] + d1 * dv39[3 * b_i + 1]) + d2
          * dv39[3 * b_i + 2];
      }
    }

    //  Wheel inertia tensor vehicle frame
    //  Positions
    r_0V0[0] = x[0];
    r_0V0[1] = x[1];
    r_0V0[2] = x[2];

    //  Velocities
    //  Vehicle velocity vehicle frame
    v_0VV_idx_0 = x[14];
    v_0VV_idx_1 = x[15];
    v_0VV_idx_2 = x[16];

    //  Vel. @ CoG
    omega_0VV[0] = x[17];
    omega_0VV[1] = x[18];
    omega_0VV[2] = x[19];

    //  Angular vel. @ CoG
    v_0FAV_tmp = v_0VV_idx_0 + (omega_0VV[1] * -v[6] - 0.0 * omega_0VV[2]);
    v_0FAV[0] = v_0FAV_tmp;
    b_u = omega_0VV[0] * -v[6];
    v_0FAV[1] = v_0VV_idx_1 + (v[2] * omega_0VV[2] - b_u);
    v_0FAV[2] = v_0VV_idx_2 + (omega_0VV[0] * 0.0 - omega_0VV[1] * v[2]);

    //  Vel. @ front axle
    //  Norm of vel. @ front axle
    v_0RAV[0] = v_0FAV_tmp;
    v_0RAV[1] = v_0VV_idx_1 + (-v[3] * omega_0VV[2] - b_u);
    v_0RAV[2] = v_0VV_idx_2 + (omega_0VV[0] * 0.0 - -v[3] * omega_0VV[1]);

    //  Vel. @ rear axle
    //  Norm of vel. @ rear axle
    //  Wheel center velocity vehicle frame
    //  Wheel center total velocity vehicle frame
    //  Wheel center velocities origin frame
    //  Upright angular velocity vehicle frame
    //  Upright total angular velocity vehicle frame
    //  Upright angular velocity origin frame
    //  Wheel angular velocity vehicle frame
    //  Wheel angular velocity wheel frame
    //  Partial velocities
    //  Prepare skew symmetric cross product matrix from vector r
    dvFLdz[9] = 0.0;
    dvFLdz[10] = -r_VFLV[2];
    dvFLdz[11] = r_VFLV[1];
    dvFLdz[12] = r_VFLV[2];
    dvFLdz[13] = 0.0;
    dvFLdz[14] = -r_VFLV[0];
    dvFLdz[15] = -r_VFLV[1];
    dvFLdz[16] = r_VFLV[0];
    dvFLdz[17] = 0.0;
    d = x[20];
    d1 = x[21];
    d2 = x[22];
    d3 = x[23];
    for (b_i = 0; b_i < 3; b_i++) {
      d4 = A_z_tmp[b_i];
      d5 = A_z_tmp[b_i + 3];
      d6 = A_z_tmp[b_i + 6];
      for (i = 0; i < 3; i++) {
        I_URRV[b_i + 3 * i] = (d4 * I_URRV_tmp[3 * i] + d5 * I_URRV_tmp[3 * i +
          1]) + d6 * I_URRV_tmp[3 * i + 2];
      }

      d4 = A_VQRR[b_i];
      d5 = A_VQRR[b_i + 3];
      d6 = A_VQRR[b_i + 6];
      for (i = 0; i < 3; i++) {
        A_z_tmp[b_i + 3 * i] = (d4 * dv40[3 * i] + d5 * dv40[3 * i + 1]) + d6 *
          dv40[3 * i + 2];
      }

      d4 = A_z_tmp[b_i];
      d5 = A_z_tmp[b_i + 3];
      d6 = A_z_tmp[b_i + 6];
      for (i = 0; i < 3; i++) {
        b_I_WFLV[b_i + 3 * i] = (d4 * I_UFLV_tmp[3 * i] + d5 * I_UFLV_tmp[3 * i
          + 1]) + d6 * I_UFLV_tmp[3 * i + 2];
      }

      d4 = A_VQRL[b_i];
      d5 = A_VQRL[b_i + 3];
      d6 = A_VQRL[b_i + 6];
      for (i = 0; i < 3; i++) {
        A_z_tmp[b_i + 3 * i] = (d4 * dv40[3 * i] + d5 * dv40[3 * i + 1]) + d6 *
          dv40[3 * i + 2];
      }

      d4 = A_z_tmp[b_i];
      d5 = A_z_tmp[b_i + 3];
      d6 = A_z_tmp[b_i + 6];
      for (i = 0; i < 3; i++) {
        b_I_WFRV[b_i + 3 * i] = (d4 * I_UFRV_tmp[3 * i] + d5 * I_UFRV_tmp[3 * i
          + 1]) + d6 * I_UFRV_tmp[3 * i + 2];
      }

      d4 = A_VRL[b_i];
      d5 = A_VRL[b_i + 3];
      d6 = A_VRL[b_i + 6];
      for (i = 0; i < 3; i++) {
        A_z_tmp[b_i + 3 * i] = (d4 * dv40[3 * i] + d5 * dv40[3 * i + 1]) + d6 *
          dv40[3 * i + 2];
      }

      d4 = A_z_tmp[b_i];
      d5 = A_z_tmp[b_i + 3];
      d6 = A_z_tmp[b_i + 6];
      for (i = 0; i < 3; i++) {
        I_WRLV[b_i + 3 * i] = (d4 * I_URLV_tmp[3 * i] + d5 * I_URLV_tmp[3 * i +
          1]) + d6 * I_URLV_tmp[3 * i + 2];
      }

      d4 = A_VQFR[b_i];
      d5 = A_VQFR[b_i + 3];
      d6 = A_VQFR[b_i + 6];
      for (i = 0; i < 3; i++) {
        A_z_tmp[b_i + 3 * i] = (d4 * dv40[3 * i] + d5 * dv40[3 * i + 1]) + d6 *
          dv40[3 * i + 2];
      }

      d4 = A_z_tmp[b_i];
      d5 = A_z_tmp[b_i + 3];
      d6 = A_z_tmp[b_i + 6];
      for (i = 0; i < 3; i++) {
        I_WRRV[b_i + 3 * i] = (d4 * I_URRV_tmp[3 * i] + d5 * I_URRV_tmp[3 * i +
          1]) + d6 * I_URRV_tmp[3 * i + 2];
      }

      d4 = dvdphi_FL[b_i];
      r_VFLV_dot[b_i] = d4 * d;
      r_VFRV_dot[b_i] = dvdphi_FR[b_i] * d1;
      r_VRLV_dot[b_i] = I_WFRV[b_i] * d2;
      r_VRRV_dot[b_i] = dvdphi_RR[b_i] * d3;
      d5 = domdphi_FL[b_i] * d;
      omega_VUFLV[b_i] = d5;
      d6 = domdphi_FR[b_i] * d1;
      omega_VUFRV[b_i] = d6;
      d7 = I_WFLV[b_i] * d2;
      omega_VURLV[b_i] = d7;
      d8 = domdphi_RR[b_i] * d3;
      omega_VURRV[b_i] = d8;
      d9 = omega_0VV[b_i];
      d5 += d9;
      omega_0UFLV[b_i] = d5;
      d6 += d9;
      omega_0UFRV[b_i] = d6;
      d7 += d9;
      omega_0URLV[b_i] = d7;
      d8 += d9;
      omega_0URRV[b_i] = d8;
      d9 = e_yFLV[b_i] * x[24];
      omega_0WFLV_tmp[b_i] = d9;
      omega_0WFLV[b_i] = d5 + d9;
      d5 = e_yFRV[b_i] * x[25];
      omega_0WFRV_tmp[b_i] = d5;
      omega_0WFRV[b_i] = d6 + d5;
      d5 = e_yRLV[b_i] * x[26];
      omega_0WRLV_tmp[b_i] = d5;
      omega_0WRLV[b_i] = d7 + d5;
      d5 = e_yRRV[b_i] * x[27];
      omega_0WRRV_tmp[b_i] = d5;
      omega_0WRRV[b_i] = d8 + d5;
      dvFLdz[3 * b_i] = iv1[3 * b_i];
      e_yFLV_tmp = 3 * b_i + 1;
      dvFLdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
      e_yFLV_tmp = 3 * b_i + 2;
      dvFLdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
      dvFLdz[b_i + 18] = d4;
    }

    v_0FLV_idx_0 = (v_0VV_idx_0 + (omega_0VV[1] * r_VFLV[2] - r_VFLV[1] *
      omega_0VV[2])) + r_VFLV_dot[0];
    v_0FLV_idx_1 = (v_0VV_idx_1 + (r_VFLV[0] * omega_0VV[2] - omega_0VV[0] *
      r_VFLV[2])) + r_VFLV_dot[1];
    v_0FLV_idx_2 = (v_0VV_idx_2 + (omega_0VV[0] * r_VFLV[1] - r_VFLV[0] *
      omega_0VV[1])) + r_VFLV_dot[2];
    v_0FRV_idx_0 = (v_0VV_idx_0 + (omega_0VV[1] * r_VFRV[2] - r_VFRV[1] *
      omega_0VV[2])) + r_VFRV_dot[0];
    v_0FRV_idx_1 = (v_0VV_idx_1 + (r_VFRV[0] * omega_0VV[2] - omega_0VV[0] *
      r_VFRV[2])) + r_VFRV_dot[1];
    v_0FRV_idx_2 = (v_0VV_idx_2 + (omega_0VV[0] * r_VFRV[1] - r_VFRV[0] *
      omega_0VV[1])) + r_VFRV_dot[2];
    v_0RLV_idx_0 = (v_0VV_idx_0 + (omega_0VV[1] * r_VRLV[2] - r_VRLV[1] *
      omega_0VV[2])) + r_VRLV_dot[0];
    v_0RLV_idx_1 = (v_0VV_idx_1 + (r_VRLV[0] * omega_0VV[2] - omega_0VV[0] *
      r_VRLV[2])) + r_VRLV_dot[1];
    v_0RLV_idx_2 = (v_0VV_idx_2 + (omega_0VV[0] * r_VRLV[1] - r_VRLV[0] *
      omega_0VV[1])) + r_VRLV_dot[2];
    v_0RRV_idx_0 = (v_0VV_idx_0 + (omega_0VV[1] * r_VRRV[2] - r_VRRV[1] *
      omega_0VV[2])) + r_VRRV_dot[0];
    v_0RRV_idx_1 = (v_0VV_idx_1 + (r_VRRV[0] * omega_0VV[2] - omega_0VV[0] *
      r_VRRV[2])) + r_VRRV_dot[1];
    v_0RRV_idx_2 = (v_0VV_idx_2 + (omega_0VV[0] * r_VRRV[1] - r_VRRV[0] *
      omega_0VV[1])) + r_VRRV_dot[2];
    for (i = 0; i < 7; i++) {
      e_yFLV_tmp = 3 * (i + 7);
      dvFLdz[e_yFLV_tmp] = 0.0;
      dvFLdz[e_yFLV_tmp + 1] = 0.0;
      dvFLdz[e_yFLV_tmp + 2] = 0.0;
    }

    //  Prepare skew symmetric cross product matrix from vector r
    dvFRdz[9] = 0.0;
    dvFRdz[10] = -r_VFRV[2];
    dvFRdz[11] = r_VFRV[1];
    dvFRdz[12] = r_VFRV[2];
    dvFRdz[13] = 0.0;
    dvFRdz[14] = -r_VFRV[0];
    dvFRdz[15] = -r_VFRV[1];
    dvFRdz[16] = r_VFRV[0];
    dvFRdz[17] = 0.0;
    for (i = 0; i < 3; i++) {
      dvFRdz[3 * i] = iv1[3 * i];
      e_yFLV_tmp = 3 * i + 1;
      dvFRdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
      e_yFLV_tmp = 3 * i + 2;
      dvFRdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
      dvFRdz[i + 18] = 0.0;
      dvFRdz[i + 21] = dvdphi_FR[i];
    }

    for (i = 0; i < 6; i++) {
      e_yFLV_tmp = 3 * (i + 8);
      dvFRdz[e_yFLV_tmp] = 0.0;
      dvFRdz[e_yFLV_tmp + 1] = 0.0;
      dvFRdz[e_yFLV_tmp + 2] = 0.0;
    }

    //  Prepare skew symmetric cross product matrix from vector r
    for (i = 0; i < 3; i++) {
      dvRLdz[3 * i] = iv1[3 * i];
      e_yFLV_tmp = 3 * i + 1;
      dvRLdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
      e_yFLV_tmp = 3 * i + 2;
      dvRLdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
    }

    dvRLdz[9] = 0.0;
    dvRLdz[10] = -r_VRLV[2];
    dvRLdz[11] = r_VRLV[1];
    dvRLdz[12] = r_VRLV[2];
    dvRLdz[13] = 0.0;
    dvRLdz[14] = -r_VRLV[0];
    dvRLdz[15] = -r_VRLV[1];
    dvRLdz[16] = r_VRLV[0];
    dvRLdz[17] = 0.0;
    for (i = 0; i < 2; i++) {
      e_yFLV_tmp = 3 * (i + 6);
      dvRLdz[e_yFLV_tmp] = 0.0;
      dvRLdz[e_yFLV_tmp + 1] = 0.0;
      dvRLdz[e_yFLV_tmp + 2] = 0.0;
    }

    dvRLdz[24] = I_WFRV[0];
    dvRLdz[25] = I_WFRV[1];
    dvRLdz[26] = I_WFRV[2];
    for (i = 0; i < 5; i++) {
      e_yFLV_tmp = 3 * (i + 9);
      dvRLdz[e_yFLV_tmp] = 0.0;
      dvRLdz[e_yFLV_tmp + 1] = 0.0;
      dvRLdz[e_yFLV_tmp + 2] = 0.0;
    }

    //  Prepare skew symmetric cross product matrix from vector r
    dvRRdz[9] = 0.0;
    dvRRdz[10] = -r_VRRV[2];
    dvRRdz[11] = r_VRRV[1];
    dvRRdz[12] = r_VRRV[2];
    dvRRdz[13] = 0.0;
    dvRRdz[14] = -r_VRRV[0];
    dvRRdz[15] = -r_VRRV[1];
    dvRRdz[16] = r_VRRV[0];
    dvRRdz[17] = 0.0;
    for (i = 0; i < 3; i++) {
      dvRRdz[3 * i] = iv1[3 * i];
      e_yFLV_tmp = 3 * (i + 6);
      dvRRdz[e_yFLV_tmp] = 0.0;
      dvRRdz_tmp = 3 * i + 1;
      dvRRdz[dvRRdz_tmp] = iv1[dvRRdz_tmp];
      dvRRdz[e_yFLV_tmp + 1] = 0.0;
      dvRRdz_tmp = 3 * i + 2;
      dvRRdz[dvRRdz_tmp] = iv1[dvRRdz_tmp];
      dvRRdz[e_yFLV_tmp + 2] = 0.0;
      dvRRdz[i + 27] = dvdphi_RR[i];
    }

    for (i = 0; i < 4; i++) {
      e_yFLV_tmp = 3 * (i + 10);
      dvRRdz[e_yFLV_tmp] = 0.0;
      dvRRdz[e_yFLV_tmp + 1] = 0.0;
      dvRRdz[e_yFLV_tmp + 2] = 0.0;
    }

    //  Partial angular velocities
    //  Wheels
    coder::eye(A_VQRR);
    r = _mm_loadu_pd(&e_yFLV[0]);
    r1 = _mm_set1_pd(0.0);
    r2 = _mm_loadu_pd(&e_yFRV[0]);
    for (i = 0; i < 3; i++) {
      r3 = _mm_loadu_pd(&A_VQRR[3 * i]);
      _mm_storeu_pd(&domWFLdz[3 * i], r1);
      b_i = 3 * (i + 3);
      _mm_storeu_pd(&domWFLdz[b_i], _mm_sub_pd(r3, _mm_add_pd(r1, _mm_mul_pd(r,
        _mm_set1_pd(e_yFLV[i])))));
      e_yFLV_tmp = 3 * i + 2;
      domWFLdz[e_yFLV_tmp] = 0.0;
      b_u = A_VQRR[e_yFLV_tmp];
      domWFLdz[b_i + 2] = b_u - e_yFLV[2] * e_yFLV[i];
      domWFLdz[i + 18] = domdphi_FL[i];
      dvRRdz_tmp = 3 * (i + 7);
      domWFLdz[dvRRdz_tmp] = 0.0;
      domWFLdz[dvRRdz_tmp + 1] = 0.0;
      domWFLdz[dvRRdz_tmp + 2] = 0.0;
      domWFLdz[i + 30] = e_yFLV[i];
      i1 = 3 * (i + 11);
      _mm_storeu_pd(&domWFLdz[i1], r1);
      r3 = _mm_loadu_pd(&A_VQRR[3 * i]);
      _mm_storeu_pd(&domWFRdz[3 * i], r1);
      _mm_storeu_pd(&domWFRdz[b_i], _mm_sub_pd(r3, _mm_add_pd(r1, _mm_mul_pd(r2,
        _mm_set1_pd(e_yFRV[i])))));
      domWFLdz[i1 + 2] = 0.0;
      domWFRdz[e_yFLV_tmp] = 0.0;
      domWFRdz[b_i + 2] = b_u - e_yFRV[2] * e_yFRV[i];
      domWFRdz[i + 18] = 0.0;
      domWFRdz[i + 21] = domdphi_FR[i];
      e_yFLV_tmp = 3 * (i + 8);
      domWFRdz[e_yFLV_tmp] = 0.0;
      domWFRdz[e_yFLV_tmp + 1] = 0.0;
      domWFRdz[e_yFLV_tmp + 2] = 0.0;
      domWFRdz[i + 33] = e_yFRV[i];
    }

    r = _mm_loadu_pd(&e_yRLV[0]);
    r1 = _mm_loadu_pd(&A_VQRR[0]);
    r2 = _mm_set1_pd(0.0);
    _mm_storeu_pd(&domWRLdz[0], r2);
    _mm_storeu_pd(&domWRLdz[9], _mm_sub_pd(r1, _mm_add_pd(r2, _mm_mul_pd(r,
      _mm_set1_pd(e_yRLV[0])))));
    domWRLdz[2] = 0.0;
    domWRLdz[11] = A_VQRR[2] - e_yRLV[0] * e_yRLV[2];
    r = _mm_loadu_pd(&e_yRLV[0]);
    r1 = _mm_loadu_pd(&A_VQRR[3]);
    _mm_storeu_pd(&domWRLdz[3], r2);
    _mm_storeu_pd(&domWRLdz[12], _mm_sub_pd(r1, _mm_add_pd(r2, _mm_mul_pd(r,
      _mm_set1_pd(e_yRLV[1])))));
    domWRLdz[5] = 0.0;
    domWRLdz[14] = A_VQRR[5] - e_yRLV[1] * e_yRLV[2];
    r = _mm_loadu_pd(&e_yRLV[0]);
    r1 = _mm_loadu_pd(&A_VQRR[6]);
    _mm_storeu_pd(&domWRLdz[6], r2);
    _mm_storeu_pd(&domWRLdz[15], _mm_sub_pd(r1, _mm_add_pd(r2, _mm_mul_pd(r,
      _mm_set1_pd(e_yRLV[2])))));
    domWRLdz[8] = 0.0;
    domWRLdz[17] = A_VQRR[8] - e_yRLV[2] * e_yRLV[2];
    for (i = 0; i < 2; i++) {
      e_yFLV_tmp = 3 * (i + 12);
      domWFRdz[e_yFLV_tmp] = 0.0;
      domWFRdz[e_yFLV_tmp + 1] = 0.0;
      domWFRdz[e_yFLV_tmp + 2] = 0.0;
      e_yFLV_tmp = 3 * (i + 6);
      domWRLdz[e_yFLV_tmp] = 0.0;
      domWRLdz[e_yFLV_tmp + 1] = 0.0;
      domWRLdz[e_yFLV_tmp + 2] = 0.0;
    }

    //  Uprights
    r = _mm_loadu_pd(&e_yRRV[0]);
    for (i = 0; i < 3; i++) {
      domWRLdz[i + 24] = I_WFLV[i];
      b_i = 3 * (i + 9);
      _mm_storeu_pd(&domWRLdz[b_i], r2);
      r1 = _mm_loadu_pd(&A_VQRR[3 * i]);
      _mm_storeu_pd(&domWRRdz[3 * i], r2);
      i1 = 3 * (i + 3);
      _mm_storeu_pd(&domWRRdz[i1], _mm_sub_pd(r1, _mm_add_pd(r2, _mm_mul_pd(r,
        _mm_set1_pd(e_yRRV[i])))));
      i2 = 3 * (i + 6);
      _mm_storeu_pd(&domWRRdz[i2], r2);
      domWRLdz[b_i + 2] = 0.0;
      b_i = 3 * i + 2;
      domWRRdz[b_i] = 0.0;
      domWRRdz[i1 + 2] = A_VQRR[b_i] - e_yRRV[2] * e_yRRV[i];
      domWRRdz[i2 + 2] = 0.0;
      domWRLdz[i + 36] = e_yRLV[i];
      domWRLdz[i + 39] = 0.0;
      domWRRdz[i + 27] = domdphi_RR[i];
      dvRRdz_tmp = 3 * (i + 10);
      domWRRdz[dvRRdz_tmp] = 0.0;
      domUFLdz[3 * i] = 0.0;
      domUFLdz[i1] = iv1[3 * i];
      domWRRdz[dvRRdz_tmp + 1] = 0.0;
      e_yFLV_tmp = 3 * i + 1;
      domUFLdz[e_yFLV_tmp] = 0.0;
      domUFLdz[i1 + 1] = iv1[e_yFLV_tmp];
      domWRRdz[dvRRdz_tmp + 2] = 0.0;
      domUFLdz[b_i] = 0.0;
      domUFLdz[i1 + 2] = iv1[b_i];
      domWRRdz[i + 39] = e_yRRV[i];
      domUFLdz[i + 18] = domdphi_FL[i];
    }

    for (i = 0; i < 7; i++) {
      e_yFLV_tmp = 3 * (i + 7);
      domUFLdz[e_yFLV_tmp] = 0.0;
      domUFLdz[e_yFLV_tmp + 1] = 0.0;
      domUFLdz[e_yFLV_tmp + 2] = 0.0;
    }

    for (i = 0; i < 3; i++) {
      domUFRdz[3 * i] = 0.0;
      e_yFLV_tmp = 3 * (i + 3);
      domUFRdz[e_yFLV_tmp] = iv1[3 * i];
      dvRRdz_tmp = 3 * i + 1;
      domUFRdz[dvRRdz_tmp] = 0.0;
      domUFRdz[e_yFLV_tmp + 1] = iv1[dvRRdz_tmp];
      dvRRdz_tmp = 3 * i + 2;
      domUFRdz[dvRRdz_tmp] = 0.0;
      domUFRdz[e_yFLV_tmp + 2] = iv1[dvRRdz_tmp];
      domUFRdz[i + 18] = 0.0;
      domUFRdz[i + 21] = domdphi_FR[i];
    }

    for (i = 0; i < 6; i++) {
      e_yFLV_tmp = 3 * (i + 8);
      domUFRdz[e_yFLV_tmp] = 0.0;
      domUFRdz[e_yFLV_tmp + 1] = 0.0;
      domUFRdz[e_yFLV_tmp + 2] = 0.0;
    }

    for (i = 0; i < 3; i++) {
      domURLdz[3 * i] = 0.0;
      e_yFLV_tmp = 3 * (i + 3);
      domURLdz[e_yFLV_tmp] = iv1[3 * i];
      dvRRdz_tmp = 3 * i + 1;
      domURLdz[dvRRdz_tmp] = 0.0;
      domURLdz[e_yFLV_tmp + 1] = iv1[dvRRdz_tmp];
      dvRRdz_tmp = 3 * i + 2;
      domURLdz[dvRRdz_tmp] = 0.0;
      domURLdz[e_yFLV_tmp + 2] = iv1[dvRRdz_tmp];
    }

    for (i = 0; i < 2; i++) {
      e_yFLV_tmp = 3 * (i + 6);
      domURLdz[e_yFLV_tmp] = 0.0;
      domURLdz[e_yFLV_tmp + 1] = 0.0;
      domURLdz[e_yFLV_tmp + 2] = 0.0;
    }

    domURLdz[24] = I_WFLV[0];
    domURLdz[25] = I_WFLV[1];
    domURLdz[26] = I_WFLV[2];
    for (i = 0; i < 5; i++) {
      e_yFLV_tmp = 3 * (i + 9);
      domURLdz[e_yFLV_tmp] = 0.0;
      domURLdz[e_yFLV_tmp + 1] = 0.0;
      domURLdz[e_yFLV_tmp + 2] = 0.0;
    }

    for (i = 0; i < 3; i++) {
      domURRdz[3 * i] = 0.0;
      dvRRdz_tmp = 3 * (i + 3);
      domURRdz[dvRRdz_tmp] = iv1[3 * i];
      e_yFLV_tmp = 3 * (i + 6);
      domURRdz[e_yFLV_tmp] = 0.0;
      b_i = 3 * i + 1;
      domURRdz[b_i] = 0.0;
      domURRdz[dvRRdz_tmp + 1] = iv1[b_i];
      domURRdz[e_yFLV_tmp + 1] = 0.0;
      b_i = 3 * i + 2;
      domURRdz[b_i] = 0.0;
      domURRdz[dvRRdz_tmp + 2] = iv1[b_i];
      domURRdz[e_yFLV_tmp + 2] = 0.0;
      domURRdz[i + 27] = domdphi_RR[i];
    }

    for (i = 0; i < 4; i++) {
      dvRRdz_tmp = 3 * (i + 10);
      domURRdz[dvRRdz_tmp] = 0.0;
      domURRdz[dvRRdz_tmp + 1] = 0.0;
      domURRdz[dvRRdz_tmp + 2] = 0.0;
    }

    //  Tire road contact
    d = r_VFLV[0];
    d1 = r_VFLV[1];
    d2 = r_VFLV[2];
    d3 = omega_0UFLV[0];
    d4 = omega_0UFLV[1];
    d5 = omega_0UFLV[2];
    d6 = e_yFLV[0];
    d7 = e_yFLV[1];
    d8 = e_yFLV[2];
    r = _mm_loadu_pd(&A_0V[0]);
    r1 = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_mul_pd(r, _mm_set1_pd(v_0FLV_idx_0));
    r3 = _mm_mul_pd(r, _mm_set1_pd(d3));
    r = _mm_mul_pd(r, _mm_set1_pd(d6));
    r4 = _mm_loadu_pd(&A_0V[3]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d1));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0FLV_idx_1));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d4));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d7));
    r = _mm_add_pd(r, r4);
    r4 = _mm_loadu_pd(&A_0V[6]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d2));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0FLV_idx_2));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d5));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d8));
    r = _mm_add_pd(r, r4);
    _mm_storeu_pd(&d_A_0V[0], r);
    _mm_storeu_pd(&c_A_0V[0], r3);
    _mm_storeu_pd(&b_A_0V[0], r2);
    r = _mm_loadu_pd(&r_0V0[0]);
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&b_r_0V0[0], r);
    d9 = A_0V[2];
    d10 = d9 * d;
    d11 = d9 * v_0FLV_idx_0;
    d12 = d9 * d3;
    d13 = d9 * d6;
    d9 = A_0V[5];
    d10 += d9 * d1;
    d11 += d9 * v_0FLV_idx_1;
    d12 += d9 * d4;
    d13 += d9 * d7;
    d9 = A_0V[8];
    d10 += d9 * d2;
    d11 += d9 * v_0FLV_idx_2;
    d12 += d9 * d5;
    d13 += d9 * d8;
    d_A_0V[2] = d13;
    c_A_0V[2] = d12;
    b_A_0V[2] = d11;
    b_r_0V0[2] = r_0V0[2] + d10;
    gamma_FL = f_roadContact(b_expl_temp.x, b_expl_temp.y, rz, b_expl_temp.Mu,
      b_r_0V0, b_A_0V, c_A_0V, d_A_0V, dvdphi_FL, A_VQRL, r_stFL, b_u, uFR,
      I_WFRV, uRL);
    d = r_VFRV[0];
    d1 = r_VFRV[1];
    d2 = r_VFRV[2];
    d3 = omega_0UFRV[0];
    d4 = omega_0UFRV[1];
    d5 = omega_0UFRV[2];
    d6 = e_yFRV[0];
    d7 = e_yFRV[1];
    d8 = e_yFRV[2];
    r = _mm_loadu_pd(&A_0V[0]);
    r1 = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_mul_pd(r, _mm_set1_pd(v_0FRV_idx_0));
    r3 = _mm_mul_pd(r, _mm_set1_pd(d3));
    r = _mm_mul_pd(r, _mm_set1_pd(d6));
    r4 = _mm_loadu_pd(&A_0V[3]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d1));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0FRV_idx_1));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d4));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d7));
    r = _mm_add_pd(r, r4);
    r4 = _mm_loadu_pd(&A_0V[6]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d2));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0FRV_idx_2));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d5));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d8));
    r = _mm_add_pd(r, r4);
    _mm_storeu_pd(&d_A_0V[0], r);
    _mm_storeu_pd(&c_A_0V[0], r3);
    _mm_storeu_pd(&b_A_0V[0], r2);
    r = _mm_loadu_pd(&r_0V0[0]);
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&b_r_0V0[0], r);
    d9 = A_0V[2];
    d10 = d9 * d;
    d11 = d9 * v_0FRV_idx_0;
    d12 = d9 * d3;
    d13 = d9 * d6;
    d9 = A_0V[5];
    d10 += d9 * d1;
    d11 += d9 * v_0FRV_idx_1;
    d12 += d9 * d4;
    d13 += d9 * d7;
    d9 = A_0V[8];
    d10 += d9 * d2;
    d11 += d9 * v_0FRV_idx_2;
    d12 += d9 * d5;
    d13 += d9 * d8;
    d_A_0V[2] = d13;
    c_A_0V[2] = d12;
    b_A_0V[2] = d11;
    b_r_0V0[2] = r_0V0[2] + d10;
    gamma_FR = f_roadContact(b_expl_temp.x, b_expl_temp.y, rz, b_expl_temp.Mu,
      b_r_0V0, b_A_0V, c_A_0V, d_A_0V, domdphi_FL, A_VRL, r_stFR, uRR,
      A_x_tmp_tmp, r_MQFR0, b_A_x_tmp_tmp);
    d = r_VRLV[0];
    d1 = r_VRLV[1];
    d2 = r_VRLV[2];
    d3 = omega_0URLV[0];
    d4 = omega_0URLV[1];
    d5 = omega_0URLV[2];
    d6 = e_yRLV[0];
    d7 = e_yRLV[1];
    d8 = e_yRLV[2];
    r = _mm_loadu_pd(&A_0V[0]);
    r1 = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_mul_pd(r, _mm_set1_pd(v_0RLV_idx_0));
    r3 = _mm_mul_pd(r, _mm_set1_pd(d3));
    r = _mm_mul_pd(r, _mm_set1_pd(d6));
    r4 = _mm_loadu_pd(&A_0V[3]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d1));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0RLV_idx_1));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d4));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d7));
    r = _mm_add_pd(r, r4);
    r4 = _mm_loadu_pd(&A_0V[6]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d2));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0RLV_idx_2));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d5));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d8));
    r = _mm_add_pd(r, r4);
    _mm_storeu_pd(&d_A_0V[0], r);
    _mm_storeu_pd(&c_A_0V[0], r3);
    _mm_storeu_pd(&b_A_0V[0], r2);
    r = _mm_loadu_pd(&r_0V0[0]);
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&b_r_0V0[0], r);
    d9 = A_0V[2];
    d10 = d9 * d;
    d11 = d9 * v_0RLV_idx_0;
    d12 = d9 * d3;
    d13 = d9 * d6;
    d9 = A_0V[5];
    d10 += d9 * d1;
    d11 += d9 * v_0RLV_idx_1;
    d12 += d9 * d4;
    d13 += d9 * d7;
    d9 = A_0V[8];
    d10 += d9 * d2;
    d11 += d9 * v_0RLV_idx_2;
    d12 += d9 * d5;
    d13 += d9 * d8;
    d_A_0V[2] = d13;
    c_A_0V[2] = d12;
    b_A_0V[2] = d11;
    b_r_0V0[2] = r_0V0[2] + d10;
    gamma_RL = f_roadContact(b_expl_temp.x, b_expl_temp.y, rz, b_expl_temp.Mu,
      b_r_0V0, b_A_0V, c_A_0V, d_A_0V, dvdphi_FR, A_VQRR, r_stRL, dz_RL, w_RL,
      r_MQRL0, mu_RRL);
    d = r_VRRV[0];
    d1 = r_VRRV[1];
    d2 = r_VRRV[2];
    d3 = omega_0URRV[0];
    d4 = omega_0URRV[1];
    d5 = omega_0URRV[2];
    d6 = e_yRRV[0];
    d7 = e_yRRV[1];
    d8 = e_yRRV[2];
    r = _mm_loadu_pd(&A_0V[0]);
    r1 = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_mul_pd(r, _mm_set1_pd(v_0RRV_idx_0));
    r3 = _mm_mul_pd(r, _mm_set1_pd(d3));
    r = _mm_mul_pd(r, _mm_set1_pd(d6));
    r4 = _mm_loadu_pd(&A_0V[3]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d1));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0RRV_idx_1));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d4));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d7));
    r = _mm_add_pd(r, r4);
    r4 = _mm_loadu_pd(&A_0V[6]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d2));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0RRV_idx_2));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d5));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d8));
    r = _mm_add_pd(r, r4);
    _mm_storeu_pd(&d_A_0V[0], r);
    _mm_storeu_pd(&c_A_0V[0], r3);
    _mm_storeu_pd(&b_A_0V[0], r2);
    r = _mm_loadu_pd(&r_0V0[0]);
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&b_r_0V0[0], r);
    d9 = A_0V[2];
    d10 = d9 * d;
    d11 = d9 * v_0RRV_idx_0;
    d12 = d9 * d3;
    d13 = d9 * d6;
    d9 = A_0V[5];
    d10 += d9 * d1;
    d11 += d9 * v_0RRV_idx_1;
    d12 += d9 * d4;
    d13 += d9 * d7;
    d9 = A_0V[8];
    d10 += d9 * d2;
    d11 += d9 * v_0RRV_idx_2;
    d12 += d9 * d5;
    d13 += d9 * d8;
    d_A_0V[2] = d13;
    c_A_0V[2] = d12;
    b_A_0V[2] = d11;
    b_r_0V0[2] = r_0V0[2] + d10;
    gamma_RR = f_roadContact(b_expl_temp.x, b_expl_temp.y, rz, b_expl_temp.Mu,
      b_r_0V0, b_A_0V, c_A_0V, d_A_0V, domdphi_FR, A_z_tmp, r_stRR, dz_RR, w_RR,
      r_MQRR0, mu_RRR);
    for (i = 0; i < 3; i++) {
      d = A_y_tmp[i];
      d1 = A_y_tmp[i + 3];
      d2 = A_y_tmp[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        i1 = 3 * b_i + 1;
        i2 = 3 * b_i + 2;
        dvRRdz_tmp = i + 3 * b_i;
        A_VQFR[dvRRdz_tmp] = (d * A_VRL[3 * b_i] + d1 * A_VRL[i1]) + d2 *
          A_VRL[i2];
        A_0V[dvRRdz_tmp] = (d * A_VQRL[3 * b_i] + d1 * A_VQRL[i1]) + d2 *
          A_VQRL[i2];
      }
    }

    for (i = 0; i < 3; i++) {
      d = A_y_tmp[i];
      d1 = A_y_tmp[i + 3];
      d2 = A_y_tmp[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        A_VQRL[i + 3 * b_i] = (d * A_VQRR[3 * b_i] + d1 * A_VQRR[3 * b_i + 1]) +
          d2 * A_VQRR[3 * b_i + 2];
      }
    }

    //  Tire forces
    //  TMeasy
    for (i = 0; i < 3; i++) {
      d = A_y_tmp[i];
      d1 = A_y_tmp[i + 3];
      d2 = A_y_tmp[i + 6];
      d3 = 0.0;
      for (b_i = 0; b_i < 3; b_i++) {
        e_yFLV_tmp = i + 3 * b_i;
        A_VQRR[e_yFLV_tmp] = (d * A_z_tmp[3 * b_i] + d1 * A_z_tmp[3 * b_i + 1])
          + d2 * A_z_tmp[3 * b_i + 2];
        d3 += I_UFLV_tmp[e_yFLV_tmp] * omega_0WFLV[b_i];
      }

      c_A_0V[i] = d3;
    }

    r_dynFL = f_tireTMeasy(dvdphi_FL, c_A_0V, r_stFL, b_u, uFR, gamma_FL, 0.7 *
      u[5] * uRL, &x[28], F_TFL, I_WFLV, s_FL, x_TFL_dot, F_ycFL, dF_x0FL,
      F_xMFL, dF_y0FL, F_yMFL);
    d = omega_0WFRV[0];
    d1 = omega_0WFRV[1];
    d2 = omega_0WFRV[2];
    r = _mm_loadu_pd(&I_UFRV_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&I_UFRV_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&I_UFRV_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&b_r_0V0[0], r);
    b_r_0V0[2] = (I_UFRV_tmp[2] * d + I_UFRV_tmp[5] * d1) + I_UFRV_tmp[8] * d2;
    r_dynFR = f_tireTMeasy(domdphi_FL, b_r_0V0, r_stFR, uRR, A_x_tmp_tmp,
      gamma_FR, 0.7 * u[5] * b_A_x_tmp_tmp, &x[31], F_TFR, dvdphi_RR, s_FR,
      x_TFR_dot, F_ycFR, dF_x0FR, F_xMFR, dF_y0FR, F_yMFR);
    d = omega_0WRLV[0];
    d1 = omega_0WRLV[1];
    d2 = omega_0WRLV[2];
    r = _mm_loadu_pd(&I_URLV_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&I_URLV_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&I_URLV_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&b_r_0V0[0], r);
    b_r_0V0[2] = (I_URLV_tmp[2] * d + I_URLV_tmp[5] * d1) + I_URLV_tmp[8] * d2;
    r_dynRL = f_tireTMeasy(dvdphi_FR, b_r_0V0, r_stRL, dz_RL, w_RL, gamma_RL,
      0.7 * u[5] * mu_RRL, &x[34], F_TRL, T_TRL, s_RL, x_TRL_dot, F_ycRL,
      dF_x0RL, F_xMRL, dF_y0RL, F_yMRL);
    d = omega_0WRRV[0];
    d1 = omega_0WRRV[1];
    d2 = omega_0WRRV[2];
    r = _mm_loadu_pd(&I_URRV_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&I_URRV_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&I_URRV_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&b_r_0V0[0], r);
    b_r_0V0[2] = (I_URRV_tmp[2] * d + I_URRV_tmp[5] * d1) + I_URRV_tmp[8] * d2;
    r_dynRR = f_tireTMeasy(domdphi_FR, b_r_0V0, r_stRR, dz_RR, w_RR, gamma_RR,
      0.7 * u[5] * mu_RRR, &x[37], F_TRR, T_TRR, s_RR, x_TRR_dot, F_ycRR,
      dF_x0RR, F_xMRR, dF_y0RR, F_yMRR);

    //  Suspension forces
    z_FL = f_suspensionForces(r_VFLV, r_VFRV, r_VRLV, r_VRRV, r_VFLV_dot,
      r_VFRV_dot, r_VRLV_dot, r_VRRV_dot, v[6], v[8], v[9], v[4], v[4], v[5], v
      [5], z_FR, z_RL, z_RR, z_FL_dot, z_FR_dot, z_RL_dot, z_RR_dot, b_u, uFR,
      uRL, uRR, A_x_tmp_tmp, b_A_x_tmp_tmp, dz_RR, w_RR, F_SFL, F_SFR, F_SRL,
      F_SRR);

    //  Aerodynamic Forces
    d = v_0FAV_tmp * v_0FAV_tmp;
    f_aerodynamics(sqrt((d + v_0FAV[1] * v_0FAV[1]) + v_0FAV[2] * v_0FAV[2]),
                   v_0FAV, sqrt((d + v_0RAV[1] * v_0RAV[1]) + v_0RAV[2] *
      v_0RAV[2]), v_0RAV, dv41, car[1], car[2], car[0], dvdphi_FL, domdphi_FL,
                   dvdphi_FR, domdphi_FR, r_0V0, F_ARAV, out_aero);

    //  Applied forces & torques
    //  Wheel rot. friction damping coef.
    //  Powertrain
    f_powertrain(x[24], x[25], x[26], x[27], x, u[3], v[7], veh_i_G, uFR, uRL,
                 uRR, T_DFL, T_DFR, T_DRL, T_DRR);

    //  Brake torques
    T_BFL = -39.111507258094768 * x[24];
    b_u = u[1] * 2.62119;

    //  Limit each absolute value of val0 to limit
    if (fabs(T_BFL) > b_u) {
      if (rtIsNaN(T_BFL)) {
        d = rtNaN;
      } else if (T_BFL < 0.0) {
        d = -1.0;
      } else {
        d = (T_BFL > 0.0);
      }

      T_BFL = b_u * d;
    }

    T_BFR = -39.111507258094768 * x[25];

    //  Limit each absolute value of val0 to limit
    if (fabs(T_BFR) > b_u) {
      if (rtIsNaN(T_BFR)) {
        d = rtNaN;
      } else if (T_BFR < 0.0) {
        d = -1.0;
      } else {
        d = (T_BFR > 0.0);
      }

      T_BFR = b_u * d;
    }

    T_BRL = -39.111507258094768 * x[26];
    b_u = u[2] * 2.4887362499999997;

    //  Limit each absolute value of val0 to limit
    if (fabs(T_BRL) > b_u) {
      if (rtIsNaN(T_BRL)) {
        d = rtNaN;
      } else if (T_BRL < 0.0) {
        d = -1.0;
      } else {
        d = (T_BRL > 0.0);
      }

      T_BRL = b_u * d;
    }

    T_BRR = -39.111507258094768 * x[27];

    //  Limit each absolute value of val0 to limit
    if (fabs(T_BRR) > b_u) {
      if (rtIsNaN(T_BRR)) {
        d = rtNaN;
      } else if (T_BRR < 0.0) {
        d = -1.0;
      } else {
        d = (T_BRR > 0.0);
      }

      T_BRR = b_u * d;
    }

    //  Applied torques
    //  Wheels
    b_u = T_BFL + T_DFL;
    d = I_WFRV[0];
    d1 = I_WFRV[1];
    d2 = I_WFRV[2];
    d3 = F_TFL[0];
    d4 = F_TFL[1];
    d5 = F_TFL[2];
    r = _mm_loadu_pd(&A_y_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&A_y_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&A_y_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&domdphi_RR[0], r);
    r = _mm_loadu_pd(&A_0V[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d3));
    r1 = _mm_loadu_pd(&A_0V[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d4));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&A_0V[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d5));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&F_FL[0], r);
    domdphi_RR[2] = (A_y_tmp[2] * d + A_y_tmp[5] * d1) + A_y_tmp[8] * d2;
    F_FL[2] = (A_0V[2] * d3 + A_0V[5] * d4) + A_0V[8] * d5;
    b_r_0V0[0] = domdphi_RR[1] * F_FL[2] - F_FL[1] * domdphi_RR[2];
    b_r_0V0[1] = F_FL[0] * domdphi_RR[2] - domdphi_RR[0] * F_FL[2];
    b_r_0V0[2] = domdphi_RR[0] * F_FL[1] - F_FL[0] * domdphi_RR[1];

    //  Wheel FL
    A_x_tmp_tmp = T_BFR + T_DFR;
    d = r_MQFR0[0];
    d1 = r_MQFR0[1];
    d2 = r_MQFR0[2];
    d3 = F_TFR[0];
    d4 = F_TFR[1];
    d5 = F_TFR[2];
    d6 = I_WFLV[0];
    d7 = I_WFLV[1];
    d8 = I_WFLV[2];
    r = _mm_loadu_pd(&A_y_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&A_y_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&A_y_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&domdphi_RR[0], r);
    r = _mm_loadu_pd(&A_VQFR[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d3));
    r1 = _mm_loadu_pd(&A_VQFR[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d4));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&A_VQFR[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d5));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&F_FR[0], r);
    r = _mm_loadu_pd(&e_yFLV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(b_u));
    r1 = _mm_loadu_pd(&b_r_0V0[0]);
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&A_0V[0]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d6));
    r2 = _mm_loadu_pd(&A_0V[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d7));
    r1 = _mm_add_pd(r1, r2);
    r2 = _mm_loadu_pd(&A_0V[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d8));
    r1 = _mm_add_pd(r1, r2);
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&T_WFL[0], r);
    domdphi_RR[2] = (A_y_tmp[2] * d + A_y_tmp[5] * d1) + A_y_tmp[8] * d2;
    F_FR[2] = (A_VQFR[2] * d3 + A_VQFR[5] * d4) + A_VQFR[8] * d5;
    T_WFL[2] = (e_yFLV[2] * b_u + b_r_0V0[2]) + ((A_0V[2] * d6 + A_0V[5] * d7) +
      A_0V[8] * d8);
    b_r_0V0[0] = domdphi_RR[1] * F_FR[2] - F_FR[1] * domdphi_RR[2];
    b_r_0V0[1] = F_FR[0] * domdphi_RR[2] - domdphi_RR[0] * F_FR[2];
    b_r_0V0[2] = domdphi_RR[0] * F_FR[1] - F_FR[0] * domdphi_RR[1];

    //  Wheel FR
    b_tmp = T_BRL + T_DRL;

    //  Wheel RL
    b_b_tmp = T_BRR + T_DRR;

    //  Wheel RR
    //  Hub-Motors
    //  Uprights
    //  Upright FL
    //  Upright FR
    //  Upright RL
    //  Upright RR
    //  Chassis
    d = F_TRR[0];
    d1 = F_TRR[1];
    d2 = F_TRR[2];
    d3 = F_TRL[0];
    d4 = F_TRL[1];
    d5 = F_TRL[2];
    d6 = dvdphi_RR[0];
    d7 = dvdphi_RR[1];
    d8 = dvdphi_RR[2];
    for (b_i = 0; b_i < 3; b_i++) {
      dvdphi_FR[b_i] = (A_VQRR[b_i] * d + A_VQRR[b_i + 3] * d1) + A_VQRR[b_i + 6]
        * d2;
      domdphi_FL[b_i] = (A_VQRL[b_i] * d3 + A_VQRL[b_i + 3] * d4) + A_VQRL[b_i +
        6] * d5;
      d9 = e_yFRV[b_i];
      T_WFR[b_i] = (d9 * A_x_tmp_tmp + b_r_0V0[b_i]) + ((A_VQFR[b_i] * d6 +
        A_VQFR[b_i + 3] * d7) + A_VQFR[b_i + 6] * d8);
      e_yFLV[b_i] = -e_yFLV[b_i] * b_u;
      d9 = -d9 * A_x_tmp_tmp;
      e_yFRV[b_i] = d9;
      i = static_cast<int>(dv41[b_i]);
      domdphi_FR[b_i] = F_SFL * static_cast<double>(i);
      I_WFRV[b_i] = F_SFR * static_cast<double>(i);
      I_WFLV[b_i] = F_SRL * static_cast<double>(i);
    }

    //  Applied forces
    b_u = ((F_SFL + F_SFR) + F_SRL) + F_SRR;

    //  Chassis
    //  FL
    //  FR
    //  RL
    //  RR
    //  Applied force vector
    for (b_i = 0; b_i < 3; b_i++) {
      dvdphi_RR[b_i] = F_SRR * dv41[b_i];
      v_0RAV[b_i] = r_0V0[b_i] + F_ARAV[b_i];
      F_FL[b_i] -= domdphi_FR[b_i];
      F_FR[b_i] -= I_WFRV[b_i];
      for (i = 0; i < 14; i++) {
        e_yFLV_tmp = b_i + 3 * i;
        dvRRdz_tmp = i + 14 * b_i;
        y_tmp[dvRRdz_tmp] = dvFRdz[e_yFLV_tmp];
        b_y_tmp[dvRRdz_tmp] = dvRLdz[e_yFLV_tmp];
        c_y_tmp[dvRRdz_tmp] = dvRRdz[e_yFLV_tmp];
        d_y_tmp[dvRRdz_tmp] = domUFLdz[e_yFLV_tmp];
        e_y_tmp[dvRRdz_tmp] = domUFRdz[e_yFLV_tmp];
        f_y_tmp[dvRRdz_tmp] = domURLdz[e_yFLV_tmp];
        g_y_tmp[dvRRdz_tmp] = domURRdz[e_yFLV_tmp];
        h_y_tmp[dvRRdz_tmp] = domWFLdz[e_yFLV_tmp];
        i_y_tmp[dvRRdz_tmp] = domWFRdz[e_yFLV_tmp];
        j_y_tmp[dvRRdz_tmp] = domWRLdz[e_yFLV_tmp];
        k_y_tmp[dvRRdz_tmp] = domWRRdz[e_yFLV_tmp];
      }

      d = A_y_tmp[b_i];
      d1 = d * r_MQRL0[0];
      d2 = d * r_MQRR0[0];
      d = A_y_tmp[b_i + 3];
      d1 += d * r_MQRL0[1];
      d2 += d * r_MQRR0[1];
      d = A_y_tmp[b_i + 6];
      d1 += d * r_MQRL0[2];
      d2 += d * r_MQRR0[2];
      dvdphi_FL[b_i] = d2;
      domdphi_RR[b_i] = d1;
    }

    b_r_VFLV[0] = r_VFLV[1] * domdphi_FR[2] - domdphi_FR[1] * r_VFLV[2];
    b_r_VFLV[1] = domdphi_FR[0] * r_VFLV[2] - r_VFLV[0] * domdphi_FR[2];
    b_r_VFLV[2] = r_VFLV[0] * domdphi_FR[1] - domdphi_FR[0] * r_VFLV[1];
    r_MQRL0[0] = r_VFRV[1] * I_WFRV[2] - I_WFRV[1] * r_VFRV[2];
    r_MQRL0[1] = I_WFRV[0] * r_VFRV[2] - r_VFRV[0] * I_WFRV[2];
    r_MQRL0[2] = r_VFRV[0] * I_WFRV[1] - I_WFRV[0] * r_VFRV[1];
    d_A_0V[0] = r_VRLV[1] * I_WFLV[2] - I_WFLV[1] * r_VRLV[2];
    d_A_0V[1] = I_WFLV[0] * r_VRLV[2] - r_VRLV[0] * I_WFLV[2];
    d_A_0V[2] = r_VRLV[0] * I_WFLV[1] - I_WFLV[0] * r_VRLV[1];
    c_A_0V[0] = r_VRRV[1] * dvdphi_RR[2] - dvdphi_RR[1] * r_VRRV[2];
    c_A_0V[1] = dvdphi_RR[0] * r_VRRV[2] - r_VRRV[0] * dvdphi_RR[2];
    c_A_0V[2] = r_VRRV[0] * dvdphi_RR[1] - dvdphi_RR[0] * r_VRRV[1];
    b_A_0V[0] = 0.0 * r_0V0[2] - r_0V0[1] * -v[6];
    b_A_0V[1] = r_0V0[0] * -v[6] - v[2] * r_0V0[2];
    b_A_0V[2] = r_0V0[1] * v[2] - r_0V0[0] * 0.0;
    b_r_0V0[0] = 0.0 * F_ARAV[2] - F_ARAV[1] * -v[6];
    b_r_0V0[1] = F_ARAV[0] * -v[6] - -v[3] * F_ARAV[2];
    b_r_0V0[2] = -v[3] * F_ARAV[1] - F_ARAV[0] * 0.0;
    r = _mm_loadu_pd(&v_0RAV[0]);
    _mm_storeu_pd(&r_MQRR0[0], _mm_add_pd(r, _mm_mul_pd(_mm_set1_pd(b_u),
      _mm_loadu_pd(&dv41[0]))));
    r = _mm_loadu_pd(&b_r_VFLV[0]);
    r1 = _mm_loadu_pd(&r_MQRL0[0]);
    r2 = _mm_loadu_pd(&d_A_0V[0]);
    r3 = _mm_loadu_pd(&c_A_0V[0]);
    r4 = _mm_loadu_pd(&b_A_0V[0]);
    r5 = _mm_loadu_pd(&b_r_0V0[0]);
    _mm_storeu_pd(&b_r_VFLV[0], _mm_add_pd(_mm_add_pd(_mm_add_pd(_mm_add_pd
      (_mm_add_pd(r, r1), r2), r3), r4), r5));
    b_r_VFLV[2] = ((((b_r_VFLV[2] + r_MQRL0[2]) + d_A_0V[2]) + c_A_0V[2]) +
                   b_A_0V[2]) + b_r_0V0[2];
    r = _mm_loadu_pd(&domdphi_FL[0]);
    r1 = _mm_loadu_pd(&I_WFLV[0]);
    _mm_storeu_pd(&I_WFLV[0], _mm_sub_pd(r, r1));
    r = _mm_loadu_pd(&dvdphi_FR[0]);
    r1 = _mm_loadu_pd(&dvdphi_RR[0]);
    _mm_storeu_pd(&dvdphi_RR[0], _mm_sub_pd(r, r1));
    I_WFLV[2] = domdphi_FL[2] - I_WFLV[2];
    dvdphi_RR[2] = dvdphi_FR[2] - dvdphi_RR[2];
    r = _mm_loadu_pd(&e_yRLV[0]);
    r1 = _mm_set1_pd(-1.0);
    _mm_storeu_pd(&c_A_0V[0], _mm_mul_pd(_mm_mul_pd(r, r1), _mm_set1_pd(b_tmp)));
    r = _mm_loadu_pd(&e_yRRV[0]);
    _mm_storeu_pd(&b_A_0V[0], _mm_mul_pd(_mm_mul_pd(r, r1), _mm_set1_pd(b_b_tmp)));
    d = r_MQRR0[0];
    d1 = r_MQRR0[1];
    d2 = v_0RAV[2] + b_u;
    d3 = b_r_VFLV[0];
    d4 = b_r_VFLV[1];
    d5 = b_r_VFLV[2];
    d6 = F_FL[0];
    d7 = F_FL[1];
    d8 = F_FL[2];
    d9 = F_FR[0];
    d10 = F_FR[1];
    d11 = F_FR[2];
    d12 = I_WFLV[0];
    d13 = I_WFLV[1];
    b_u = I_WFLV[2];
    A_x_tmp_tmp = dvdphi_RR[0];
    b_A_x_tmp_tmp = dvdphi_RR[1];
    dz_RR = dvdphi_RR[2];
    w_RR = e_yFLV[0];
    mu_RRR = e_yFLV[1];
    dz_RL = e_yFLV[2];
    w_RL = e_yFRV[0];
    mu_RRL = e_yFRV[1];
    uFR = e_yFRV[2];
    uRL = c_A_0V[0];
    uRR = c_A_0V[1];
    b_domdu_FR = -e_yRLV[2] * b_tmp;
    v_0FAV_tmp = b_A_0V[0];
    d14 = b_A_0V[1];
    d15 = -e_yRRV[2] * b_b_tmp;
    d16 = T_WFL[0];
    d17 = T_WFL[1];
    d18 = T_WFL[2];
    d19 = T_WFR[0];
    d20 = T_WFR[1];
    d21 = T_WFR[2];
    for (i = 0; i < 14; i++) {
      f[i] = (((((((((((static_cast<double>(iv2[i]) * d + static_cast<double>
                        (iv2[i + 14]) * d1) + static_cast<double>(iv2[i + 28]) *
                       d2) + ((static_cast<double>(iv4[i]) * d3 + static_cast<
        double>(iv4[i + 14]) * d4) + static_cast<double>(iv4[i + 28]) * d5)) +
                     ((dvFLdz[3 * i] * d6 + dvFLdz[3 * i + 1] * d7) + dvFLdz[3 *
                      i + 2] * d8)) + ((y_tmp[i] * d9 + y_tmp[i + 14] * d10) +
        y_tmp[i + 28] * d11)) + ((b_y_tmp[i] * d12 + b_y_tmp[i + 14] * d13) +
        b_y_tmp[i + 28] * b_u)) + ((c_y_tmp[i] * A_x_tmp_tmp + c_y_tmp[i + 14] *
        b_A_x_tmp_tmp) + c_y_tmp[i + 28] * dz_RR)) + ((d_y_tmp[i] * w_RR +
        d_y_tmp[i + 14] * mu_RRR) + d_y_tmp[i + 28] * dz_RL)) + ((e_y_tmp[i] *
                  w_RL + e_y_tmp[i + 14] * mu_RRL) + e_y_tmp[i + 28] * uFR)) +
               ((f_y_tmp[i] * uRL + f_y_tmp[i + 14] * uRR) + f_y_tmp[i + 28] *
                b_domdu_FR)) + ((g_y_tmp[i] * v_0FAV_tmp + g_y_tmp[i + 14] * d14)
               + g_y_tmp[i + 28] * d15)) + ((h_y_tmp[i] * d16 + h_y_tmp[i + 14] *
        d17) + h_y_tmp[i + 28] * d18);
      l_y_tmp[i] = (i_y_tmp[i] * d19 + i_y_tmp[i + 14] * d20) + i_y_tmp[i + 28] *
        d21;
    }

    b_r_0V0[0] = domdphi_RR[1] * domdphi_FL[2] - domdphi_FL[1] * domdphi_RR[2];
    b_r_0V0[1] = domdphi_FL[0] * domdphi_RR[2] - domdphi_RR[0] * domdphi_FL[2];
    b_r_0V0[2] = domdphi_RR[0] * domdphi_FL[1] - domdphi_FL[0] * domdphi_RR[1];
    d = T_TRL[0];
    d1 = T_TRL[1];
    d2 = T_TRL[2];
    r = _mm_loadu_pd(&e_yRLV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(b_tmp));
    r2 = _mm_loadu_pd(&b_r_0V0[0]);
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_VQRL[0]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
    r3 = _mm_loadu_pd(&A_VQRL[3]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
    r2 = _mm_add_pd(r2, r3);
    r3 = _mm_loadu_pd(&A_VQRL[6]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
    r2 = _mm_add_pd(r2, r3);
    r = _mm_add_pd(r, r2);
    _mm_storeu_pd(&c_A_0V[0], r);
    c_A_0V[2] = (e_yRLV[2] * b_tmp + b_r_0V0[2]) + ((A_VQRL[2] * d + A_VQRL[5] *
      d1) + A_VQRL[8] * d2);
    b_r_0V0[0] = dvdphi_FL[1] * dvdphi_FR[2] - dvdphi_FR[1] * dvdphi_FL[2];
    b_r_0V0[1] = dvdphi_FR[0] * dvdphi_FL[2] - dvdphi_FL[0] * dvdphi_FR[2];
    b_r_0V0[2] = dvdphi_FL[0] * dvdphi_FR[1] - dvdphi_FR[0] * dvdphi_FL[1];
    d = T_TRR[0];
    d1 = T_TRR[1];
    d2 = T_TRR[2];
    r = _mm_loadu_pd(&e_yRRV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(b_b_tmp));
    r2 = _mm_loadu_pd(&b_r_0V0[0]);
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_VQRR[0]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
    r3 = _mm_loadu_pd(&A_VQRR[3]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
    r2 = _mm_add_pd(r2, r3);
    r3 = _mm_loadu_pd(&A_VQRR[6]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
    r2 = _mm_add_pd(r2, r3);
    r = _mm_add_pd(r, r2);
    _mm_storeu_pd(&b_A_0V[0], r);
    b_A_0V[2] = (e_yRRV[2] * b_b_tmp + b_r_0V0[2]) + ((A_VQRR[2] * d + A_VQRR[5]
      * d1) + A_VQRR[8] * d2);
    d = c_A_0V[0];
    d1 = c_A_0V[1];
    d2 = c_A_0V[2];
    d3 = b_A_0V[0];
    d4 = b_A_0V[1];
    d5 = b_A_0V[2];
    for (i = 0; i <= 12; i += 2) {
      r = _mm_loadu_pd(&f[i]);
      r2 = _mm_loadu_pd(&l_y_tmp[i]);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&j_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
      r3 = _mm_loadu_pd(&j_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&j_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&k_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d3));
      r3 = _mm_loadu_pd(&k_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d4));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&k_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d5));
      r2 = _mm_add_pd(r2, r3);
      _mm_storeu_pd(&l_y_tmp[i], r2);
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&c_f[i], r);
    }

    //       % Chassis force
    //       % Chassis torque
    //     % Force FL
    //     % Force FR
    //     % Force RL
    //     % Force RR
    //  % Upright torque FL
    //  % Upright torque FR
    //  % Upright torque RL
    //  % Upright torque RR
    //  % Wheel torque FL
    //  % Wheel torque FR
    //  % Wheel torque RL
    //  Wheel torque RR
    //  Generalized force vector
    for (i = 0; i < 42; i++) {
      q_tmp[i] = static_cast<double>(iv2[i]) * v[1];
    }

    for (i = 0; i < 14; i++) {
      b_q_tmp[i] = dvFLdz[3 * i] * 11.6;
      b_q_tmp[i + 14] = dvFLdz[3 * i + 1] * 11.6;
      b_q_tmp[i + 28] = dvFLdz[3 * i + 2] * 11.6;
    }

    r = _mm_set1_pd(11.6);
    for (i = 0; i <= 40; i += 2) {
      r2 = _mm_loadu_pd(&y_tmp[i]);
      _mm_storeu_pd(&y_tmp[i], _mm_mul_pd(r2, r));
      r2 = _mm_loadu_pd(&b_y_tmp[i]);
      _mm_storeu_pd(&b_y_tmp[i], _mm_mul_pd(r2, r));
      r2 = _mm_loadu_pd(&c_y_tmp[i]);
      _mm_storeu_pd(&c_y_tmp[i], _mm_mul_pd(r2, r));
    }

    v_0FAV[0] = g_V[0] - ((omega_0VV[1] * r_VFLV_dot[2] - r_VFLV_dot[1] *
      omega_0VV[2]) + (omega_0VV[1] * v_0FLV_idx_2 - v_0FLV_idx_1 * omega_0VV[2]));
    v_0FAV[1] = g_V[1] - ((r_VFLV_dot[0] * omega_0VV[2] - omega_0VV[0] *
      r_VFLV_dot[2]) + (v_0FLV_idx_0 * omega_0VV[2] - omega_0VV[0] *
                        v_0FLV_idx_2));
    v_0FAV[2] = g_V[2] - ((omega_0VV[0] * r_VFLV_dot[1] - r_VFLV_dot[0] *
      omega_0VV[1]) + (omega_0VV[0] * v_0FLV_idx_1 - v_0FLV_idx_0 * omega_0VV[1]));
    r_MQFR0[0] = g_V[0] - ((omega_0VV[1] * r_VFRV_dot[2] - r_VFRV_dot[1] *
      omega_0VV[2]) + (omega_0VV[1] * v_0FRV_idx_2 - v_0FRV_idx_1 * omega_0VV[2]));
    r_MQFR0[1] = g_V[1] - ((r_VFRV_dot[0] * omega_0VV[2] - omega_0VV[0] *
      r_VFRV_dot[2]) + (v_0FRV_idx_0 * omega_0VV[2] - omega_0VV[0] *
                        v_0FRV_idx_2));
    r_MQFR0[2] = g_V[2] - ((omega_0VV[0] * r_VFRV_dot[1] - r_VFRV_dot[0] *
      omega_0VV[1]) + (omega_0VV[0] * v_0FRV_idx_1 - v_0FRV_idx_0 * omega_0VV[1]));
    d = omega_0UFLV[0];
    d1 = omega_0UFLV[1];
    d2 = omega_0UFLV[2];
    r = _mm_loadu_pd(&I_UFLV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_loadu_pd(&I_UFLV[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&I_UFLV[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r2);
    _mm_storeu_pd(&dvdphi_RR[0], r);
    dvdphi_RR[2] = (I_UFLV[2] * d + I_UFLV[5] * d1) + I_UFLV[8] * d2;
    r_0V0[0] = -(omega_0UFLV[1] * dvdphi_RR[2] - dvdphi_RR[1] * omega_0UFLV[2]);
    r_0V0[1] = -(dvdphi_RR[0] * omega_0UFLV[2] - omega_0UFLV[0] * dvdphi_RR[2]);
    r_0V0[2] = -(omega_0UFLV[0] * dvdphi_RR[1] - dvdphi_RR[0] * omega_0UFLV[1]);
    d = omega_0UFRV[0];
    d1 = omega_0UFRV[1];
    d2 = omega_0UFRV[2];
    r = _mm_loadu_pd(&I_UFRV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_loadu_pd(&I_UFRV[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&I_UFRV[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r2);
    _mm_storeu_pd(&dvdphi_RR[0], r);
    dvdphi_RR[2] = (I_UFRV[2] * d + I_UFRV[5] * d1) + I_UFRV[8] * d2;
    domdphi_RR[0] = -(omega_0UFRV[1] * dvdphi_RR[2] - dvdphi_RR[1] *
                      omega_0UFRV[2]);
    domdphi_RR[1] = -(dvdphi_RR[0] * omega_0UFRV[2] - omega_0UFRV[0] *
                      dvdphi_RR[2]);
    domdphi_RR[2] = -(omega_0UFRV[0] * dvdphi_RR[1] - dvdphi_RR[0] *
                      omega_0UFRV[1]);
    d = omega_0WFLV[0];
    d1 = omega_0WFLV[1];
    d2 = omega_0WFLV[2];
    r = _mm_loadu_pd(&b_I_WFLV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_loadu_pd(&b_I_WFLV[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&b_I_WFLV[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r2);
    _mm_storeu_pd(&dvdphi_RR[0], r);
    dvdphi_RR[2] = (b_I_WFLV[2] * d + b_I_WFLV[5] * d1) + b_I_WFLV[8] * d2;
    d_A_0V[0] = omega_0VV[1] * omega_0WFLV[2] - omega_0WFLV[1] * omega_0VV[2];
    d_A_0V[1] = omega_0WFLV[0] * omega_0VV[2] - omega_0VV[0] * omega_0WFLV[2];
    d_A_0V[2] = omega_0VV[0] * omega_0WFLV[1] - omega_0WFLV[0] * omega_0VV[1];
    b_A_0V[0] = omega_VUFLV[1] * omega_0WFLV_tmp[2] - omega_0WFLV_tmp[1] *
      omega_VUFLV[2];
    b_A_0V[1] = omega_0WFLV_tmp[0] * omega_VUFLV[2] - omega_VUFLV[0] *
      omega_0WFLV_tmp[2];
    b_A_0V[2] = omega_VUFLV[0] * omega_0WFLV_tmp[1] - omega_0WFLV_tmp[0] *
      omega_VUFLV[1];
    r = _mm_loadu_pd(&b_I_WFLV[0]);
    _mm_storeu_pd(&A_z_tmp[0], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&b_I_WFLV[2]);
    _mm_storeu_pd(&A_z_tmp[2], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&b_I_WFLV[4]);
    _mm_storeu_pd(&A_z_tmp[4], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&b_I_WFLV[6]);
    _mm_storeu_pd(&A_z_tmp[6], _mm_mul_pd(r, r1));
    A_z_tmp[8] = -b_I_WFLV[8];
    r = _mm_loadu_pd(&d_A_0V[0]);
    r2 = _mm_loadu_pd(&b_A_0V[0]);
    _mm_storeu_pd(&d_A_0V[0], _mm_add_pd(r, r2));
    d_A_0V[2] += b_A_0V[2];
    c_A_0V[0] = omega_0WFLV[1] * dvdphi_RR[2] - dvdphi_RR[1] * omega_0WFLV[2];
    c_A_0V[1] = dvdphi_RR[0] * omega_0WFLV[2] - omega_0WFLV[0] * dvdphi_RR[2];
    c_A_0V[2] = omega_0WFLV[0] * dvdphi_RR[1] - dvdphi_RR[0] * omega_0WFLV[1];
    d = omega_0WFRV[0];
    d1 = omega_0WFRV[1];
    d2 = omega_0WFRV[2];
    d3 = d_A_0V[0];
    d4 = d_A_0V[1];
    d5 = d_A_0V[2];
    r = _mm_loadu_pd(&b_I_WFRV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_loadu_pd(&b_I_WFRV[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&b_I_WFRV[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r2);
    _mm_storeu_pd(&dvdphi_RR[0], r);
    r = _mm_loadu_pd(&A_z_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d3));
    r2 = _mm_loadu_pd(&A_z_tmp[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d4));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_z_tmp[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d5));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&c_A_0V[0]);
    r = _mm_sub_pd(r, r2);
    _mm_storeu_pd(&I_WFLV[0], r);
    dvdphi_RR[2] = (b_I_WFRV[2] * d + b_I_WFRV[5] * d1) + b_I_WFRV[8] * d2;
    I_WFLV[2] = ((A_z_tmp[2] * d3 + A_z_tmp[5] * d4) + A_z_tmp[8] * d5) -
      c_A_0V[2];
    d_A_0V[0] = omega_0VV[1] * omega_0WFRV[2] - omega_0WFRV[1] * omega_0VV[2];
    d_A_0V[1] = omega_0WFRV[0] * omega_0VV[2] - omega_0VV[0] * omega_0WFRV[2];
    d_A_0V[2] = omega_0VV[0] * omega_0WFRV[1] - omega_0WFRV[0] * omega_0VV[1];
    b_r_0V0[0] = omega_VUFRV[1] * omega_0WFRV_tmp[2] - omega_0WFRV_tmp[1] *
      omega_VUFRV[2];
    b_r_0V0[1] = omega_0WFRV_tmp[0] * omega_VUFRV[2] - omega_VUFRV[0] *
      omega_0WFRV_tmp[2];
    b_r_0V0[2] = omega_VUFRV[0] * omega_0WFRV_tmp[1] - omega_0WFRV_tmp[0] *
      omega_VUFRV[1];
    r = _mm_loadu_pd(&b_I_WFRV[0]);
    _mm_storeu_pd(&A_z_tmp[0], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&b_I_WFRV[2]);
    _mm_storeu_pd(&A_z_tmp[2], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&b_I_WFRV[4]);
    _mm_storeu_pd(&A_z_tmp[4], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&b_I_WFRV[6]);
    _mm_storeu_pd(&A_z_tmp[6], _mm_mul_pd(r, r1));
    A_z_tmp[8] = -b_I_WFRV[8];
    r = _mm_loadu_pd(&d_A_0V[0]);
    r2 = _mm_loadu_pd(&b_r_0V0[0]);
    _mm_storeu_pd(&d_A_0V[0], _mm_add_pd(r, r2));
    d_A_0V[2] += b_r_0V0[2];
    b_r_0V0[0] = omega_0WFRV[1] * dvdphi_RR[2] - dvdphi_RR[1] * omega_0WFRV[2];
    b_r_0V0[1] = dvdphi_RR[0] * omega_0WFRV[2] - omega_0WFRV[0] * dvdphi_RR[2];
    b_r_0V0[2] = omega_0WFRV[0] * dvdphi_RR[1] - dvdphi_RR[0] * omega_0WFRV[1];
    d = d_A_0V[0];
    d1 = d_A_0V[1];
    d2 = d_A_0V[2];
    r = _mm_loadu_pd(&A_z_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_loadu_pd(&A_z_tmp[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_z_tmp[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&b_r_0V0[0]);
    r = _mm_sub_pd(r, r2);
    _mm_storeu_pd(&I_WFRV[0], r);
    I_WFRV[2] = ((A_z_tmp[2] * d + A_z_tmp[5] * d1) + A_z_tmp[8] * d2) -
      b_r_0V0[2];

    //  % Chassis force
    //  % Chassis torque
    //  % Force FL
    //  % Force FR
    //  % Force RL
    //  % Force RR
    //  % Upright torque FL
    //  % Upright torque FR
    //  % Upright torque RL
    //  % Upright torque RR
    //  % Wheel torque FL
    //  % Wheel torque FR
    //  % Wheel torque RL
    //  Wheel torque RR
    //  Generalized mass matrix
    for (i = 0; i < 14; i++) {
      d = d_y_tmp[i];
      d1 = d_y_tmp[i + 14];
      d2 = d_y_tmp[i + 28];
      for (b_i = 0; b_i < 3; b_i++) {
        m_y_tmp[i + 14 * b_i] = (d * I_UFLV[3 * b_i] + d1 * I_UFLV[3 * b_i + 1])
          + d2 * I_UFLV[3 * b_i + 2];
      }

      d = m_y_tmp[i];
      d1 = m_y_tmp[i + 14];
      d2 = m_y_tmp[i + 28];
      d3 = b_q_tmp[i];
      d4 = b_q_tmp[i + 14];
      d5 = b_q_tmp[i + 28];
      for (b_i = 0; b_i < 14; b_i++) {
        i1 = 3 * b_i + 1;
        i2 = 3 * b_i + 2;
        e_yFLV_tmp = i + 14 * b_i;
        n_y_tmp[e_yFLV_tmp] = (d * domUFLdz[3 * b_i] + d1 * domUFLdz[i1]) + d2 *
          domUFLdz[i2];
        c_q_tmp[e_yFLV_tmp] = (d3 * dvFLdz[3 * b_i] + d4 * dvFLdz[i1]) + d5 *
          dvFLdz[i2];
      }

      d = h_y_tmp[i];
      d1 = h_y_tmp[i + 14];
      d2 = h_y_tmp[i + 28];
      for (b_i = 0; b_i < 3; b_i++) {
        m_y_tmp[i + 14 * b_i] = (d * b_I_WFLV[3 * b_i] + d1 * b_I_WFLV[3 * b_i +
          1]) + d2 * b_I_WFLV[3 * b_i + 2];
      }

      d = q_tmp[i];
      d1 = q_tmp[i + 14];
      d2 = q_tmp[i + 28];
      d3 = m_y_tmp[i];
      d4 = m_y_tmp[i + 14];
      d5 = m_y_tmp[i + 28];
      for (b_i = 0; b_i < 14; b_i++) {
        i1 = 3 * b_i + 1;
        i2 = 3 * b_i + 2;
        dvRRdz_tmp = i + 14 * b_i;
        M[dvRRdz_tmp] = ((d * static_cast<double>(iv5[3 * b_i]) + d1 *
                          static_cast<double>(iv5[i1])) + d2 * static_cast<
                         double>(iv5[i2])) + static_cast<double>(iv6[dvRRdz_tmp]);
        d_q_tmp[dvRRdz_tmp] = (c_q_tmp[dvRRdz_tmp] + n_y_tmp[dvRRdz_tmp]) + ((d3
          * domWFLdz[3 * b_i] + d4 * domWFLdz[i1]) + d5 * domWFLdz[i2]);
      }

      d = e_y_tmp[i];
      d1 = e_y_tmp[i + 14];
      d2 = e_y_tmp[i + 28];
      for (b_i = 0; b_i < 3; b_i++) {
        m_y_tmp[i + 14 * b_i] = (d * I_UFRV[3 * b_i] + d1 * I_UFRV[3 * b_i + 1])
          + d2 * I_UFRV[3 * b_i + 2];
      }

      d = y_tmp[i];
      d1 = y_tmp[i + 14];
      d2 = y_tmp[i + 28];
      for (b_i = 0; b_i < 14; b_i++) {
        n_y_tmp[i + 14 * b_i] = (d * dvFRdz[3 * b_i] + d1 * dvFRdz[3 * b_i + 1])
          + d2 * dvFRdz[3 * b_i + 2];
      }
    }

    for (i = 0; i < 14; i++) {
      d = m_y_tmp[i];
      d1 = m_y_tmp[i + 14];
      d2 = m_y_tmp[i + 28];
      for (b_i = 0; b_i < 14; b_i++) {
        o_y_tmp[i + 14 * b_i] = (d * domUFRdz[3 * b_i] + d1 * domUFRdz[3 * b_i +
          1]) + d2 * domUFRdz[3 * b_i + 2];
      }

      d = f_y_tmp[i];
      d1 = f_y_tmp[i + 14];
      d2 = f_y_tmp[i + 28];
      d3 = i_y_tmp[i];
      d4 = i_y_tmp[i + 14];
      d5 = i_y_tmp[i + 28];
      for (b_i = 0; b_i < 3; b_i++) {
        i1 = 3 * b_i + 1;
        i2 = 3 * b_i + 2;
        e_yFLV_tmp = i + 14 * b_i;
        domWFLdz[e_yFLV_tmp] = (d * I_URLV[3 * b_i] + d1 * I_URLV[i1]) + d2 *
          I_URLV[i2];
        m_y_tmp[e_yFLV_tmp] = (d3 * b_I_WFRV[3 * b_i] + d4 * b_I_WFRV[i1]) + d5 *
          b_I_WFRV[i2];
      }

      d = domWFLdz[i];
      d1 = domWFLdz[i + 14];
      d2 = domWFLdz[i + 28];
      d3 = b_y_tmp[i];
      d4 = b_y_tmp[i + 14];
      d5 = b_y_tmp[i + 28];
      for (b_i = 0; b_i < 14; b_i++) {
        i1 = 3 * b_i + 1;
        i2 = 3 * b_i + 2;
        e_yFLV_tmp = i + 14 * b_i;
        q_y_tmp[e_yFLV_tmp] = (d * domURLdz[3 * b_i] + d1 * domURLdz[i1]) + d2 *
          domURLdz[i2];
        p_y_tmp[e_yFLV_tmp] = (d3 * dvRLdz[3 * b_i] + d4 * dvRLdz[i1]) + d5 *
          dvRLdz[i2];
      }

      d = j_y_tmp[i];
      d1 = j_y_tmp[i + 14];
      d2 = j_y_tmp[i + 28];
      for (b_i = 0; b_i < 3; b_i++) {
        domWFLdz[i + 14 * b_i] = (d * I_WRLV[3 * b_i] + d1 * I_WRLV[3 * b_i + 1])
          + d2 * I_WRLV[3 * b_i + 2];
      }

      d = m_y_tmp[i];
      d1 = m_y_tmp[i + 14];
      d2 = m_y_tmp[i + 28];
      d3 = domWFLdz[i];
      d4 = domWFLdz[i + 14];
      d5 = domWFLdz[i + 28];
      for (b_i = 0; b_i < 14; b_i++) {
        i1 = 3 * b_i + 1;
        i2 = 3 * b_i + 2;
        dvRRdz_tmp = i + 14 * b_i;
        c_q_tmp[dvRRdz_tmp] = (M[dvRRdz_tmp] + d_q_tmp[dvRRdz_tmp]) +
          ((n_y_tmp[dvRRdz_tmp] + o_y_tmp[dvRRdz_tmp]) + ((d * domWFRdz[3 * b_i]
             + d1 * domWFRdz[i1]) + d2 * domWFRdz[i2]));
        n_y_tmp[dvRRdz_tmp] = (p_y_tmp[dvRRdz_tmp] + q_y_tmp[dvRRdz_tmp]) + ((d3
          * domWRLdz[3 * b_i] + d4 * domWRLdz[i1]) + d5 * domWRLdz[i2]);
      }

      d = g_y_tmp[i];
      d1 = g_y_tmp[i + 14];
      d2 = g_y_tmp[i + 28];
      for (b_i = 0; b_i < 3; b_i++) {
        m_y_tmp[i + 14 * b_i] = (d * I_URRV[3 * b_i] + d1 * I_URRV[3 * b_i + 1])
          + d2 * I_URRV[3 * b_i + 2];
      }

      d = m_y_tmp[i];
      d1 = m_y_tmp[i + 14];
      d2 = m_y_tmp[i + 28];
      d3 = c_y_tmp[i];
      d4 = c_y_tmp[i + 14];
      d5 = c_y_tmp[i + 28];
      for (b_i = 0; b_i < 14; b_i++) {
        i1 = 3 * b_i + 1;
        i2 = 3 * b_i + 2;
        e_yFLV_tmp = i + 14 * b_i;
        p_y_tmp[e_yFLV_tmp] = (d * domURRdz[3 * b_i] + d1 * domURRdz[i1]) + d2 *
          domURRdz[i2];
        o_y_tmp[e_yFLV_tmp] = (d3 * dvRRdz[3 * b_i] + d4 * dvRRdz[i1]) + d5 *
          dvRRdz[i2];
      }

      d = k_y_tmp[i];
      d1 = k_y_tmp[i + 14];
      d2 = k_y_tmp[i + 28];
      for (b_i = 0; b_i < 3; b_i++) {
        m_y_tmp[i + 14 * b_i] = (d * I_WRRV[3 * b_i] + d1 * I_WRRV[3 * b_i + 1])
          + d2 * I_WRRV[3 * b_i + 2];
      }

      d = m_y_tmp[i];
      d1 = m_y_tmp[i + 14];
      d2 = m_y_tmp[i + 28];
      for (b_i = 0; b_i < 14; b_i++) {
        dvRRdz_tmp = i + 14 * b_i;
        M[dvRRdz_tmp] = (c_q_tmp[dvRRdz_tmp] + n_y_tmp[dvRRdz_tmp]) +
          ((o_y_tmp[dvRRdz_tmp] + p_y_tmp[dvRRdz_tmp]) + ((d * domWRRdz[3 * b_i]
             + d1 * domWRRdz[3 * b_i + 1]) + d2 * domWRRdz[3 * b_i + 2]));
      }
    }

    //  % Chassis
    //  % FL
    //  % FR
    //  % RL
    //  RR
    //  Solve for state derivatives
    //  Generalized coordinates derivatives
    //  Generalized velocities derivatives
    d = omega_0WRRV[0];
    d1 = omega_0WRRV[1];
    d2 = omega_0WRRV[2];
    d3 = omega_0WRLV[0];
    d4 = omega_0WRLV[1];
    d5 = omega_0WRLV[2];
    d6 = omega_0URRV[0];
    d7 = omega_0URRV[1];
    d8 = omega_0URRV[2];
    d9 = omega_0URLV[0];
    d10 = omega_0URLV[1];
    d11 = omega_0URLV[2];
    d12 = omega_0VV[0];
    d13 = omega_0VV[1];
    b_u = omega_0VV[2];
    for (i = 0; i < 3; i++) {
      domdphi_FR[i] = (I_WRRV[i] * d + I_WRRV[i + 3] * d1) + I_WRRV[i + 6] * d2;
      dvdphi_FR[i] = (I_WRLV[i] * d3 + I_WRLV[i + 3] * d4) + I_WRLV[i + 6] * d5;
      domdphi_FL[i] = (I_URRV[i] * d6 + I_URRV[i + 3] * d7) + I_URRV[i + 6] * d8;
      dvdphi_FL[i] = (I_URLV[i] * d9 + I_URLV[i + 3] * d10) + I_URLV[i + 6] *
        d11;
      dvdphi_RR[i] = (static_cast<double>(iv3[i]) * d12 + static_cast<double>
                      (iv3[i + 3]) * d13) + static_cast<double>(iv3[i + 6]) *
        b_u;
    }

    memcpy(&dv[0], &x[14], 14U * sizeof(double));
    coder::mldivide(K, dv);
    d = g_V[0] - (omega_0VV[1] * v_0VV_idx_2 - v_0VV_idx_1 * omega_0VV[2]);
    d1 = g_V[1] - (v_0VV_idx_0 * omega_0VV[2] - omega_0VV[0] * v_0VV_idx_2);
    d2 = g_V[2] - (omega_0VV[0] * v_0VV_idx_1 - v_0VV_idx_0 * omega_0VV[1]);
    d3 = -(omega_0VV[1] * dvdphi_RR[2] - dvdphi_RR[1] * omega_0VV[2]);
    d4 = -(dvdphi_RR[0] * omega_0VV[2] - omega_0VV[0] * dvdphi_RR[2]);
    d5 = -(omega_0VV[0] * dvdphi_RR[1] - dvdphi_RR[0] * omega_0VV[1]);
    d6 = v_0FAV[0];
    d7 = v_0FAV[1];
    d8 = v_0FAV[2];
    d9 = r_MQFR0[0];
    d10 = r_MQFR0[1];
    d11 = r_MQFR0[2];
    for (i = 0; i < 14; i++) {
      f[i] = ((c_f[i] + ((q_tmp[i] * d + q_tmp[i + 14] * d1) + q_tmp[i + 28] *
                         d2)) + ((static_cast<double>(iv4[i]) * d3 +
                static_cast<double>(iv4[i + 14]) * d4) + static_cast<double>
               (iv4[i + 28]) * d5)) + ((b_q_tmp[i] * d6 + b_q_tmp[i + 14] * d7)
        + b_q_tmp[i + 28] * d8);
      l_y_tmp[i] = (y_tmp[i] * d9 + y_tmp[i + 14] * d10) + y_tmp[i + 28] * d11;
    }

    d_A_0V[0] = omega_0VV[1] * r_VRLV_dot[2] - r_VRLV_dot[1] * omega_0VV[2];
    d_A_0V[1] = r_VRLV_dot[0] * omega_0VV[2] - omega_0VV[0] * r_VRLV_dot[2];
    d_A_0V[2] = omega_0VV[0] * r_VRLV_dot[1] - r_VRLV_dot[0] * omega_0VV[1];
    c_A_0V[0] = omega_0VV[1] * v_0RLV_idx_2 - v_0RLV_idx_1 * omega_0VV[2];
    c_A_0V[1] = v_0RLV_idx_0 * omega_0VV[2] - omega_0VV[0] * v_0RLV_idx_2;
    c_A_0V[2] = omega_0VV[0] * v_0RLV_idx_1 - v_0RLV_idx_0 * omega_0VV[1];
    r = _mm_loadu_pd(&d_A_0V[0]);
    r2 = _mm_loadu_pd(&c_A_0V[0]);
    r3 = _mm_loadu_pd(&g_V[0]);
    _mm_storeu_pd(&d_A_0V[0], _mm_sub_pd(r3, _mm_add_pd(r, r2)));
    d_A_0V[2] = g_V[2] - (d_A_0V[2] + c_A_0V[2]);
    c_A_0V[0] = omega_0VV[1] * r_VRRV_dot[2] - r_VRRV_dot[1] * omega_0VV[2];
    c_A_0V[1] = r_VRRV_dot[0] * omega_0VV[2] - omega_0VV[0] * r_VRRV_dot[2];
    c_A_0V[2] = omega_0VV[0] * r_VRRV_dot[1] - r_VRRV_dot[0] * omega_0VV[1];
    b_A_0V[0] = omega_0VV[1] * v_0RRV_idx_2 - v_0RRV_idx_1 * omega_0VV[2];
    b_A_0V[1] = v_0RRV_idx_0 * omega_0VV[2] - omega_0VV[0] * v_0RRV_idx_2;
    b_A_0V[2] = omega_0VV[0] * v_0RRV_idx_1 - v_0RRV_idx_0 * omega_0VV[1];
    r = _mm_loadu_pd(&c_A_0V[0]);
    r2 = _mm_loadu_pd(&b_A_0V[0]);
    r3 = _mm_loadu_pd(&g_V[0]);
    _mm_storeu_pd(&g_V[0], _mm_sub_pd(r3, _mm_add_pd(r, r2)));
    g_V[2] -= c_A_0V[2] + b_A_0V[2];
    d = d_A_0V[0];
    d1 = d_A_0V[1];
    d2 = d_A_0V[2];
    d3 = g_V[0];
    d4 = g_V[1];
    d5 = g_V[2];
    d6 = r_0V0[0];
    d7 = r_0V0[1];
    d8 = r_0V0[2];
    d9 = domdphi_RR[0];
    d10 = domdphi_RR[1];
    d11 = domdphi_RR[2];
    d12 = -(omega_0URLV[1] * dvdphi_FL[2] - dvdphi_FL[1] * omega_0URLV[2]);
    d13 = -(dvdphi_FL[0] * omega_0URLV[2] - omega_0URLV[0] * dvdphi_FL[2]);
    b_u = -(omega_0URLV[0] * dvdphi_FL[1] - dvdphi_FL[0] * omega_0URLV[1]);
    A_x_tmp_tmp = -(omega_0URRV[1] * domdphi_FL[2] - domdphi_FL[1] *
                    omega_0URRV[2]);
    b_A_x_tmp_tmp = -(domdphi_FL[0] * omega_0URRV[2] - omega_0URRV[0] *
                      domdphi_FL[2]);
    dz_RR = -(omega_0URRV[0] * domdphi_FL[1] - domdphi_FL[0] * omega_0URRV[1]);
    w_RR = I_WFLV[0];
    mu_RRR = I_WFLV[1];
    dz_RL = I_WFLV[2];
    w_RL = I_WFRV[0];
    mu_RRL = I_WFRV[1];
    uFR = I_WFRV[2];
    for (i = 0; i <= 12; i += 2) {
      r = _mm_loadu_pd(&f[i]);
      r2 = _mm_loadu_pd(&l_y_tmp[i]);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&b_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
      r3 = _mm_loadu_pd(&b_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&b_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&c_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d3));
      r3 = _mm_loadu_pd(&c_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d4));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&c_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d5));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&d_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d6));
      r3 = _mm_loadu_pd(&d_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d7));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&d_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d8));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&e_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d9));
      r3 = _mm_loadu_pd(&e_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d10));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&e_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d11));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&f_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d12));
      r3 = _mm_loadu_pd(&f_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d13));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&f_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(b_u));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&g_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(A_x_tmp_tmp));
      r3 = _mm_loadu_pd(&g_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(b_A_x_tmp_tmp));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&g_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(dz_RR));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&h_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(w_RR));
      r3 = _mm_loadu_pd(&h_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(mu_RRR));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&h_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(dz_RL));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&f[i], r);
      r = _mm_loadu_pd(&i_y_tmp[i]);
      r = _mm_mul_pd(r, _mm_set1_pd(w_RL));
      r2 = _mm_loadu_pd(&i_y_tmp[i + 14]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(mu_RRL));
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&i_y_tmp[i + 28]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(uFR));
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&l_y_tmp[i], r);
    }

    d_A_0V[0] = omega_0VV[1] * omega_0WRLV[2] - omega_0WRLV[1] * omega_0VV[2];
    d_A_0V[1] = omega_0WRLV[0] * omega_0VV[2] - omega_0VV[0] * omega_0WRLV[2];
    d_A_0V[2] = omega_0VV[0] * omega_0WRLV[1] - omega_0WRLV[0] * omega_0VV[1];
    c_A_0V[0] = omega_VURLV[1] * omega_0WRLV_tmp[2] - omega_0WRLV_tmp[1] *
      omega_VURLV[2];
    c_A_0V[1] = omega_0WRLV_tmp[0] * omega_VURLV[2] - omega_VURLV[0] *
      omega_0WRLV_tmp[2];
    c_A_0V[2] = omega_VURLV[0] * omega_0WRLV_tmp[1] - omega_0WRLV_tmp[0] *
      omega_VURLV[1];
    r = _mm_loadu_pd(&I_WRLV[0]);
    _mm_storeu_pd(&I_WRLV[0], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRLV[2]);
    _mm_storeu_pd(&I_WRLV[2], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRLV[4]);
    _mm_storeu_pd(&I_WRLV[4], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRLV[6]);
    _mm_storeu_pd(&I_WRLV[6], _mm_mul_pd(r, r1));
    I_WRLV[8] = -I_WRLV[8];
    r = _mm_loadu_pd(&d_A_0V[0]);
    r2 = _mm_loadu_pd(&c_A_0V[0]);
    _mm_storeu_pd(&d_A_0V[0], _mm_add_pd(r, r2));
    d_A_0V[2] += c_A_0V[2];
    b_r_0V0[0] = omega_0WRLV[1] * dvdphi_FR[2] - dvdphi_FR[1] * omega_0WRLV[2];
    b_r_0V0[1] = dvdphi_FR[0] * omega_0WRLV[2] - omega_0WRLV[0] * dvdphi_FR[2];
    b_r_0V0[2] = omega_0WRLV[0] * dvdphi_FR[1] - dvdphi_FR[0] * omega_0WRLV[1];
    d = d_A_0V[0];
    d1 = d_A_0V[1];
    d2 = d_A_0V[2];
    r = _mm_loadu_pd(&I_WRLV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_loadu_pd(&I_WRLV[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&I_WRLV[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&b_r_0V0[0]);
    r = _mm_sub_pd(r, r2);
    _mm_storeu_pd(&b_A_0V[0], r);
    b_A_0V[2] = ((I_WRLV[2] * d + I_WRLV[5] * d1) + I_WRLV[8] * d2) - b_r_0V0[2];
    d_A_0V[0] = omega_0VV[1] * omega_0WRRV[2] - omega_0WRRV[1] * omega_0VV[2];
    d_A_0V[1] = omega_0WRRV[0] * omega_0VV[2] - omega_0VV[0] * omega_0WRRV[2];
    d_A_0V[2] = omega_0VV[0] * omega_0WRRV[1] - omega_0WRRV[0] * omega_0VV[1];
    c_A_0V[0] = omega_VURRV[1] * omega_0WRRV_tmp[2] - omega_0WRRV_tmp[1] *
      omega_VURRV[2];
    c_A_0V[1] = omega_0WRRV_tmp[0] * omega_VURRV[2] - omega_VURRV[0] *
      omega_0WRRV_tmp[2];
    c_A_0V[2] = omega_VURRV[0] * omega_0WRRV_tmp[1] - omega_0WRRV_tmp[0] *
      omega_VURRV[1];
    r = _mm_loadu_pd(&I_WRRV[0]);
    _mm_storeu_pd(&I_WRRV[0], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRRV[2]);
    _mm_storeu_pd(&I_WRRV[2], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRRV[4]);
    _mm_storeu_pd(&I_WRRV[4], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRRV[6]);
    _mm_storeu_pd(&I_WRRV[6], _mm_mul_pd(r, r1));
    I_WRRV[8] = -I_WRRV[8];
    r = _mm_loadu_pd(&d_A_0V[0]);
    r1 = _mm_loadu_pd(&c_A_0V[0]);
    _mm_storeu_pd(&d_A_0V[0], _mm_add_pd(r, r1));
    d_A_0V[2] += c_A_0V[2];
    c_A_0V[0] = omega_0WRRV[1] * domdphi_FR[2] - domdphi_FR[1] * omega_0WRRV[2];
    c_A_0V[1] = domdphi_FR[0] * omega_0WRRV[2] - omega_0WRRV[0] * domdphi_FR[2];
    c_A_0V[2] = omega_0WRRV[0] * domdphi_FR[1] - domdphi_FR[0] * omega_0WRRV[1];
    d = d_A_0V[0];
    d1 = d_A_0V[1];
    d2 = d_A_0V[2];
    r = _mm_loadu_pd(&I_WRRV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&I_WRRV[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&I_WRRV[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&c_A_0V[0]);
    r = _mm_sub_pd(r, r1);
    _mm_storeu_pd(&b_r_0V0[0], r);
    b_r_0V0[2] = ((I_WRRV[2] * d + I_WRRV[5] * d1) + I_WRRV[8] * d2) - c_A_0V[2];
    d = b_A_0V[0];
    d1 = b_A_0V[1];
    d2 = b_A_0V[2];
    d3 = b_r_0V0[0];
    d4 = b_r_0V0[1];
    d5 = b_r_0V0[2];
    for (i = 0; i <= 12; i += 2) {
      r = _mm_loadu_pd(&f[i]);
      r1 = _mm_loadu_pd(&l_y_tmp[i]);
      r = _mm_add_pd(r, r1);
      r1 = _mm_loadu_pd(&j_y_tmp[i]);
      r1 = _mm_mul_pd(r1, _mm_set1_pd(d));
      r2 = _mm_loadu_pd(&j_y_tmp[i + 14]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
      r1 = _mm_add_pd(r1, r2);
      r2 = _mm_loadu_pd(&j_y_tmp[i + 28]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
      r1 = _mm_add_pd(r1, r2);
      r = _mm_add_pd(r, r1);
      r1 = _mm_loadu_pd(&k_y_tmp[i]);
      r1 = _mm_mul_pd(r1, _mm_set1_pd(d3));
      r2 = _mm_loadu_pd(&k_y_tmp[i + 14]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d4));
      r1 = _mm_add_pd(r1, r2);
      r2 = _mm_loadu_pd(&k_y_tmp[i + 28]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d5));
      r1 = _mm_add_pd(r1, r2);
      _mm_storeu_pd(&l_y_tmp[i], r1);
      r = _mm_add_pd(r, r1);
      _mm_storeu_pd(&b_f[i], r);
    }

    coder::mldivide(M, b_f);
    for (i = 0; i < 14; i++) {
      dvFLdz[i] = dv[i];
      dvFLdz[i + 14] = b_f[i];
    }

    dvFLdz[28] = x_TFL_dot[0];
    dvFLdz[31] = x_TFR_dot[0];
    dvFLdz[34] = x_TRL_dot[0];
    dvFLdz[37] = x_TRR_dot[0];
    dvFLdz[29] = x_TFL_dot[1];
    dvFLdz[32] = x_TFR_dot[1];
    dvFLdz[35] = x_TRL_dot[1];
    dvFLdz[38] = x_TRR_dot[1];
    dvFLdz[30] = x_TFL_dot[2];
    dvFLdz[33] = x_TFR_dot[2];
    dvFLdz[36] = x_TRL_dot[2];
    dvFLdz[39] = x_TRR_dot[2];
    dvFLdz[40] = 0.0;
    dvFLdz[41] = 0.0;

    //  State derivatives
    //  Additional output
    //  Inertial acceleration
    //  Combined aerodynamic forces
    //  Steering Torque
    //  --- Tire utilization (per wheel + global)
    if (F_xMFL >= 2.2204460492503131E-16) {
      d = F_xMFL;
    } else {
      d = 2.2204460492503131E-16;
    }

    if (F_yMFL >= 2.2204460492503131E-16) {
      d1 = F_yMFL;
    } else {
      d1 = 2.2204460492503131E-16;
    }

    b_u = coder::b_hypot(fabs(F_TFL[0]) / d, fabs(F_TFL[1]) / d1);
    if (F_xMFR >= 2.2204460492503131E-16) {
      d = F_xMFR;
    } else {
      d = 2.2204460492503131E-16;
    }

    if (F_yMFR >= 2.2204460492503131E-16) {
      d1 = F_yMFR;
    } else {
      d1 = 2.2204460492503131E-16;
    }

    uFR = coder::b_hypot(fabs(F_TFR[0]) / d, fabs(F_TFR[1]) / d1);
    if (F_xMRL >= 2.2204460492503131E-16) {
      d = F_xMRL;
    } else {
      d = 2.2204460492503131E-16;
    }

    if (F_yMRL >= 2.2204460492503131E-16) {
      d1 = F_yMRL;
    } else {
      d1 = 2.2204460492503131E-16;
    }

    uRL = coder::b_hypot(fabs(F_TRL[0]) / d, fabs(F_TRL[1]) / d1);
    if (F_xMRR >= 2.2204460492503131E-16) {
      d = F_xMRR;
    } else {
      d = 2.2204460492503131E-16;
    }

    if (F_yMRR >= 2.2204460492503131E-16) {
      d1 = F_yMRR;
    } else {
      d1 = 2.2204460492503131E-16;
    }

    uRR = coder::b_hypot(fabs(F_TRR[0]) / d, fabs(F_TRR[1]) / d1);

    //  Output vector
    A_x_tmp_tmp = 0.0;
    b_A_x_tmp_tmp = 0.0;
    dz_RR = 0.0;
    w_RR = 0.0;
    mu_RRR = 0.0;
    dz_RL = 0.0;
    w_RL = 0.0;
    mu_RRL = 0.0;
    v_0FAV_tmp = 0.0;
    b_domdu_FR = 0.0;
    for (i = 0; i < 3; i++) {
      A_z_tmp[3 * i] = M[14 * i];
      A_z_tmp[3 * i + 1] = M[14 * i + 1];
      A_z_tmp[3 * i + 2] = M[14 * i + 2];
      d = dvdu_FL[i];
      A_x_tmp_tmp += d * F_FL[i];
      d1 = dvdu_FR[i];
      b_A_x_tmp_tmp += d1 * F_FR[i];
      d2 = domdu_FL[i];
      dz_RR += d2 * (e_yFLV[i] + T_WFL[i]);
      d3 = domdu_FR[i];
      w_RR += d3 * (e_yFRV[i] + T_WFR[i]);
      mu_RRR += d * 11.6 * v_0FAV[i];
      dz_RL += d1 * 11.6 * r_MQFR0[i];
      w_RL += d2 * r_0V0[i];
      mu_RRL += d3 * domdphi_RR[i];
      v_0FAV_tmp += d2 * I_WFLV[i];
      b_domdu_FR += d3 * I_WFRV[i];
    }

    double uFL[4];
    coder::mldivide(A_z_tmp, &c_f[0], dvdphi_FL);
    uFL[0] = b_u;
    uFL[1] = uFR;
    uFL[2] = uRL;
    uFL[3] = uRR;
    out[0] = dvdphi_FL[0];
    out[3] = F_TFL[0];
    out[6] = F_TFR[0];
    out[9] = F_TRL[0];
    out[12] = F_TRR[0];
    out[1] = dvdphi_FL[1];
    out[4] = F_TFL[1];
    out[7] = F_TFR[1];
    out[10] = F_TRL[1];
    out[13] = F_TRR[1];
    out[2] = dvdphi_FL[2];
    out[5] = F_TFL[2];
    out[8] = F_TFR[2];
    out[11] = F_TRL[2];
    out[14] = F_TRR[2];
    out[15] = s_FL[0];
    out[19] = s_FR[0];
    out[23] = s_RL[0];
    out[27] = s_RR[0];
    out[16] = s_FL[1];
    out[20] = s_FR[1];
    out[24] = s_RL[1];
    out[28] = s_RR[1];
    out[17] = s_FL[2];
    out[21] = s_FR[2];
    out[25] = s_RL[2];
    out[29] = s_RR[2];
    out[18] = s_FL[3];
    out[22] = s_FR[3];
    out[26] = s_RL[3];
    out[30] = s_RR[3];
    out[31] = F_ycFL;
    out[32] = F_ycFR;
    out[33] = F_ycRL;
    out[34] = F_ycRR;
    out[35] = dF_x0FL;
    out[36] = dF_x0FR;
    out[37] = dF_x0RL;
    out[38] = dF_x0RR;
    out[39] = F_xMFL;
    out[40] = F_xMFR;
    out[41] = F_xMRL;
    out[42] = F_xMRR;
    out[43] = dF_y0FL;
    out[44] = dF_y0FR;
    out[45] = dF_y0RL;
    out[46] = dF_y0RR;
    out[47] = F_yMFL;
    out[48] = F_yMFR;
    out[49] = F_yMRL;
    out[50] = F_yMRR;
    out[51] = r_stFL;
    out[52] = r_stFR;
    out[53] = r_stRL;
    out[54] = r_stRR;
    out[55] = r_dynFL;
    out[56] = r_dynFR;
    out[57] = r_dynRL;
    out[58] = r_dynRR;
    out[59] = z_FL;
    out[60] = z_FR;
    out[61] = z_RL;
    out[62] = z_RR;
    out[63] = z_FL_dot;
    out[64] = z_FR_dot;
    out[65] = z_RL_dot;
    out[66] = z_RR_dot;
    out[67] = F_SFL;
    out[68] = F_SFR;
    out[69] = F_SRL;
    out[70] = F_SRR;
    out[71] = delta_FL;
    out[72] = delta_FR;
    out[73] = delta_RL;
    out[74] = delta_RR;
    out[75] = gamma_FL;
    out[76] = gamma_FR;
    out[77] = gamma_RL;
    out[78] = gamma_RR;
    out[79] = T_BFL;
    out[80] = T_BFR;
    out[81] = T_BRL;
    out[82] = T_BRR;
    out[83] = T_DFL;
    out[84] = T_DFR;
    out[85] = T_DRL;
    out[86] = T_DRR;
    out[87] = rtNaN;
    out[88] = rtNaN;
    out[89] = v_0RAV[0];
    out[90] = v_0RAV[1];
    out[91] = v_0RAV[2];
    out[92] = (((((((((A_x_tmp_tmp + b_A_x_tmp_tmp) + dz_RR) + w_RR) + mu_RRR) +
                   dz_RL) + w_RL) + mu_RRL) + v_0FAV_tmp) + b_domdu_FR) *
      0.014584380239693683;
    out[93] = b_u;
    out[94] = uFR;
    out[95] = uRL;
    out[96] = uRR;
    out[97] = coder::internal::maximum(uFL);
    out[98] = (b_u * F_TFL[2] + uFR * F_TFR[2]) / (F_TFL[2] + F_TFR[2]);
    out[99] = (uRL * F_TRL[2] + uRR * F_TRR[2]) / (F_TRL[2] + F_TRR[2]);
    memcpy(&out[100], &out_aero[0], 8U * sizeof(double));
    out[108] = u[0];
    out[109] = u[3];
    b_u = dt / 2.0;
    expl_temp.i_G = veh_i_G;
    expl_temp.T_MMax = v[7];
    expl_temp.C_D = car[0];
    expl_temp.C_LRA = car[2];
    expl_temp.C_LFA = car[1];
    expl_temp.F_SRR0 = v[5];
    expl_temp.F_SRL0 = v[5];
    expl_temp.F_SFR0 = v[4];
    expl_temp.F_SFL0 = v[4];
    expl_temp.c_RollRA = v[9];
    expl_temp.c_RollFA = v[8];
    expl_temp.l_RA = v[3];
    expl_temp.l_FA = v[2];
    expl_temp.z_cg = v[6];
    expl_temp.m_C = v[1];
    memcpy(&b_expl_temp.Z[0], &rz[0], 3721U * sizeof(double));
    for (i = 0; i <= 40; i += 2) {
      r = _mm_loadu_pd(&dvFLdz[i]);
      r1 = _mm_loadu_pd(&x[i]);
      _mm_storeu_pd(&domWFLdz[i], _mm_add_pd(r1, _mm_mul_pd(_mm_set1_pd(b_u), r)));
    }

    f_vehicleModel(u[0], u[1], u[2], u[3], u[5], domWFLdz, expl_temp,
                   b_expl_temp, dvFRdz);
    for (i = 0; i <= 40; i += 2) {
      r = _mm_loadu_pd(&dvFRdz[i]);
      r1 = _mm_loadu_pd(&x[i]);
      _mm_storeu_pd(&domWFLdz[i], _mm_add_pd(r1, _mm_mul_pd(_mm_set1_pd(b_u), r)));
    }

    f_vehicleModel(u[0], u[1], u[2], u[3], u[5], domWFLdz, expl_temp,
                   b_expl_temp, dvRLdz);
    for (i = 0; i <= 40; i += 2) {
      r = _mm_loadu_pd(&dvRLdz[i]);
      r1 = _mm_loadu_pd(&x[i]);
      _mm_storeu_pd(&domWFLdz[i], _mm_add_pd(r1, _mm_mul_pd(_mm_set1_pd(dt), r)));
    }

    f_vehicleModel(u[0], u[1], u[2], u[3], u[5], domWFLdz, expl_temp,
                   b_expl_temp, dvRRdz);
    r = _mm_set1_pd(2.0);
    for (i = 0; i <= 40; i += 2) {
      r1 = _mm_loadu_pd(&dvFRdz[i]);
      r2 = _mm_loadu_pd(&dvFLdz[i]);
      r3 = _mm_loadu_pd(&dvRLdz[i]);
      r4 = _mm_loadu_pd(&dvRRdz[i]);
      r5 = _mm_loadu_pd(&x[i]);
      _mm_storeu_pd(&x[i], _mm_add_pd(r5, _mm_div_pd(_mm_mul_pd(_mm_set1_pd(dt),
        _mm_add_pd(_mm_add_pd(_mm_add_pd(r2, _mm_mul_pd(r, r1)), _mm_mul_pd(r,
        r3)), r4)), _mm_set1_pd(6.0))));
    }
  }

  void adtm_step_initialize()
  {
    rt_InitInfAndNaN();
    isInitialized_adtm_step = true;
  }

  void adtm_step_terminate()
  {
    isInitialized_adtm_step = false;
  }

  void adtm_step_torque(double x[42], const double u[10], const double car[9],
                        double, double dt, const double rz[3721], const double
                        r0[2], double out[110])
  {
    static const signed char b[3] = { 0, 1, 0 };

    __m128d r;
    __m128d r1;
    __m128d r2;
    __m128d r3;
    __m128d r4;
    __m128d r5;
    b_struct_T expl_temp;
    struct_T b_expl_temp;
    double K[196];
    double M[196];
    double c_q_tmp[196];
    double d_q_tmp[196];
    double n_y_tmp[196];
    double o_y_tmp[196];
    double p_y_tmp[196];
    double q_y_tmp[196];
    double b_q_tmp[42];
    double b_y_tmp[42];
    double c_y_tmp[42];
    double d_y_tmp[42];
    double domUFLdz[42];
    double domUFRdz[42];
    double domURLdz[42];
    double domURRdz[42];
    double domWFLdz[42];
    double domWFRdz[42];
    double domWRLdz[42];
    double domWRRdz[42];
    double dvFLdz[42];
    double dvFRdz[42];
    double dvRLdz[42];
    double dvRRdz[42];
    double e_y_tmp[42];
    double f_y_tmp[42];
    double g_y_tmp[42];
    double h_y_tmp[42];
    double i_y_tmp[42];
    double j_y_tmp[42];
    double k_y_tmp[42];
    double m_y_tmp[42];
    double q_tmp[42];
    double y_tmp[42];
    double b_f[14];
    double c_f[14];
    double dv[14];
    double f[14];
    double l_y_tmp[14];
    double v[10];
    double A_0V[9];
    double A_VQFR[9];
    double A_VQRL[9];
    double A_VQRR[9];
    double A_VRL[9];
    double A_y_tmp[9];
    double A_z_tmp[9];
    double I_UFLV[9];
    double I_UFLV_tmp[9];
    double I_UFRV[9];
    double I_UFRV_tmp[9];
    double I_URLV[9];
    double I_URLV_tmp[9];
    double I_URRV[9];
    double I_URRV_tmp[9];
    double I_WRLV[9];
    double I_WRRV[9];
    double b_I_WFLV[9];
    double b_I_WFRV[9];
    double out_aero[8];
    double s_FL[4];
    double s_FR[4];
    double s_RL[4];
    double s_RR[4];
    double F_ARAV[3];
    double F_FL[3];
    double F_FR[3];
    double F_TFL[3];
    double F_TFR[3];
    double F_TRL[3];
    double F_TRR[3];
    double I_WFLV[3];
    double I_WFRV[3];
    double T_TRL[3];
    double T_TRR[3];
    double T_WFL[3];
    double T_WFR[3];
    double b_A_0V[3];
    double b_r_0V0[3];
    double b_r_VFLV[3];
    double c_A_0V[3];
    double d_A_0V[3];
    double domdphi_FL[3];
    double domdphi_FR[3];
    double domdphi_RR[3];
    double domdu_FL[3];
    double domdu_FR[3];
    double dvdphi_FL[3];
    double dvdphi_FR[3];
    double dvdphi_RR[3];
    double dvdu_FL[3];
    double dvdu_FR[3];
    double e_yFLV[3];
    double e_yFRV[3];
    double e_yRLV[3];
    double e_yRRV[3];
    double g_V[3];
    double omega_0UFLV[3];
    double omega_0UFRV[3];
    double omega_0URLV[3];
    double omega_0URRV[3];
    double omega_0VV[3];
    double omega_0WFLV[3];
    double omega_0WFLV_tmp[3];
    double omega_0WFRV[3];
    double omega_0WFRV_tmp[3];
    double omega_0WRLV[3];
    double omega_0WRLV_tmp[3];
    double omega_0WRRV[3];
    double omega_0WRRV_tmp[3];
    double omega_VUFLV[3];
    double omega_VUFRV[3];
    double omega_VURLV[3];
    double omega_VURRV[3];
    double r_0V0[3];
    double r_MQFR0[3];
    double r_MQRL0[3];
    double r_MQRR0[3];
    double r_VFLV[3];
    double r_VFLV_dot[3];
    double r_VFRV[3];
    double r_VFRV_dot[3];
    double r_VRLV[3];
    double r_VRLV_dot[3];
    double r_VRRV[3];
    double r_VRRV_dot[3];
    double v_0FAV[3];
    double v_0RAV[3];
    double x_TFL_dot[3];
    double x_TFR_dot[3];
    double x_TRL_dot[3];
    double x_TRR_dot[3];
    double A_x_tmp_tmp;
    double F_SFL;
    double F_SFR;
    double F_SRL;
    double F_SRR;
    double F_xMFL;
    double F_xMFR;
    double F_xMRL;
    double F_xMRR;
    double F_yMFL;
    double F_yMFR;
    double F_yMRL;
    double F_yMRR;
    double F_ycFL;
    double F_ycFR;
    double F_ycRL;
    double F_ycRR;
    double T_BFL;
    double T_BFR;
    double T_BRL;
    double T_BRR;
    double T_DFL;
    double T_DFR;
    double T_DRL;
    double T_DRR;
    double b_A_x_tmp_tmp;
    double b_b_tmp;
    double b_domdu_FR;
    double b_tmp;
    double b_u;
    double d;
    double d1;
    double d10;
    double d11;
    double d12;
    double d13;
    double d14;
    double d15;
    double d16;
    double d17;
    double d18;
    double d19;
    double d2;
    double d20;
    double d21;
    double d3;
    double d4;
    double d5;
    double d6;
    double d7;
    double d8;
    double d9;
    double dF_x0FL;
    double dF_x0FR;
    double dF_x0RL;
    double dF_x0RR;
    double dF_y0FL;
    double dF_y0FR;
    double dF_y0RL;
    double dF_y0RR;
    double delta_FL;
    double delta_FR;
    double delta_RL;
    double delta_RR;
    double dz_RL;
    double dz_RR;
    double gamma_FL;
    double gamma_FR;
    double gamma_RL;
    double gamma_RR;
    double mu_RRL;
    double mu_RRR;
    double r_dynFL;
    double r_dynFR;
    double r_dynRL;
    double r_dynRR;
    double r_stFL;
    double r_stFR;
    double r_stRL;
    double r_stRR;
    double uFR;
    double uRL;
    double uRR;
    double v_0FAV_tmp;
    double v_0FLV_idx_0;
    double v_0FLV_idx_1;
    double v_0FLV_idx_2;
    double v_0FRV_idx_0;
    double v_0FRV_idx_1;
    double v_0FRV_idx_2;
    double v_0RLV_idx_0;
    double v_0RLV_idx_1;
    double v_0RLV_idx_2;
    double v_0RRV_idx_0;
    double v_0RRV_idx_1;
    double v_0RRV_idx_2;
    double v_0VV_idx_0;
    double v_0VV_idx_1;
    double v_0VV_idx_2;
    double veh_i_G;
    double w_RL;
    double w_RR;
    double z_FL;
    double z_FL_dot;
    double z_FR;
    double z_FR_dot;
    double z_RL;
    double z_RL_dot;
    double z_RR;
    double z_RR_dot;
    int b_i;
    int dvRRdz_tmp;
    int e_yFLV_tmp;
    int i;
    int i1;
    int i2;
    if (!isInitialized_adtm_step) {
      adtm_step_initialize();
    }

    // ADTM_STEP_TORQUE One Runge-Kutta step of the ADTM driven by four motor torque commands, as a VCU sends them.
    //    As adtm_step, but u = [delta_D (rad at the steering wheel), p_BFA, p_BRA (bar), throttle (unused), gear, mu_N,
    //    T_MCmdFL, T_MCmdFR, T_MCmdRL, T_MCmdRR (Nm at each motor)]. The ADTM's own torque control (f_eMotTorqueCtrl)
    //    is skipped: this is its "Simulink" input path (opts.isSimulink), which for hub motors only changes where the
    //    torque commands come from. The motor limits (torque, power, speed) stay in the model (f_powertrain).
    //    rz, r0: the road's heights around the car, see adtm_road.
    adtm_vehicle(car, v);
    veh_i_G = 12.421 * car[8];

    // ADTM_ROAD The ADTM's 3D road (f_roadHeight, f_roadMu) as a window of 61 x 61 heights on its 0.1 m grid around the car.
    //    rz(i, j): the height (m) at x = (r0(1) + i - 1) / 10, y = (r0(2) + j - 1) / 10 (m), in the frame of the ADTM's x_V, y_V;
    //    r0: the window's corner in tenths of a metre (whole numbers). FS Lap moves the window with the car (HoMoLapAdtm.cpp);
    //    a flat track gives zeros. The road's friction factor is 1 everywhere: FS Lap's grip stays in mu_N.
    d = r0[0];
    d1 = r0[1];
    for (i = 0; i < 61; i++) {
      b_expl_temp.x[i] = d + static_cast<double>(i);
      b_expl_temp.y[i] = d1 + static_cast<double>(i);
    }

    for (i = 0; i < 3721; i++) {
      b_expl_temp.Mu[i] = 1.0;
    }

    //  the ground around the car (FS Lap's terrain; zeros on a flat track)
    //       Vehicle Model Equations            %%
    //  ----------------------------------------- %
    //  Version: V1.19 - 2026.01.05               %
    //  Compatible ADTM Release: ADTM_1.4         %
    //  Author: Simon Frank simon.sf.frank@tum.de %
    //  Modified by: Tim Braungardt  (V1.19)      %
    //  Input:                                    %
    //    inputs  -   struct of model input       %
    //                values at time t            %
    //    t       -   current simulation time     %
    //    x       -   state vector                %
    //    veh     -   struct containing vehicle   %
    //                parameters                  %
    //    tirXX   -   struct containing tire      %
    //                parameters for one tire     %
    //                where XX is FL/FR/RL/RR     %
    //    env     -   struct containing           %
    //                environment parameters      %
    //    road    -   struct of road              %
    //                x, y, Z, and Mu data        %
    //    opts    -   struct of additional model  %
    //                options                     %
    //  Output:                                   %
    //    x_dot   -   state derivative vector     %
    //    out     -   model output vector         %
    //  ----------------------------------------- %
    //  Model states
    //  Generalized coordinates
    //  Vehicle position
    //  Euler angles
    //  Lower wishbone angles
    //  Wheel rotation angles
    //  Generalized velocities
    //  Vehicle velocities
    //  Vehicle angular velocities
    //  Lower wishbone angular velocities
    //  Wheel rotation angular velocities
    //  Tire model internal states
    //  Check states
    //  Model Input
    //  Use code when NOT running in Simulink
    //  Default motor torque commands
    //  Motor torque commands from Simulink
    //  Rotations
    //  Direction vectors
    //  Chassis to earth
    //  Rotation matrix for angle ang about Z axis
    b_u = sin(x[5]);
    uFR = cos(x[5]);

    //  Rotation matrix for angle ang about Y axis
    uRL = sin(x[4]);
    uRR = cos(x[4]);

    //  Rotation matrix for angle ang about X axis
    A_x_tmp_tmp = sin(x[3]);
    b_A_x_tmp_tmp = cos(x[3]);
    A_z_tmp[0] = uFR;
    A_z_tmp[3] = -b_u;
    A_z_tmp[6] = 0.0;
    A_z_tmp[1] = b_u;
    A_z_tmp[4] = uFR;
    A_z_tmp[7] = 0.0;
    A_y_tmp[0] = uRR;
    A_y_tmp[3] = 0.0;
    A_y_tmp[6] = uRL;
    A_z_tmp[2] = 0.0;
    A_y_tmp[1] = 0.0;
    A_z_tmp[5] = 0.0;
    A_y_tmp[4] = 1.0;
    A_z_tmp[8] = 1.0;
    A_y_tmp[7] = 0.0;
    A_y_tmp[2] = -uRL;
    A_y_tmp[5] = 0.0;
    A_y_tmp[8] = uRR;
    for (i = 0; i < 3; i++) {
      d = A_z_tmp[i];
      d1 = A_z_tmp[i + 3];
      b_i = static_cast<int>(A_z_tmp[i + 6]);
      for (i1 = 0; i1 < 3; i1++) {
        A_VQRR[i + 3 * i1] = (d * A_y_tmp[3 * i1] + d1 * A_y_tmp[3 * i1 + 1]) +
          static_cast<double>(b_i) * A_y_tmp[3 * i1 + 2];
      }
    }

    A_z_tmp[0] = 1.0;
    A_z_tmp[3] = 0.0;
    A_z_tmp[6] = 0.0;
    A_z_tmp[1] = 0.0;
    A_z_tmp[4] = b_A_x_tmp_tmp;
    A_z_tmp[7] = -A_x_tmp_tmp;
    A_z_tmp[2] = 0.0;
    A_z_tmp[5] = A_x_tmp_tmp;
    A_z_tmp[8] = b_A_x_tmp_tmp;
    for (i = 0; i < 3; i++) {
      d = A_VQRR[i];
      d1 = A_VQRR[i + 3];
      d2 = A_VQRR[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        A_0V[i + 3 * b_i] = (d * A_z_tmp[3 * b_i] + d1 * A_z_tmp[3 * b_i + 1]) +
          d2 * A_z_tmp[3 * b_i + 2];
      }
    }

    //  Velocities to coordinates
    for (i = 0; i < 196; i++) {
      K[i] = iv[i];
    }

    for (i = 0; i < 3; i++) {
      d = A_0V[i];
      A_y_tmp[3 * i] = d;
      K[14 * i] = d;
      d = A_0V[i + 3];
      A_y_tmp[3 * i + 1] = d;
      K[14 * i + 1] = d;
      d = A_0V[i + 6];
      A_y_tmp[3 * i + 2] = d;
      K[14 * i + 2] = d;
    }

    K[45] = 1.0;
    K[59] = 0.0;
    K[73] = -sin(x[4]);
    K[46] = 0.0;
    K[60] = b_A_x_tmp_tmp;
    K[74] = A_x_tmp_tmp * uRR;
    K[47] = 0.0;
    K[61] = -A_x_tmp_tmp;
    K[75] = b_A_x_tmp_tmp * uRR;
    r = _mm_loadu_pd(&A_y_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(9.80665));
    r = _mm_mul_pd(r, _mm_set1_pd(0.0));
    r1 = _mm_loadu_pd(&A_y_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(9.80665));
    r1 = _mm_mul_pd(r1, _mm_set1_pd(0.0));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&A_y_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(9.80665));
    r = _mm_sub_pd(r, r1);
    _mm_storeu_pd(&g_V[0], r);
    g_V[2] = (A_y_tmp[2] * 9.80665 * 0.0 + A_y_tmp[5] * 9.80665 * 0.0) -
      A_y_tmp[8] * 9.80665;

    //  Suspension kinematics
    //  Front axle
    b_u = 0.014584380239693683 * u[0];

    //  Limit each absolute value of val0 to limit
    if (fabs(b_u) > 0.028) {
      if (rtIsNaN(b_u)) {
        d = rtNaN;
      } else if (b_u < 0.0) {
        d = -1.0;
      } else {
        d = (b_u > 0.0);
      }

      b_u = 0.028 * d;
    }

    //  Front steering rack travel
    //  Double-Wishbone
    delta_FL = f_suspensionDWB(0.0034906585039886592, 0.017453292519943295, dv3,
      dv4, dv5, dv6, dv7, dv8, dv9, dv10, dv11, x[6], b_u, A_VQRR, r_VFLV,
      dvdphi_FL, domdphi_FL, dvdu_FL, domdu_FL);
    delta_FR = f_suspensionDWB(-0.0034906585039886592, -0.017453292519943295,
      dv12, dv13, dv14, dv15, dv16, dv17, dv18, dv19, dv20, x[7], b_u, A_VQRL,
      r_VFRV, dvdphi_FR, domdphi_FR, dvdu_FR, domdu_FR);

    //  Rear axle
    //  Double-Wishbone
    delta_RL = f_suspensionDWB(-0.0087266462599716477, 0.017453292519943295,
      dv21, dv22, dv23, dv24, dv25, dv26, dv27, dv28, dv29, x[8], A_VRL, r_VRLV,
      I_WFRV, I_WFLV);
    delta_RR = f_suspensionDWB(0.0087266462599716477, -0.017453292519943295,
      dv30, dv31, dv32, dv33, dv34, dv35, dv36, dv37, dv38, x[9], A_VQFR, r_VRRV,
      dvdphi_RR, domdphi_RR);

    //  Wheel rot. axis vehicle frame
    //  Wheel rot. axis origin frame
    //  Upright inertia tensor vehicle frame
    for (i = 0; i < 3; i++) {
      d = 0.0;
      d1 = 0.0;
      d2 = 0.0;
      d3 = 0.0;
      for (b_i = 0; b_i < 3; b_i++) {
        i1 = i + 3 * b_i;
        d4 = A_VQRR[i1];
        e_yFLV_tmp = b[b_i];
        d += d4 * static_cast<double>(e_yFLV_tmp);
        d1 += A_VQRL[i1] * static_cast<double>(e_yFLV_tmp);
        d2 += A_VRL[i1] * static_cast<double>(e_yFLV_tmp);
        d3 += A_VQFR[i1] * static_cast<double>(e_yFLV_tmp);
        I_UFLV_tmp[b_i + 3 * i] = d4;
        A_z_tmp[i1] = (A_VQRR[i] * dv39[3 * b_i] + A_VQRR[i + 3] * dv39[3 * b_i
                       + 1]) + A_VQRR[i + 6] * dv39[3 * b_i + 2];
      }

      e_yRRV[i] = d3;
      e_yRLV[i] = d2;
      e_yFRV[i] = d1;
      e_yFLV[i] = d;
    }

    for (i = 0; i < 3; i++) {
      d = A_z_tmp[i];
      d1 = A_z_tmp[i + 3];
      d2 = A_z_tmp[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        e_yFLV_tmp = i + 3 * b_i;
        I_UFLV[e_yFLV_tmp] = (d * I_UFLV_tmp[3 * b_i] + d1 * I_UFLV_tmp[3 * b_i
                              + 1]) + d2 * I_UFLV_tmp[3 * b_i + 2];
        I_UFRV_tmp[b_i + 3 * i] = A_VQRL[e_yFLV_tmp];
      }

      d = A_VQRL[i];
      d1 = A_VQRL[i + 3];
      d2 = A_VQRL[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        A_z_tmp[i + 3 * b_i] = (d * dv39[3 * b_i] + d1 * dv39[3 * b_i + 1]) + d2
          * dv39[3 * b_i + 2];
      }
    }

    for (i = 0; i < 3; i++) {
      d = A_z_tmp[i];
      d1 = A_z_tmp[i + 3];
      d2 = A_z_tmp[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        e_yFLV_tmp = i + 3 * b_i;
        I_UFRV[e_yFLV_tmp] = (d * I_UFRV_tmp[3 * b_i] + d1 * I_UFRV_tmp[3 * b_i
                              + 1]) + d2 * I_UFRV_tmp[3 * b_i + 2];
        I_URLV_tmp[b_i + 3 * i] = A_VRL[e_yFLV_tmp];
      }

      d = A_VRL[i];
      d1 = A_VRL[i + 3];
      d2 = A_VRL[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        A_z_tmp[i + 3 * b_i] = (d * dv39[3 * b_i] + d1 * dv39[3 * b_i + 1]) + d2
          * dv39[3 * b_i + 2];
      }
    }

    for (i = 0; i < 3; i++) {
      d = A_z_tmp[i];
      d1 = A_z_tmp[i + 3];
      d2 = A_z_tmp[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        e_yFLV_tmp = i + 3 * b_i;
        I_URLV[e_yFLV_tmp] = (d * I_URLV_tmp[3 * b_i] + d1 * I_URLV_tmp[3 * b_i
                              + 1]) + d2 * I_URLV_tmp[3 * b_i + 2];
        I_URRV_tmp[b_i + 3 * i] = A_VQFR[e_yFLV_tmp];
      }

      d = A_VQFR[i];
      d1 = A_VQFR[i + 3];
      d2 = A_VQFR[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        A_z_tmp[i + 3 * b_i] = (d * dv39[3 * b_i] + d1 * dv39[3 * b_i + 1]) + d2
          * dv39[3 * b_i + 2];
      }
    }

    //  Wheel inertia tensor vehicle frame
    //  Positions
    r_0V0[0] = x[0];
    r_0V0[1] = x[1];
    r_0V0[2] = x[2];

    //  Velocities
    //  Vehicle velocity vehicle frame
    v_0VV_idx_0 = x[14];
    v_0VV_idx_1 = x[15];
    v_0VV_idx_2 = x[16];

    //  Vel. @ CoG
    omega_0VV[0] = x[17];
    omega_0VV[1] = x[18];
    omega_0VV[2] = x[19];

    //  Angular vel. @ CoG
    v_0FAV_tmp = v_0VV_idx_0 + (omega_0VV[1] * -v[6] - 0.0 * omega_0VV[2]);
    v_0FAV[0] = v_0FAV_tmp;
    b_u = omega_0VV[0] * -v[6];
    v_0FAV[1] = v_0VV_idx_1 + (v[2] * omega_0VV[2] - b_u);
    v_0FAV[2] = v_0VV_idx_2 + (omega_0VV[0] * 0.0 - omega_0VV[1] * v[2]);

    //  Vel. @ front axle
    //  Norm of vel. @ front axle
    v_0RAV[0] = v_0FAV_tmp;
    v_0RAV[1] = v_0VV_idx_1 + (-v[3] * omega_0VV[2] - b_u);
    v_0RAV[2] = v_0VV_idx_2 + (omega_0VV[0] * 0.0 - -v[3] * omega_0VV[1]);

    //  Vel. @ rear axle
    //  Norm of vel. @ rear axle
    //  Wheel center velocity vehicle frame
    //  Wheel center total velocity vehicle frame
    //  Wheel center velocities origin frame
    //  Upright angular velocity vehicle frame
    //  Upright total angular velocity vehicle frame
    //  Upright angular velocity origin frame
    //  Wheel angular velocity vehicle frame
    //  Wheel angular velocity wheel frame
    //  Partial velocities
    //  Prepare skew symmetric cross product matrix from vector r
    dvFLdz[9] = 0.0;
    dvFLdz[10] = -r_VFLV[2];
    dvFLdz[11] = r_VFLV[1];
    dvFLdz[12] = r_VFLV[2];
    dvFLdz[13] = 0.0;
    dvFLdz[14] = -r_VFLV[0];
    dvFLdz[15] = -r_VFLV[1];
    dvFLdz[16] = r_VFLV[0];
    dvFLdz[17] = 0.0;
    d = x[20];
    d1 = x[21];
    d2 = x[22];
    d3 = x[23];
    for (b_i = 0; b_i < 3; b_i++) {
      d4 = A_z_tmp[b_i];
      d5 = A_z_tmp[b_i + 3];
      d6 = A_z_tmp[b_i + 6];
      for (i = 0; i < 3; i++) {
        I_URRV[b_i + 3 * i] = (d4 * I_URRV_tmp[3 * i] + d5 * I_URRV_tmp[3 * i +
          1]) + d6 * I_URRV_tmp[3 * i + 2];
      }

      d4 = A_VQRR[b_i];
      d5 = A_VQRR[b_i + 3];
      d6 = A_VQRR[b_i + 6];
      for (i = 0; i < 3; i++) {
        A_z_tmp[b_i + 3 * i] = (d4 * dv40[3 * i] + d5 * dv40[3 * i + 1]) + d6 *
          dv40[3 * i + 2];
      }

      d4 = A_z_tmp[b_i];
      d5 = A_z_tmp[b_i + 3];
      d6 = A_z_tmp[b_i + 6];
      for (i = 0; i < 3; i++) {
        b_I_WFLV[b_i + 3 * i] = (d4 * I_UFLV_tmp[3 * i] + d5 * I_UFLV_tmp[3 * i
          + 1]) + d6 * I_UFLV_tmp[3 * i + 2];
      }

      d4 = A_VQRL[b_i];
      d5 = A_VQRL[b_i + 3];
      d6 = A_VQRL[b_i + 6];
      for (i = 0; i < 3; i++) {
        A_z_tmp[b_i + 3 * i] = (d4 * dv40[3 * i] + d5 * dv40[3 * i + 1]) + d6 *
          dv40[3 * i + 2];
      }

      d4 = A_z_tmp[b_i];
      d5 = A_z_tmp[b_i + 3];
      d6 = A_z_tmp[b_i + 6];
      for (i = 0; i < 3; i++) {
        b_I_WFRV[b_i + 3 * i] = (d4 * I_UFRV_tmp[3 * i] + d5 * I_UFRV_tmp[3 * i
          + 1]) + d6 * I_UFRV_tmp[3 * i + 2];
      }

      d4 = A_VRL[b_i];
      d5 = A_VRL[b_i + 3];
      d6 = A_VRL[b_i + 6];
      for (i = 0; i < 3; i++) {
        A_z_tmp[b_i + 3 * i] = (d4 * dv40[3 * i] + d5 * dv40[3 * i + 1]) + d6 *
          dv40[3 * i + 2];
      }

      d4 = A_z_tmp[b_i];
      d5 = A_z_tmp[b_i + 3];
      d6 = A_z_tmp[b_i + 6];
      for (i = 0; i < 3; i++) {
        I_WRLV[b_i + 3 * i] = (d4 * I_URLV_tmp[3 * i] + d5 * I_URLV_tmp[3 * i +
          1]) + d6 * I_URLV_tmp[3 * i + 2];
      }

      d4 = A_VQFR[b_i];
      d5 = A_VQFR[b_i + 3];
      d6 = A_VQFR[b_i + 6];
      for (i = 0; i < 3; i++) {
        A_z_tmp[b_i + 3 * i] = (d4 * dv40[3 * i] + d5 * dv40[3 * i + 1]) + d6 *
          dv40[3 * i + 2];
      }

      d4 = A_z_tmp[b_i];
      d5 = A_z_tmp[b_i + 3];
      d6 = A_z_tmp[b_i + 6];
      for (i = 0; i < 3; i++) {
        I_WRRV[b_i + 3 * i] = (d4 * I_URRV_tmp[3 * i] + d5 * I_URRV_tmp[3 * i +
          1]) + d6 * I_URRV_tmp[3 * i + 2];
      }

      d4 = dvdphi_FL[b_i];
      r_VFLV_dot[b_i] = d4 * d;
      r_VFRV_dot[b_i] = dvdphi_FR[b_i] * d1;
      r_VRLV_dot[b_i] = I_WFRV[b_i] * d2;
      r_VRRV_dot[b_i] = dvdphi_RR[b_i] * d3;
      d5 = domdphi_FL[b_i] * d;
      omega_VUFLV[b_i] = d5;
      d6 = domdphi_FR[b_i] * d1;
      omega_VUFRV[b_i] = d6;
      d7 = I_WFLV[b_i] * d2;
      omega_VURLV[b_i] = d7;
      d8 = domdphi_RR[b_i] * d3;
      omega_VURRV[b_i] = d8;
      d9 = omega_0VV[b_i];
      d5 += d9;
      omega_0UFLV[b_i] = d5;
      d6 += d9;
      omega_0UFRV[b_i] = d6;
      d7 += d9;
      omega_0URLV[b_i] = d7;
      d8 += d9;
      omega_0URRV[b_i] = d8;
      d9 = e_yFLV[b_i] * x[24];
      omega_0WFLV_tmp[b_i] = d9;
      omega_0WFLV[b_i] = d5 + d9;
      d5 = e_yFRV[b_i] * x[25];
      omega_0WFRV_tmp[b_i] = d5;
      omega_0WFRV[b_i] = d6 + d5;
      d5 = e_yRLV[b_i] * x[26];
      omega_0WRLV_tmp[b_i] = d5;
      omega_0WRLV[b_i] = d7 + d5;
      d5 = e_yRRV[b_i] * x[27];
      omega_0WRRV_tmp[b_i] = d5;
      omega_0WRRV[b_i] = d8 + d5;
      dvFLdz[3 * b_i] = iv1[3 * b_i];
      e_yFLV_tmp = 3 * b_i + 1;
      dvFLdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
      e_yFLV_tmp = 3 * b_i + 2;
      dvFLdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
      dvFLdz[b_i + 18] = d4;
    }

    v_0FLV_idx_0 = (v_0VV_idx_0 + (omega_0VV[1] * r_VFLV[2] - r_VFLV[1] *
      omega_0VV[2])) + r_VFLV_dot[0];
    v_0FLV_idx_1 = (v_0VV_idx_1 + (r_VFLV[0] * omega_0VV[2] - omega_0VV[0] *
      r_VFLV[2])) + r_VFLV_dot[1];
    v_0FLV_idx_2 = (v_0VV_idx_2 + (omega_0VV[0] * r_VFLV[1] - r_VFLV[0] *
      omega_0VV[1])) + r_VFLV_dot[2];
    v_0FRV_idx_0 = (v_0VV_idx_0 + (omega_0VV[1] * r_VFRV[2] - r_VFRV[1] *
      omega_0VV[2])) + r_VFRV_dot[0];
    v_0FRV_idx_1 = (v_0VV_idx_1 + (r_VFRV[0] * omega_0VV[2] - omega_0VV[0] *
      r_VFRV[2])) + r_VFRV_dot[1];
    v_0FRV_idx_2 = (v_0VV_idx_2 + (omega_0VV[0] * r_VFRV[1] - r_VFRV[0] *
      omega_0VV[1])) + r_VFRV_dot[2];
    v_0RLV_idx_0 = (v_0VV_idx_0 + (omega_0VV[1] * r_VRLV[2] - r_VRLV[1] *
      omega_0VV[2])) + r_VRLV_dot[0];
    v_0RLV_idx_1 = (v_0VV_idx_1 + (r_VRLV[0] * omega_0VV[2] - omega_0VV[0] *
      r_VRLV[2])) + r_VRLV_dot[1];
    v_0RLV_idx_2 = (v_0VV_idx_2 + (omega_0VV[0] * r_VRLV[1] - r_VRLV[0] *
      omega_0VV[1])) + r_VRLV_dot[2];
    v_0RRV_idx_0 = (v_0VV_idx_0 + (omega_0VV[1] * r_VRRV[2] - r_VRRV[1] *
      omega_0VV[2])) + r_VRRV_dot[0];
    v_0RRV_idx_1 = (v_0VV_idx_1 + (r_VRRV[0] * omega_0VV[2] - omega_0VV[0] *
      r_VRRV[2])) + r_VRRV_dot[1];
    v_0RRV_idx_2 = (v_0VV_idx_2 + (omega_0VV[0] * r_VRRV[1] - r_VRRV[0] *
      omega_0VV[1])) + r_VRRV_dot[2];
    for (i = 0; i < 7; i++) {
      e_yFLV_tmp = 3 * (i + 7);
      dvFLdz[e_yFLV_tmp] = 0.0;
      dvFLdz[e_yFLV_tmp + 1] = 0.0;
      dvFLdz[e_yFLV_tmp + 2] = 0.0;
    }

    //  Prepare skew symmetric cross product matrix from vector r
    dvFRdz[9] = 0.0;
    dvFRdz[10] = -r_VFRV[2];
    dvFRdz[11] = r_VFRV[1];
    dvFRdz[12] = r_VFRV[2];
    dvFRdz[13] = 0.0;
    dvFRdz[14] = -r_VFRV[0];
    dvFRdz[15] = -r_VFRV[1];
    dvFRdz[16] = r_VFRV[0];
    dvFRdz[17] = 0.0;
    for (i = 0; i < 3; i++) {
      dvFRdz[3 * i] = iv1[3 * i];
      e_yFLV_tmp = 3 * i + 1;
      dvFRdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
      e_yFLV_tmp = 3 * i + 2;
      dvFRdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
      dvFRdz[i + 18] = 0.0;
      dvFRdz[i + 21] = dvdphi_FR[i];
    }

    for (i = 0; i < 6; i++) {
      e_yFLV_tmp = 3 * (i + 8);
      dvFRdz[e_yFLV_tmp] = 0.0;
      dvFRdz[e_yFLV_tmp + 1] = 0.0;
      dvFRdz[e_yFLV_tmp + 2] = 0.0;
    }

    //  Prepare skew symmetric cross product matrix from vector r
    for (i = 0; i < 3; i++) {
      dvRLdz[3 * i] = iv1[3 * i];
      e_yFLV_tmp = 3 * i + 1;
      dvRLdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
      e_yFLV_tmp = 3 * i + 2;
      dvRLdz[e_yFLV_tmp] = iv1[e_yFLV_tmp];
    }

    dvRLdz[9] = 0.0;
    dvRLdz[10] = -r_VRLV[2];
    dvRLdz[11] = r_VRLV[1];
    dvRLdz[12] = r_VRLV[2];
    dvRLdz[13] = 0.0;
    dvRLdz[14] = -r_VRLV[0];
    dvRLdz[15] = -r_VRLV[1];
    dvRLdz[16] = r_VRLV[0];
    dvRLdz[17] = 0.0;
    for (i = 0; i < 2; i++) {
      e_yFLV_tmp = 3 * (i + 6);
      dvRLdz[e_yFLV_tmp] = 0.0;
      dvRLdz[e_yFLV_tmp + 1] = 0.0;
      dvRLdz[e_yFLV_tmp + 2] = 0.0;
    }

    dvRLdz[24] = I_WFRV[0];
    dvRLdz[25] = I_WFRV[1];
    dvRLdz[26] = I_WFRV[2];
    for (i = 0; i < 5; i++) {
      e_yFLV_tmp = 3 * (i + 9);
      dvRLdz[e_yFLV_tmp] = 0.0;
      dvRLdz[e_yFLV_tmp + 1] = 0.0;
      dvRLdz[e_yFLV_tmp + 2] = 0.0;
    }

    //  Prepare skew symmetric cross product matrix from vector r
    dvRRdz[9] = 0.0;
    dvRRdz[10] = -r_VRRV[2];
    dvRRdz[11] = r_VRRV[1];
    dvRRdz[12] = r_VRRV[2];
    dvRRdz[13] = 0.0;
    dvRRdz[14] = -r_VRRV[0];
    dvRRdz[15] = -r_VRRV[1];
    dvRRdz[16] = r_VRRV[0];
    dvRRdz[17] = 0.0;
    for (i = 0; i < 3; i++) {
      dvRRdz[3 * i] = iv1[3 * i];
      e_yFLV_tmp = 3 * (i + 6);
      dvRRdz[e_yFLV_tmp] = 0.0;
      dvRRdz_tmp = 3 * i + 1;
      dvRRdz[dvRRdz_tmp] = iv1[dvRRdz_tmp];
      dvRRdz[e_yFLV_tmp + 1] = 0.0;
      dvRRdz_tmp = 3 * i + 2;
      dvRRdz[dvRRdz_tmp] = iv1[dvRRdz_tmp];
      dvRRdz[e_yFLV_tmp + 2] = 0.0;
      dvRRdz[i + 27] = dvdphi_RR[i];
    }

    for (i = 0; i < 4; i++) {
      e_yFLV_tmp = 3 * (i + 10);
      dvRRdz[e_yFLV_tmp] = 0.0;
      dvRRdz[e_yFLV_tmp + 1] = 0.0;
      dvRRdz[e_yFLV_tmp + 2] = 0.0;
    }

    //  Partial angular velocities
    //  Wheels
    coder::eye(A_VQRR);
    r = _mm_loadu_pd(&e_yFLV[0]);
    r1 = _mm_set1_pd(0.0);
    r2 = _mm_loadu_pd(&e_yFRV[0]);
    for (i = 0; i < 3; i++) {
      r3 = _mm_loadu_pd(&A_VQRR[3 * i]);
      _mm_storeu_pd(&domWFLdz[3 * i], r1);
      b_i = 3 * (i + 3);
      _mm_storeu_pd(&domWFLdz[b_i], _mm_sub_pd(r3, _mm_add_pd(r1, _mm_mul_pd(r,
        _mm_set1_pd(e_yFLV[i])))));
      e_yFLV_tmp = 3 * i + 2;
      domWFLdz[e_yFLV_tmp] = 0.0;
      b_u = A_VQRR[e_yFLV_tmp];
      domWFLdz[b_i + 2] = b_u - e_yFLV[2] * e_yFLV[i];
      domWFLdz[i + 18] = domdphi_FL[i];
      dvRRdz_tmp = 3 * (i + 7);
      domWFLdz[dvRRdz_tmp] = 0.0;
      domWFLdz[dvRRdz_tmp + 1] = 0.0;
      domWFLdz[dvRRdz_tmp + 2] = 0.0;
      domWFLdz[i + 30] = e_yFLV[i];
      i1 = 3 * (i + 11);
      _mm_storeu_pd(&domWFLdz[i1], r1);
      r3 = _mm_loadu_pd(&A_VQRR[3 * i]);
      _mm_storeu_pd(&domWFRdz[3 * i], r1);
      _mm_storeu_pd(&domWFRdz[b_i], _mm_sub_pd(r3, _mm_add_pd(r1, _mm_mul_pd(r2,
        _mm_set1_pd(e_yFRV[i])))));
      domWFLdz[i1 + 2] = 0.0;
      domWFRdz[e_yFLV_tmp] = 0.0;
      domWFRdz[b_i + 2] = b_u - e_yFRV[2] * e_yFRV[i];
      domWFRdz[i + 18] = 0.0;
      domWFRdz[i + 21] = domdphi_FR[i];
      e_yFLV_tmp = 3 * (i + 8);
      domWFRdz[e_yFLV_tmp] = 0.0;
      domWFRdz[e_yFLV_tmp + 1] = 0.0;
      domWFRdz[e_yFLV_tmp + 2] = 0.0;
      domWFRdz[i + 33] = e_yFRV[i];
    }

    r = _mm_loadu_pd(&e_yRLV[0]);
    r1 = _mm_loadu_pd(&A_VQRR[0]);
    r2 = _mm_set1_pd(0.0);
    _mm_storeu_pd(&domWRLdz[0], r2);
    _mm_storeu_pd(&domWRLdz[9], _mm_sub_pd(r1, _mm_add_pd(r2, _mm_mul_pd(r,
      _mm_set1_pd(e_yRLV[0])))));
    domWRLdz[2] = 0.0;
    domWRLdz[11] = A_VQRR[2] - e_yRLV[0] * e_yRLV[2];
    r = _mm_loadu_pd(&e_yRLV[0]);
    r1 = _mm_loadu_pd(&A_VQRR[3]);
    _mm_storeu_pd(&domWRLdz[3], r2);
    _mm_storeu_pd(&domWRLdz[12], _mm_sub_pd(r1, _mm_add_pd(r2, _mm_mul_pd(r,
      _mm_set1_pd(e_yRLV[1])))));
    domWRLdz[5] = 0.0;
    domWRLdz[14] = A_VQRR[5] - e_yRLV[1] * e_yRLV[2];
    r = _mm_loadu_pd(&e_yRLV[0]);
    r1 = _mm_loadu_pd(&A_VQRR[6]);
    _mm_storeu_pd(&domWRLdz[6], r2);
    _mm_storeu_pd(&domWRLdz[15], _mm_sub_pd(r1, _mm_add_pd(r2, _mm_mul_pd(r,
      _mm_set1_pd(e_yRLV[2])))));
    domWRLdz[8] = 0.0;
    domWRLdz[17] = A_VQRR[8] - e_yRLV[2] * e_yRLV[2];
    for (i = 0; i < 2; i++) {
      e_yFLV_tmp = 3 * (i + 12);
      domWFRdz[e_yFLV_tmp] = 0.0;
      domWFRdz[e_yFLV_tmp + 1] = 0.0;
      domWFRdz[e_yFLV_tmp + 2] = 0.0;
      e_yFLV_tmp = 3 * (i + 6);
      domWRLdz[e_yFLV_tmp] = 0.0;
      domWRLdz[e_yFLV_tmp + 1] = 0.0;
      domWRLdz[e_yFLV_tmp + 2] = 0.0;
    }

    //  Uprights
    r = _mm_loadu_pd(&e_yRRV[0]);
    for (i = 0; i < 3; i++) {
      domWRLdz[i + 24] = I_WFLV[i];
      b_i = 3 * (i + 9);
      _mm_storeu_pd(&domWRLdz[b_i], r2);
      r1 = _mm_loadu_pd(&A_VQRR[3 * i]);
      _mm_storeu_pd(&domWRRdz[3 * i], r2);
      i1 = 3 * (i + 3);
      _mm_storeu_pd(&domWRRdz[i1], _mm_sub_pd(r1, _mm_add_pd(r2, _mm_mul_pd(r,
        _mm_set1_pd(e_yRRV[i])))));
      i2 = 3 * (i + 6);
      _mm_storeu_pd(&domWRRdz[i2], r2);
      domWRLdz[b_i + 2] = 0.0;
      b_i = 3 * i + 2;
      domWRRdz[b_i] = 0.0;
      domWRRdz[i1 + 2] = A_VQRR[b_i] - e_yRRV[2] * e_yRRV[i];
      domWRRdz[i2 + 2] = 0.0;
      domWRLdz[i + 36] = e_yRLV[i];
      domWRLdz[i + 39] = 0.0;
      domWRRdz[i + 27] = domdphi_RR[i];
      dvRRdz_tmp = 3 * (i + 10);
      domWRRdz[dvRRdz_tmp] = 0.0;
      domUFLdz[3 * i] = 0.0;
      domUFLdz[i1] = iv1[3 * i];
      domWRRdz[dvRRdz_tmp + 1] = 0.0;
      e_yFLV_tmp = 3 * i + 1;
      domUFLdz[e_yFLV_tmp] = 0.0;
      domUFLdz[i1 + 1] = iv1[e_yFLV_tmp];
      domWRRdz[dvRRdz_tmp + 2] = 0.0;
      domUFLdz[b_i] = 0.0;
      domUFLdz[i1 + 2] = iv1[b_i];
      domWRRdz[i + 39] = e_yRRV[i];
      domUFLdz[i + 18] = domdphi_FL[i];
    }

    for (i = 0; i < 7; i++) {
      e_yFLV_tmp = 3 * (i + 7);
      domUFLdz[e_yFLV_tmp] = 0.0;
      domUFLdz[e_yFLV_tmp + 1] = 0.0;
      domUFLdz[e_yFLV_tmp + 2] = 0.0;
    }

    for (i = 0; i < 3; i++) {
      domUFRdz[3 * i] = 0.0;
      e_yFLV_tmp = 3 * (i + 3);
      domUFRdz[e_yFLV_tmp] = iv1[3 * i];
      dvRRdz_tmp = 3 * i + 1;
      domUFRdz[dvRRdz_tmp] = 0.0;
      domUFRdz[e_yFLV_tmp + 1] = iv1[dvRRdz_tmp];
      dvRRdz_tmp = 3 * i + 2;
      domUFRdz[dvRRdz_tmp] = 0.0;
      domUFRdz[e_yFLV_tmp + 2] = iv1[dvRRdz_tmp];
      domUFRdz[i + 18] = 0.0;
      domUFRdz[i + 21] = domdphi_FR[i];
    }

    for (i = 0; i < 6; i++) {
      e_yFLV_tmp = 3 * (i + 8);
      domUFRdz[e_yFLV_tmp] = 0.0;
      domUFRdz[e_yFLV_tmp + 1] = 0.0;
      domUFRdz[e_yFLV_tmp + 2] = 0.0;
    }

    for (i = 0; i < 3; i++) {
      domURLdz[3 * i] = 0.0;
      e_yFLV_tmp = 3 * (i + 3);
      domURLdz[e_yFLV_tmp] = iv1[3 * i];
      dvRRdz_tmp = 3 * i + 1;
      domURLdz[dvRRdz_tmp] = 0.0;
      domURLdz[e_yFLV_tmp + 1] = iv1[dvRRdz_tmp];
      dvRRdz_tmp = 3 * i + 2;
      domURLdz[dvRRdz_tmp] = 0.0;
      domURLdz[e_yFLV_tmp + 2] = iv1[dvRRdz_tmp];
    }

    for (i = 0; i < 2; i++) {
      e_yFLV_tmp = 3 * (i + 6);
      domURLdz[e_yFLV_tmp] = 0.0;
      domURLdz[e_yFLV_tmp + 1] = 0.0;
      domURLdz[e_yFLV_tmp + 2] = 0.0;
    }

    domURLdz[24] = I_WFLV[0];
    domURLdz[25] = I_WFLV[1];
    domURLdz[26] = I_WFLV[2];
    for (i = 0; i < 5; i++) {
      e_yFLV_tmp = 3 * (i + 9);
      domURLdz[e_yFLV_tmp] = 0.0;
      domURLdz[e_yFLV_tmp + 1] = 0.0;
      domURLdz[e_yFLV_tmp + 2] = 0.0;
    }

    for (i = 0; i < 3; i++) {
      domURRdz[3 * i] = 0.0;
      dvRRdz_tmp = 3 * (i + 3);
      domURRdz[dvRRdz_tmp] = iv1[3 * i];
      e_yFLV_tmp = 3 * (i + 6);
      domURRdz[e_yFLV_tmp] = 0.0;
      b_i = 3 * i + 1;
      domURRdz[b_i] = 0.0;
      domURRdz[dvRRdz_tmp + 1] = iv1[b_i];
      domURRdz[e_yFLV_tmp + 1] = 0.0;
      b_i = 3 * i + 2;
      domURRdz[b_i] = 0.0;
      domURRdz[dvRRdz_tmp + 2] = iv1[b_i];
      domURRdz[e_yFLV_tmp + 2] = 0.0;
      domURRdz[i + 27] = domdphi_RR[i];
    }

    for (i = 0; i < 4; i++) {
      dvRRdz_tmp = 3 * (i + 10);
      domURRdz[dvRRdz_tmp] = 0.0;
      domURRdz[dvRRdz_tmp + 1] = 0.0;
      domURRdz[dvRRdz_tmp + 2] = 0.0;
    }

    //  Tire road contact
    d = r_VFLV[0];
    d1 = r_VFLV[1];
    d2 = r_VFLV[2];
    d3 = omega_0UFLV[0];
    d4 = omega_0UFLV[1];
    d5 = omega_0UFLV[2];
    d6 = e_yFLV[0];
    d7 = e_yFLV[1];
    d8 = e_yFLV[2];
    r = _mm_loadu_pd(&A_0V[0]);
    r1 = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_mul_pd(r, _mm_set1_pd(v_0FLV_idx_0));
    r3 = _mm_mul_pd(r, _mm_set1_pd(d3));
    r = _mm_mul_pd(r, _mm_set1_pd(d6));
    r4 = _mm_loadu_pd(&A_0V[3]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d1));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0FLV_idx_1));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d4));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d7));
    r = _mm_add_pd(r, r4);
    r4 = _mm_loadu_pd(&A_0V[6]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d2));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0FLV_idx_2));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d5));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d8));
    r = _mm_add_pd(r, r4);
    _mm_storeu_pd(&d_A_0V[0], r);
    _mm_storeu_pd(&c_A_0V[0], r3);
    _mm_storeu_pd(&b_A_0V[0], r2);
    r = _mm_loadu_pd(&r_0V0[0]);
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&b_r_0V0[0], r);
    d9 = A_0V[2];
    d10 = d9 * d;
    d11 = d9 * v_0FLV_idx_0;
    d12 = d9 * d3;
    d13 = d9 * d6;
    d9 = A_0V[5];
    d10 += d9 * d1;
    d11 += d9 * v_0FLV_idx_1;
    d12 += d9 * d4;
    d13 += d9 * d7;
    d9 = A_0V[8];
    d10 += d9 * d2;
    d11 += d9 * v_0FLV_idx_2;
    d12 += d9 * d5;
    d13 += d9 * d8;
    d_A_0V[2] = d13;
    c_A_0V[2] = d12;
    b_A_0V[2] = d11;
    b_r_0V0[2] = r_0V0[2] + d10;
    gamma_FL = f_roadContact(b_expl_temp.x, b_expl_temp.y, rz, b_expl_temp.Mu,
      b_r_0V0, b_A_0V, c_A_0V, d_A_0V, dvdphi_FL, A_VQRL, r_stFL, b_u, uFR,
      I_WFRV, uRL);
    d = r_VFRV[0];
    d1 = r_VFRV[1];
    d2 = r_VFRV[2];
    d3 = omega_0UFRV[0];
    d4 = omega_0UFRV[1];
    d5 = omega_0UFRV[2];
    d6 = e_yFRV[0];
    d7 = e_yFRV[1];
    d8 = e_yFRV[2];
    r = _mm_loadu_pd(&A_0V[0]);
    r1 = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_mul_pd(r, _mm_set1_pd(v_0FRV_idx_0));
    r3 = _mm_mul_pd(r, _mm_set1_pd(d3));
    r = _mm_mul_pd(r, _mm_set1_pd(d6));
    r4 = _mm_loadu_pd(&A_0V[3]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d1));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0FRV_idx_1));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d4));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d7));
    r = _mm_add_pd(r, r4);
    r4 = _mm_loadu_pd(&A_0V[6]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d2));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0FRV_idx_2));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d5));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d8));
    r = _mm_add_pd(r, r4);
    _mm_storeu_pd(&d_A_0V[0], r);
    _mm_storeu_pd(&c_A_0V[0], r3);
    _mm_storeu_pd(&b_A_0V[0], r2);
    r = _mm_loadu_pd(&r_0V0[0]);
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&b_r_0V0[0], r);
    d9 = A_0V[2];
    d10 = d9 * d;
    d11 = d9 * v_0FRV_idx_0;
    d12 = d9 * d3;
    d13 = d9 * d6;
    d9 = A_0V[5];
    d10 += d9 * d1;
    d11 += d9 * v_0FRV_idx_1;
    d12 += d9 * d4;
    d13 += d9 * d7;
    d9 = A_0V[8];
    d10 += d9 * d2;
    d11 += d9 * v_0FRV_idx_2;
    d12 += d9 * d5;
    d13 += d9 * d8;
    d_A_0V[2] = d13;
    c_A_0V[2] = d12;
    b_A_0V[2] = d11;
    b_r_0V0[2] = r_0V0[2] + d10;
    gamma_FR = f_roadContact(b_expl_temp.x, b_expl_temp.y, rz, b_expl_temp.Mu,
      b_r_0V0, b_A_0V, c_A_0V, d_A_0V, domdphi_FL, A_VRL, r_stFR, uRR,
      A_x_tmp_tmp, r_MQFR0, b_A_x_tmp_tmp);
    d = r_VRLV[0];
    d1 = r_VRLV[1];
    d2 = r_VRLV[2];
    d3 = omega_0URLV[0];
    d4 = omega_0URLV[1];
    d5 = omega_0URLV[2];
    d6 = e_yRLV[0];
    d7 = e_yRLV[1];
    d8 = e_yRLV[2];
    r = _mm_loadu_pd(&A_0V[0]);
    r1 = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_mul_pd(r, _mm_set1_pd(v_0RLV_idx_0));
    r3 = _mm_mul_pd(r, _mm_set1_pd(d3));
    r = _mm_mul_pd(r, _mm_set1_pd(d6));
    r4 = _mm_loadu_pd(&A_0V[3]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d1));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0RLV_idx_1));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d4));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d7));
    r = _mm_add_pd(r, r4);
    r4 = _mm_loadu_pd(&A_0V[6]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d2));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0RLV_idx_2));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d5));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d8));
    r = _mm_add_pd(r, r4);
    _mm_storeu_pd(&d_A_0V[0], r);
    _mm_storeu_pd(&c_A_0V[0], r3);
    _mm_storeu_pd(&b_A_0V[0], r2);
    r = _mm_loadu_pd(&r_0V0[0]);
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&b_r_0V0[0], r);
    d9 = A_0V[2];
    d10 = d9 * d;
    d11 = d9 * v_0RLV_idx_0;
    d12 = d9 * d3;
    d13 = d9 * d6;
    d9 = A_0V[5];
    d10 += d9 * d1;
    d11 += d9 * v_0RLV_idx_1;
    d12 += d9 * d4;
    d13 += d9 * d7;
    d9 = A_0V[8];
    d10 += d9 * d2;
    d11 += d9 * v_0RLV_idx_2;
    d12 += d9 * d5;
    d13 += d9 * d8;
    d_A_0V[2] = d13;
    c_A_0V[2] = d12;
    b_A_0V[2] = d11;
    b_r_0V0[2] = r_0V0[2] + d10;
    gamma_RL = f_roadContact(b_expl_temp.x, b_expl_temp.y, rz, b_expl_temp.Mu,
      b_r_0V0, b_A_0V, c_A_0V, d_A_0V, dvdphi_FR, A_VQRR, r_stRL, dz_RL, w_RL,
      r_MQRL0, mu_RRL);
    d = r_VRRV[0];
    d1 = r_VRRV[1];
    d2 = r_VRRV[2];
    d3 = omega_0URRV[0];
    d4 = omega_0URRV[1];
    d5 = omega_0URRV[2];
    d6 = e_yRRV[0];
    d7 = e_yRRV[1];
    d8 = e_yRRV[2];
    r = _mm_loadu_pd(&A_0V[0]);
    r1 = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_mul_pd(r, _mm_set1_pd(v_0RRV_idx_0));
    r3 = _mm_mul_pd(r, _mm_set1_pd(d3));
    r = _mm_mul_pd(r, _mm_set1_pd(d6));
    r4 = _mm_loadu_pd(&A_0V[3]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d1));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0RRV_idx_1));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d4));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d7));
    r = _mm_add_pd(r, r4);
    r4 = _mm_loadu_pd(&A_0V[6]);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d2));
    r1 = _mm_add_pd(r1, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(v_0RRV_idx_2));
    r2 = _mm_add_pd(r2, r5);
    r5 = _mm_mul_pd(r4, _mm_set1_pd(d5));
    r3 = _mm_add_pd(r3, r5);
    r4 = _mm_mul_pd(r4, _mm_set1_pd(d8));
    r = _mm_add_pd(r, r4);
    _mm_storeu_pd(&d_A_0V[0], r);
    _mm_storeu_pd(&c_A_0V[0], r3);
    _mm_storeu_pd(&b_A_0V[0], r2);
    r = _mm_loadu_pd(&r_0V0[0]);
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&b_r_0V0[0], r);
    d9 = A_0V[2];
    d10 = d9 * d;
    d11 = d9 * v_0RRV_idx_0;
    d12 = d9 * d3;
    d13 = d9 * d6;
    d9 = A_0V[5];
    d10 += d9 * d1;
    d11 += d9 * v_0RRV_idx_1;
    d12 += d9 * d4;
    d13 += d9 * d7;
    d9 = A_0V[8];
    d10 += d9 * d2;
    d11 += d9 * v_0RRV_idx_2;
    d12 += d9 * d5;
    d13 += d9 * d8;
    d_A_0V[2] = d13;
    c_A_0V[2] = d12;
    b_A_0V[2] = d11;
    b_r_0V0[2] = r_0V0[2] + d10;
    gamma_RR = f_roadContact(b_expl_temp.x, b_expl_temp.y, rz, b_expl_temp.Mu,
      b_r_0V0, b_A_0V, c_A_0V, d_A_0V, domdphi_FR, A_z_tmp, r_stRR, dz_RR, w_RR,
      r_MQRR0, mu_RRR);
    for (i = 0; i < 3; i++) {
      d = A_y_tmp[i];
      d1 = A_y_tmp[i + 3];
      d2 = A_y_tmp[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        i1 = 3 * b_i + 1;
        i2 = 3 * b_i + 2;
        dvRRdz_tmp = i + 3 * b_i;
        A_VQFR[dvRRdz_tmp] = (d * A_VRL[3 * b_i] + d1 * A_VRL[i1]) + d2 *
          A_VRL[i2];
        A_0V[dvRRdz_tmp] = (d * A_VQRL[3 * b_i] + d1 * A_VQRL[i1]) + d2 *
          A_VQRL[i2];
      }
    }

    for (i = 0; i < 3; i++) {
      d = A_y_tmp[i];
      d1 = A_y_tmp[i + 3];
      d2 = A_y_tmp[i + 6];
      for (b_i = 0; b_i < 3; b_i++) {
        A_VQRL[i + 3 * b_i] = (d * A_VQRR[3 * b_i] + d1 * A_VQRR[3 * b_i + 1]) +
          d2 * A_VQRR[3 * b_i + 2];
      }
    }

    //  Tire forces
    //  TMeasy
    for (i = 0; i < 3; i++) {
      d = A_y_tmp[i];
      d1 = A_y_tmp[i + 3];
      d2 = A_y_tmp[i + 6];
      d3 = 0.0;
      for (b_i = 0; b_i < 3; b_i++) {
        e_yFLV_tmp = i + 3 * b_i;
        A_VQRR[e_yFLV_tmp] = (d * A_z_tmp[3 * b_i] + d1 * A_z_tmp[3 * b_i + 1])
          + d2 * A_z_tmp[3 * b_i + 2];
        d3 += I_UFLV_tmp[e_yFLV_tmp] * omega_0WFLV[b_i];
      }

      c_A_0V[i] = d3;
    }

    r_dynFL = f_tireTMeasy(dvdphi_FL, c_A_0V, r_stFL, b_u, uFR, gamma_FL, 0.7 *
      u[5] * uRL, &x[28], F_TFL, I_WFLV, s_FL, x_TFL_dot, F_ycFL, dF_x0FL,
      F_xMFL, dF_y0FL, F_yMFL);
    d = omega_0WFRV[0];
    d1 = omega_0WFRV[1];
    d2 = omega_0WFRV[2];
    r = _mm_loadu_pd(&I_UFRV_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&I_UFRV_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&I_UFRV_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&b_r_0V0[0], r);
    b_r_0V0[2] = (I_UFRV_tmp[2] * d + I_UFRV_tmp[5] * d1) + I_UFRV_tmp[8] * d2;
    r_dynFR = f_tireTMeasy(domdphi_FL, b_r_0V0, r_stFR, uRR, A_x_tmp_tmp,
      gamma_FR, 0.7 * u[5] * b_A_x_tmp_tmp, &x[31], F_TFR, dvdphi_RR, s_FR,
      x_TFR_dot, F_ycFR, dF_x0FR, F_xMFR, dF_y0FR, F_yMFR);
    d = omega_0WRLV[0];
    d1 = omega_0WRLV[1];
    d2 = omega_0WRLV[2];
    r = _mm_loadu_pd(&I_URLV_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&I_URLV_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&I_URLV_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&b_r_0V0[0], r);
    b_r_0V0[2] = (I_URLV_tmp[2] * d + I_URLV_tmp[5] * d1) + I_URLV_tmp[8] * d2;
    r_dynRL = f_tireTMeasy(dvdphi_FR, b_r_0V0, r_stRL, dz_RL, w_RL, gamma_RL,
      0.7 * u[5] * mu_RRL, &x[34], F_TRL, T_TRL, s_RL, x_TRL_dot, F_ycRL,
      dF_x0RL, F_xMRL, dF_y0RL, F_yMRL);
    d = omega_0WRRV[0];
    d1 = omega_0WRRV[1];
    d2 = omega_0WRRV[2];
    r = _mm_loadu_pd(&I_URRV_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&I_URRV_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&I_URRV_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&b_r_0V0[0], r);
    b_r_0V0[2] = (I_URRV_tmp[2] * d + I_URRV_tmp[5] * d1) + I_URRV_tmp[8] * d2;
    r_dynRR = f_tireTMeasy(domdphi_FR, b_r_0V0, r_stRR, dz_RR, w_RR, gamma_RR,
      0.7 * u[5] * mu_RRR, &x[37], F_TRR, T_TRR, s_RR, x_TRR_dot, F_ycRR,
      dF_x0RR, F_xMRR, dF_y0RR, F_yMRR);

    //  Suspension forces
    z_FL = f_suspensionForces(r_VFLV, r_VFRV, r_VRLV, r_VRRV, r_VFLV_dot,
      r_VFRV_dot, r_VRLV_dot, r_VRRV_dot, v[6], v[8], v[9], v[4], v[4], v[5], v
      [5], z_FR, z_RL, z_RR, z_FL_dot, z_FR_dot, z_RL_dot, z_RR_dot, b_u, uFR,
      uRL, uRR, A_x_tmp_tmp, b_A_x_tmp_tmp, dz_RR, w_RR, F_SFL, F_SFR, F_SRL,
      F_SRR);

    //  Aerodynamic Forces
    d = v_0FAV_tmp * v_0FAV_tmp;
    f_aerodynamics(sqrt((d + v_0FAV[1] * v_0FAV[1]) + v_0FAV[2] * v_0FAV[2]),
                   v_0FAV, sqrt((d + v_0RAV[1] * v_0RAV[1]) + v_0RAV[2] *
      v_0RAV[2]), v_0RAV, dv41, car[1], car[2], car[0], dvdphi_FL, domdphi_FL,
                   dvdphi_FR, domdphi_FR, r_0V0, F_ARAV, out_aero);

    //  Applied forces & torques
    //  Wheel rot. friction damping coef.
    //  Powertrain
    f_powertrain(x[24], x[25], x[26], x[27], u[6], u[7], u[8], u[9], v[7],
                 veh_i_G, uFR, uRL, uRR, T_DFL, T_DFR, T_DRL, T_DRR);

    //  Brake torques
    T_BFL = -39.111507258094768 * x[24];
    b_u = u[1] * 2.62119;

    //  Limit each absolute value of val0 to limit
    if (fabs(T_BFL) > b_u) {
      if (rtIsNaN(T_BFL)) {
        d = rtNaN;
      } else if (T_BFL < 0.0) {
        d = -1.0;
      } else {
        d = (T_BFL > 0.0);
      }

      T_BFL = b_u * d;
    }

    T_BFR = -39.111507258094768 * x[25];

    //  Limit each absolute value of val0 to limit
    if (fabs(T_BFR) > b_u) {
      if (rtIsNaN(T_BFR)) {
        d = rtNaN;
      } else if (T_BFR < 0.0) {
        d = -1.0;
      } else {
        d = (T_BFR > 0.0);
      }

      T_BFR = b_u * d;
    }

    T_BRL = -39.111507258094768 * x[26];
    b_u = u[2] * 2.4887362499999997;

    //  Limit each absolute value of val0 to limit
    if (fabs(T_BRL) > b_u) {
      if (rtIsNaN(T_BRL)) {
        d = rtNaN;
      } else if (T_BRL < 0.0) {
        d = -1.0;
      } else {
        d = (T_BRL > 0.0);
      }

      T_BRL = b_u * d;
    }

    T_BRR = -39.111507258094768 * x[27];

    //  Limit each absolute value of val0 to limit
    if (fabs(T_BRR) > b_u) {
      if (rtIsNaN(T_BRR)) {
        d = rtNaN;
      } else if (T_BRR < 0.0) {
        d = -1.0;
      } else {
        d = (T_BRR > 0.0);
      }

      T_BRR = b_u * d;
    }

    //  Applied torques
    //  Wheels
    b_u = T_BFL + T_DFL;
    d = I_WFRV[0];
    d1 = I_WFRV[1];
    d2 = I_WFRV[2];
    d3 = F_TFL[0];
    d4 = F_TFL[1];
    d5 = F_TFL[2];
    r = _mm_loadu_pd(&A_y_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&A_y_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&A_y_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&domdphi_RR[0], r);
    r = _mm_loadu_pd(&A_0V[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d3));
    r1 = _mm_loadu_pd(&A_0V[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d4));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&A_0V[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d5));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&F_FL[0], r);
    domdphi_RR[2] = (A_y_tmp[2] * d + A_y_tmp[5] * d1) + A_y_tmp[8] * d2;
    F_FL[2] = (A_0V[2] * d3 + A_0V[5] * d4) + A_0V[8] * d5;
    b_r_0V0[0] = domdphi_RR[1] * F_FL[2] - F_FL[1] * domdphi_RR[2];
    b_r_0V0[1] = F_FL[0] * domdphi_RR[2] - domdphi_RR[0] * F_FL[2];
    b_r_0V0[2] = domdphi_RR[0] * F_FL[1] - F_FL[0] * domdphi_RR[1];

    //  Wheel FL
    A_x_tmp_tmp = T_BFR + T_DFR;
    d = r_MQFR0[0];
    d1 = r_MQFR0[1];
    d2 = r_MQFR0[2];
    d3 = F_TFR[0];
    d4 = F_TFR[1];
    d5 = F_TFR[2];
    d6 = I_WFLV[0];
    d7 = I_WFLV[1];
    d8 = I_WFLV[2];
    r = _mm_loadu_pd(&A_y_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&A_y_tmp[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&A_y_tmp[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&domdphi_RR[0], r);
    r = _mm_loadu_pd(&A_VQFR[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d3));
    r1 = _mm_loadu_pd(&A_VQFR[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d4));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&A_VQFR[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d5));
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&F_FR[0], r);
    r = _mm_loadu_pd(&e_yFLV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(b_u));
    r1 = _mm_loadu_pd(&b_r_0V0[0]);
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&A_0V[0]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d6));
    r2 = _mm_loadu_pd(&A_0V[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d7));
    r1 = _mm_add_pd(r1, r2);
    r2 = _mm_loadu_pd(&A_0V[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d8));
    r1 = _mm_add_pd(r1, r2);
    r = _mm_add_pd(r, r1);
    _mm_storeu_pd(&T_WFL[0], r);
    domdphi_RR[2] = (A_y_tmp[2] * d + A_y_tmp[5] * d1) + A_y_tmp[8] * d2;
    F_FR[2] = (A_VQFR[2] * d3 + A_VQFR[5] * d4) + A_VQFR[8] * d5;
    T_WFL[2] = (e_yFLV[2] * b_u + b_r_0V0[2]) + ((A_0V[2] * d6 + A_0V[5] * d7) +
      A_0V[8] * d8);
    b_r_0V0[0] = domdphi_RR[1] * F_FR[2] - F_FR[1] * domdphi_RR[2];
    b_r_0V0[1] = F_FR[0] * domdphi_RR[2] - domdphi_RR[0] * F_FR[2];
    b_r_0V0[2] = domdphi_RR[0] * F_FR[1] - F_FR[0] * domdphi_RR[1];

    //  Wheel FR
    b_tmp = T_BRL + T_DRL;

    //  Wheel RL
    b_b_tmp = T_BRR + T_DRR;

    //  Wheel RR
    //  Hub-Motors
    //  Uprights
    //  Upright FL
    //  Upright FR
    //  Upright RL
    //  Upright RR
    //  Chassis
    d = F_TRR[0];
    d1 = F_TRR[1];
    d2 = F_TRR[2];
    d3 = F_TRL[0];
    d4 = F_TRL[1];
    d5 = F_TRL[2];
    d6 = dvdphi_RR[0];
    d7 = dvdphi_RR[1];
    d8 = dvdphi_RR[2];
    for (b_i = 0; b_i < 3; b_i++) {
      dvdphi_FR[b_i] = (A_VQRR[b_i] * d + A_VQRR[b_i + 3] * d1) + A_VQRR[b_i + 6]
        * d2;
      domdphi_FL[b_i] = (A_VQRL[b_i] * d3 + A_VQRL[b_i + 3] * d4) + A_VQRL[b_i +
        6] * d5;
      d9 = e_yFRV[b_i];
      T_WFR[b_i] = (d9 * A_x_tmp_tmp + b_r_0V0[b_i]) + ((A_VQFR[b_i] * d6 +
        A_VQFR[b_i + 3] * d7) + A_VQFR[b_i + 6] * d8);
      e_yFLV[b_i] = -e_yFLV[b_i] * b_u;
      d9 = -d9 * A_x_tmp_tmp;
      e_yFRV[b_i] = d9;
      i = static_cast<int>(dv41[b_i]);
      domdphi_FR[b_i] = F_SFL * static_cast<double>(i);
      I_WFRV[b_i] = F_SFR * static_cast<double>(i);
      I_WFLV[b_i] = F_SRL * static_cast<double>(i);
    }

    //  Applied forces
    b_u = ((F_SFL + F_SFR) + F_SRL) + F_SRR;

    //  Chassis
    //  FL
    //  FR
    //  RL
    //  RR
    //  Applied force vector
    for (b_i = 0; b_i < 3; b_i++) {
      dvdphi_RR[b_i] = F_SRR * dv41[b_i];
      v_0RAV[b_i] = r_0V0[b_i] + F_ARAV[b_i];
      F_FL[b_i] -= domdphi_FR[b_i];
      F_FR[b_i] -= I_WFRV[b_i];
      for (i = 0; i < 14; i++) {
        e_yFLV_tmp = b_i + 3 * i;
        dvRRdz_tmp = i + 14 * b_i;
        y_tmp[dvRRdz_tmp] = dvFRdz[e_yFLV_tmp];
        b_y_tmp[dvRRdz_tmp] = dvRLdz[e_yFLV_tmp];
        c_y_tmp[dvRRdz_tmp] = dvRRdz[e_yFLV_tmp];
        d_y_tmp[dvRRdz_tmp] = domUFLdz[e_yFLV_tmp];
        e_y_tmp[dvRRdz_tmp] = domUFRdz[e_yFLV_tmp];
        f_y_tmp[dvRRdz_tmp] = domURLdz[e_yFLV_tmp];
        g_y_tmp[dvRRdz_tmp] = domURRdz[e_yFLV_tmp];
        h_y_tmp[dvRRdz_tmp] = domWFLdz[e_yFLV_tmp];
        i_y_tmp[dvRRdz_tmp] = domWFRdz[e_yFLV_tmp];
        j_y_tmp[dvRRdz_tmp] = domWRLdz[e_yFLV_tmp];
        k_y_tmp[dvRRdz_tmp] = domWRRdz[e_yFLV_tmp];
      }

      d = A_y_tmp[b_i];
      d1 = d * r_MQRL0[0];
      d2 = d * r_MQRR0[0];
      d = A_y_tmp[b_i + 3];
      d1 += d * r_MQRL0[1];
      d2 += d * r_MQRR0[1];
      d = A_y_tmp[b_i + 6];
      d1 += d * r_MQRL0[2];
      d2 += d * r_MQRR0[2];
      dvdphi_FL[b_i] = d2;
      domdphi_RR[b_i] = d1;
    }

    b_r_VFLV[0] = r_VFLV[1] * domdphi_FR[2] - domdphi_FR[1] * r_VFLV[2];
    b_r_VFLV[1] = domdphi_FR[0] * r_VFLV[2] - r_VFLV[0] * domdphi_FR[2];
    b_r_VFLV[2] = r_VFLV[0] * domdphi_FR[1] - domdphi_FR[0] * r_VFLV[1];
    r_MQRL0[0] = r_VFRV[1] * I_WFRV[2] - I_WFRV[1] * r_VFRV[2];
    r_MQRL0[1] = I_WFRV[0] * r_VFRV[2] - r_VFRV[0] * I_WFRV[2];
    r_MQRL0[2] = r_VFRV[0] * I_WFRV[1] - I_WFRV[0] * r_VFRV[1];
    d_A_0V[0] = r_VRLV[1] * I_WFLV[2] - I_WFLV[1] * r_VRLV[2];
    d_A_0V[1] = I_WFLV[0] * r_VRLV[2] - r_VRLV[0] * I_WFLV[2];
    d_A_0V[2] = r_VRLV[0] * I_WFLV[1] - I_WFLV[0] * r_VRLV[1];
    c_A_0V[0] = r_VRRV[1] * dvdphi_RR[2] - dvdphi_RR[1] * r_VRRV[2];
    c_A_0V[1] = dvdphi_RR[0] * r_VRRV[2] - r_VRRV[0] * dvdphi_RR[2];
    c_A_0V[2] = r_VRRV[0] * dvdphi_RR[1] - dvdphi_RR[0] * r_VRRV[1];
    b_A_0V[0] = 0.0 * r_0V0[2] - r_0V0[1] * -v[6];
    b_A_0V[1] = r_0V0[0] * -v[6] - v[2] * r_0V0[2];
    b_A_0V[2] = r_0V0[1] * v[2] - r_0V0[0] * 0.0;
    b_r_0V0[0] = 0.0 * F_ARAV[2] - F_ARAV[1] * -v[6];
    b_r_0V0[1] = F_ARAV[0] * -v[6] - -v[3] * F_ARAV[2];
    b_r_0V0[2] = -v[3] * F_ARAV[1] - F_ARAV[0] * 0.0;
    r = _mm_loadu_pd(&v_0RAV[0]);
    _mm_storeu_pd(&r_MQRR0[0], _mm_add_pd(r, _mm_mul_pd(_mm_set1_pd(b_u),
      _mm_loadu_pd(&dv41[0]))));
    r = _mm_loadu_pd(&b_r_VFLV[0]);
    r1 = _mm_loadu_pd(&r_MQRL0[0]);
    r2 = _mm_loadu_pd(&d_A_0V[0]);
    r3 = _mm_loadu_pd(&c_A_0V[0]);
    r4 = _mm_loadu_pd(&b_A_0V[0]);
    r5 = _mm_loadu_pd(&b_r_0V0[0]);
    _mm_storeu_pd(&b_r_VFLV[0], _mm_add_pd(_mm_add_pd(_mm_add_pd(_mm_add_pd
      (_mm_add_pd(r, r1), r2), r3), r4), r5));
    b_r_VFLV[2] = ((((b_r_VFLV[2] + r_MQRL0[2]) + d_A_0V[2]) + c_A_0V[2]) +
                   b_A_0V[2]) + b_r_0V0[2];
    r = _mm_loadu_pd(&domdphi_FL[0]);
    r1 = _mm_loadu_pd(&I_WFLV[0]);
    _mm_storeu_pd(&I_WFLV[0], _mm_sub_pd(r, r1));
    r = _mm_loadu_pd(&dvdphi_FR[0]);
    r1 = _mm_loadu_pd(&dvdphi_RR[0]);
    _mm_storeu_pd(&dvdphi_RR[0], _mm_sub_pd(r, r1));
    I_WFLV[2] = domdphi_FL[2] - I_WFLV[2];
    dvdphi_RR[2] = dvdphi_FR[2] - dvdphi_RR[2];
    r = _mm_loadu_pd(&e_yRLV[0]);
    r1 = _mm_set1_pd(-1.0);
    _mm_storeu_pd(&c_A_0V[0], _mm_mul_pd(_mm_mul_pd(r, r1), _mm_set1_pd(b_tmp)));
    r = _mm_loadu_pd(&e_yRRV[0]);
    _mm_storeu_pd(&b_A_0V[0], _mm_mul_pd(_mm_mul_pd(r, r1), _mm_set1_pd(b_b_tmp)));
    d = r_MQRR0[0];
    d1 = r_MQRR0[1];
    d2 = v_0RAV[2] + b_u;
    d3 = b_r_VFLV[0];
    d4 = b_r_VFLV[1];
    d5 = b_r_VFLV[2];
    d6 = F_FL[0];
    d7 = F_FL[1];
    d8 = F_FL[2];
    d9 = F_FR[0];
    d10 = F_FR[1];
    d11 = F_FR[2];
    d12 = I_WFLV[0];
    d13 = I_WFLV[1];
    b_u = I_WFLV[2];
    A_x_tmp_tmp = dvdphi_RR[0];
    b_A_x_tmp_tmp = dvdphi_RR[1];
    dz_RR = dvdphi_RR[2];
    w_RR = e_yFLV[0];
    mu_RRR = e_yFLV[1];
    dz_RL = e_yFLV[2];
    w_RL = e_yFRV[0];
    mu_RRL = e_yFRV[1];
    uFR = e_yFRV[2];
    uRL = c_A_0V[0];
    uRR = c_A_0V[1];
    b_domdu_FR = -e_yRLV[2] * b_tmp;
    v_0FAV_tmp = b_A_0V[0];
    d14 = b_A_0V[1];
    d15 = -e_yRRV[2] * b_b_tmp;
    d16 = T_WFL[0];
    d17 = T_WFL[1];
    d18 = T_WFL[2];
    d19 = T_WFR[0];
    d20 = T_WFR[1];
    d21 = T_WFR[2];
    for (i = 0; i < 14; i++) {
      f[i] = (((((((((((static_cast<double>(iv2[i]) * d + static_cast<double>
                        (iv2[i + 14]) * d1) + static_cast<double>(iv2[i + 28]) *
                       d2) + ((static_cast<double>(iv4[i]) * d3 + static_cast<
        double>(iv4[i + 14]) * d4) + static_cast<double>(iv4[i + 28]) * d5)) +
                     ((dvFLdz[3 * i] * d6 + dvFLdz[3 * i + 1] * d7) + dvFLdz[3 *
                      i + 2] * d8)) + ((y_tmp[i] * d9 + y_tmp[i + 14] * d10) +
        y_tmp[i + 28] * d11)) + ((b_y_tmp[i] * d12 + b_y_tmp[i + 14] * d13) +
        b_y_tmp[i + 28] * b_u)) + ((c_y_tmp[i] * A_x_tmp_tmp + c_y_tmp[i + 14] *
        b_A_x_tmp_tmp) + c_y_tmp[i + 28] * dz_RR)) + ((d_y_tmp[i] * w_RR +
        d_y_tmp[i + 14] * mu_RRR) + d_y_tmp[i + 28] * dz_RL)) + ((e_y_tmp[i] *
                  w_RL + e_y_tmp[i + 14] * mu_RRL) + e_y_tmp[i + 28] * uFR)) +
               ((f_y_tmp[i] * uRL + f_y_tmp[i + 14] * uRR) + f_y_tmp[i + 28] *
                b_domdu_FR)) + ((g_y_tmp[i] * v_0FAV_tmp + g_y_tmp[i + 14] * d14)
               + g_y_tmp[i + 28] * d15)) + ((h_y_tmp[i] * d16 + h_y_tmp[i + 14] *
        d17) + h_y_tmp[i + 28] * d18);
      l_y_tmp[i] = (i_y_tmp[i] * d19 + i_y_tmp[i + 14] * d20) + i_y_tmp[i + 28] *
        d21;
    }

    b_r_0V0[0] = domdphi_RR[1] * domdphi_FL[2] - domdphi_FL[1] * domdphi_RR[2];
    b_r_0V0[1] = domdphi_FL[0] * domdphi_RR[2] - domdphi_RR[0] * domdphi_FL[2];
    b_r_0V0[2] = domdphi_RR[0] * domdphi_FL[1] - domdphi_FL[0] * domdphi_RR[1];
    d = T_TRL[0];
    d1 = T_TRL[1];
    d2 = T_TRL[2];
    r = _mm_loadu_pd(&e_yRLV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(b_tmp));
    r2 = _mm_loadu_pd(&b_r_0V0[0]);
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_VQRL[0]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
    r3 = _mm_loadu_pd(&A_VQRL[3]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
    r2 = _mm_add_pd(r2, r3);
    r3 = _mm_loadu_pd(&A_VQRL[6]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
    r2 = _mm_add_pd(r2, r3);
    r = _mm_add_pd(r, r2);
    _mm_storeu_pd(&c_A_0V[0], r);
    c_A_0V[2] = (e_yRLV[2] * b_tmp + b_r_0V0[2]) + ((A_VQRL[2] * d + A_VQRL[5] *
      d1) + A_VQRL[8] * d2);
    b_r_0V0[0] = dvdphi_FL[1] * dvdphi_FR[2] - dvdphi_FR[1] * dvdphi_FL[2];
    b_r_0V0[1] = dvdphi_FR[0] * dvdphi_FL[2] - dvdphi_FL[0] * dvdphi_FR[2];
    b_r_0V0[2] = dvdphi_FL[0] * dvdphi_FR[1] - dvdphi_FR[0] * dvdphi_FL[1];
    d = T_TRR[0];
    d1 = T_TRR[1];
    d2 = T_TRR[2];
    r = _mm_loadu_pd(&e_yRRV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(b_b_tmp));
    r2 = _mm_loadu_pd(&b_r_0V0[0]);
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_VQRR[0]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
    r3 = _mm_loadu_pd(&A_VQRR[3]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
    r2 = _mm_add_pd(r2, r3);
    r3 = _mm_loadu_pd(&A_VQRR[6]);
    r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
    r2 = _mm_add_pd(r2, r3);
    r = _mm_add_pd(r, r2);
    _mm_storeu_pd(&b_A_0V[0], r);
    b_A_0V[2] = (e_yRRV[2] * b_b_tmp + b_r_0V0[2]) + ((A_VQRR[2] * d + A_VQRR[5]
      * d1) + A_VQRR[8] * d2);
    d = c_A_0V[0];
    d1 = c_A_0V[1];
    d2 = c_A_0V[2];
    d3 = b_A_0V[0];
    d4 = b_A_0V[1];
    d5 = b_A_0V[2];
    for (i = 0; i <= 12; i += 2) {
      r = _mm_loadu_pd(&f[i]);
      r2 = _mm_loadu_pd(&l_y_tmp[i]);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&j_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
      r3 = _mm_loadu_pd(&j_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&j_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&k_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d3));
      r3 = _mm_loadu_pd(&k_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d4));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&k_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d5));
      r2 = _mm_add_pd(r2, r3);
      _mm_storeu_pd(&l_y_tmp[i], r2);
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&c_f[i], r);
    }

    //       % Chassis force
    //       % Chassis torque
    //     % Force FL
    //     % Force FR
    //     % Force RL
    //     % Force RR
    //  % Upright torque FL
    //  % Upright torque FR
    //  % Upright torque RL
    //  % Upright torque RR
    //  % Wheel torque FL
    //  % Wheel torque FR
    //  % Wheel torque RL
    //  Wheel torque RR
    //  Generalized force vector
    for (i = 0; i < 42; i++) {
      q_tmp[i] = static_cast<double>(iv2[i]) * v[1];
    }

    for (i = 0; i < 14; i++) {
      b_q_tmp[i] = dvFLdz[3 * i] * 11.6;
      b_q_tmp[i + 14] = dvFLdz[3 * i + 1] * 11.6;
      b_q_tmp[i + 28] = dvFLdz[3 * i + 2] * 11.6;
    }

    r = _mm_set1_pd(11.6);
    for (i = 0; i <= 40; i += 2) {
      r2 = _mm_loadu_pd(&y_tmp[i]);
      _mm_storeu_pd(&y_tmp[i], _mm_mul_pd(r2, r));
      r2 = _mm_loadu_pd(&b_y_tmp[i]);
      _mm_storeu_pd(&b_y_tmp[i], _mm_mul_pd(r2, r));
      r2 = _mm_loadu_pd(&c_y_tmp[i]);
      _mm_storeu_pd(&c_y_tmp[i], _mm_mul_pd(r2, r));
    }

    v_0FAV[0] = g_V[0] - ((omega_0VV[1] * r_VFLV_dot[2] - r_VFLV_dot[1] *
      omega_0VV[2]) + (omega_0VV[1] * v_0FLV_idx_2 - v_0FLV_idx_1 * omega_0VV[2]));
    v_0FAV[1] = g_V[1] - ((r_VFLV_dot[0] * omega_0VV[2] - omega_0VV[0] *
      r_VFLV_dot[2]) + (v_0FLV_idx_0 * omega_0VV[2] - omega_0VV[0] *
                        v_0FLV_idx_2));
    v_0FAV[2] = g_V[2] - ((omega_0VV[0] * r_VFLV_dot[1] - r_VFLV_dot[0] *
      omega_0VV[1]) + (omega_0VV[0] * v_0FLV_idx_1 - v_0FLV_idx_0 * omega_0VV[1]));
    r_MQFR0[0] = g_V[0] - ((omega_0VV[1] * r_VFRV_dot[2] - r_VFRV_dot[1] *
      omega_0VV[2]) + (omega_0VV[1] * v_0FRV_idx_2 - v_0FRV_idx_1 * omega_0VV[2]));
    r_MQFR0[1] = g_V[1] - ((r_VFRV_dot[0] * omega_0VV[2] - omega_0VV[0] *
      r_VFRV_dot[2]) + (v_0FRV_idx_0 * omega_0VV[2] - omega_0VV[0] *
                        v_0FRV_idx_2));
    r_MQFR0[2] = g_V[2] - ((omega_0VV[0] * r_VFRV_dot[1] - r_VFRV_dot[0] *
      omega_0VV[1]) + (omega_0VV[0] * v_0FRV_idx_1 - v_0FRV_idx_0 * omega_0VV[1]));
    d = omega_0UFLV[0];
    d1 = omega_0UFLV[1];
    d2 = omega_0UFLV[2];
    r = _mm_loadu_pd(&I_UFLV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_loadu_pd(&I_UFLV[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&I_UFLV[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r2);
    _mm_storeu_pd(&dvdphi_RR[0], r);
    dvdphi_RR[2] = (I_UFLV[2] * d + I_UFLV[5] * d1) + I_UFLV[8] * d2;
    r_0V0[0] = -(omega_0UFLV[1] * dvdphi_RR[2] - dvdphi_RR[1] * omega_0UFLV[2]);
    r_0V0[1] = -(dvdphi_RR[0] * omega_0UFLV[2] - omega_0UFLV[0] * dvdphi_RR[2]);
    r_0V0[2] = -(omega_0UFLV[0] * dvdphi_RR[1] - dvdphi_RR[0] * omega_0UFLV[1]);
    d = omega_0UFRV[0];
    d1 = omega_0UFRV[1];
    d2 = omega_0UFRV[2];
    r = _mm_loadu_pd(&I_UFRV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_loadu_pd(&I_UFRV[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&I_UFRV[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r2);
    _mm_storeu_pd(&dvdphi_RR[0], r);
    dvdphi_RR[2] = (I_UFRV[2] * d + I_UFRV[5] * d1) + I_UFRV[8] * d2;
    domdphi_RR[0] = -(omega_0UFRV[1] * dvdphi_RR[2] - dvdphi_RR[1] *
                      omega_0UFRV[2]);
    domdphi_RR[1] = -(dvdphi_RR[0] * omega_0UFRV[2] - omega_0UFRV[0] *
                      dvdphi_RR[2]);
    domdphi_RR[2] = -(omega_0UFRV[0] * dvdphi_RR[1] - dvdphi_RR[0] *
                      omega_0UFRV[1]);
    d = omega_0WFLV[0];
    d1 = omega_0WFLV[1];
    d2 = omega_0WFLV[2];
    r = _mm_loadu_pd(&b_I_WFLV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_loadu_pd(&b_I_WFLV[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&b_I_WFLV[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r2);
    _mm_storeu_pd(&dvdphi_RR[0], r);
    dvdphi_RR[2] = (b_I_WFLV[2] * d + b_I_WFLV[5] * d1) + b_I_WFLV[8] * d2;
    d_A_0V[0] = omega_0VV[1] * omega_0WFLV[2] - omega_0WFLV[1] * omega_0VV[2];
    d_A_0V[1] = omega_0WFLV[0] * omega_0VV[2] - omega_0VV[0] * omega_0WFLV[2];
    d_A_0V[2] = omega_0VV[0] * omega_0WFLV[1] - omega_0WFLV[0] * omega_0VV[1];
    b_A_0V[0] = omega_VUFLV[1] * omega_0WFLV_tmp[2] - omega_0WFLV_tmp[1] *
      omega_VUFLV[2];
    b_A_0V[1] = omega_0WFLV_tmp[0] * omega_VUFLV[2] - omega_VUFLV[0] *
      omega_0WFLV_tmp[2];
    b_A_0V[2] = omega_VUFLV[0] * omega_0WFLV_tmp[1] - omega_0WFLV_tmp[0] *
      omega_VUFLV[1];
    r = _mm_loadu_pd(&b_I_WFLV[0]);
    _mm_storeu_pd(&A_z_tmp[0], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&b_I_WFLV[2]);
    _mm_storeu_pd(&A_z_tmp[2], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&b_I_WFLV[4]);
    _mm_storeu_pd(&A_z_tmp[4], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&b_I_WFLV[6]);
    _mm_storeu_pd(&A_z_tmp[6], _mm_mul_pd(r, r1));
    A_z_tmp[8] = -b_I_WFLV[8];
    r = _mm_loadu_pd(&d_A_0V[0]);
    r2 = _mm_loadu_pd(&b_A_0V[0]);
    _mm_storeu_pd(&d_A_0V[0], _mm_add_pd(r, r2));
    d_A_0V[2] += b_A_0V[2];
    c_A_0V[0] = omega_0WFLV[1] * dvdphi_RR[2] - dvdphi_RR[1] * omega_0WFLV[2];
    c_A_0V[1] = dvdphi_RR[0] * omega_0WFLV[2] - omega_0WFLV[0] * dvdphi_RR[2];
    c_A_0V[2] = omega_0WFLV[0] * dvdphi_RR[1] - dvdphi_RR[0] * omega_0WFLV[1];
    d = omega_0WFRV[0];
    d1 = omega_0WFRV[1];
    d2 = omega_0WFRV[2];
    d3 = d_A_0V[0];
    d4 = d_A_0V[1];
    d5 = d_A_0V[2];
    r = _mm_loadu_pd(&b_I_WFRV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_loadu_pd(&b_I_WFRV[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&b_I_WFRV[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r2);
    _mm_storeu_pd(&dvdphi_RR[0], r);
    r = _mm_loadu_pd(&A_z_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d3));
    r2 = _mm_loadu_pd(&A_z_tmp[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d4));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_z_tmp[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d5));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&c_A_0V[0]);
    r = _mm_sub_pd(r, r2);
    _mm_storeu_pd(&I_WFLV[0], r);
    dvdphi_RR[2] = (b_I_WFRV[2] * d + b_I_WFRV[5] * d1) + b_I_WFRV[8] * d2;
    I_WFLV[2] = ((A_z_tmp[2] * d3 + A_z_tmp[5] * d4) + A_z_tmp[8] * d5) -
      c_A_0V[2];
    d_A_0V[0] = omega_0VV[1] * omega_0WFRV[2] - omega_0WFRV[1] * omega_0VV[2];
    d_A_0V[1] = omega_0WFRV[0] * omega_0VV[2] - omega_0VV[0] * omega_0WFRV[2];
    d_A_0V[2] = omega_0VV[0] * omega_0WFRV[1] - omega_0WFRV[0] * omega_0VV[1];
    b_r_0V0[0] = omega_VUFRV[1] * omega_0WFRV_tmp[2] - omega_0WFRV_tmp[1] *
      omega_VUFRV[2];
    b_r_0V0[1] = omega_0WFRV_tmp[0] * omega_VUFRV[2] - omega_VUFRV[0] *
      omega_0WFRV_tmp[2];
    b_r_0V0[2] = omega_VUFRV[0] * omega_0WFRV_tmp[1] - omega_0WFRV_tmp[0] *
      omega_VUFRV[1];
    r = _mm_loadu_pd(&b_I_WFRV[0]);
    _mm_storeu_pd(&A_z_tmp[0], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&b_I_WFRV[2]);
    _mm_storeu_pd(&A_z_tmp[2], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&b_I_WFRV[4]);
    _mm_storeu_pd(&A_z_tmp[4], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&b_I_WFRV[6]);
    _mm_storeu_pd(&A_z_tmp[6], _mm_mul_pd(r, r1));
    A_z_tmp[8] = -b_I_WFRV[8];
    r = _mm_loadu_pd(&d_A_0V[0]);
    r2 = _mm_loadu_pd(&b_r_0V0[0]);
    _mm_storeu_pd(&d_A_0V[0], _mm_add_pd(r, r2));
    d_A_0V[2] += b_r_0V0[2];
    b_r_0V0[0] = omega_0WFRV[1] * dvdphi_RR[2] - dvdphi_RR[1] * omega_0WFRV[2];
    b_r_0V0[1] = dvdphi_RR[0] * omega_0WFRV[2] - omega_0WFRV[0] * dvdphi_RR[2];
    b_r_0V0[2] = omega_0WFRV[0] * dvdphi_RR[1] - dvdphi_RR[0] * omega_0WFRV[1];
    d = d_A_0V[0];
    d1 = d_A_0V[1];
    d2 = d_A_0V[2];
    r = _mm_loadu_pd(&A_z_tmp[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_loadu_pd(&A_z_tmp[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&A_z_tmp[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&b_r_0V0[0]);
    r = _mm_sub_pd(r, r2);
    _mm_storeu_pd(&I_WFRV[0], r);
    I_WFRV[2] = ((A_z_tmp[2] * d + A_z_tmp[5] * d1) + A_z_tmp[8] * d2) -
      b_r_0V0[2];

    //  % Chassis force
    //  % Chassis torque
    //  % Force FL
    //  % Force FR
    //  % Force RL
    //  % Force RR
    //  % Upright torque FL
    //  % Upright torque FR
    //  % Upright torque RL
    //  % Upright torque RR
    //  % Wheel torque FL
    //  % Wheel torque FR
    //  % Wheel torque RL
    //  Wheel torque RR
    //  Generalized mass matrix
    for (i = 0; i < 14; i++) {
      d = d_y_tmp[i];
      d1 = d_y_tmp[i + 14];
      d2 = d_y_tmp[i + 28];
      for (b_i = 0; b_i < 3; b_i++) {
        m_y_tmp[i + 14 * b_i] = (d * I_UFLV[3 * b_i] + d1 * I_UFLV[3 * b_i + 1])
          + d2 * I_UFLV[3 * b_i + 2];
      }

      d = m_y_tmp[i];
      d1 = m_y_tmp[i + 14];
      d2 = m_y_tmp[i + 28];
      d3 = b_q_tmp[i];
      d4 = b_q_tmp[i + 14];
      d5 = b_q_tmp[i + 28];
      for (b_i = 0; b_i < 14; b_i++) {
        i1 = 3 * b_i + 1;
        i2 = 3 * b_i + 2;
        e_yFLV_tmp = i + 14 * b_i;
        n_y_tmp[e_yFLV_tmp] = (d * domUFLdz[3 * b_i] + d1 * domUFLdz[i1]) + d2 *
          domUFLdz[i2];
        c_q_tmp[e_yFLV_tmp] = (d3 * dvFLdz[3 * b_i] + d4 * dvFLdz[i1]) + d5 *
          dvFLdz[i2];
      }

      d = h_y_tmp[i];
      d1 = h_y_tmp[i + 14];
      d2 = h_y_tmp[i + 28];
      for (b_i = 0; b_i < 3; b_i++) {
        m_y_tmp[i + 14 * b_i] = (d * b_I_WFLV[3 * b_i] + d1 * b_I_WFLV[3 * b_i +
          1]) + d2 * b_I_WFLV[3 * b_i + 2];
      }

      d = q_tmp[i];
      d1 = q_tmp[i + 14];
      d2 = q_tmp[i + 28];
      d3 = m_y_tmp[i];
      d4 = m_y_tmp[i + 14];
      d5 = m_y_tmp[i + 28];
      for (b_i = 0; b_i < 14; b_i++) {
        i1 = 3 * b_i + 1;
        i2 = 3 * b_i + 2;
        dvRRdz_tmp = i + 14 * b_i;
        M[dvRRdz_tmp] = ((d * static_cast<double>(iv5[3 * b_i]) + d1 *
                          static_cast<double>(iv5[i1])) + d2 * static_cast<
                         double>(iv5[i2])) + static_cast<double>(iv6[dvRRdz_tmp]);
        d_q_tmp[dvRRdz_tmp] = (c_q_tmp[dvRRdz_tmp] + n_y_tmp[dvRRdz_tmp]) + ((d3
          * domWFLdz[3 * b_i] + d4 * domWFLdz[i1]) + d5 * domWFLdz[i2]);
      }

      d = e_y_tmp[i];
      d1 = e_y_tmp[i + 14];
      d2 = e_y_tmp[i + 28];
      for (b_i = 0; b_i < 3; b_i++) {
        m_y_tmp[i + 14 * b_i] = (d * I_UFRV[3 * b_i] + d1 * I_UFRV[3 * b_i + 1])
          + d2 * I_UFRV[3 * b_i + 2];
      }

      d = y_tmp[i];
      d1 = y_tmp[i + 14];
      d2 = y_tmp[i + 28];
      for (b_i = 0; b_i < 14; b_i++) {
        n_y_tmp[i + 14 * b_i] = (d * dvFRdz[3 * b_i] + d1 * dvFRdz[3 * b_i + 1])
          + d2 * dvFRdz[3 * b_i + 2];
      }
    }

    for (i = 0; i < 14; i++) {
      d = m_y_tmp[i];
      d1 = m_y_tmp[i + 14];
      d2 = m_y_tmp[i + 28];
      for (b_i = 0; b_i < 14; b_i++) {
        o_y_tmp[i + 14 * b_i] = (d * domUFRdz[3 * b_i] + d1 * domUFRdz[3 * b_i +
          1]) + d2 * domUFRdz[3 * b_i + 2];
      }

      d = f_y_tmp[i];
      d1 = f_y_tmp[i + 14];
      d2 = f_y_tmp[i + 28];
      d3 = i_y_tmp[i];
      d4 = i_y_tmp[i + 14];
      d5 = i_y_tmp[i + 28];
      for (b_i = 0; b_i < 3; b_i++) {
        i1 = 3 * b_i + 1;
        i2 = 3 * b_i + 2;
        e_yFLV_tmp = i + 14 * b_i;
        domWFLdz[e_yFLV_tmp] = (d * I_URLV[3 * b_i] + d1 * I_URLV[i1]) + d2 *
          I_URLV[i2];
        m_y_tmp[e_yFLV_tmp] = (d3 * b_I_WFRV[3 * b_i] + d4 * b_I_WFRV[i1]) + d5 *
          b_I_WFRV[i2];
      }

      d = domWFLdz[i];
      d1 = domWFLdz[i + 14];
      d2 = domWFLdz[i + 28];
      d3 = b_y_tmp[i];
      d4 = b_y_tmp[i + 14];
      d5 = b_y_tmp[i + 28];
      for (b_i = 0; b_i < 14; b_i++) {
        i1 = 3 * b_i + 1;
        i2 = 3 * b_i + 2;
        e_yFLV_tmp = i + 14 * b_i;
        q_y_tmp[e_yFLV_tmp] = (d * domURLdz[3 * b_i] + d1 * domURLdz[i1]) + d2 *
          domURLdz[i2];
        p_y_tmp[e_yFLV_tmp] = (d3 * dvRLdz[3 * b_i] + d4 * dvRLdz[i1]) + d5 *
          dvRLdz[i2];
      }

      d = j_y_tmp[i];
      d1 = j_y_tmp[i + 14];
      d2 = j_y_tmp[i + 28];
      for (b_i = 0; b_i < 3; b_i++) {
        domWFLdz[i + 14 * b_i] = (d * I_WRLV[3 * b_i] + d1 * I_WRLV[3 * b_i + 1])
          + d2 * I_WRLV[3 * b_i + 2];
      }

      d = m_y_tmp[i];
      d1 = m_y_tmp[i + 14];
      d2 = m_y_tmp[i + 28];
      d3 = domWFLdz[i];
      d4 = domWFLdz[i + 14];
      d5 = domWFLdz[i + 28];
      for (b_i = 0; b_i < 14; b_i++) {
        i1 = 3 * b_i + 1;
        i2 = 3 * b_i + 2;
        dvRRdz_tmp = i + 14 * b_i;
        c_q_tmp[dvRRdz_tmp] = (M[dvRRdz_tmp] + d_q_tmp[dvRRdz_tmp]) +
          ((n_y_tmp[dvRRdz_tmp] + o_y_tmp[dvRRdz_tmp]) + ((d * domWFRdz[3 * b_i]
             + d1 * domWFRdz[i1]) + d2 * domWFRdz[i2]));
        n_y_tmp[dvRRdz_tmp] = (p_y_tmp[dvRRdz_tmp] + q_y_tmp[dvRRdz_tmp]) + ((d3
          * domWRLdz[3 * b_i] + d4 * domWRLdz[i1]) + d5 * domWRLdz[i2]);
      }

      d = g_y_tmp[i];
      d1 = g_y_tmp[i + 14];
      d2 = g_y_tmp[i + 28];
      for (b_i = 0; b_i < 3; b_i++) {
        m_y_tmp[i + 14 * b_i] = (d * I_URRV[3 * b_i] + d1 * I_URRV[3 * b_i + 1])
          + d2 * I_URRV[3 * b_i + 2];
      }

      d = m_y_tmp[i];
      d1 = m_y_tmp[i + 14];
      d2 = m_y_tmp[i + 28];
      d3 = c_y_tmp[i];
      d4 = c_y_tmp[i + 14];
      d5 = c_y_tmp[i + 28];
      for (b_i = 0; b_i < 14; b_i++) {
        i1 = 3 * b_i + 1;
        i2 = 3 * b_i + 2;
        e_yFLV_tmp = i + 14 * b_i;
        p_y_tmp[e_yFLV_tmp] = (d * domURRdz[3 * b_i] + d1 * domURRdz[i1]) + d2 *
          domURRdz[i2];
        o_y_tmp[e_yFLV_tmp] = (d3 * dvRRdz[3 * b_i] + d4 * dvRRdz[i1]) + d5 *
          dvRRdz[i2];
      }

      d = k_y_tmp[i];
      d1 = k_y_tmp[i + 14];
      d2 = k_y_tmp[i + 28];
      for (b_i = 0; b_i < 3; b_i++) {
        m_y_tmp[i + 14 * b_i] = (d * I_WRRV[3 * b_i] + d1 * I_WRRV[3 * b_i + 1])
          + d2 * I_WRRV[3 * b_i + 2];
      }

      d = m_y_tmp[i];
      d1 = m_y_tmp[i + 14];
      d2 = m_y_tmp[i + 28];
      for (b_i = 0; b_i < 14; b_i++) {
        dvRRdz_tmp = i + 14 * b_i;
        M[dvRRdz_tmp] = (c_q_tmp[dvRRdz_tmp] + n_y_tmp[dvRRdz_tmp]) +
          ((o_y_tmp[dvRRdz_tmp] + p_y_tmp[dvRRdz_tmp]) + ((d * domWRRdz[3 * b_i]
             + d1 * domWRRdz[3 * b_i + 1]) + d2 * domWRRdz[3 * b_i + 2]));
      }
    }

    //  % Chassis
    //  % FL
    //  % FR
    //  % RL
    //  RR
    //  Solve for state derivatives
    //  Generalized coordinates derivatives
    //  Generalized velocities derivatives
    d = omega_0WRRV[0];
    d1 = omega_0WRRV[1];
    d2 = omega_0WRRV[2];
    d3 = omega_0WRLV[0];
    d4 = omega_0WRLV[1];
    d5 = omega_0WRLV[2];
    d6 = omega_0URRV[0];
    d7 = omega_0URRV[1];
    d8 = omega_0URRV[2];
    d9 = omega_0URLV[0];
    d10 = omega_0URLV[1];
    d11 = omega_0URLV[2];
    d12 = omega_0VV[0];
    d13 = omega_0VV[1];
    b_u = omega_0VV[2];
    for (i = 0; i < 3; i++) {
      domdphi_FR[i] = (I_WRRV[i] * d + I_WRRV[i + 3] * d1) + I_WRRV[i + 6] * d2;
      dvdphi_FR[i] = (I_WRLV[i] * d3 + I_WRLV[i + 3] * d4) + I_WRLV[i + 6] * d5;
      domdphi_FL[i] = (I_URRV[i] * d6 + I_URRV[i + 3] * d7) + I_URRV[i + 6] * d8;
      dvdphi_FL[i] = (I_URLV[i] * d9 + I_URLV[i + 3] * d10) + I_URLV[i + 6] *
        d11;
      dvdphi_RR[i] = (static_cast<double>(iv3[i]) * d12 + static_cast<double>
                      (iv3[i + 3]) * d13) + static_cast<double>(iv3[i + 6]) *
        b_u;
    }

    memcpy(&dv[0], &x[14], 14U * sizeof(double));
    coder::mldivide(K, dv);
    d = g_V[0] - (omega_0VV[1] * v_0VV_idx_2 - v_0VV_idx_1 * omega_0VV[2]);
    d1 = g_V[1] - (v_0VV_idx_0 * omega_0VV[2] - omega_0VV[0] * v_0VV_idx_2);
    d2 = g_V[2] - (omega_0VV[0] * v_0VV_idx_1 - v_0VV_idx_0 * omega_0VV[1]);
    d3 = -(omega_0VV[1] * dvdphi_RR[2] - dvdphi_RR[1] * omega_0VV[2]);
    d4 = -(dvdphi_RR[0] * omega_0VV[2] - omega_0VV[0] * dvdphi_RR[2]);
    d5 = -(omega_0VV[0] * dvdphi_RR[1] - dvdphi_RR[0] * omega_0VV[1]);
    d6 = v_0FAV[0];
    d7 = v_0FAV[1];
    d8 = v_0FAV[2];
    d9 = r_MQFR0[0];
    d10 = r_MQFR0[1];
    d11 = r_MQFR0[2];
    for (i = 0; i < 14; i++) {
      f[i] = ((c_f[i] + ((q_tmp[i] * d + q_tmp[i + 14] * d1) + q_tmp[i + 28] *
                         d2)) + ((static_cast<double>(iv4[i]) * d3 +
                static_cast<double>(iv4[i + 14]) * d4) + static_cast<double>
               (iv4[i + 28]) * d5)) + ((b_q_tmp[i] * d6 + b_q_tmp[i + 14] * d7)
        + b_q_tmp[i + 28] * d8);
      l_y_tmp[i] = (y_tmp[i] * d9 + y_tmp[i + 14] * d10) + y_tmp[i + 28] * d11;
    }

    d_A_0V[0] = omega_0VV[1] * r_VRLV_dot[2] - r_VRLV_dot[1] * omega_0VV[2];
    d_A_0V[1] = r_VRLV_dot[0] * omega_0VV[2] - omega_0VV[0] * r_VRLV_dot[2];
    d_A_0V[2] = omega_0VV[0] * r_VRLV_dot[1] - r_VRLV_dot[0] * omega_0VV[1];
    c_A_0V[0] = omega_0VV[1] * v_0RLV_idx_2 - v_0RLV_idx_1 * omega_0VV[2];
    c_A_0V[1] = v_0RLV_idx_0 * omega_0VV[2] - omega_0VV[0] * v_0RLV_idx_2;
    c_A_0V[2] = omega_0VV[0] * v_0RLV_idx_1 - v_0RLV_idx_0 * omega_0VV[1];
    r = _mm_loadu_pd(&d_A_0V[0]);
    r2 = _mm_loadu_pd(&c_A_0V[0]);
    r3 = _mm_loadu_pd(&g_V[0]);
    _mm_storeu_pd(&d_A_0V[0], _mm_sub_pd(r3, _mm_add_pd(r, r2)));
    d_A_0V[2] = g_V[2] - (d_A_0V[2] + c_A_0V[2]);
    c_A_0V[0] = omega_0VV[1] * r_VRRV_dot[2] - r_VRRV_dot[1] * omega_0VV[2];
    c_A_0V[1] = r_VRRV_dot[0] * omega_0VV[2] - omega_0VV[0] * r_VRRV_dot[2];
    c_A_0V[2] = omega_0VV[0] * r_VRRV_dot[1] - r_VRRV_dot[0] * omega_0VV[1];
    b_A_0V[0] = omega_0VV[1] * v_0RRV_idx_2 - v_0RRV_idx_1 * omega_0VV[2];
    b_A_0V[1] = v_0RRV_idx_0 * omega_0VV[2] - omega_0VV[0] * v_0RRV_idx_2;
    b_A_0V[2] = omega_0VV[0] * v_0RRV_idx_1 - v_0RRV_idx_0 * omega_0VV[1];
    r = _mm_loadu_pd(&c_A_0V[0]);
    r2 = _mm_loadu_pd(&b_A_0V[0]);
    r3 = _mm_loadu_pd(&g_V[0]);
    _mm_storeu_pd(&g_V[0], _mm_sub_pd(r3, _mm_add_pd(r, r2)));
    g_V[2] -= c_A_0V[2] + b_A_0V[2];
    d = d_A_0V[0];
    d1 = d_A_0V[1];
    d2 = d_A_0V[2];
    d3 = g_V[0];
    d4 = g_V[1];
    d5 = g_V[2];
    d6 = r_0V0[0];
    d7 = r_0V0[1];
    d8 = r_0V0[2];
    d9 = domdphi_RR[0];
    d10 = domdphi_RR[1];
    d11 = domdphi_RR[2];
    d12 = -(omega_0URLV[1] * dvdphi_FL[2] - dvdphi_FL[1] * omega_0URLV[2]);
    d13 = -(dvdphi_FL[0] * omega_0URLV[2] - omega_0URLV[0] * dvdphi_FL[2]);
    b_u = -(omega_0URLV[0] * dvdphi_FL[1] - dvdphi_FL[0] * omega_0URLV[1]);
    A_x_tmp_tmp = -(omega_0URRV[1] * domdphi_FL[2] - domdphi_FL[1] *
                    omega_0URRV[2]);
    b_A_x_tmp_tmp = -(domdphi_FL[0] * omega_0URRV[2] - omega_0URRV[0] *
                      domdphi_FL[2]);
    dz_RR = -(omega_0URRV[0] * domdphi_FL[1] - domdphi_FL[0] * omega_0URRV[1]);
    w_RR = I_WFLV[0];
    mu_RRR = I_WFLV[1];
    dz_RL = I_WFLV[2];
    w_RL = I_WFRV[0];
    mu_RRL = I_WFRV[1];
    uFR = I_WFRV[2];
    for (i = 0; i <= 12; i += 2) {
      r = _mm_loadu_pd(&f[i]);
      r2 = _mm_loadu_pd(&l_y_tmp[i]);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&b_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d));
      r3 = _mm_loadu_pd(&b_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d1));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&b_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d2));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&c_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d3));
      r3 = _mm_loadu_pd(&c_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d4));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&c_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d5));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&d_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d6));
      r3 = _mm_loadu_pd(&d_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d7));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&d_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d8));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&e_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d9));
      r3 = _mm_loadu_pd(&e_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d10));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&e_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d11));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&f_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d12));
      r3 = _mm_loadu_pd(&f_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(d13));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&f_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(b_u));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&g_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(A_x_tmp_tmp));
      r3 = _mm_loadu_pd(&g_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(b_A_x_tmp_tmp));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&g_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(dz_RR));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&h_y_tmp[i]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(w_RR));
      r3 = _mm_loadu_pd(&h_y_tmp[i + 14]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(mu_RRR));
      r2 = _mm_add_pd(r2, r3);
      r3 = _mm_loadu_pd(&h_y_tmp[i + 28]);
      r3 = _mm_mul_pd(r3, _mm_set1_pd(dz_RL));
      r2 = _mm_add_pd(r2, r3);
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&f[i], r);
      r = _mm_loadu_pd(&i_y_tmp[i]);
      r = _mm_mul_pd(r, _mm_set1_pd(w_RL));
      r2 = _mm_loadu_pd(&i_y_tmp[i + 14]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(mu_RRL));
      r = _mm_add_pd(r, r2);
      r2 = _mm_loadu_pd(&i_y_tmp[i + 28]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(uFR));
      r = _mm_add_pd(r, r2);
      _mm_storeu_pd(&l_y_tmp[i], r);
    }

    d_A_0V[0] = omega_0VV[1] * omega_0WRLV[2] - omega_0WRLV[1] * omega_0VV[2];
    d_A_0V[1] = omega_0WRLV[0] * omega_0VV[2] - omega_0VV[0] * omega_0WRLV[2];
    d_A_0V[2] = omega_0VV[0] * omega_0WRLV[1] - omega_0WRLV[0] * omega_0VV[1];
    c_A_0V[0] = omega_VURLV[1] * omega_0WRLV_tmp[2] - omega_0WRLV_tmp[1] *
      omega_VURLV[2];
    c_A_0V[1] = omega_0WRLV_tmp[0] * omega_VURLV[2] - omega_VURLV[0] *
      omega_0WRLV_tmp[2];
    c_A_0V[2] = omega_VURLV[0] * omega_0WRLV_tmp[1] - omega_0WRLV_tmp[0] *
      omega_VURLV[1];
    r = _mm_loadu_pd(&I_WRLV[0]);
    _mm_storeu_pd(&I_WRLV[0], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRLV[2]);
    _mm_storeu_pd(&I_WRLV[2], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRLV[4]);
    _mm_storeu_pd(&I_WRLV[4], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRLV[6]);
    _mm_storeu_pd(&I_WRLV[6], _mm_mul_pd(r, r1));
    I_WRLV[8] = -I_WRLV[8];
    r = _mm_loadu_pd(&d_A_0V[0]);
    r2 = _mm_loadu_pd(&c_A_0V[0]);
    _mm_storeu_pd(&d_A_0V[0], _mm_add_pd(r, r2));
    d_A_0V[2] += c_A_0V[2];
    b_r_0V0[0] = omega_0WRLV[1] * dvdphi_FR[2] - dvdphi_FR[1] * omega_0WRLV[2];
    b_r_0V0[1] = dvdphi_FR[0] * omega_0WRLV[2] - omega_0WRLV[0] * dvdphi_FR[2];
    b_r_0V0[2] = omega_0WRLV[0] * dvdphi_FR[1] - dvdphi_FR[0] * omega_0WRLV[1];
    d = d_A_0V[0];
    d1 = d_A_0V[1];
    d2 = d_A_0V[2];
    r = _mm_loadu_pd(&I_WRLV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r2 = _mm_loadu_pd(&I_WRLV[3]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&I_WRLV[6]);
    r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r2);
    r2 = _mm_loadu_pd(&b_r_0V0[0]);
    r = _mm_sub_pd(r, r2);
    _mm_storeu_pd(&b_A_0V[0], r);
    b_A_0V[2] = ((I_WRLV[2] * d + I_WRLV[5] * d1) + I_WRLV[8] * d2) - b_r_0V0[2];
    d_A_0V[0] = omega_0VV[1] * omega_0WRRV[2] - omega_0WRRV[1] * omega_0VV[2];
    d_A_0V[1] = omega_0WRRV[0] * omega_0VV[2] - omega_0VV[0] * omega_0WRRV[2];
    d_A_0V[2] = omega_0VV[0] * omega_0WRRV[1] - omega_0WRRV[0] * omega_0VV[1];
    c_A_0V[0] = omega_VURRV[1] * omega_0WRRV_tmp[2] - omega_0WRRV_tmp[1] *
      omega_VURRV[2];
    c_A_0V[1] = omega_0WRRV_tmp[0] * omega_VURRV[2] - omega_VURRV[0] *
      omega_0WRRV_tmp[2];
    c_A_0V[2] = omega_VURRV[0] * omega_0WRRV_tmp[1] - omega_0WRRV_tmp[0] *
      omega_VURRV[1];
    r = _mm_loadu_pd(&I_WRRV[0]);
    _mm_storeu_pd(&I_WRRV[0], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRRV[2]);
    _mm_storeu_pd(&I_WRRV[2], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRRV[4]);
    _mm_storeu_pd(&I_WRRV[4], _mm_mul_pd(r, r1));
    r = _mm_loadu_pd(&I_WRRV[6]);
    _mm_storeu_pd(&I_WRRV[6], _mm_mul_pd(r, r1));
    I_WRRV[8] = -I_WRRV[8];
    r = _mm_loadu_pd(&d_A_0V[0]);
    r1 = _mm_loadu_pd(&c_A_0V[0]);
    _mm_storeu_pd(&d_A_0V[0], _mm_add_pd(r, r1));
    d_A_0V[2] += c_A_0V[2];
    c_A_0V[0] = omega_0WRRV[1] * domdphi_FR[2] - domdphi_FR[1] * omega_0WRRV[2];
    c_A_0V[1] = domdphi_FR[0] * omega_0WRRV[2] - omega_0WRRV[0] * domdphi_FR[2];
    c_A_0V[2] = omega_0WRRV[0] * domdphi_FR[1] - domdphi_FR[0] * omega_0WRRV[1];
    d = d_A_0V[0];
    d1 = d_A_0V[1];
    d2 = d_A_0V[2];
    r = _mm_loadu_pd(&I_WRRV[0]);
    r = _mm_mul_pd(r, _mm_set1_pd(d));
    r1 = _mm_loadu_pd(&I_WRRV[3]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d1));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&I_WRRV[6]);
    r1 = _mm_mul_pd(r1, _mm_set1_pd(d2));
    r = _mm_add_pd(r, r1);
    r1 = _mm_loadu_pd(&c_A_0V[0]);
    r = _mm_sub_pd(r, r1);
    _mm_storeu_pd(&b_r_0V0[0], r);
    b_r_0V0[2] = ((I_WRRV[2] * d + I_WRRV[5] * d1) + I_WRRV[8] * d2) - c_A_0V[2];
    d = b_A_0V[0];
    d1 = b_A_0V[1];
    d2 = b_A_0V[2];
    d3 = b_r_0V0[0];
    d4 = b_r_0V0[1];
    d5 = b_r_0V0[2];
    for (i = 0; i <= 12; i += 2) {
      r = _mm_loadu_pd(&f[i]);
      r1 = _mm_loadu_pd(&l_y_tmp[i]);
      r = _mm_add_pd(r, r1);
      r1 = _mm_loadu_pd(&j_y_tmp[i]);
      r1 = _mm_mul_pd(r1, _mm_set1_pd(d));
      r2 = _mm_loadu_pd(&j_y_tmp[i + 14]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d1));
      r1 = _mm_add_pd(r1, r2);
      r2 = _mm_loadu_pd(&j_y_tmp[i + 28]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d2));
      r1 = _mm_add_pd(r1, r2);
      r = _mm_add_pd(r, r1);
      r1 = _mm_loadu_pd(&k_y_tmp[i]);
      r1 = _mm_mul_pd(r1, _mm_set1_pd(d3));
      r2 = _mm_loadu_pd(&k_y_tmp[i + 14]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d4));
      r1 = _mm_add_pd(r1, r2);
      r2 = _mm_loadu_pd(&k_y_tmp[i + 28]);
      r2 = _mm_mul_pd(r2, _mm_set1_pd(d5));
      r1 = _mm_add_pd(r1, r2);
      _mm_storeu_pd(&l_y_tmp[i], r1);
      r = _mm_add_pd(r, r1);
      _mm_storeu_pd(&b_f[i], r);
    }

    coder::mldivide(M, b_f);
    for (i = 0; i < 14; i++) {
      dvFLdz[i] = dv[i];
      dvFLdz[i + 14] = b_f[i];
    }

    dvFLdz[28] = x_TFL_dot[0];
    dvFLdz[31] = x_TFR_dot[0];
    dvFLdz[34] = x_TRL_dot[0];
    dvFLdz[37] = x_TRR_dot[0];
    dvFLdz[29] = x_TFL_dot[1];
    dvFLdz[32] = x_TFR_dot[1];
    dvFLdz[35] = x_TRL_dot[1];
    dvFLdz[38] = x_TRR_dot[1];
    dvFLdz[30] = x_TFL_dot[2];
    dvFLdz[33] = x_TFR_dot[2];
    dvFLdz[36] = x_TRL_dot[2];
    dvFLdz[39] = x_TRR_dot[2];
    dvFLdz[40] = 0.0;
    dvFLdz[41] = 0.0;

    //  State derivatives
    //  Additional output
    //  Inertial acceleration
    //  Combined aerodynamic forces
    //  Steering Torque
    //  --- Tire utilization (per wheel + global)
    if (F_xMFL >= 2.2204460492503131E-16) {
      d = F_xMFL;
    } else {
      d = 2.2204460492503131E-16;
    }

    if (F_yMFL >= 2.2204460492503131E-16) {
      d1 = F_yMFL;
    } else {
      d1 = 2.2204460492503131E-16;
    }

    b_u = coder::b_hypot(fabs(F_TFL[0]) / d, fabs(F_TFL[1]) / d1);
    if (F_xMFR >= 2.2204460492503131E-16) {
      d = F_xMFR;
    } else {
      d = 2.2204460492503131E-16;
    }

    if (F_yMFR >= 2.2204460492503131E-16) {
      d1 = F_yMFR;
    } else {
      d1 = 2.2204460492503131E-16;
    }

    uFR = coder::b_hypot(fabs(F_TFR[0]) / d, fabs(F_TFR[1]) / d1);
    if (F_xMRL >= 2.2204460492503131E-16) {
      d = F_xMRL;
    } else {
      d = 2.2204460492503131E-16;
    }

    if (F_yMRL >= 2.2204460492503131E-16) {
      d1 = F_yMRL;
    } else {
      d1 = 2.2204460492503131E-16;
    }

    uRL = coder::b_hypot(fabs(F_TRL[0]) / d, fabs(F_TRL[1]) / d1);
    if (F_xMRR >= 2.2204460492503131E-16) {
      d = F_xMRR;
    } else {
      d = 2.2204460492503131E-16;
    }

    if (F_yMRR >= 2.2204460492503131E-16) {
      d1 = F_yMRR;
    } else {
      d1 = 2.2204460492503131E-16;
    }

    uRR = coder::b_hypot(fabs(F_TRR[0]) / d, fabs(F_TRR[1]) / d1);

    //  Output vector
    A_x_tmp_tmp = 0.0;
    b_A_x_tmp_tmp = 0.0;
    dz_RR = 0.0;
    w_RR = 0.0;
    mu_RRR = 0.0;
    dz_RL = 0.0;
    w_RL = 0.0;
    mu_RRL = 0.0;
    v_0FAV_tmp = 0.0;
    b_domdu_FR = 0.0;
    for (i = 0; i < 3; i++) {
      A_z_tmp[3 * i] = M[14 * i];
      A_z_tmp[3 * i + 1] = M[14 * i + 1];
      A_z_tmp[3 * i + 2] = M[14 * i + 2];
      d = dvdu_FL[i];
      A_x_tmp_tmp += d * F_FL[i];
      d1 = dvdu_FR[i];
      b_A_x_tmp_tmp += d1 * F_FR[i];
      d2 = domdu_FL[i];
      dz_RR += d2 * (e_yFLV[i] + T_WFL[i]);
      d3 = domdu_FR[i];
      w_RR += d3 * (e_yFRV[i] + T_WFR[i]);
      mu_RRR += d * 11.6 * v_0FAV[i];
      dz_RL += d1 * 11.6 * r_MQFR0[i];
      w_RL += d2 * r_0V0[i];
      mu_RRL += d3 * domdphi_RR[i];
      v_0FAV_tmp += d2 * I_WFLV[i];
      b_domdu_FR += d3 * I_WFRV[i];
    }

    double uFL[4];
    coder::mldivide(A_z_tmp, &c_f[0], dvdphi_FL);
    uFL[0] = b_u;
    uFL[1] = uFR;
    uFL[2] = uRL;
    uFL[3] = uRR;
    out[0] = dvdphi_FL[0];
    out[3] = F_TFL[0];
    out[6] = F_TFR[0];
    out[9] = F_TRL[0];
    out[12] = F_TRR[0];
    out[1] = dvdphi_FL[1];
    out[4] = F_TFL[1];
    out[7] = F_TFR[1];
    out[10] = F_TRL[1];
    out[13] = F_TRR[1];
    out[2] = dvdphi_FL[2];
    out[5] = F_TFL[2];
    out[8] = F_TFR[2];
    out[11] = F_TRL[2];
    out[14] = F_TRR[2];
    out[15] = s_FL[0];
    out[19] = s_FR[0];
    out[23] = s_RL[0];
    out[27] = s_RR[0];
    out[16] = s_FL[1];
    out[20] = s_FR[1];
    out[24] = s_RL[1];
    out[28] = s_RR[1];
    out[17] = s_FL[2];
    out[21] = s_FR[2];
    out[25] = s_RL[2];
    out[29] = s_RR[2];
    out[18] = s_FL[3];
    out[22] = s_FR[3];
    out[26] = s_RL[3];
    out[30] = s_RR[3];
    out[31] = F_ycFL;
    out[32] = F_ycFR;
    out[33] = F_ycRL;
    out[34] = F_ycRR;
    out[35] = dF_x0FL;
    out[36] = dF_x0FR;
    out[37] = dF_x0RL;
    out[38] = dF_x0RR;
    out[39] = F_xMFL;
    out[40] = F_xMFR;
    out[41] = F_xMRL;
    out[42] = F_xMRR;
    out[43] = dF_y0FL;
    out[44] = dF_y0FR;
    out[45] = dF_y0RL;
    out[46] = dF_y0RR;
    out[47] = F_yMFL;
    out[48] = F_yMFR;
    out[49] = F_yMRL;
    out[50] = F_yMRR;
    out[51] = r_stFL;
    out[52] = r_stFR;
    out[53] = r_stRL;
    out[54] = r_stRR;
    out[55] = r_dynFL;
    out[56] = r_dynFR;
    out[57] = r_dynRL;
    out[58] = r_dynRR;
    out[59] = z_FL;
    out[60] = z_FR;
    out[61] = z_RL;
    out[62] = z_RR;
    out[63] = z_FL_dot;
    out[64] = z_FR_dot;
    out[65] = z_RL_dot;
    out[66] = z_RR_dot;
    out[67] = F_SFL;
    out[68] = F_SFR;
    out[69] = F_SRL;
    out[70] = F_SRR;
    out[71] = delta_FL;
    out[72] = delta_FR;
    out[73] = delta_RL;
    out[74] = delta_RR;
    out[75] = gamma_FL;
    out[76] = gamma_FR;
    out[77] = gamma_RL;
    out[78] = gamma_RR;
    out[79] = T_BFL;
    out[80] = T_BFR;
    out[81] = T_BRL;
    out[82] = T_BRR;
    out[83] = T_DFL;
    out[84] = T_DFR;
    out[85] = T_DRL;
    out[86] = T_DRR;
    out[87] = rtNaN;
    out[88] = rtNaN;
    out[89] = v_0RAV[0];
    out[90] = v_0RAV[1];
    out[91] = v_0RAV[2];
    out[92] = (((((((((A_x_tmp_tmp + b_A_x_tmp_tmp) + dz_RR) + w_RR) + mu_RRR) +
                   dz_RL) + w_RL) + mu_RRL) + v_0FAV_tmp) + b_domdu_FR) *
      0.014584380239693683;
    out[93] = b_u;
    out[94] = uFR;
    out[95] = uRL;
    out[96] = uRR;
    out[97] = coder::internal::maximum(uFL);
    out[98] = (b_u * F_TFL[2] + uFR * F_TFR[2]) / (F_TFL[2] + F_TFR[2]);
    out[99] = (uRL * F_TRL[2] + uRR * F_TRR[2]) / (F_TRL[2] + F_TRR[2]);
    memcpy(&out[100], &out_aero[0], 8U * sizeof(double));
    out[108] = u[0];
    out[109] = u[3];
    b_u = dt / 2.0;
    expl_temp.i_G = veh_i_G;
    expl_temp.T_MMax = v[7];
    expl_temp.C_D = car[0];
    expl_temp.C_LRA = car[2];
    expl_temp.C_LFA = car[1];
    expl_temp.F_SRR0 = v[5];
    expl_temp.F_SRL0 = v[5];
    expl_temp.F_SFR0 = v[4];
    expl_temp.F_SFL0 = v[4];
    expl_temp.c_RollRA = v[9];
    expl_temp.c_RollFA = v[8];
    expl_temp.l_RA = v[3];
    expl_temp.l_FA = v[2];
    expl_temp.z_cg = v[6];
    expl_temp.m_C = v[1];
    memcpy(&b_expl_temp.Z[0], &rz[0], 3721U * sizeof(double));
    for (i = 0; i <= 40; i += 2) {
      r = _mm_loadu_pd(&dvFLdz[i]);
      r1 = _mm_loadu_pd(&x[i]);
      _mm_storeu_pd(&domWFLdz[i], _mm_add_pd(r1, _mm_mul_pd(_mm_set1_pd(b_u), r)));
    }

    f_vehicleModel(u[0], u[1], u[2], u[5], u[6], u[7], u[8], u[9], domWFLdz,
                   expl_temp, b_expl_temp, dvFRdz);
    for (i = 0; i <= 40; i += 2) {
      r = _mm_loadu_pd(&dvFRdz[i]);
      r1 = _mm_loadu_pd(&x[i]);
      _mm_storeu_pd(&domWFLdz[i], _mm_add_pd(r1, _mm_mul_pd(_mm_set1_pd(b_u), r)));
    }

    f_vehicleModel(u[0], u[1], u[2], u[5], u[6], u[7], u[8], u[9], domWFLdz,
                   expl_temp, b_expl_temp, dvRLdz);
    for (i = 0; i <= 40; i += 2) {
      r = _mm_loadu_pd(&dvRLdz[i]);
      r1 = _mm_loadu_pd(&x[i]);
      _mm_storeu_pd(&domWFLdz[i], _mm_add_pd(r1, _mm_mul_pd(_mm_set1_pd(dt), r)));
    }

    f_vehicleModel(u[0], u[1], u[2], u[5], u[6], u[7], u[8], u[9], domWFLdz,
                   expl_temp, b_expl_temp, dvRRdz);
    r = _mm_set1_pd(2.0);
    for (i = 0; i <= 40; i += 2) {
      r1 = _mm_loadu_pd(&dvFRdz[i]);
      r2 = _mm_loadu_pd(&dvFLdz[i]);
      r3 = _mm_loadu_pd(&dvRLdz[i]);
      r4 = _mm_loadu_pd(&dvRRdz[i]);
      r5 = _mm_loadu_pd(&x[i]);
      _mm_storeu_pd(&x[i], _mm_add_pd(r5, _mm_div_pd(_mm_mul_pd(_mm_set1_pd(dt),
        _mm_add_pd(_mm_add_pd(_mm_add_pd(r2, _mm_mul_pd(r, r1)), _mm_mul_pd(r,
        r3)), r4)), _mm_set1_pd(6.0))));
    }
  }
}

// End of code generation (adtm_step.cpp)
