#include <iostream>
#include <Problem.h>
#include <Solver.h>

typedef Vector<double> vec;

int main(int argc, char** argv){
    std::vector<double> init ={2};
    vec y0(init);

    double t0 = 0;
    double lambda = 0.6;
    double stepSize = 0.5;
    int maxIterations = 10;

    std::unique_ptr<ODE_Problem<double>> problem = std::make_unique<Linear_ODE>(y0, t0, lambda);
    RK4<double> solver(std::move(problem), stepSize, maxIterations);

    solver.solve();


    return 0;
};