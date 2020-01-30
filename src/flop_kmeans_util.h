
#ifndef POKER_HAND_CLUSTERING_FLOP_KMEANS_UTIL_H_
#define POKER_HAND_CLUSTERING_FLOP_KMEANS_UTIL_H_
#include <optional>
#include <unordered_map>
#include <vector>

namespace poker {
// calculate the (approximate) distance between flop_histogram and mean_map
// and return the distance. however, if the distance is proved to be bigger than
// abort_threshold_dist, returns std::nullopt.
std::optional<double> calc_flop_hist_mean_dist_opt(
    const std::vector<size_t>& flop_histogram,
    const std::unordered_map<size_t, double>& mean_map,
    const std::vector<std::vector<std::pair<double, size_t>>>
        sorted_distances_with_clusters,
    const double abort_threshold_dist);

inline double get_turn_clus_dist(
    const std::vector<std::vector<double>>& turn_clus_distances,
    size_t clus_idx1, size_t clus_idx2) {
  size_t max_idx = std::max(clus_idx1, clus_idx2);
  size_t min_idx = std::min(clus_idx1, clus_idx2);
  return turn_clus_distances[max_idx][min_idx];
}

std::vector<std::vector<std::pair<double, size_t>>>
calc_dist_clus_idx_pairs_sorted_by_dist(
    const std::vector<std::pair<size_t, double>>& means,
    const std::vector<std::vector<double>>& turn_clus_distances);

inline std::unordered_map<size_t, double> convert_means_to_unordered_map(
    const std::vector<std::pair<size_t, double>>& means) {
  std::unordered_map<size_t, double> ret_mean_map;
  for (const auto& [k, v] : means) {
    ret_mean_map[k] = v;
  }
  return ret_mean_map;
}
}  // namespace poker

#endif  // POKER_HAND_CLUSTERING_FLOP_KMEANS_UTIL_H_
