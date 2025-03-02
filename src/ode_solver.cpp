#include <iostream>
#include <Problem.h>

int main(int argc, char** argv){

    N_Body<9> nin;

    std::vector<Body> in, out;

    nin.eval(in, out);
    nin.init(in);
    std::cout << nin.dim << " " << nin.particles << std::endl;


    std::cout << "Hello World!\n";
    return 0;
};