#pragma once

#include <cassert>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include <spdlog/spdlog.h>


enum PROBLEM_TYPE { LINEAR_ODE, N_BODY };


template <typename T> class Vector {
public:
  Vector(int dim_) {
    this->dim = dim_;
    this->data = std::vector<T>(this->dim);
    setXYZ();
  }
  Vector(std::vector<T> &data_) : data(data_), dim(data.size()) {setXYZ();}
  Vector(){
    this->dim = 0;
    this->data = std::vector<T>(0); 
  };
  Vector(const Vector &other) {
    if(other.dim > 0){
      this->data = std::vector<T>(other.dim);
      this->data = other.data;
      this->dim = other.dim;
      setXYZ();
    }
  }

  void setXYZ(){
    if(dim > 3){
      this->x = data[0];
      this->y = data[1];
      this->z = data[2];
    }
    else if(dim == 2){
      this->x = data[0];
      this->y = data[1];
    }
    else if(dim == 1){
      this->x = data[0];
    }
  }

  Vector operator+(const Vector &other) const {
    assert(this->dim == other.dim);
    Vector res(this->dim);
    for (int i = 0; i < this->dim; i++) {
      res.data[i] = this->data[i] + other.data[i];
    }
    return res;
  };

  Vector operator-(const Vector &other) const { return this + (-1) * other; }

  Vector operator*(const double &scalar) const {
    Vector res(this->dim);
    for (int i = 0; i < this->dim; i++) {
      res.data[i] = this->data[i] * scalar;
    }
    return res;
  };
  friend Vector operator*(const double &scalar, const Vector &vec) {
    return vec * scalar;
  }

  Vector operator/(const double &scalar) const {
    Vector res(this->dim);
    for (int i = 0; i < this->dim; i++) {
      res.data[i] = this->data[i] / scalar;
    }
    return res;
  };

  friend Vector operator/(const double &scalar, const Vector &vec) {
    return vec / scalar;
  }

  T& operator[](int index) { return this->data[index]; }
  const T& operator[](int index) const { return this->data[index]; }
  T getX() { return this->x; };
  T getY() { return this->y; };
  T getZ() { return this->z; };

  int getDim() { return this->dim; }

  std::string toStr() {
    std::string res = "[ ";
    for (int i = 0; i < data.size() - 1; i++) {
      res += std::to_string(data[i]) + ", ";
    }
    res += std::to_string(data[data.size() - 1]) + " ]";
    return res;
  };

private:
  std::vector<T> data;
  T x;
  T y;
  T z;
  int dim = 1;
};

typedef Vector<double> vec;

struct Body {
  Body(double m, double rx_, double ry_, double vx_, double vy_) : mass(m) {
    std::vector<double> pos = {rx_, ry_};
    std::vector<double> vel = {vx_, vy_};
    position = Vector<double>(pos);
    velocity = Vector<double>(vel);
    // objectID = objectCount++;
  };
  Body(double rx_, double ry_, double vx_, double vy_)
      : Body(1, rx_, ry_, vx_, vy_){};
  Body() : Body(0, 0, 0, 0, 0){};

  double dist(Body &other) {
    return sqrt(pow(position[0] - other.position[0], 2) +
                pow(position[1] - other.position[1], 2));
  }

  std::string toStr() {
    std::string res = "Body id : " + std::to_string(objectID) + "\n\t r: [ ";
    for (int i = 0; i < position.getDim() - 1; i++) {
      res += std::to_string(position[i]) + ", ";
    }
    res += std::to_string(position[position.getDim() - 1]) + " ]\n\t v: [ ";

    for (int i = 0; i < velocity.getDim() - 1; i++) {
      res += std::to_string(velocity[i]) + ", ";
    }
    res += std::to_string(velocity[velocity.getDim() - 1]) + " ]";
    return res;
  };

  double mass;
  Vector<double> position;
  Vector<double> velocity;
  // static int objectCount;
  int objectID;
};

// int Body::objectCount = 0;

// class containing a initial value problem, defintion of evaluatin f and
// initial values used for it
template <typename T> class ODE_Problem {
public:
  ODE_Problem(){};
  virtual Vector<T> eval(double t_i, const Vector<T> &r_i) {
    return Vector<T>(0);
  };
  void init(double t_0_, Vector<T>& r_0_) {
    this->t_0 = t_0_;
    this->r_0 = r_0_;
  };
  virtual ~ODE_Problem() = default;

  int dim = 1;
  int nBodies = 1;
  std::string descr;

  Vector<T> r_0;
  double t_0;
};

class Linear_ODE : public ODE_Problem<double> {
public:
  
  Linear_ODE(){};
  Linear_ODE(Vector<double>& r_0_, double t_0_,
             double lambda_) {
    this->descr = "Linear ODE f(r) = lambda * r";
    this->r_0 = r_0_;
    this->t_0 = t_0_;
    this->lambda = lambda_;
    this->dim = 1;
  };

  Vector<double> eval(double t_i, const Vector<double> &r_i) override {
    Vector<double> r_j = r_i * lambda;
    return lambda * r_i;
    // return r_j;
  };

  double lambda;
};

class LorenzAttractor : public ODE_Problem<double>{
public:
  LorenzAttractor(){};
  LorenzAttractor(Vector<double>& r_0_, double t_0_,
    double a_ = 10.0, double b_ = 28.0, double c_ = (8.0/3.0)){
    this->descr = "Lorenz Attractor";
    this->r_0 = r_0_;
    this->t_0 = t_0_;
    this->a = a_;
    this->b = b_;
    this->c = c_;
    this->dim = 3;
  };
  Vector<double> eval(double t_i, const Vector<double> &r_i) override {
    Vector<double> r_j(this->dim);
    r_j[0] = a*(r_i[1] - r_i[0]);
    r_j[1] = r_i[0] * (b - r_i[2]) - r_i[1];
    r_j[2] = r_i[0] * r_i[1] - c * r_i[2];
    return r_j;
  };

  double a;
  double b;
  double c;
};

class LotkaVolterra : public ODE_Problem<double>{
  public:
    LotkaVolterra(){};
    LotkaVolterra(Vector<double>& r_0_, double t_0_,double alpha_ = 0.5, double beta_ = 0.05, double delta_ = 0.05, double gamma_ = 0.5){
      this->descr = "Lotka Volterra";
      this->r_0 = r_0_;
      this->t_0 = t_0_;
      this->alpha = alpha_;
      this->beta = beta_;
      this->delta = delta_;
      this->gamma = gamma_;
      this->dim = 2;
    };
    Vector<double> eval(double t_i, const Vector<double> &r_i) override {
      Vector<double> r_j(this->dim);
      r_j[0] = this->alpha * r_i[0] - this->beta * r_i[0] * r_i[1];
      r_j[1] = this->delta * r_i[0] * r_i[1] - this->gamma * r_i[1];
      return r_j;
    };
  
    double alpha;
    double beta;
    double delta;
    double gamma;
  };

class N_Body : public ODE_Problem<Body> {
public:
  N_Body(int nBodies, double gravity_) {
    this->descr = "N Body Problem";
    this->nBodies = nBodies;
    this->dim = 2;
  };

  N_Body() {
    this->descr = "N Body Problem";
    this->gravity = 1;
    this->nBodies = 0;
    this->dim = 2;
  };

  N_Body(double gravity_) : gravity(gravity_) {
    this->descr = "N Body Problem";
    this->gravity = gravity_;
    this->nBodies = 0;
    this->dim = 2;
  };
  // yn is given in the format, that the first n elements correspond to the
  // position of the ith body, and the second n elements correspond to the
  // velocity of the ith body
  Vector<Body> eval(double t_0, const Vector<Body> &r_i) override;

  double gravity;
};
