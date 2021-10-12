#include "er.hpp"

#include <random>
#include <string>
#include <tuple>
#include <vector>
#include <unordered_set>

Graph erdos_renyi_p(unsigned n, double p, std::default_random_engine& generator) {
  Graph G(n);
  std::geometric_distribution<int> distr(p);

  auto next_pair = [](Node u, Node v, unsigned skip) {
    while (skip + v >= u) {
      skip -= u - v;
      u++;
      v = 0;
    }
    v += skip;
    return std::make_pair(u, v);
  };

  auto [u, v] = next_pair(1, 0, distr(generator));
  while (u < n) {
    G.add_edge(u, v);
    std::tie(u, v) = next_pair(u, v, 1 + distr(generator));
  }
  return G;
}
 
Graph erdos_renyi_m(unsigned n, unsigned m, std::default_random_engine& generator) {
  Graph G(n);
  std::uniform_int_distribution<int> distr(0, n - 1);
  std::unordered_set<std::string> edges_seen;
  auto edge_name = [](Node u, Node v) {
    return std::to_string(std::min(u, v)) + "-" +
           std::to_string(std::max(u, v));
  };
  for (unsigned i = 0; i < m; ++i) {
    Node u, v;
    do { // rejection sampling
      u = distr(generator);
      v = distr(generator);
    } while (u == v || edges_seen.count(edge_name(u, v)));
    G.add_edge(u, v);
  }
  return G;
}
