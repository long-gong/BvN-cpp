#ifndef MATRIX_UTILS_HPP
#define MATRIX_UTILS_HPP

#include <vector>
#include <exception>
#include <cassert>
#include <iostream>
#include <iomanip> // std::setw
#include <queue>   // std::priority_queue
#include <utility> // std::pair & std::make_pair
#include <chrono>
#include <random>

#include <armadillo>

using namespace arma;

namespace std
{
template <typename Integer>
ostream &operator<<(ostream &os, const std::vector<std::vector<Integer>> &m)
{
    for (const auto &row : m)
    {
        for (const auto elm : row)
            os << std::setw(11) << elm;
        os << "\n";
    }
    os << "\n";
    return os;
}
} // namespace std


namespace matutil {
template<typename Integer>
inline Integer &get(std::vector<std::vector<Integer>> &m, size_t i, size_t j) {
  return m[i][j];
}

template<typename Integer>
inline const Integer &get(const std::vector<std::vector<Integer>> &m, size_t i, size_t j) {
  return m[i][j];
}

inline s64 &get(imat &m, size_t i, size_t j) {
  return m(i, j);
}

inline u64 &get(umat &m, size_t i, size_t j) {
  return m(i, j);
}

inline const s64 &get(const imat &m, size_t i, size_t j) {
  return m(i, j);
}

inline const u64 &get(const umat &m, size_t i, size_t j) {
  return m(i, j);
}

template<typename Integer = unsigned int, typename Matrix = std::vector<std::vector<Integer>>>
inline size_t n_rows(const Matrix &m) {
  return m.size();
}

template<>
inline size_t n_rows(const umat &m) {
  return m.n_rows;
}

template<>
inline size_t n_rows(const imat &m) {
  return m.n_rows;
}

template<typename Integer = unsigned int, typename Matrix = std::vector<std::vector<Integer>>>
inline size_t n_cols(const Matrix &m) {
  return (m.empty() ? 0 : m.front().size());
}

template<>
inline size_t n_cols(const umat &m) {
  return m.n_cols;
}

template<>
inline size_t n_cols(const imat &m) {
  return m.n_cols;
}

template<typename Integer = unsigned int, typename Matrix = std::vector<std::vector<Integer>>>
inline Integer row_sum(const Matrix &m, size_t r) {
  assert(r < m.size());
  Integer sum = 0;
  for (const auto d : m[r])
    sum += d;
  return sum;
}

template<>
inline unsigned int row_sum(const umat &m, size_t r) {
  return sum(m.row(r));
}

template<>
inline int row_sum(const imat &m, size_t r) {
  return sum(m.row(r));
}

template<typename Integer = unsigned int, typename Matrix = std::vector<std::vector<Integer>>>
inline Integer column_sum(const Matrix &m, size_t c) {
  assert((!m.empty()) && (c < m.front().size()));
  Integer sum = 0;
  auto n_rows = m.size();
  for (int r = 0; r < n_rows; ++r)
    sum += m[r][c];
  return sum;
}

template<>
inline unsigned int column_sum(const umat &m, size_t c) {
  return sum(m.col(c));
}

template<>
inline int column_sum(const imat &m, size_t c) {
  return sum(m.col(c));
}

template<typename Integer>
inline void fill(std::vector<std::vector<Integer>> &M, Integer x) {
  auto m = n_rows(M);
  auto n = n_cols(M);
  for (size_t i = 0; i < m; ++i) {
    for (size_t j = 0; j < n; ++j) {
      get(M, i, j) = x;
    }
  }
}

template<typename Matrix>
void random_doubly_stochastic(Matrix &M, size_t m, bool reset = true) {
  using Integer = decltype(get(M, 0, 0));
  if (reset) {
    int _v = 0;
    fill(M, (Integer)_v);
  }
  auto n = n_rows(M);
  const int max_iter = 50;
  assert(n == n_cols(M));
  // obtain a seed from the system clock:
  unsigned seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
  std::mt19937 g(seed);  // mt19937 is a standard mersenne_twister_engine
  std::vector<size_t> identity_perm(n);

  auto append_randperm = [&](Matrix& M, size_t coefficient){
    for (size_t i = 0; i < n; ++i) identity_perm[i] = i;
    shuffle(identity_perm.begin(), identity_perm.end(), g);
    for (size_t i = 0; i < n; ++i) {
      auto j = identity_perm[i];
      get(M, i, j) += coefficient;
    }
  };

  if (m > max_iter) {
    size_t alpha = m / max_iter;
    for (size_t k = 0; k < max_iter; ++k) append_randperm(M, alpha);
    auto remainder = m - alpha * max_iter;
    if (remainder > 0) append_randperm(M, remainder);
  } else {
    for (size_t k = 0; k < m; ++k) append_randperm(M, 1);
  }

}

}// namespace matutil

#endif // MATRIX_UTILS_HPP