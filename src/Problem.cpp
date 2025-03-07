#include <Problem.h>


Vector<Body> N_Body::eval(double time, const Vector<Body>& in){
    std::cout << "Test eval" << std::endl;
    return Vector<Body>(0);
}

bool N_Body::init(double time, Vector<Body>& initial_conditions){
    std::cout << "Test init" << std::endl;
    return true;
}