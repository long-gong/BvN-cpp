// matching_wrapper.hpp

#ifndef MATCHING_WRAPPER_HPP
#define MATCHING_WRAPPER_HPP

#include <lemon/list_graph.h>
#include <lemon/matching.h>
#include <cassert>
#include <vector>

using namespace lemon;
using lemon::INVALID;

class BpGraphWrapper {
  typedef ListGraph Graph;
  typedef Graph::Node Node;
  typedef Graph::Edge Edge;
  typedef std::unordered_map<uint32_t, Edge> EdgeMap;
  typedef MaxMatching<Graph> MaxMatching;
protected:
  std::uint16_t _m; // size of U
  std::uint16_t _n; // size of V
  std::vector<Node> _U; // vertex set U
  std::vector<Node> _V; // vertex set V
  Graph _g;
  EdgeMap _edges;
public:
  BpGraphWrapper() = delete;
  BpGraphWrapper(std::uint16_t m, std::uint16_t n) : _m(m), _n(n), _U(), _V(), _g(), _edges() {
    for(auto i = 0;i < _m;++ i) _U.push_back(_g.addNode());
    for(auto j = 0;j < _n;++ j) _V.push_back(_g.addNode());
  }

  explicit BpGraphWrapper(std::vector<std::vector<unsigned>> const& mat): BpGraphWrapper(mat.size(), mat[0].size()){
    assert(!mat.empty());
    assert(_m >= 2 && _n >= 2);
    Edge e;
    for(uint16_t i = 0;i < _m;++ i){
      for(uint16_t j = 0;j < _n;++ j) {
        if (mat[i][j] > 0) {
          e = _g.addEdge(_U[i], _V[j]);
          auto eid = edgeId(i, j);
          _edges[eid] = e;
        }
      }
    }
  }


  bool addEdge(uint16_t u, uint16_t v) {
    validate(u, v);
    if (_edges.count(edgeId(u, v)) > 0)
      return false; // failed, edge already exists
    Edge e = _g.addEdge(_U[u], _V[v]);
    _edges[edgeId(u, v)] = e;
    return true; // succeed
  }

  bool eraseEdge(uint16_t u, uint16_t v) {
    validate(u, v);
    auto e = edgeId(u, v);
    if(_edges.count(e) == 0) return false;
    _g.erase(_edges[e]);
    _edges.erase(e);
    return true;
  }

  std::vector<int> maximumMatching() const {
    MaxMatching mm(_g);
    mm.run();

    std::vector<int> matching(_m, -1);

    Node u, v;

    for(uint16_t i = 0;i < _m;++ i) {
      u = _U[i];
      v = mm.mate(u);
      if(v != INVALID) {
        auto vid = BpGraphWrapper::Graph::id(v) - _m;
        assert(vid >= 0);
        matching[i] = vid - _m;
      }
    }

    return matching;
  }

  bool empty() const {
    return _edges.empty();
  }

  bool size() const {
    return _edges.size();
  }
private:
  void validate(uint16_t u, uint16_t v) {
    assert(u < _m && v < _n);
  }
  uint32_t edgeId(uint16_t u, uint16_t v) const {
    return (((uint32_t)(u)) << 16 ) | ((uint32_t)v);
  }
};
#endif //MATCHING_WRAPPER_HPP
