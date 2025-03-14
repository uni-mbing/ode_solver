#pragma once

#include <nlohmann/json.hpp>
#include <iostream>
#include <Solver.h>
#include <Problem.h>

void test(){
    std::cout << "Testing the Saver module" << std::endl;
}

template<typename DataType, typename ProblemType>
class Saver{
public:
    Saver(ODE_Problem<DataType>& prob, Solver<ProblemType, DataType>& sol){
        std::cout << "Instantiated a solver instance" << std::endl;
        return;
    }
};