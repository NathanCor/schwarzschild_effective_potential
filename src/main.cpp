// ============================================================
//  main.cpp — Simulation of the fall toward the event horizon
//
//  Pipeline:
//    1. Initialize initial conditions
//    2. RK4 loop until absorption or N_STEPS reached
//    3. CSV export  →  trajectory.csv
//
//  CSV format:
//    tau, r, phi, x, y_cart, Veff, rdot
//
//    tau    : proper time
//    r      : radial coordinate (units of M)
//    phi    : azimuthal angle (rad)
//    x, y   : Cartesian coordinates  x=r·cos φ,  y=r·sin φ
//    Veff   : effective potential at current point
//    rdot   : radial velocity dr/dτ
// ============================================================

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

#include "params.h"
#include "schwarzschild.h"
#include "rk4.h"

int main()
{
    std::cout << "=== Schwarzschild — fall toward the event horizon ===\n"
        << "  M  = " << M << "  (G=c=1)\n"
        << "  rs = " << RS << "\n"
        << "  L  = " << L << "\n"
        << "  E  = " << E << "\n"
        << "  r0 = " << R0 << "  M\n"
        << "  dτ = " << DTAU << "\n"
        << "  N  = " << N_STEPS << " max steps\n\n";

    // ── Initial condition ─────────────────────────────────────
    // ṙ₀ = -√(E² - Veff²(r0))  (minus sign: particle falling inward)
    const double rdot0 = compute_rdot0(R0, -1.0);

    std::cout << "  Veff(r0) = " << Veff(R0, L) << "\n"
        << "  ṙ0       = " << rdot0 << "\n\n";

    State y = { R0, rdot0, PHI0 };

    // ── Open CSV file ─────────────────────────────────────────
    std::ofstream csv(CSV_FILE);
    if (!csv.is_open()) {
        std::cerr << "[ERROR] Cannot open " << CSV_FILE << "\n";
        return 1;
    }

    csv << std::fixed << std::setprecision(8);
    csv << "tau,r,phi,x,y_cart,Veff,rdot\n";

    // ── Integration loop ──────────────────────────────────────
    double tau = 0.0;
    int    n_actual = 0;

    for (int step = 0; step < N_STEPS; ++step)
    {
        const double r = y[0];
        const double phi = y[2];

        // Stopping condition: absorption by the black hole.
        //   If r < STOP_FACTOR * RS, print a message and exit.
        if (r < STOP_FACTOR * RS)
        {
            std::cout << "  → Particle absorbed at tau = " << tau
                << ",  r = " << r << " (< "
                << STOP_FACTOR * RS << " = STOP_FACTOR * rs)\n";
            break;
        }

        //   x      = r · cos(phi)
        //   y_cart = r · sin(phi)
        //   (std::cos and std::sin expect radians)
        const double x = r * cos(phi);
        const double y_cart = r * sin(phi);

        //   Write one line to the CSV.
        //   Columns: tau, r, phi, x, y_cart, Veff(r,L), y[1]
        //   Separator: comma. End with '\n'.
        csv << tau << ","
            << r << ","
            << phi << ","
            << x << ","
            << y_cart << ","
            << Veff(r, L) << ","
            << y[1] << "\n";

        // ── Advance one RK4 step ──────────────────────────────
        y = rk4_step(y, DTAU);
        tau += DTAU;
        ++n_actual;
    }

    csv.close();

    // ── Summary ───────────────────────────────────────────────
    std::cout << "\n[OK]  " << n_actual << " steps written to "
        << CSV_FILE << "\n"
        << "  final r   = " << y[0] << "  M\n"
        << "  final phi = " << y[2] << "  rad  ("
        << y[2] / (2.0 * M_PI) << " orbits)\n";

    return 0;
}