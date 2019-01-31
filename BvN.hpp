#ifndef BvN_HPP
#define BvN_HPP
#include "matrix_utils.hpp"
#include "mcm_wrapper.hpp"

template <typename Integer = unsigned int, typename Matrix = std::vector<std::vector<Integer>>>
void stuffing(Matrix &m)
{
    size_t n = n_rows(m);
    assert(n == n_cols(m));

    std::vector<unsigned int> row_sums_v, col_sums_v;

    unsigned int max_sum = 0;
    // calculate row sums
    for (size_t r = 0; r < n; ++r)
    {
        auto rs = row_sum(m, r);
        max_sum = std::max(rs, max_sum);
        row_sums_v.push_back(rs);
    }

    // calculate column sums
    for (size_t c = 0; c < n; ++c)
    {
        auto cs = column_sum(m, c);
        max_sum = std::max(cs, max_sum);
        col_sums_v.push_back(cs);
    }

    using vi_pair_t = std::pair<unsigned int, size_t>;
    using priority_queue_t = std::priority_queue<vi_pair_t, std::vector<vi_pair_t>, std::greater<vi_pair_t>>;
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
            assert(cs == max_sum && cs == max_sum);
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
        get(m, r, c) += delta;
#ifdef DEBUG
        std::cerr << m << "\n";
#endif
    }
}

template <typename Matrix>
std::vector<std::pair<size_t /* coefficient */, std::vector<int> /* matching */>>
BvN(Matrix &M)
{
}
#endif // BvN_HPP