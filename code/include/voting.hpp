#include <random>
#include <vector>

class Graph;

std::vector<bool> random_colors(unsigned n,
                                std::default_random_engine& generator);

std::vector<bool> random_colors_equal(unsigned n,
                                      std::default_random_engine& generator);

unsigned nr_color_changes(const std::vector<bool>& colors_old,
                          const std::vector<bool>& colors_new);

unsigned minority_count(const std::vector<bool>& colors);

unsigned monochromatic_edges(const Graph& G, const std::vector<bool>& colors);

std::vector<bool> voting_result(const Graph& G, const std::vector<bool>& colors,
                                std::default_random_engine& generator);
