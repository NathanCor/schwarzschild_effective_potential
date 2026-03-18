#pragma once
// ============================================================
//  params.h — Paramètres physiques et numériques
//
//  Unités naturelles : G = c = 1
//  Plan équatorial   : θ = π/2  →  sin²θ = 1, dθ = 0
//
//  Métrique de Schwarzschild :
//    ds² = -(1 - 2M/r) dt²
//        + (1 - 2M/r)⁻¹ dr²
//        + r²(dθ² + sin²θ dφ²)
// ============================================================

constexpr double M_PI = 3.14159265358979323846;

// ── Masse du trou noir ────────────────────────────────────────
constexpr double M = 1.0;
constexpr double RS = 2.0 * M;   // rayon de Schwarzschild rs = 2M

// ── Constantes du mouvement ───────────────────────────────────
//    Ces deux quantités sont conservées le long de la géodésique.
//    E < 1  → particule liée (orbite ou chute depuis le repos)
//    L      → moment cinétique spécifique (L² < 12M² → chute inévitable)

constexpr double L = 3.46;     // ≈ √12 M  (proche de l'ISCO)
constexpr double E = 0.96;     // énergie spécifique sub-relativiste

// ── Conditions initiales ──────────────────────────────────────
constexpr double R0 = 9.0;    // rayon initial  (doit être > rs = 2M)
constexpr double PHI0 = 0.0;    // angle initial  (rad)

// ── Intégrateur RK4 ──────────────────────────────────────────
constexpr double DTAU = 0.02;    // pas de temps propre
constexpr int    N_STEPS = 200000;  // nombre de pas maximum

// ── Fichier de sortie ─────────────────────────────────────────
constexpr const char* CSV_FILE = "trajectory.csv";

// ── Critère d'arrêt ───────────────────────────────────────────
// On arrête la simulation quand r < STOP_FACTOR * rs
// pour éviter la singularité numérique à l'horizon.
constexpr double STOP_FACTOR = 1.05;