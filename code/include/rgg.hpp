#pragma once

#include <vector>
#include <random>

#include "graph.hpp"

struct Point {
  double x;
  double y;
};

struct GeometricGraph {
  Graph graph;
  std::vector<Point> points;
  double radius;
};

GeometricGraph random_geometric_graph(unsigned int n, double avg_deg,
                                      std::default_random_engine& generator);
