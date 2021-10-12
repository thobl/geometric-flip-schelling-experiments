#pragma once

#include <random>
#include <vector>

#include "graph.hpp"

struct Point {
  double x;
  double y;
};

struct GeometricGraph {
  Graph graph;
  std::vector<Point> points;
  double radius;
  bool torus;
};

GeometricGraph random_geometric_graph(unsigned int n, double avg_deg,
                                      std::default_random_engine& generator,
                                      bool torus = false);
