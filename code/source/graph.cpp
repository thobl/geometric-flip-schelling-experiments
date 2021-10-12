
#include "graph.hpp"

#include <algorithm>
#include <cassert>
#include <vector>

Graph::Graph(unsigned n) : n_(n), adj_list_(n) {}

void Graph::add_edge(Node u, Node v) {
  assert(u < n_);
  assert(v < n_);
  adj_list_[u].push_back(v);
  adj_list_[v].push_back(u);
  ++m_;
}

unsigned Graph::n() const { return n_; }

unsigned Graph::m() const { return m_; }

const std::vector<Node>& Graph::neighbors(Node v) const {
  return adj_list_[v];
}

unsigned int Graph::deg(Node v) const { return neighbors(v).size(); }


