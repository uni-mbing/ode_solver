#include <Problem.h>

Vector<Body> N_Body::eval(double time, const Vector<Body> &in) {
  return Vector<Body>(0);
}

bool N_Body::init(double time, Vector<Body> &initial_conditions) {
  return true;
}