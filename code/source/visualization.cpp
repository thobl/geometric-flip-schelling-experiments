
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
      ipe.line(GG.points[u].x, GG.points[u].y, GG.points[v].x, GG.points[v].y,
               color);
    }
  }

  for (Node u = 0; u < G.n(); ++u) {
    ipe.point(GG.points[u].x, GG.points[u].y, colors[u] ? color1 : color2);
  }
}

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  std::default_random_engine generator(seed);

  GeometricGraph GG = random_geometric_graph(n, avg_deg, generator);
  Graph& G = GG.graph;

  IpeFile ipe("testoutput.ipe", 300);
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
