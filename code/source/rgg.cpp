
#include "rgg.hpp"

GeometricGraph random_geometric_graph(unsigned int n, double avg_deg,
                                      std::default_random_engine& generator) {
  
  double threshold_square = avg_deg / ((n - 1) * M_PI);
  double threshold = std::sqrt(threshold_square);
  GeometricGraph G{{n}, std::vector<Point>{n}, threshold};

  // generate points
  std::uniform_real_distribution<double> distribution(0.0, 1.0);
  for (auto& p : G.points) {
    p.x = distribution(generator);
    p.y = distribution(generator);
  }

  // sort points into a grid
  int nr_cells = std::ceil(1 / threshold) + 1;
  std::vector<std::vector<std::vector<Node>>> cells(
      nr_cells, std::vector<std::vector<Node>>(nr_cells));

  for (Node v = 0; v < n; ++v) {
    int ix = std::floor(G.points[v].x / threshold);
    int iy = std::floor(G.points[v].y / threshold);
    cells[ix][iy].push_back(v);
  }

  // generate edges
  for (Node v = 0; v < n; ++v) {
    int ix = std::floor(G.points[v].x / threshold);
    int iy = std::floor(G.points[v].y / threshold);
    for (int dx = -1; dx <= 1; ++dx) {
      for (int dy = -1; dy <= 1; ++dy) {
        if (ix + dx < 0 || ix + dx >= nr_cells || iy + dy < 0 ||
            iy + dy >= nr_cells) {
          continue;
        }
        for (auto u : cells[ix + dx][iy + dy]) {
          if (u >= v) {
            continue;
          }
          double dist_square = std::pow(G.points[u].x - G.points[v].x, 2) +
                               std::pow(G.points[u].y - G.points[v].y, 2);
          if (dist_square < threshold_square) {
            G.graph.add_edge(u, v);
          }
        }
      }
    }
  }

  return G;
}
