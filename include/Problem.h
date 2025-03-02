#include <vector>
#include <iostream>

template<int DIM, int PARTICLES, typename T>
class ODE_Problem{
public:
    virtual void eval(std::vector<T>& in, std::vector<T>& out) = 0;
    virtual bool init(std::vector<T>& initial_conditions) = 0;
    int dim = DIM;
    int particles = PARTICLES;
};

struct Body{
    Body(double m, double rx_, double ry_, double vx_, double vy_) : mass(m), rx(rx_), ry(ry_), vx(vx_), vy(vy_) {};
    Body(double rx_, double ry_, double vx_, double vy_) : mass(1), rx(rx_), ry(ry_), vx(vx_), vy(vy_) {};
    double mass;
    double rx, ry, vx, vy;
};

template<int N>
class N_Body : public ODE_Problem<2, N, Body>{
public:
    N_Body(double gravity_) : gravity(gravity_) {};
    void eval(std::vector<Body>& in, std::vector<Body>& out){
        std::cout << "Test eval" << std::endl;
    };

    bool init(std::vector<Body>& initial_conditions){
        std::cout << "Test Init" << std::endl;
        return true;
    };

    double gravity;
};

