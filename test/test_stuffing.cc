// test_stuffing.cc

// main() provided by Catch in file test_main.cc.

#include <catch2/catch.hpp>

#include <BvN.hpp>

using namespace arma;


TEST_CASE("2: random_stochastic_matrix", "[multi-file:2]"){
    SECTION("std::vector<std::vector<int>> as Matrix"){
      size_t n = 4;
      std::vector<std::vector<unsigned int>> M(n, std::vector<unsigned int>(n, 0));
      size_t T = 10;
      unsigned int row_sum_ = 39;
      for(size_t t = 0;t < T;++ t) {
          matutil::random_doubly_stochastic(M, row_sum_);
          for(size_t k = 0;k < n;++ k) {
            REQUIRE(matutil::row_sum(M, k) == row_sum_);
            REQUIRE(matutil::column_sum(M, k) == row_sum_);
          }

      }
    }
}


