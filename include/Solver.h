#pragma once

#include "Problem.h"
#include <memory>

template<typename T>
class Solver{
public:
    virtual void step() = 0;
    double stepSize;
    std::unique_ptr<ODE_Problem<T>> problemDefintion;
    std::vector<Vector<T>> yn;
    std::vector<double> times;
    int currentIteration = 0;
    int maxIteration;

};

template<typename T>
class RK4 : public Solver<T> {
public:
    RK4(std::unique_ptr<ODE_Problem<T>> problem, double stepSize, int maxIteration_ = 10000){
        this->problemDefintion = std::move(problem);
        this->stepSize = stepSize;
        this->yn = {this->problemDefintion->y0};
        this->times = {this->problemDefintion->t0};
        this->maxIteration = maxIteration_;
    };

    void step() override {
        double currentTime = this->times[this->times.size()-1];

        Vector<T> current = this->yn[this->yn.size()-1];
        Vector<T> k1 = this->problemDefintion->eval(currentTime, current);
        Vector<T> t1 = current + k1 * (this->stepSize/2);
        Vector<T> k2 = this->problemDefintion->eval(currentTime + this->stepSize / 2, t1);
        Vector<T> t2 = current + k2 * (this->stepSize/2);
        Vector<T> k3 = this->problemDefintion->eval(currentTime + this->stepSize / 2, t2);
        Vector<T> t3 = current + k3 * this->stepSize;
        Vector<T> k4 = this->problemDefintion->eval(currentTime + this->stepSize, t3);
        Vector<T> res = current + k1 / 6 + k2 / 3 + k3 / 3 + k4 / 6;

        std::cout << "\t xn_1:" << (currentTime + this->stepSize) << " \tyn_1:" << res.toStr() << std::endl;
        this->yn.push_back(res);
        this->times.push_back(currentTime + this->stepSize);
        this->currentIteration++;
    };

    void solve(){
        while(this->currentIteration < this->maxIteration){
            step();
        }
        std::cout << "=============================================" << std::endl;
    };
};