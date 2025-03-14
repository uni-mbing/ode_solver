#include <CLI/CLI.hpp>
#include <Problem.h>
#include <Solver.h>
#include <Saver.h>
#include <iostream>

int main(int argc, char **argv) {
  CLI::App app{"ODE Solver App"};

  // Definiere eine optionale Eingabe für eine Zahl
  std::string solverType = "RK4";
  app.add_option("-s,--solver", solverType,
                 "Input solver type: RUNGE_KUTTA_4, LEAP_FROG, EE");

  // Definiere eine optionale Eingabe für einen Namen
  std::string problemType;
  app.add_option("-p,--problem", problemType,
                 "Input problem type: LINEAR, N_BODY");

  int maxIterations = 1000;
  app.add_option("-m,--maxIterations", maxIterations);

  double stepSize = 0.001;
  app.add_option("-t,--stepSize", stepSize);

  // Parse die Kommandozeilenargumente
  CLI11_PARSE(app, argc, argv);

  std::vector<double> r0_ = {1.0, 0.0, 0.0};
  vec r0(r0_);

  double t0 = 0;

  LorenzAttractor problem(r0, t0);


  RungeKutta4<LorenzAttractor, double> rk4_solver(problem, stepSize, maxIterations);
  ExplicitEuler<LorenzAttractor, double> ee_solver(problem, stepSize, maxIterations);

  // spdlog::info("Starting to solve");

  rk4_solver.setPrint(true);
  // ee_solver.setPrint(true);

  rk4_solver.solve();
  // ee_solver.solve();

  rk4_solver.write("/home/max/Documents/Code/bin/Lorenz_RK4.json");
  // ee_solver.write("/home/max/Documents/Code/bin/Lorenz_EE.json");

  return 0;
};