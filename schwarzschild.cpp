// ============================================================
//  schwarzschild.cpp — Potentiel effectif & équations du mouvement
// ============================================================

#include "schwarzschild.h"
#include "params.h"
#include <cmath>
#include <iostream>

// ============================================================
//  Veff(r, L)
//
//  Potentiel effectif d'une particule massive — forme énergie.
//  Découle de la condition de normalisation g_μν u^μ u^ν = -1
//  après substitution de E et L (constantes de Killing) :
//
//      V²eff(r) = (1 - 2M/r)(1 + L²/r²)
//      Veff(r)  = √(V²eff)             (branche positive)
//
//  Domaine valide : r > rs = 2M  (extérieur de l'horizon)
// ============================================================
double Veff(double r, double L_)
{
    if (r <= RS) return 0.0;

    //   Décomposez en deux facteurs :
    //     f1 = (1 - 2M/r)         ← facteur métrique
    //     f2 = (1 + L²/r²)        ← facteur centrifuge + repos
    //
    //   std::sqrt(x), et M et RS sont dans params.h.

    const double f1 = 1 - 2*M/r;
    const double f2 = 1 + L*L/(r*r);
    return std::sqrt(f1 * f2);
}


// ============================================================
//  dVeff_dr(r, L)
//
//  Dérivée analytique de Veff par rapport à r.
//  On dérive V²eff = f1(r)·f2(r) par la règle du produit :
//
//      d(V²eff)/dr = f1'·f2 + f1·f2'
//
//      f1  = (1 - 2M/r)      f1' = +2M/r²
//      f2  = (1 + L²/r²)     f2' = -2L²/r³
//
//  Puis :  dVeff/dr = d(V²eff)/dr / (2·Veff)
// ============================================================
double dVeff_dr(double r, double L_)
{
    if (r <= RS) return 0.0;
    const double v = Veff(r, L_);
    if (v < 1e-14) return 0.0;


    const double f1 = 1.0 - 2.0 * M / r;
    const double f2 = 1.0 + L_ * L_ / (r * r);
    const double df1_dr = 2*M/(r*r);   // dérivée de f1
    const double df2_dr = -2*L*L/(r*r*r);   // dérivée de f2
    const double dV2_dr = df1_dr * f2 + f1 * df2_dr;   // règle du produit
    return dV2_dr / (2.0 * v);
}


// ============================================================
//  compute_rdot0
//
//  Initialise la vitesse radiale depuis les constantes du mouvement.
//
//  Équation radiale : (dr/dτ)² = E² - V²eff(r0)
//  ⟹ ṙ₀ = sign · √(E² - V²eff(r0))
//
//  Si E² < V²eff(r0), la particule est dans une zone interdite :
//  on affiche un avertissement et on retourne 0.
// ============================================================
double compute_rdot0(double r0, double sign)
{
    const double v = Veff(r0, L);
    const double disc = E * E - v * v;

    if (disc < 0.0) {
        std::cerr << "[AVERTISSEMENT] r0=" << r0
            << " est dans une region classiquement interdite"
            << "  (E²-V²eff = " << disc << ")\n";
        return 0.0;
    }

    return sign * sqrt(disc);
}


// ============================================================
//  derivatives(y)
//
//  Calcule dy/dτ pour le vecteur d'état y = {r, ṙ, φ}.
//
//  Équations du mouvement (géodésiques, plan équatorial) :
//
//    dy[0]/dτ = ṙ                           (trivial)
//    dy[1]/dτ = -M/r² + L²(r - 3M)/r⁴      (force effective)
//    dy[2]/dτ = L / r²                      (conservation L)
//
//  Le terme L²(r-3M)/r⁴ est la dérivée de -V²eff/2 par rapport
//  à r, ce qui correspond à la force ressentie par la particule.
// ============================================================
State derivatives(const State& y)
{
    const double r = y[0];
    const double rdt = y[1];   // ṙ = dr/dτ

    State dy;

    // dy[0] : vitesse = dérivée de la position
    dy[0] = rdt;

    //   Accélération radiale effective.
    //   Formule : -M/r² + L²*(r - 3M)/r⁴

    const double r2 = r * r;
    const double r4 = r2 * r2;

    dy[1] = -M/r2 + L*L*(r-3*M)/r4;

    // Vitesse angulaire.
    //   Formule : L / r²
    dy[2] = L/r2;

    return dy;
}