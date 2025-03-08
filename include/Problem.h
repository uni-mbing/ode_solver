#pragma once

#include <cassert>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include <spdlog/spdlog.h>

template <typename T> class Vector {
public:
  Vector(int dim_) {
    this->dim = dim;
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

  T operator[](int index) { return this->data[index]; }
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
  ODE_Problem(){
    spdlog::info("Calling Default ODE_Problem Constructor");
  };
  virtual Vector<T> eval(double t_i, const Vector<T> &r_i) {
    return Vector<T>(0);
  };
  void init(double t_0_, Vector<T>& r_0_) {
    this->t_0 = t_0_;
    this->r_0 = r_0_;
  };
  virtual ~ODE_Problem() = default;

  int dim = 1;
  int nParticles = 1;
  Vector<T> r_0;
  double t_0;
};

class Linear_ODE : public ODE_Problem<double> {
public:
  Linear_ODE(){};
  Linear_ODE(Vector<double>& r_0_, double t_0_,
             double lambda_) {
    spdlog::info("starting construction");
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
  LorenzAttractor(){
    ;
  };
  Vector<double> eval(double t_i, const Vector<double> &r_i) override {
    return Vector<double>(0);
  };
};

class N_Body : public ODE_Problem<Body> {
public:
  N_Body(int nBodies, double gravity_) {
    this->nParticles = nBodies;
    this->dim = 2;
  };

  N_Body() {
    this->gravity = 1;
    this->nParticles = 0;
    this->dim = 2;
  };

  N_Body(double gravity_) : gravity(gravity_) {
    this->gravity = gravity_;
    this->nParticles = 0;
    this->dim = 2;
  };
  // yn is given in the format, that the first n elements correspond to the
  // position of the ith body, and the second n elements correspond to the
  // velocity of the ith body
  Vector<Body> eval(double t_0, const Vector<Body> &r_i) override;

  double gravity;
};
