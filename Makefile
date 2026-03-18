# ============================================================
#  Makefile — Schwarzschild : chute vers l'horizon
#
#  Usage :
#    make          →  compile + simule + trace
#    make build    →  compilation seule
#    make run      →  lance la simulation  (génère trajectory.csv)
#    make plot     →  lance le script Python
#    make clean    →  supprime les binaires et le CSV
# ============================================================

CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
TARGET   = schwarzschild
SRCS     = main.cpp schwarzschild.cpp rk4.cpp
OBJS     = $(SRCS:.cpp=.o)

# ── Cible par défaut ─────────────────────────────────────────
all: build run plot

# ── Compilation ──────────────────────────────────────────────
build: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ── Dépendances des headers ───────────────────────────────────
main.o:          main.cpp params.h schwarzschild.h rk4.h
schwarzschild.o: schwarzschild.cpp schwarzschild.h params.h
rk4.o:           rk4.cpp rk4.h schwarzschild.h

# ── Simulation ───────────────────────────────────────────────
run: $(TARGET)
	./$(TARGET)

# ── Graphiques Python ─────────────────────────────────────────
plot:
	python3 plot.py

# ── Nettoyage ─────────────────────────────────────────────────
clean:
	rm -f $(OBJS) $(TARGET) trajectory.csv schwarzschild_plots.png

.PHONY: all build run plot clean
