#pragma once
#include <array>

// ============================================================
//  schwarzschild.h — Interface physique
//
//  Potentiel effectif et équations du mouvement pour une
//  particule massive dans la métrique de Schwarzschild,
//  restreinte au plan équatorial (θ = π/2).
// ============================================================

// Vecteur d'état : y = { r,  dr/dτ,  φ }
using State = std::array<double, 3>;

// ── Potentiel effectif ────────────────────────────────────────
//   V²eff(r) = (1 - 2M/r)(1 + L²/r²)
double Veff(double r, double L_);

// ── Dérivée du potentiel effectif ────────────────────────────
//   dVeff/dr  (utile pour repérer les orbites circulaires)
double dVeff_dr(double r, double L_);

// ── Condition initiale radiale ────────────────────────────────
//   ṙ₀ = ±√(E² - V²eff(r0))
//   sign = -1.0  →  particule se rapproche du trou noir
double compute_rdot0(double r0, double sign = -1.0);

// ── Dérivées du vecteur d'état ────────────────────────────────
//   Retourne { dr/dτ, d²r/dτ², dφ/dτ }
State derivatives(const State& y);