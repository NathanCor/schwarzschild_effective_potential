# ============================================================
#  Makefile — Schwarzschild: fall toward the event horizon
#
#  Usage:
#    make          →  compile + simulate + plot
#    make build    →  compilation only
#    make run      →  run the simulation  (generates trajectory.csv)
#    make plot     →  run the Python script
#    make clean    →  remove binaries and the CSV
# ============================================================

CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
TARGET   = schwarzschild.exe
SRCS     = src/main.cpp src/schwarzschild.cpp src/rk4.cpp

# ── Default target ────────────────────────────────────────────
all: build run plot

# ── Compilation ───────────────────────────────────────────────
build:
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# ── Simulation ────────────────────────────────────────────────
run: $(TARGET)
	./$(TARGET)

# ── Python plots ──────────────────────────────────────────────
plot:
	python3 scripts/plot.py

# ── Cleanup ───────────────────────────────────────────────────
clean:
	rm -f $(OBJS) $(TARGET) trajectory.csv schwarzschild_plots.png

.PHONY: all build run plot clean