#pragma once

#include <vector>
#include <iostream>
#include <math.h>
#include <string>
#include <cassert>

template<typename T>
struct Vector{
public:
    Vector(int dim){
        data = std::vector<T>(dim);
        this->dim = data.size();
    }
    Vector(std::vector<T>& data_) : data(data_), dim(data.size()){}
    Vector() : Vector(0){};
    Vector(const Vector& other){
        data = other.data;
        dim = other.dim;
    }
    
    Vector operator+(const Vector& other) const {
        assert(this->dim == other.dim);
        Vector res(this->dim);
        for(int i = 0; i < this->dim; i++){
            res.data[i] = this->data[i] + other.data[i];
        }
        return res;
    };

    Vector operator-(const Vector& other) const {
        return this + (-1) * other; 
    }

    Vector operator*(const double& scalar) const {
        Vector res(this->dim);
        for(int i = 0; i < this->dim; i++){
            res.data[i] = this->data[i] * scalar;
        }
        return res;
    };
    friend Vector operator*(const double& scalar, const Vector& vec){
        return vec * scalar;
    }

    Vector operator/(const double& scalar) const {
        Vector res(this->dim);
        for(int i = 0; i < this->dim; i++){
            res.data[i] = this->data[i] / scalar;
        }
        return res;
    };
    
    friend Vector operator/(const double& scalar, const Vector& vec){
        return vec / scalar;
    } 

    T operator[](int index){
        return this->data[index];
    }

    int getDim(){
        return this->dim;
    }

    std::string toStr(){
        std::string res = "[ ";
        for(int i = 0; i < data.size() - 1; i++){
            res += std::to_string(data[i]) + ", ";
        }
        res += std::to_string(data[data.size() - 1]) + " ]; DIM: " + std::to_string(this->dim);
        return res;
    };

private:
    std::vector<T> data;
    int dim;
};



struct Body{
    Body(double m, double rx_, double ry_, double vx_, double vy_) : mass(m){
        std::vector<double> pos = {rx_, ry_};
        std::vector<double> vel = {vx_, vy_};
        position = Vector<double>(pos);
        velocity = Vector<double>(vel);
        objectID = objectCount++;
    };
    Body(double rx_, double ry_, double vx_, double vy_) : Body(1, rx_, ry_, vx_, vy_) {};
    Body() : Body(0,0,0,0,0){};


    double dist(Body& other){
        return sqrt(pow(position[0] - other.position[0],2) + pow(position[1] - other.position[1],2));
    }

    std::string toStr(){
        std::string res = "Body id : "+ std::to_string(objectID) + "\n\t r: [ ";
        for(int i = 0; i < position.getDim() - 1; i++){
            res += std::to_string(position[i]) + ", ";
        }
        res += std::to_string(position[position.getDim() - 1]) + " ]\n\t v: [ ";

        for(int i = 0; i < velocity.getDim() - 1; i++){
            res += std::to_string(velocity[i]) + ", ";
        }
        res += std::to_string(velocity[velocity.getDim() - 1]) + " ]";
        return res;
    };

    double mass;
    Vector<double> position;
    Vector<double> velocity;
    static int objectCount;
    int objectID;

};

int Body::objectCount = 0;

// class containing a initial value problem, defintion of evaluatin f and initial values used for it
template<typename T>
class ODE_Problem{
public:
    virtual Vector<T> eval(double time, const Vector<T>& yn){
        return Vector<T>(0);
    };
    virtual bool init(double time, Vector<T>& initial_conditions){return true;};
    virtual ~ODE_Problem() = default;

    int dim;
    int nParticles;
    Vector<T> y0;
    Vector<T> x0;
    double t0;
};

class Linear_ODE : public ODE_Problem<double>{
public:
    Linear_ODE(){};
    Linear_ODE(Vector<double> y0_, Vector<double> x0_, double t0_, double lambda_){
        this->y0 = y0_;
        this->x0 = x0_;
        this->t0 = t0_;
        this->lambda = lambda_;
    };

    Vector<double> eval(double time, const Vector<double>& yn) override {
        Vector<double> res = yn * lambda; 
        return res;
    };
    bool init(double time, Vector<double>& initial_conditions) override {
        return true;
    };

    int dim = 1;
    int nParticles = 1;
    double lambda;
};


class N_Body : public ODE_Problem<Body>{
public:
    N_Body(int nBodies, double gravity_){
        this->nParticles = nBodies;
        this->dim = 2;
    };

    N_Body(){
        this->gravity = 1;
        this->nParticles = 0;
        this->dim = 2;
    };

    N_Body(double gravity_) : gravity(gravity_) {
        this->gravity = gravity_;
        this->nParticles = 0;
        this->dim = 2;
    };
    // yn is given in the format, that the first n elements correspond to the position of the ith body, and the second n elements correspond to the velocity of the ith body
    Vector<Body> eval(double time, const Vector<Body>& yn) override;
    bool init(double time, Vector<Body>& initial_conditions);

    double gravity;
};

