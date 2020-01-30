#include "flop_kmeans_plusplus_init.h"
#include <algorithm>
#include <cfloat>
#include <random>
#include <unordered_map>
#include "common.h"
#include "flop_kmeans_util.h"
using std::pair;
using std::unordered_map;
using std::vector;

namespace {
vector<pair<size_t, double>> regard_flop_hist_as_mean(
    const vector<size_t>& flop_hist) {
  size_t flop_hist_size = flop_hist.size();
  vector<pair<size_t, double>> mean(flop_hist_size);
  for (size_t hist_idx = 0; hist_idx < flop_hist_size; hist_idx++) {
    mean[hist_idx] = std::make_pair(flop_hist[hist_idx], 1.0);
  }
  return mean;
}
auto squared = [](auto d) { return d * d; };
}  // namespace

namespace poker {
vector<size_t> calc_init_flop_clus_by_kmeans_plusplus(
    const vector<vector<size_t>>& flop_histograms,
    const vector<vector<double>> turn_clus_distances, size_t flop_clus_size) {
  size_t flop_size = flop_histograms.size();
  vector<double> min_dists(flop_size, DBL_MAX);
  vector<size_t> flop_clustering(flop_size);
  std::mt19937 mt{std::random_device{}()};

  size_t turn_clus_size = turn_clus_distances.size();

  // choose first turn histogram randomly
  std::vector<size_t> chosen_hist = flop_histograms[(
      std::uniform_int_distribution(0, static_cast<int32_t>(flop_size))(mt))];

  for (size_t fc_idx = 0; fc_idx < flop_clus_size; fc_idx++) {
    vector<pair<size_t, double>> means = regard_flop_hist_as_mean(chosen_hist);
    vector<vector<pair<double, size_t>>> sorted_distances_with_clusters =
        calc_dist_clus_idx_pairs_sorted_by_dist(means, turn_clus_distances);

    auto mean_map = convert_means_to_unordered_map(means);

    std::vector<int64_t> cumulative_dist_squared_vec(flop_size);

    for (size_t flop_idx = 0; flop_idx < flop_size; flop_idx++) {
      auto dist_opt = calc_flop_hist_mean_dist_opt(
          flop_histograms[flop_idx], mean_map, sorted_distances_with_clusters,
          min_dists[flop_idx]);

      if (dist_opt) {
        min_dists[flop_idx] = *dist_opt;
        flop_clustering[flop_idx] = fc_idx;
      }
      cumulative_dist_squared_vec[flop_idx] =
          squared(min_dists[flop_idx]) +
          (flop_idx == 0 ? 0 : cumulative_dist_squared_vec[flop_idx - 1]);
    }
    if (fc_idx == flop_clus_size - 1) {
      return flop_clustering;
    }

    // choose next histogram randomly with probability
    // propotional to distance squared
    int64_t random_no = std::uniform_int_distribution<int64_t>(
        0, cumulative_dist_squared_vec[flop_size - 1] - 1)(mt);

    auto it = std::upper_bound(cumulative_dist_squared_vec.begin(),
                               cumulative_dist_squared_vec.end(), random_no);
    auto chosen_idx = it - cumulative_dist_squared_vec.begin();
    chosen_hist = flop_histograms[chosen_idx];
  }
}
}  // namespace poker
