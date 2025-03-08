#include <CLI/CLI.hpp>
#include <Problem.h>
#include <Solver.h>
#include <iostream>

typedef Vector<double> vec;

enum SOLVER_TYPE { RUNGE_KUTTA_4, LEAP_FROG, EE };

enum SIMULATION_TYPE { LINEAR_ODE, N_BODY };

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

  // Parse die Kommandozeilenargumente
  CLI11_PARSE(app, argc, argv);

  std::vector<double> r0_ = {2};
  vec r0(r0_);

  
  double t0 = 0;
  double lambda = 0.6;
  double stepSize = 0.5;
  int maxIterations = 10;

  Linear_ODE problem(r0, t0, lambda);


  RungeKutta4<Linear_ODE, double> rk4_solver(problem, stepSize, maxIterations);
  spdlog::info("RK4 instansiated");
  ExplicitEuler<Linear_ODE, double> ee_solver(problem, stepSize, maxIterations);

  spdlog::info("Starting to solve");

  rk4_solver.setPrint(true);
  ee_solver.setPrint(true);

  rk4_solver.solve();
  ee_solver.solve();

  return 0;
};