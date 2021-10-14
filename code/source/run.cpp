
#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <random>
#include <sstream>
#include <utility>
#include <vector>

#include "CLI11.hpp"
#include "er.hpp"
#include "rgg.hpp"
#include "voting.hpp"

// constexpr auto seed = 17u;
// constexpr auto samples = 1000u;
// constexpr auto iterations = 40u;
// constexpr auto ns = {5000u, 10000u, 15000u, 20000u, 25000u};
// constexpr auto avg_degs = {2.0, 4.0, 8.0, 16.0, 32.0};

constexpr auto header =
    "model,n,m,deg_avg,deg_avg_exp,seed,iteration,monochrome,color_changes,"
    "minority_count";

void run(std::string model, unsigned n, unsigned avg_deg, unsigned seed,
         unsigned iterations, std::default_random_engine& generator,
         bool skip_intermediate) {
  std::function<Graph(unsigned, double)> graph_generator;

  if (model == "rgg_torus") {
    graph_generator = [&generator](unsigned n, double avg_deg) {
      return random_geometric_graph(n, avg_deg, generator, true).graph;
    };
  } else if (model == "rgg_square") {
    graph_generator = [&generator](unsigned n, double avg_deg) {
      return random_geometric_graph(n, avg_deg, generator).graph;
    };
  } else if (model == "er_p") {
    graph_generator = [&generator](unsigned n, double avg_deg) {
      return erdos_renyi_p(n, avg_deg_to_p(n, avg_deg), generator);
    };
  } else if (model == "er_m") {
    graph_generator = [&generator](unsigned n, double avg_deg) {
      return erdos_renyi_m(n, avg_deg * n / 2, generator);
    };
  }

  Graph G = graph_generator(n, avg_deg);

  auto colors_old = random_colors(n, generator);

  std::cout << model << "," << G.n() << "," << G.m() << ","
            << 2.0 * G.m() / G.n() << "," << avg_deg << "," << seed << "," << 0
            << "," << monochromatic_edges(G, colors_old) << "," << 0 << ","
            << minority_count(colors_old) << "\n";

  for (unsigned i = 1; i <= iterations; ++i) {
    auto colors_new = voting_result(G, colors_old, generator);

    if (!skip_intermediate || i == iterations) {
      std::cout << model << "," << G.n() << "," << G.m() << ","
                << 2.0 * G.m() / G.n() << "," << avg_deg << "," << seed << ","
                << i << "," << monochromatic_edges(G, colors_new) << ","
                << nr_color_changes(colors_old, colors_new) << ","
                << minority_count(colors_new) << "\n";
    }

    colors_old = std::move(colors_new);
  }
}

int main(int argc, char* argv[]) {
  CLI::App app("geometric flip schelling");

  std::string model;
  app.add_option(
         "-m,--model", model,
         "Random graph model.  Options are random geometric graph on the "
         "torus or in the square, G(n, p), and G(n, m).")
      ->default_val("rgg_torus")
      ->check(CLI::IsMember({"rgg_torus", "rgg_square", "er_p", "er_m"}));

  unsigned n;
  app.add_option("-n,--number_vertices", n, "Number of vertices of the graph.")
      ->default_val(10000u);

  double avg_deg;
  app.add_option("-d,--avg_deg", avg_deg, "Expected average degree.")
      ->default_val(16.0);

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  app.add_option("-s,--seed", seed, "Seed for generating randomness.");

  unsigned repetitions;
  app.add_option("-r,--repetitions", repetitions, "Number of repetitions.")
      ->default_val(1u);

  unsigned iterations;
  app.add_option("-i,--iterations", iterations,
                 "Number of iterations of the flip Schelling process.")
      ->default_val(50u);

  bool skip_intermediate;
  app.add_flag("--skip-intermediate", skip_intermediate,
               "Only output the initial configuration and the result after the "
               "last iteration.")
      ->default_val(false);

  bool only_header, no_header;
  app.add_flag("--only-header", only_header, "Only print the csv-header.")
      ->default_val(false);
  app.add_flag("--no-header", no_header,
               "Only print the results and not the csv-header.")
      ->default_val(false);

  CLI11_PARSE(app, argc, argv);

  if (!no_header) {
    std::cout << header << "\n";
  }
  if (only_header) {
    return 0;
  }

  std::default_random_engine generator(seed);
  for (unsigned i = 0; i < repetitions; ++i) {
    run(model, n, avg_deg, seed, iterations, generator, skip_intermediate);
  }

  return 0;
}
