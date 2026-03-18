// ============================================================
//  schwarzschild.cpp — Effective potential & equations of motion
// ============================================================

#include "schwarzschild.h"
#include "params.h"
#include <cmath>
#include <iostream>

// ============================================================
//  Veff(r, L)
//
//  Effective potential for a massive particle — energy form.
//  Derived from the normalization condition g_μν u^μ u^ν = -1
//  after substituting E and L (Killing constants of motion):
//
//      V²eff(r) = (1 - 2M/r)(1 + L²/r²)
//      Veff(r)  = √(V²eff)             (positive branch)
//
//  Valid domain: r > rs = 2M  (outside the horizon)
// ============================================================
double Veff(double r, double L_)
{
    if (r <= RS) return 0.0;

    //   Split into two factors:
    //     f1 = (1 - 2M/r)         ← metric factor
    //     f2 = (1 + L²/r²)        ← centrifugal + rest-mass factor
    //
    //   std::sqrt(x), and M and RS are defined in params.h.

    const double f1 = 1.0 - 2.0 * M / r;
    const double f2 = 1.0 + L_ * L_ / (r * r);
    return std::sqrt(f1 * f2);
}


// ============================================================
//  dVeff_dr(r, L)
//
//  Analytical derivative of Veff with respect to r.
//  We differentiate V²eff = f1(r)·f2(r) using the product rule:
//
//      d(V²eff)/dr = f1'·f2 + f1·f2'
//
//      f1  = (1 - 2M/r)      f1' = +2M/r²
//      f2  = (1 + L²/r²)     f2' = -2L²/r³
//
//  Then:  dVeff/dr = d(V²eff)/dr / (2·Veff)
// ============================================================
double dVeff_dr(double r, double L_)
{
    if (r <= RS) return 0.0;
    const double v = Veff(r, L_);
    if (v < 1e-14) return 0.0;

    const double f1 = 1.0 - 2.0 * M / r;
    const double f2 = 1.0 + L_ * L_ / (r * r);
    const double df1_dr = 2*M/(r*r);        // derivative of f1
    const double df2_dr = -2*L*L/(r*r*r);   // derivative of f2
    const double dV2_dr = df1_dr * f2 + f1 * df2_dr;   // product rule
    return dV2_dr / (2.0 * v);
}


// ============================================================
//  compute_rdot0
//
//  Initializes the radial velocity from the constants of motion.
//
//  Radial equation: (dr/dτ)² = E² - V²eff(r0)
//  ⟹ ṙ₀ = sign · √(E² - V²eff(r0))
//
//  If E² < V²eff(r0), the particle is in a classically forbidden
//  region: a warning is printed and 0 is returned.
// ============================================================
double compute_rdot0(double r0, double sign)
{
    const double v = Veff(r0, L);
    const double disc = E * E - v * v;

    if (disc < 0.0) {
        std::cerr << "[WARNING] r0=" << r0
            << " is in a classically forbidden region"
            << "  (E²-V²eff = " << disc << ")\n";
        return 0.0;
    }

    return sign * sqrt(disc);
}


// ============================================================
//  derivatives(y)
//
//  Computes dy/dτ for the state vector y = {r, ṙ, φ}.
//
//  Equations of motion (geodesics, equatorial plane):
//
//    dy[0]/dτ = ṙ                           (trivial)
//    dy[1]/dτ = -M/r² + L²(r - 3M)/r⁴      (effective force)
//    dy[2]/dτ = L / r²                      (conservation of L)
//
//  The term L²(r-3M)/r⁴ is the derivative of -V²eff/2 with
//  respect to r, corresponding to the force felt by the particle.
// ============================================================
State derivatives(const State& y)
{
    const double r = y[0];
    const double rdt = y[1];   // ṙ = dr/dτ

    State dy;

    // dy[0]: velocity = derivative of position
    dy[0] = rdt;

    //   Effective radial acceleration.
    //   Formula: -M/r² + L²*(r - 3M)/r⁴

    const double r2 = r * r;
    const double r4 = r2 * r2;

    dy[1] = -M/r2 + L*L*(r-3*M)/r4;

    // Angular velocity.
    //   Formula: L / r²
    dy[2] = L/r2;

    return dy;
}