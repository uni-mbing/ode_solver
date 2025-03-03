#include <iostream>
#include <Problem.h>
#include <Solver.h>
#include <CLI/CLI.hpp>

typedef Vector<double> vec;

enum SOLVER_TYPE{
    RUNGE_KUTTA_4,
    LEAP_FROG,
    EE
};

enum SIMULATION_TYPE{
    LINEAR_ODE,
    N_BODY
};

int main(int argc, char** argv){
    CLI::App app{"ODE Solver App"};

    // Definiere eine optionale Eingabe für eine Zahl
    std::string solverType = "RK4";
    app.add_option("-s,--solver", solverType, "Input solver type: RUNGE_KUTTA_4, LEAP_FROG, EE");

    // Definiere eine optionale Eingabe für einen Namen
    std::string problemType;
    app.add_option("-p,--problem", problemType, "Input problem type: LINEAR, N_BODY");

    // Parse die Kommandozeilenargumente
    CLI11_PARSE(app, argc, argv);

    std::cout << problemType << " " << solverType << std::endl;

    std::vector<double> init ={2};
    vec y0(init);

    double t0 = 0;
    double lambda = 0.6;
    double stepSize = 0.5;
    int maxIterations = 10;

    std::unique_ptr<ODE_Problem<double>> problem = std::make_unique<Linear_ODE>(y0, t0, lambda);
    RK4<double> solver(std::move(problem), stepSize, maxIterations);

    // solver.solve();


    return 0;
};