#include <CLI/CLI.hpp>
#include <Problem.h>
#include <Solver.h>
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

  std::vector<double> r0_lorenz = {1.0, 0.0, 0.0};
  std::vector<double> r0_lotkaVolterra = {30, 15};
  vec r0Lorenz(r0_lorenz);
  vec r0LotkaVolerra(r0_lotkaVolterra);

  double t0 = 0;

  LorenzAttractor lorenzProblem(r0Lorenz, t0);
  LotkaVolterra lotkaVolterraProblem(r0LotkaVolerra, t0);



  RungeKutta4<LorenzAttractor, double> rk4LorenzSolver(lorenzProblem, stepSize, maxIterations);
  ExplicitEuler<LorenzAttractor, double> eeLorenzSolver(lorenzProblem, stepSize, maxIterations);

  RungeKutta4<LotkaVolterra, double> rk4LotkaVolterraSolver(lotkaVolterraProblem, stepSize, maxIterations);
  ExplicitEuler<LotkaVolterra, double> eeLotkaVolterraSolver(lotkaVolterraProblem, stepSize, maxIterations);

  rk4LorenzSolver.setPrint(true);
  // ee_solver.setPrint(true);
  rk4LotkaVolterraSolver.setPrint(true);

  rk4LorenzSolver.solve();
  // ee_solver.solve();
  rk4LotkaVolterraSolver.solve();

  rk4LorenzSolver.write("/home/max/Documents/Code/bin/Lorenz_RK4.json");
  rk4LotkaVolterraSolver.write("/home/max/Documents/Code/bin/LotkaVolterra_RK4.json");

  return 0;
};