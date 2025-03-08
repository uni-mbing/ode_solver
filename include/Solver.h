#pragma once

#include "Problem.h"
#include <memory>
#include <spdlog/spdlog.h>

template <typename ProblemType, typename DataType> class Solver {
public:
  virtual void solve() {
    spdlog::info(this->name + " method is used...");
    spdlog::info("\ti\t\tt_i\t\tr_i");
    if (this->print) {
      spdlog::info("\t{}\t\t{}\t\t" + this->r_n.front().toStr(), this->i,
                   this->t_n.front());
    }
    while (this->i < this->maxI) {
      step();
    }
    std::cout << std::endl;
  };

  std::vector<Vector<DataType>> getYn() { return this->r_n; };
  std::vector<double> getTn() { return this->t_n; };
  void setPrint(bool print_) { print = print_; };
  std::string getName() { return this->name; };

protected:
  ProblemType problemDefintion;
  double dt = 1;
  DataType dr = DataType();
  int i = 0;
  int maxI;

  std::vector<double> t_n = {0};
  std::vector<Vector<DataType>> r_n = {DataType()};

  bool print = false;
  std::string name;

  virtual void step() = 0;
  void update(double currentTime, Vector<DataType> currentPosition) {
    this->r_n.push_back(currentPosition);
    this->t_n.push_back(currentTime);
    this->i++;
    if (this->print) {
      spdlog::info("\t{}\t\t{}\t\t" + this->r_n.back().toStr(), this->i,
                   this->t_n.back());
    }
  }
};

template <typename ProblemType, typename DataType>
class RungeKutta4 : public Solver<ProblemType, DataType> {
public:
  RungeKutta4(){};
  RungeKutta4(ProblemType problem) {
    this->problemDefintion = problem;
  };
  RungeKutta4(ProblemType problem, double dt_, int maxIteration_ = 10000) {
    this->problemDefintion = problem;
    this->dt = dt_;
    this->r_n = {this->problemDefintion.r_0};
    this->t_n = {this->problemDefintion.t_0};
    this->maxI = maxIteration_;
    this->name = "Runge Kutta 4";
  };

  void step() override {
    double t_i = this->t_n.back();
    Vector<DataType> r_i = this->r_n.back();
    Vector<DataType> k1 = this->problemDefintion.eval(t_i, r_i);
    Vector<DataType> k2 = this->problemDefintion.eval(
        t_i + this->dt / 2, r_i + (this->dt / 2) * k1);
    Vector<DataType> k3 = this->problemDefintion.eval(
        t_i + this->dt / 2, r_i + (this->dt / 2) * k2);
    Vector<DataType> k4 =
        this->problemDefintion.eval(t_i + this->dt, r_i + this->dt * k3);
    Vector<DataType> r_j = r_i + (this->dt / 6) * (k1 + k2 * 2 + k3 * 2 + k4);
    this->update(t_i + this->dt, r_j);
    return;
  };
};

template <typename ProblemType, typename DataType>
class ExplicitEuler : public Solver<ProblemType, DataType> {
public:
  ExplicitEuler(){};
  ExplicitEuler(ProblemType problemDefintion_) {
    this->problemDefintion = problemDefintion_;
  };
  ExplicitEuler(ProblemType problem, double dt_, int maxIteration_ = 10000) {
    this->problemDefintion = problem;
    this->dt = dt_;
    this->r_n = {this->problemDefintion.r_0};
    this->t_n = {this->problemDefintion.t_0};
    this->maxI = maxIteration_;
    this->name = "Explicit Euler";
  };
  void step() override {
    double t_i = this->t_n.back();
    Vector<DataType> r_i = this->r_n.back();
    Vector<DataType> r_j =
        r_i + this->problemDefintion.eval(t_i, r_i) * this->dt;
    this->update(t_i + this->dt, r_j);
    return;
  };
};