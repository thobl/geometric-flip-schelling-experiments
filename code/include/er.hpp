#include <random>

#include "graph.hpp"

inline double avg_deg_to_p(unsigned n, double avg_deg) {
  return avg_deg / (n - 1);
}

Graph erdos_renyi_p(unsigned n, double p, std::default_random_engine& generator);

Graph erdos_renyi_m(unsigned n, unsigned m, std::default_random_engine& generator);
