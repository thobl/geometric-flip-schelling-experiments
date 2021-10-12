
#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <random>
#include <sstream>
#include <utility>
#include <vector>

#include "er.hpp"
#include "rgg.hpp"
#include "voting.hpp"

constexpr auto seed = 17u;
constexpr auto iterations = 1000u;
constexpr auto ns = {
    5000u, 10000u, 15000u, 20000u, 25000u
};
constexpr auto avg_degs = {1.5, 2.0, 4.0, 8.0, 16.0};


// constexpr auto iterations = 10u;
// constexpr auto ns = {
//     10000u, 20000u, 30000u, 40000u, 50000u,
// };
// constexpr auto avg_degs = {1.1, 1.5, 2.0, 5.0};

int main(int argc, char* argv[]) {
  (void)argc;
  (void)argv;
  std::default_random_engine generator(seed);

  std::function<Graph(unsigned, double)> gen_er_p =
      [&generator](unsigned n, double avg_deg) {
        return erdos_renyi_p(n, avg_deg_to_p(n, avg_deg), generator);
      };

  std::function<Graph(unsigned, double)> gen_er_m =
      [&generator](unsigned n, double avg_deg) {
        return erdos_renyi_m(n, avg_deg * n / 2, generator);
      };

  std::function<Graph(unsigned, double)> gen_rgg =
      [&generator](unsigned n, double avg_deg) {
        return random_geometric_graph(n, avg_deg, generator).graph;
      };

  auto graph_generators = {std::make_pair(gen_er_p, "er_p"),
                           // std::make_pair(gen_er_m, "er_m"),
                           std::make_pair(gen_rgg, "rgg")};

  std::cout
      << "model,n,m,deg_avg,deg_avg_exp,monochrome_before,monochrome_after\n";

  for (auto graph_generator : graph_generators) {
    for (unsigned n : ns) {
      for (double avg_deg : avg_degs) {
        for (unsigned i = 0; i < iterations; ++i) {
          Graph G = graph_generator.first(n, avg_deg);
          // auto colors_before = random_colors_equal(n, generator);
          auto colors_before = random_colors(n, generator);
          auto colors_after = voting_result(G, colors_before, generator);

          std::cout << graph_generator.second << "," << G.n() << "," << G.m()
                    << "," << 2.0 * G.m() / G.n() << "," << avg_deg << ","
                    << monochromatic_edges(G, colors_before) << ","
                    << monochromatic_edges(G, colors_after) << "\n";
        }
      }
    }
  }

  return 0;
}
