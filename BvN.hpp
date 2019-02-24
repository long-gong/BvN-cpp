#ifndef BvN_HPP
#define BvN_HPP

#include <iomanip>
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <numeric>
#include <queue>
#include <cassert>

#include <matching_wrapper.hpp>
#include <climits>

inline unsigned row_sum(std::vector<std::vector<unsigned>> &mat, size_t row) {
 return std::accumulate(mat[row].begin(), mat[row].end(), 0U);
}

inline unsigned column_sum(std::vector<std::vector<unsigned>> &mat, size_t col) {
  unsigned sum = 0;
  for (const auto &r : mat)
    sum += r[col];
  return sum;
}


std::ostream& operator<<(std::ostream& os, std::vector<std::vector<unsigned>> const & mat){
  for (const auto &row : mat)
  {
    for (const auto elm : row)
      os << std::setw(11) << elm;
    os << "\n";
  }
  os << "\n";
  return os;
}

void stuffing(std::vector<std::vector<unsigned>> &mat)
{
  assert(!mat.empty());
  size_t n = mat.size();
  assert(n == mat.front().size());

  std::vector<unsigned> row_sums_v, col_sums_v;

  unsigned int max_sum = 0;
  // calculate row sums
  for (size_t r = 0; r < n; ++r)
  {
    auto rs = row_sum(mat, r);
    max_sum = std::max(rs, max_sum);
    row_sums_v.push_back(rs);
  }

  // calculate column sums
  for (size_t c = 0; c < n; ++c)
  {
    auto cs = column_sum(mat, c);
    max_sum = std::max(cs, max_sum);
    col_sums_v.push_back(cs);
  }

  using vi_pair_t = std::pair<unsigned int, size_t>;
  using priority_queue_t = std::priority_queue<vi_pair_t, std::vector<vi_pair_t>, std::greater<>>;
  priority_queue_t row_sums, col_sums;

  for (size_t r = 0; r < n; ++r)
  {
    if (row_sums_v[r] < max_sum)
      row_sums.push(std::make_pair(row_sums_v[r], r));
  }
  for (size_t c = 0; c < n; ++c)
  {
    if (col_sums_v[c] < max_sum)
      col_sums.push(std::make_pair(col_sums_v[c], c));
  }

  while (!(row_sums.empty() && col_sums.empty()))
  {
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
    if (rs == max_sum || cs == max_sum)
    {
      assert(cs == max_sum && rs == max_sum);
      break;
    }
    row_sums.pop();
    col_sums.pop();
    auto delta = max_sum - std::max(rs, cs);
    if (rs < cs)
    {
      row_sums.push(std::make_pair(rs + delta, r));
    }
    else if (rs > cs)
    {
      col_sums.push(std::make_pair(cs + delta, c));
    }
    mat[r][c] += delta;
#ifdef DEBUG
    std::cerr << mat << "\n";
#endif
  }
}


std::vector<std::pair<size_t /* coefficient */, std::vector<int> /* matching */>>
BvN(std::vector<std::vector<unsigned>>& sub_stochastic_mat)
{
  stuffing(sub_stochastic_mat);

  BpGraphWrapper bpg{sub_stochastic_mat};

  std::vector<std::pair<size_t /* coefficient */, std::vector<int> /* matching */>> matchings;

  while (!bpg.empty()) {
    auto matching = bpg.maximumMatching();
    unsigned coef = INT_MAX;
    for (auto i = 0;i < matching.size();++ i) {
      assert(matching[i] != -1);
      coef = std::min(coef, sub_stochastic_mat[i][matching[i]]);
    }
    for (auto i = 0;i < matching.size();++ i) {
      auto o = matching[i];
      sub_stochastic_mat[i][o] -= coef;
      if (sub_stochastic_mat[i][o] == 0) bpg.eraseEdge((uint16_t(i)), uint16_t(o));
    }

    matchings.emplace_back(coef, matching);

  }

  return matchings;
}
#endif // BvN_HPP