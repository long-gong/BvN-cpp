#include <iostream>
#include "BvN.hpp"

using namespace arma;
int main(){
    // std::vector<std::vector<int>> M = {
    //     {1, 2, 3},
    //     {4, 5, 6},
    //     {7, 8, 9}
    // };

    // std::cout << M << std::endl;
    // std::cout << row_sum(M, 0) << "\n";
    

    umat A = randi<umat>(3, 3, distr_param(0, +10));
    std::cout << A << std::endl;
    // std::cout << row_sum(A, 2) << "\n";

    stuffing(A);

    std::cout << A << std::endl;
    return 0;
}