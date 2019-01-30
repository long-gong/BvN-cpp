#ifndef BvN_HPP
#define BvN_HPP
#include <vector>
#include <exception>
#include <cassert>
#include <iostream>
#include <iomanip>      // std::setw
#include <queue> // std::priority_queue
#include <utility> // std::pair & std::make_pair

#include <armadillo>

using namespace arma;

namespace std {
template <typename Integer>
ostream& operator<<(ostream& os, const std::vector<std::vector<Integer>>& m) {
    for(const auto& row : m){
        for(const auto elm : row ) os << std::setw(11) << elm ;
        os << "\n";
    }
    os << "\n";
    return os;
}
}

template <typename Integer>
inline Integer& get(std::vector<std::vector<Integer>>& m, size_t i, size_t j){
    return m[i][j];
}

template <typename Integer>
inline const Integer& get(const std::vector<std::vector<Integer>>& m, size_t i, size_t j){
    return m[i][j];
}

inline s64& get(imat& m, size_t i, size_t j) {
    return m(i, j);
}

inline u64& get(umat& m, size_t i, size_t j) {
    return m(i, j);
}

inline const s64& get(const imat& m, size_t i, size_t j) {
    return m(i, j);
}

inline const u64& get(const umat& m, size_t i, size_t j) {
    return m(i, j);
}


template <typename Integer=unsigned int, typename Matrix=std::vector<std::vector<Integer>>>
inline size_t n_rows(const Matrix& m){
    return m.size();
}


template <>
inline size_t n_rows(const umat& m) {
    return m.n_rows;
}

template <>
inline size_t n_rows(const imat& m) {
    return m.n_rows;
}

template <typename Integer=unsigned int, typename Matrix=std::vector<std::vector<Integer>>>
inline size_t n_cols(const Matrix& m){
    return (m.empty()?0:m.front().size());
}


template <>
inline size_t n_cols(const umat& m) {
    return m.n_cols;
}

template <>
inline size_t n_cols(const imat& m) {
    return m.n_cols;
}

template <typename Integer=unsigned int, typename Matrix=std::vector<std::vector<Integer>>>
inline Integer row_sum(const Matrix& m, size_t r){
    assert (r < m.size());
    Integer sum = 0;
    for(const auto d : m[r]) sum += d;
    return sum;
}

template <>
inline  unsigned int row_sum(const umat& m, size_t r){
    return sum(m.row(r));
}

template <>
inline  int row_sum(const imat& m, size_t r){
    return sum(m.row(r));
}


template <typename Integer=unsigned int, typename Matrix=std::vector<std::vector<Integer>>>
inline Integer column_sum(const Matrix& m, size_t c){
    assert ((!m.empty()) && (c < m.front().size()));
    Integer sum = 0;
    auto n_rows = m.size();
    for(int r = 0;r < n_rows;++ r) sum += m[r][c];
    return sum;
}

template <>
inline  unsigned int column_sum(const umat& m, size_t c){
    return sum(m.col(c));
}

template <>
inline  int column_sum(const imat& m, size_t c){
    return sum(m.col(c));
}


template <typename Integer=unsigned int, typename Matrix=std::vector<std::vector<Integer>>>
void stuffing(Matrix& m) {
    size_t n = n_rows(m);
    assert(n == n_cols(m));

    std::vector<unsigned int> row_sums_v, col_sums_v;

    unsigned int max_sum = 0;
    // calculate row sums
    for(size_t r = 0;r < n;++ r) {
        auto rs = row_sum(m, r);
        max_sum = std::max(rs, max_sum);
        row_sums_v.push_back(rs);
    }

    // calculate column sums
    for(size_t c = 0;c < n;++ c) {
        auto cs = column_sum(m, c);
        max_sum = std::max(cs, max_sum);
        col_sums_v.push_back(cs);
    }

    using vi_pair_t = std::pair<unsigned int,size_t>;
    using priority_queue_t = std::priority_queue<vi_pair_t, std::vector<vi_pair_t>, std::greater<vi_pair_t>>;
    priority_queue_t row_sums, col_sums;

    for(size_t r = 0;r < n;++ r) {
        if(row_sums_v[r] < max_sum) row_sums.push(std::make_pair(row_sums_v[r], r));
    }
    for(size_t c = 0;c < n;++ c) {
        if(col_sums_v[c] < max_sum) col_sums.push(std::make_pair(col_sums_v[c], c));
    }

    while (!(row_sums.empty() && col_sums.empty())){
        assert((!row_sums.empty()) && (!col_sums.empty()));
        #if __cplusplus > 201402L
        // need c++ 17
        auto [rs, r] = row_sums.top();
        auto [cs, c] = col_sums.top();
        #else 
        // no c++ 17 support
        auto rp = row_sums.top();
        auto cp = col_sums.top();
        auto rs = rp.first;
        auto r = rp.second;
        auto cs = cp.first;
        auto c = cp.second;
        #endif 
        if(rs == max_sum || cs == max_sum){
            assert(cs == max_sum && cs == max_sum);
            break;
        }
        row_sums.pop();
        col_sums.pop();
        auto delta = max_sum - std::max(rs, cs);
        if (rs < cs){
            row_sums.push(std::make_pair(rs + delta, r));
        } else if (rs > cs) {
            col_sums.push(std::make_pair(cs + delta, c));
        }
        get(m, r, c) += delta;
        #ifdef DEBUG
        std::cerr << m << "\n";
        #endif
    }
}

#endif // BvN_HPP