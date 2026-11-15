//
// Sponsored License - for use in support of a program or activity
// sponsored by MathWorks.  Not for government, commercial or other
// non-sponsored organizational use.
//
// adtm_step.h
//
// Code generation for function 'adtm_step'
//

#ifndef ADTM_STEP_H
#define ADTM_STEP_H

// Include files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace adtm {
extern void adtm_constants(double c[9]);

extern void adtm_init(const double car[9], double x[42]);

extern void adtm_step(double x[42], const double u[6], const double car[9],
                      double t, double dt, double out[110]);

extern void adtm_step_initialize();

extern void adtm_step_terminate();

extern void adtm_step_torque(double x[42], const double u[10],
                             const double car[9], double t, double dt,
                             double out[110]);

} // namespace adtm

#endif
// End of code generation (adtm_step.h)
