// ============================================================
//  main.cpp — Simulation de la chute vers l'horizon
//
//  Pipeline :
//    1. Initialisation des conditions initiales
//    2. Boucle RK4 jusqu'à absorption ou N_STEPS atteint
//    3. Export CSV  →  trajectory.csv
//
//  Format CSV :
//    tau, r, phi, x, y_cart, Veff, rdot
//
//    tau    : temps propre
//    r      : coordonnée radiale (unités M)
//    phi    : angle azimutal (rad)
//    x, y   : coordonnées cartésiennes  x=r·cos φ,  y=r·sin φ
//    Veff   : potentiel effectif au point courant
//    rdot   : vitesse radiale dr/dτ
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
    std::cout << "=== Schwarzschild — chute vers l'horizon ===\n"
        << "  M  = " << M << "  (G=c=1)\n"
        << "  rs = " << RS << "\n"
        << "  L  = " << L << "\n"
        << "  E  = " << E << "\n"
        << "  r0 = " << R0 << "  M\n"
        << "  dτ = " << DTAU << "\n"
        << "  N  = " << N_STEPS << " pas max\n\n";

    // ── Condition initiale ────────────────────────────────────
    // ṙ₀ = -√(E² - Veff²(r0))  (signe − : particule qui plonge)
    const double rdot0 = compute_rdot0(R0, -1.0);

    std::cout << "  Veff(r0) = " << Veff(R0, L) << "\n"
        << "  ṙ0       = " << rdot0 << "\n\n";

    State y = { R0, rdot0, PHI0 };

    // ── Ouverture du CSV ──────────────────────────────────────
    std::ofstream csv(CSV_FILE);
    if (!csv.is_open()) {
        std::cerr << "[ERREUR] Impossible d'ouvrir " << CSV_FILE << "\n";
        return 1;
    }

    csv << std::fixed << std::setprecision(8);
    csv << "tau,r,phi,x,y_cart,Veff,rdot\n";

    // ── Boucle d'intégration ──────────────────────────────────
    double tau = 0.0;
    int    n_actual = 0;

    for (int step = 0; step < N_STEPS; ++step)
    {
        const double r = y[0];
        const double phi = y[2];

        // Condition d'arrêt : absorption par le trou noir.
        //   Si r < STOP_FACTOR * RS, afficher un message et sortir.
        if (r < STOP_FACTOR * RS)
        {
            std::cout << "  → Particule absorbee a tau = " << tau
                << ",  r = " << r << " (< "
                << STOP_FACTOR * RS << " = STOP_FACTOR * rs)\n";
            break;
        }

        //   x      = r · cos(phi)
        //   y_cart = r · sin(phi)
        //   (std::cos et std::sin attendent des radians)
        const double x = r * cos(phi);
        const double y_cart = r * sin(phi);

        //   Écrire une ligne dans le CSV.
        //   Colonnes : tau, r, phi, x, y_cart, Veff(r,L), y[1]
        //   Séparateur : virgule.  Terminer par '\n'.
        csv << tau << ","
            << r << ","
            << phi << ","
            << x << ","
            << y_cart << ","
            << Veff(r, L) << ","
            << y[1] << "\n";

        // ── Avancer d'un pas RK4 ─────────────────────────────
        y = rk4_step(y, DTAU);
        tau += DTAU;
        ++n_actual;
    }

    csv.close();

    // ── Récapitulatif ─────────────────────────────────────────
    std::cout << "\n[OK]  " << n_actual << " pas enregistres dans "
        << CSV_FILE << "\n"
        << "  r final   = " << y[0] << "  M\n"
        << "  phi final = " << y[2] << "  rad  ("
        << y[2] / (2.0 * M_PI) << " tours)\n";

    return 0;
}