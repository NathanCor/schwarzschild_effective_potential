// ============================================================
//  rk4.cpp — Intégrateur Runge-Kutta d'ordre 4
// ============================================================

#include "rk4.h"
#include "schwarzschild.h"   // derivatives()

// ── Opérations sur State ──────────────────────────────────────

static State add(const State& a, const State& b)
{
    return { a[0] + b[0], a[1] + b[1], a[2] + b[2] };
}

static State scale(const State& a, double s)
{
    return { a[0] * s, a[1] * s, a[2] * s };
}


// ============================================================
//  rk4_step(y, h)
//
//  Effectue un pas de durée h (temps propre) depuis l'état y.
//  Retourne le nouvel état yₙ₊₁.
// ============================================================
State rk4_step(const State& y, double h)
{
    const State k1 = scale(derivatives(y), h);
    const State k2 = scale(derivatives(add(y, scale(k1, 0.5))), h);
    const State k3 = scale(derivatives(add(y, scale(k2, 0.5))), h);
    const State k4 = scale(derivatives(add(y, k3)), h);

    State y_new;
    for (int i = 0; i < 3; ++i)
        y_new[i] = y[i] + k1[i] / 6.0 + k2[i] / 3.0 + k3[i] / 3.0 + k4[i] / 6.0;

    return y_new;
}