#pragma once

#include "Problem.h"
#include <memory>

template<typename ProblemType, typename DataType>
class Solver{
public:
    virtual void solve(){
        std::cout << this->name << " method is used..." << std::endl;
        if(this->print){
            std::cout << "\tt_0: " << tn[0] << " \ty_0:" << yn[0].toStr() << std::endl;
        }
        while(this->currentIteration < this->maxIteration){
            step();
        }
        std::cout << "=============================================" << std::endl;
    };

    std::vector<Vector<DataType>> getYn(){return this->yn;};
    std::vector<double> getTn(){return this->tn;};
    void setPrint(bool print_){print = print_;};
    std::string getName(){return this->name;};

protected:
    ProblemType problemDefintion;
    double dt = 1;
    DataType dx;
    int currentIteration = 0;
    int maxIteration;
    std::vector<double> tn;
    std::vector<DataType> xn;
    std::vector<Vector<DataType>> yn;
    bool print = false;
    std::string name;

    virtual void step() = 0;
    void update(double currentTime, Vector<DataType> currentPosition){
        this->yn.push_back(currentPosition);
        this->tn.push_back(currentTime);
        this->currentIteration++;
        if(this->print)
            std::cout << "\tt_" << std::to_string(this->currentIteration) << ":" << (this->tn.back()) << " \ty_" << std::to_string(this->currentIteration) << ":" << this->yn.back().toStr() << std::endl;
    }

};

template<typename ProblemType, typename DataType>
class RungeKutta4 : public Solver<ProblemType, DataType>{
public:
    RungeKutta4(ProblemType problem, double dt_, int maxIteration_ = 10000){
        this->problemDefintion = problem;
        this->dt = dt_;
        this->yn = {this->problemDefintion.y0};
        this->tn = {this->problemDefintion.t0};
        this->maxIteration = maxIteration_;
        this->name = "Runge Kutta 4";
    };

    void step() override {
        double t_i = this->tn.back();
        Vector<DataType> y_i = this->yn.back();
        Vector<DataType> k1 = this->problemDefintion.eval(t_i, y_i);
        Vector<DataType> k2 = this->problemDefintion.eval(t_i + this->dt / 2, y_i + (this->dt/2) * k1);
        Vector<DataType> k3 = this->problemDefintion.eval(t_i + this->dt / 2, y_i + (this->dt/2) * k2);
        Vector<DataType> k4 = this->problemDefintion.eval(t_i + this->dt, y_i + this->dt * k3);
        Vector<DataType> y_i1 = y_i + (this->dt/6) * (k1 + k2 * 2 + k3 *2 + k4);
        this->update(t_i + this->dt, y_i1); 
        return;
    };
};

template<typename ProblemType, typename DataType>
class ExplicitEuler : public Solver<ProblemType, DataType>{
public:
    ExplicitEuler(){};
    ExplicitEuler(ProblemType problemDefintion_){
        this->problemDefintion = problemDefintion_;
    };

    ExplicitEuler(ProblemType problem, double dt_, int maxIteration_ = 10000){
        this->problemDefintion = problem;
        this->dt = dt_;
        this->yn = {this->problemDefintion.y0};
        this->tn = {this->problemDefintion.t0};
        this->maxIteration = maxIteration_;
        this->name = "Explicit Euler";
    };
    void step() override {
        double t_i = this->tn.back();
        Vector<DataType> y_i = this->yn.back();
        Vector<DataType> y_i1 = y_i + this->problemDefintion.eval(t_i, y_i) * this->dt;
        this->update(t_i + this->dt, y_i1);
        return;
    };
};