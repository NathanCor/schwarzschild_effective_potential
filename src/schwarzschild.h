#pragma once
#include <array>

// ============================================================
//  schwarzschild.h — Physical interface
//
//  Effective potential and equations of motion for a massive
//  particle in the Schwarzschild metric, restricted to the
//  equatorial plane (θ = π/2).
// ============================================================

// State vector: y = { r,  dr/dτ,  φ }
using State = std::array<double, 3>;

// ── Effective potential ───────────────────────────────────────
//   V²eff(r) = (1 - 2M/r)(1 + L²/r²)
double Veff(double r, double L_);

// ── Derivative of the effective potential ─────────────────────
//   dVeff/dr  (useful for locating circular orbits)
double dVeff_dr(double r, double L_);

// ── Initial radial condition ──────────────────────────────────
//   ṙ₀ = ±√(E² - V²eff(r0))
//   sign = -1.0  →  particle falling toward the black hole
double compute_rdot0(double r0, double sign = -1.0);

// ── State vector derivatives ──────────────────────────────────
//   Returns { dr/dτ, d²r/dτ², dφ/dτ }
State derivatives(const State& y);