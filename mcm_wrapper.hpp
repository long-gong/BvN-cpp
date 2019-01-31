#ifndef MCM_WRAPPER_HPP
#define MCM_WRAPPER_HPP

#include <string>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <cassert>

#include <boost/graph/max_cardinality_matching.hpp>

#include "matrix_utils.hpp"

using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> my_graph;

namespace std
{
std::ostream &operator<<(std::ostream &out, const my_graph &g)
{
    typename graph_traits<Graph>::edge_iterator ei, ei_end;
    const int n = vertices(g) / 2;
    for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
        out << source(*ei, g) << " -$>$ " << target(*ei, g) - n << std::endl;
}
} // namespace std

template <typename Matrix, typename Matching = std::vector<int>>
class MaximumMatchingWrapper
{
  public:
    MaximumMatchingWrapper(const Matrix &M) : _n(n_rows(M)), _g(2 * _n), _mate(2 * _n)
    {
        assert(_n == n_cols(M)); // only support square matrix
        // construct a graph from matrix M
        // // Here we build a general graph instead of a bibpartite one
        // // because there is no available APIs for bipartite matchings
        // // in BGL
        for (size_t u = 0; u < _n; ++u)
        {
            for (size_t v = 0; v < _n; ++v)
            {
                if (get(M, u, v) > 0)
                    add_edge(u, v + _n, _g);
            }
        }
    }
    void run()
    {
        edmonds_maximum_cardinality_matching(_g, &_mate[0]);
    }
    void get_matching(Matching &m) const
    {
        for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi)
            if (mate[*vi] != graph_traits<my_graph>::null_vertex() && *vi < mate[*vi])
            {
                m[*vi] = mate[*vi] - _n;
            }
    }
    void add_edge(size_t u, size_t v)
    {
        assert(u < _n && v < _n);
        add_edge(u, v + _n, _g);
    }
    void remove_edge(size_t u, size_t v)
    {
        assert(u < _n && v < _n);
        remove_edge(u, v + _n, _g);
    }

  private:
    const int _n;
    my_graph _g;
    std::vector<graph_traits<my_graph>::vertex_descriptor> _mate;
};     // MCMWrapper
#endif // MCM_WRAPPER_HPP