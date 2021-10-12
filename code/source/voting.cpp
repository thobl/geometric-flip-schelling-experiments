#include "voting.hpp"

#include <algorithm>
#include <cassert>

#include "graph.hpp"

std::vector<bool> random_colors(unsigned n,
                                std::default_random_engine& generator) {
  std::bernoulli_distribution coin_flip(0.5);
  auto gen_random_bit = [&generator, &coin_flip]() {
    return coin_flip(generator);
  };
  std::vector<bool> result(n);
  std::generate(result.begin(), result.end(), gen_random_bit);
  return result;
}

std::vector<bool> random_colors_equal(unsigned n,
                                      std::default_random_engine& generator) {
  std::vector<bool> result(n / 2, false);
  result.resize(n, true);
  std::shuffle(result.begin(), result.end(), generator);
  return result;
}

unsigned monochromatic_edges(const Graph& G, const std::vector<bool>& colors) {
  unsigned double_count = 0;
  for (Node u = 0; u < G.n(); ++u) {
    for (Node v : G.neighbors(u)) {
      if (colors[u] == colors[v]) {
        double_count++;
      }
    }
  }
  return double_count / 2;
}

std::vector<bool> voting_result(const Graph& G, const std::vector<bool>& colors,
                                std::default_random_engine& generator) {
  std::bernoulli_distribution coin_flip(0.5);
  std::vector<bool> result(G.n());
  for (Node u = 0; u < G.n(); ++u) {
    std::vector<unsigned> counts = {0, 0};
    for (Node v : G.neighbors(u)) {
      counts[colors[v]]++;
    }
    if (counts[0] == counts[1]) {
      result[u] = coin_flip(generator);  // colors[u];
    } else if (counts[0] > counts[1]) {
      result[u] = 0;
    } else {
      result[u] = 1;
    }
  }
  return result;
}
