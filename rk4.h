#pragma once
#include "schwarzschild.h"   // State

// ============================================================
//  rk4.h — Intégrateur Runge-Kutta d'ordre 4
//
//  Schéma classique (erreur locale O(h⁵), globale O(h⁴)) :
//
//    k1 = h · f(yₙ)
//    k2 = h · f(yₙ + k1/2)
//    k3 = h · f(yₙ + k2/2)
//    k4 = h · f(yₙ + k3)
//
//    yₙ₊₁ = yₙ + k1/6 + k2/3 + k3/3 + k4/6
// ============================================================

// Effectue un pas de temps propre h à partir de l'état y.
State rk4_step(const State& y, double h);