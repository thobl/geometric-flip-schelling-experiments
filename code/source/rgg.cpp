
#include "rgg.hpp"

#include <functional>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GeometricGraph random_geometric_graph(unsigned int n, double avg_deg,
                                      std::default_random_engine& generator,
                                      bool torus) {
  double threshold_square = avg_deg / ((n - 1) * M_PI);
  double threshold = std::sqrt(threshold_square);
  GeometricGraph G{{n}, std::vector<Point>{n}, threshold, torus};

  // generate points
  std::uniform_real_distribution<double> distribution(0.0, 1.0);
  for (auto& p : G.points) {
    p.x = distribution(generator);
    p.y = distribution(generator);
  }

  // sort points into a grid
  int nr_cells = std::floor(1 / threshold);
  double cell_size = 1.0 / nr_cells;
  std::vector<std::vector<std::vector<Node>>> cells(
      nr_cells, std::vector<std::vector<Node>>(nr_cells));

  for (Node v = 0; v < n; ++v) {
    int ix = std::floor(G.points[v].x / cell_size);
    int iy = std::floor(G.points[v].y / cell_size);
    cells[ix][iy].push_back(v);
  }

  // squared distance in the square and in the torus
  std::function<double(Node, Node)> dist_square_square = [&G](Node u, Node v) {
    auto dx = G.points[u].x - G.points[v].x;
    auto dy = G.points[u].y - G.points[v].y;
    return std::pow(dx, 2) + std::pow(dy, 2);
  };

  std::function<double(Node, Node)> dist_square_torus = [&G](Node u, Node v) {
    auto dx = std::abs(G.points[u].x - G.points[v].x);
    auto dy = std::abs(G.points[u].y - G.points[v].y);
    dx = std::min(dx, 1 - dx);
    dy = std::min(dy, 1 - dy);
    return std::pow(dx, 2) + std::pow(dy, 2);
  };

  auto dist_square = torus ? dist_square_torus : dist_square_square;

  // generate edges
  for (Node v = 0; v < n; ++v) {
    // cell of current node
    int ix = std::floor(G.points[v].x / cell_size);
    int iy = std::floor(G.points[v].y / cell_size);
    for (int dx = -1; dx <= 1; ++dx) {
      for (int dy = -1; dy <= 1; ++dy) {
        // neighboring cell (mod nr_cells)
        int jx = ix + dx;
        int jy = iy + dy;
        if (jx == -1) jx += nr_cells;
        if (jx == nr_cells) jx -= nr_cells;
        if (jy == -1) jy += nr_cells;
        if (jy == nr_cells) jy -= nr_cells;
        for (auto u : cells[jx][jy]) {
          if (u >= v) {
            continue;
          }
          if (dist_square(u, v) < threshold_square) {
            G.graph.add_edge(u, v);
          }
        }
      }
    }
  }

  return G;
}
