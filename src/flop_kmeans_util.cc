#include "flop_kmeans_util.h"
#include <algorithm>
using std::pair;
using std::unordered_map;
using std::vector;

namespace poker {

std::vector<std::vector<std::pair<double, size_t>>>
calc_dist_clus_idx_pairs_sorted_by_dist(
    const std::vector<std::pair<size_t, double>>& means,
    const std::vector<std::vector<double>>& turn_clus_distances) {
  size_t turn_clus_size = turn_clus_distances.size();
  std::vector<std::vector<std::pair<double, size_t>>> dist_clus_idx_pairs(
      turn_clus_size);
  size_t flop_hist_size = means.size();

  for (size_t tc_idx = 0; tc_idx < turn_clus_size; tc_idx++) {
    dist_clus_idx_pairs[tc_idx].reserve(flop_hist_size);
    for (const auto& mean : means) {
      double dist = get_turn_clus_dist(turn_clus_distances, tc_idx, mean.first);
      dist_clus_idx_pairs[tc_idx].emplace_back(dist, mean.first);
    }
    std::sort(dist_clus_idx_pairs[tc_idx].begin(),
              dist_clus_idx_pairs[tc_idx].end());
  }
  return dist_clus_idx_pairs;
}

std::optional<double> calc_flop_hist_mean_dist_opt(
    const std::vector<size_t>& flop_histogram,
    const std::unordered_map<size_t, double>& mean_map,
    const std::vector<std::vector<std::pair<double, size_t>>>
        sorted_distances_with_clusters,
    const double abort_threshold_dist) {
  size_t flop_hist_size = flop_histogram.size();
  std::unordered_map<size_t, double> mean_remaining = mean_map;
  vector<double> targets(flop_hist_size, 1.0);
  vector<bool> done(flop_hist_size, false);
  double total_cost = 0.0;

  for (size_t hist_idx1 = 0; hist_idx1 < flop_hist_size; hist_idx1++) {
    for (size_t hist_idx2 = 0; hist_idx2 < flop_hist_size; hist_idx2++) {
      if (done[hist_idx2]) {
        continue;
      }
      size_t point_cluster = flop_histogram[hist_idx2];
      size_t mean_cluster =
          sorted_distances_with_clusters[point_cluster][hist_idx1].second;
      double amount_remaining = mean_remaining[mean_cluster];
      if (amount_remaining == 0.0) {
        continue;
      }
      double distance =
          sorted_distances_with_clusters[point_cluster][hist_idx1].first;
      if (amount_remaining < targets[hist_idx2]) {
        total_cost += amount_remaining * distance;
        targets[hist_idx2] -= amount_remaining;
        mean_remaining[mean_cluster] = 0.0;
      } else {
        total_cost += targets[hist_idx2] * distance;
        targets[hist_idx2] = 0.0;
        mean_remaining[mean_cluster] -= targets[hist_idx2];
        done[hist_idx2] = true;
      }
    }
    if (total_cost > abort_threshold_dist) {
      return std::nullopt;
    }
  }
  return total_cost;
}
}  // namespace poker
