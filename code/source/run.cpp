
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
constexpr auto samples = 1000u;
constexpr auto iterations = 40u;
constexpr auto ns = {
    5000u, 10000u, 15000u, 20000u, 25000u
};
constexpr auto avg_degs = {1.5, 2.0, 4.0, 8.0, 16.0};

// constexpr auto samples = 1u;
// constexpr auto iterations = 20u;
// constexpr auto ns = {1000u};
// constexpr auto avg_degs = {8.0};

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

  std::function<Graph(unsigned, double)> gen_rgg_torus =
      [&generator](unsigned n, double avg_deg) {
        return random_geometric_graph(n, avg_deg, generator, true).graph;
      };

  auto graph_generators = {std::make_pair(gen_er_p, "er_p"),
                           // std::make_pair(gen_er_m, "er_m"),
                           std::make_pair(gen_rgg, "rgg"),
                           std::make_pair(gen_rgg_torus, "rgg_torus")};

  std::cout << "model,n,m,deg_avg,deg_avg_exp,iteration,monochrome,"
               "color_changes,minority_count\n";

  for (auto graph_generator : graph_generators) {
    for (unsigned n : ns) {
      for (double avg_deg : avg_degs) {
        for (unsigned sample = 0; sample < samples; ++sample) {
          Graph G = graph_generator.first(n, avg_deg);

          // auto colors_old = random_colors_equal(n, generator);
          auto colors_old = random_colors(n, generator);

          std::cout << graph_generator.second << "," << G.n() << "," << G.m()
                    << "," << 2.0 * G.m() / G.n() << "," << avg_deg << "," << 0
                    << "," << monochromatic_edges(G, colors_old) << "," << 0
                    << "," << minority_count(colors_old) << "\n";

          for (unsigned i = 1; i <= iterations; ++i) {
            auto colors_new = voting_result(G, colors_old, generator);

            std::cout << graph_generator.second << "," << G.n() << "," << G.m()
                      << "," << 2.0 * G.m() / G.n() << "," << avg_deg << ","
                      << i << "," << monochromatic_edges(G, colors_new) << ","
                      << nr_color_changes(colors_old, colors_new) << ","
                      << minority_count(colors_new) << "\n";

            colors_old = std::move(colors_new);
          }
        }
      }
    }
  }

  return 0;
}
