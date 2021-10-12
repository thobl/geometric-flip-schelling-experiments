#pragma once

#include <vector>

typedef unsigned Node;

class Graph {
 public:
  Graph(unsigned n);

  void add_edge(Node u, Node v);
  
  unsigned n() const;
  unsigned m() const;
  const std::vector<Node>& neighbors(Node v) const;
  unsigned deg(Node v) const;

 private:
  unsigned n_ = 0;
  unsigned m_ = 0;
  std::vector<std::vector<Node>> adj_list_;
};
