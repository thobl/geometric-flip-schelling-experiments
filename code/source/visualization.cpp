
#include <algorithm>
#include <iostream>
#include <random>

#include "ipe.hpp"
#include "rgg.hpp"
#include "voting.hpp"

constexpr auto seed = 17u;
constexpr auto n = 500u;
constexpr auto avg_deg = 10.0;

constexpr auto max_iter = 20u;

constexpr auto color0 = "0.6 0.6 0.6";
constexpr auto color1 = "0.8352941 0.3686275 0";
constexpr auto color2 = "0.0 0.4470588235294118 0.6980392156862745>";
constexpr auto canvas_size = 150.0;
constexpr auto line_styling = "pen=\"1.0\" opacity=\"60%\"";

void draw_graph(const GeometricGraph& GG, const std::vector<bool> colors,
                IpeFile& ipe) {
  const Graph& G = GG.graph;
  for (Node u = 0; u < G.n(); ++u) {
    for (Node v : G.neighbors(u)) {
      if (u < v) continue;
      auto color = color0;
      if (colors[u] == colors[v]) {
        color = colors[u] ? color1 : color2;
      }
      auto x1 = GG.points[u].x;
      auto y1 = GG.points[u].y;
      auto x2 = GG.points[v].x;
      auto y2 = GG.points[v].y;

      if (GG.torus && (std::abs(x1 - x2) > 0.5 || std::abs(y1 - y2) > 0.5)) {
        ipe.start_group_with_clipping(0, 0, 1, 1);
        // line starting at (x1, y1)
        auto x_shift = std::abs(x1 - x2) < 0.5 ? 0 : (x1 < x2 ? -1.0 : 1.0);
        auto y_shift = std::abs(y1 - y2) < 0.5 ? 0 : (y1 < y2 ? -1.0 : 1.0);
        ipe.line(x1, y1, x2 + x_shift, y2 + y_shift, color, line_styling);

        // line starting at (x1, y1)
        x_shift = std::abs(x1 - x2) < 0.5 ? 0 : (x1 < x2 ? 1.0 : -1.0);
        y_shift = std::abs(y1 - y2) < 0.5 ? 0 : (y1 < y2 ? 1.0 : -1.0);
        ipe.line(x1 + x_shift, y1 + y_shift, x2, y2, color, line_styling);
        ipe.end_group();
      } else {
        ipe.line(x1, y1, x2, y2, color, line_styling);
      }
    }
  }

  for (Node u = 0; u < G.n(); ++u) {
    auto x = GG.points[u].x;
    auto y = GG.points[u].y;
    auto d = 1.8 / canvas_size;
    if (GG.torus && (x < d || x > 1 - d || y < d || y > 1 - d)) {
      ipe.start_group_with_clipping(0, 0, 1, 1);
      ipe.point(x, y, colors[u] ? color1 : color2);
      std::vector<double> x_shifts = {0.0};
      if (x < d) x_shifts.push_back(1);
      if (x > 1 - d) x_shifts.push_back(-1);
      std::vector<double> y_shifts = {0.0};
      if (y < d) y_shifts.push_back(1);
      if (y > 1 - d) y_shifts.push_back(-1);
      for (double x_shift : x_shifts) {
        for (double y_shift : y_shifts) {
          ipe.point(x + x_shift, y + y_shift, colors[u] ? color1 : color2);
        }
      }
      ipe.end_group();
    } else {
      ipe.point(x, y, colors[u] ? color1 : color2);
    }
  }

  ipe.box(0, 0, 1, 1);
}

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  std::default_random_engine generator(seed);

  GeometricGraph GG = random_geometric_graph(n, avg_deg, generator, true);
  // GeometricGraph GG = random_geometric_graph(n, avg_deg, generator);
  Graph& G = GG.graph;

  IpeFile ipe("testoutput.ipe", canvas_size);
  auto colors_old = random_colors(G.n(), generator);
  for (auto i = 0u; i < max_iter; ++i) {
    draw_graph(GG, colors_old, ipe);
    ipe.new_page();
    auto colors_new = voting_result(G, colors_old, generator);
    if (colors_new == colors_old) {
      break;
    }

    colors_old = std::move(colors_new);
  }

  return 0;
}
