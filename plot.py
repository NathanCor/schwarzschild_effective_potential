"""
plot.py — Visualisation de la chute vers l'horizon de Schwarzschild
===================================================================

Lit trajectory.csv généré par le programme C++ et produit 4 graphiques :

  Fig 1 — Orbite dans le plan (x, y) en coordonnées cartésiennes
  Fig 2 — Évolution radiale r(τ)  avec position de l'horizon
  Fig 3 — Potentiel effectif Veff(r) + niveau d'énergie E
  Fig 4 — Vitesse radiale ṙ(τ)

Dépendances : pandas, matplotlib, numpy
  pip install pandas matplotlib numpy
"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.patches as patches
from matplotlib.collections import LineCollection

CSV_FILE = "trajectory.csv"

# ── Paramètres physiques (doivent correspondre à params.h) ────
M  = 1.0
RS = 2.0 * M   # rayon de Schwarzschild
L  = 3.46
E  = 0.96


# ════════════════════════════════════════════════════════════════
#  Lecture du CSV
# ════════════════════════════════════════════════════════════════

#   Charger trajectory.csv dans un DataFrame pandas.
#   La première ligne est l'en-tête (colonnes : tau,r,phi,x,y_cart,Veff,rdot).

df = pd.read_csv(CSV_FILE, sep=',', skipinitialspace=True)
df.columns = df.columns.str.strip()
print(df.columns.tolist())


# Vérification rapide
print(f"  {len(df)} points chargés")
print(df.head(3))


# ════════════════════════════════════════════════════════════════
#  Mise en page générale
# ════════════════════════════════════════════════════════════════

fig, axes = plt.subplots(2, 2, figsize=(13, 10))
fig.suptitle(
    f"Chute vers l'horizon de Schwarzschild\n"
    f"M={M}  L={L}  E={E}  $r_s$={RS}",
    fontsize=13
)
ax_orbit, ax_r, ax_veff, ax_rdot = axes.flatten()


# ════════════════════════════════════════════════════════════════
#  Fig 1 — Orbite (x, y)
# ════════════════════════════════════════════════════════════════

ax = ax_orbit

#  Tracer la trajectoire (x, y_cart) avec une couleur
#   qui évolue le long de la trajectoire pour indiquer la progression.


sc = ax.scatter(df["x"], df["y_cart"], c=df["tau"], cmap="plasma", s=0.5)
fig.colorbar(sc, ax=ax, label="τ (temps propre)")


# Horizon de Schwarzschild (cercle de rayon rs)
# Dessiner un disque noir de rayon RS centré en (0, 0).


horizon = patches.Circle((0, 0), RS, color='black', zorder=5)
ax.add_patch(horizon)


ax.set_aspect("equal")
ax.set_xlabel("x  [M]")
ax.set_ylabel("y  [M]")
ax.set_title("Orbite dans le plan équatorial")
ax.grid(True, alpha=0.3)


# ════════════════════════════════════════════════════════════════
#  Fig 2 — Évolution radiale r(τ)
# ════════════════════════════════════════════════════════════════

ax = ax_r

#   Tracer r en fonction de tau.
#   Ajouter une ligne horizontale en pointillés rouges à r = RS
#   (horizon de Schwarzschild).



ax.plot(df["tau"], df["r"], color="steelblue", lw=1)
ax.axhline(RS, color="red", linestyle="--", label="horizon $r_s$")
ax.annotate("horizon $r_s$", xy=(df["tau"].iloc[0], RS),
            xytext=(0, 6), textcoords="offset points", color="red")
ax.legend()


ax.set_xlabel("τ  (temps propre)")
ax.set_ylabel("r  [M]")
ax.set_title("Évolution radiale")
ax.grid(True, alpha=0.3)


# ════════════════════════════════════════════════════════════════
#  Fig 3 — Potentiel effectif Veff(r)
# ════════════════════════════════════════════════════════════════
#
#  On recalcule Veff(r) analytiquement sur une grille fine,
#  indépendamment de la simulation.
#
#  V²eff(r) = (1 - 2M/r)(1 + L²/r²)

ax = ax_veff

r_grid = np.linspace(RS + 0.1, 30.0, 800)

#   Calculer Veff sur r_grid

veff_grid = np.sqrt((1 - 2*M/r_grid) * (1 + L**2/r_grid**2))

#   Tracer Veff(r) et ajouter :
#   - une ligne horizontale E = cste  (niveau d'énergie de la particule)
#   - une zone grisée pour r < RS (intérieur de l'horizon)
#   - les régions "interdites" (E < Veff) en hachuré léger


ax.plot(r_grid, veff_grid, color="darkorange", lw=2, label="$V_{eff}(r)$")
ax.axhline(E, color="steelblue", linestyle="--", label=f"E = {E}")
ax.axvspan(0, RS, color="black", alpha=0.3, label="horizon")
ax.fill_between(r_grid, veff_grid, E, where=(veff_grid > E),
                alpha=0.15, color="red", label="zone interdite")


ax.set_xlim(0, 25)
ax.set_xlabel("r  [M]")
ax.set_ylabel("$V_{\\mathrm{eff}}(r)$")
ax.set_title("Potentiel effectif")
ax.legend()
ax.grid(True, alpha=0.3)


# ════════════════════════════════════════════════════════════════
#  Fig 4 — Vitesse radiale ṙ(τ)
# ════════════════════════════════════════════════════════════════

ax = ax_rdot

# Tracer rdot (= dr/dτ) en fonction de tau.
#   Ajouter une ligne horizontale à 0 pour repérer les points de retour.


ax.plot(df["tau"], df["rdot"], color="mediumpurple", lw=1)
ax.axhline(0, color="gray", linestyle="--", lw=0.8)


ax.set_xlabel("τ  (temps propre)")
ax.set_ylabel("dr/dτ")
ax.set_title("Vitesse radiale")
ax.grid(True, alpha=0.3)


# ════════════════════════════════════════════════════════════════
#  Affichage
# ════════════════════════════════════════════════════════════════

plt.tight_layout()
plt.savefig("schwarzschild_plots.png", dpi=150)
print("\n  → schwarzschild_plots.png sauvegardé")
plt.show()