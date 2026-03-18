#pragma once
#include "schwarzschild.h"   // State

// ============================================================
//  rk4.h — Fourth-order Runge-Kutta integrator
//
//  Classic scheme (local error O(h⁵), global error O(h⁴)):
//
//    k1 = h · f(yₙ)
//    k2 = h · f(yₙ + k1/2)
//    k3 = h · f(yₙ + k2/2)
//    k4 = h · f(yₙ + k3)
//
//    yₙ₊₁ = yₙ + k1/6 + k2/3 + k3/3 + k4/6
// ============================================================

// Advances one proper time step h from state y.
State rk4_step(const State& y, double h);