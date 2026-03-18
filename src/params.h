#pragma once
// ============================================================
//  params.h — Physical and numerical parameters
//
//  Natural units: G = c = 1
//  Equatorial plane: θ = π/2  →  sin²θ = 1, dθ = 0
//
//  Schwarzschild metric:
//    ds² = -(1 - 2M/r) dt²
//        + (1 - 2M/r)⁻¹ dr²
//        + r²(dθ² + sin²θ dφ²)
// ============================================================

constexpr double M_PI = 3.14159265358979323846;

// ── Black hole mass ───────────────────────────────────────────
constexpr double M = 1.0;
constexpr double RS = 2.0 * M;   // Schwarzschild radius rs = 2M

// ── Constants of motion ───────────────────────────────────────
//    These two quantities are conserved along the geodesic.
//    E < 1  → bound particle (orbit or fall from rest)
//    L      → specific angular momentum (L² < 12M² → inevitable fall)

constexpr double L = 3.46;     // ≈ √12 M  (close to the ISCO)
constexpr double E = 0.96;     // sub-relativistic specific energy

// ── Initial conditions ────────────────────────────────────────
constexpr double R0 = 9.0;    // initial radius  (must be > rs = 2M)
constexpr double PHI0 = 0.0;    // initial angle  (rad)

// ── RK4 integrator ────────────────────────────────────────────
constexpr double DTAU = 0.02;    // proper time step
constexpr int    N_STEPS = 200000;  // maximum number of steps

// ── Output file ───────────────────────────────────────────────
constexpr const char* CSV_FILE = "trajectory.csv";

// ── Stopping criterion ────────────────────────────────────────
// Simulation stops when r < STOP_FACTOR * rs
// to avoid the numerical singularity at the horizon.
constexpr double STOP_FACTOR = 1.05;