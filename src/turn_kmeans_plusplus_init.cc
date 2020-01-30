#include "turn_kmeans_plusplus_init.h"
#include <algorithm>
#include <random>
#include "common.h"

namespace {
int32_t calc_dist_between_turn_hists(const std::vector<int32_t>& turn_hist1,
                                     const std::vector<int32_t>& turn_hist2) {
  int32_t dist = 0;
  for (size_t th_idx = 0; th_idx < poker::TURN_HIST_SIZE; th_idx++) {
    dist += abs(turn_hist1[th_idx] - turn_hist2[th_idx]);
  }
  return dist;
}

auto squared = [](auto d) { return d * d; };
}  // namespace

namespace poker {
std::vector<size_t> calc_init_turn_clus_by_kmeans_plusplus(
    const std::vector<std::vector<int32_t>>& turn_histograms,
    size_t turn_clus_size) {
  size_t turn_size = turn_histograms.size();

  std::vector<size_t> turn_clustering(turn_size);
  std::vector<int32_t> min_dist_by_turn_idx(turn_size, INT32_MAX);

  std::mt19937 mt{std::random_device{}()};

  // choose first turn histogram randomly
  std::vector<int32_t> chosen_hist = turn_histograms[(
      std::uniform_int_distribution(0, static_cast<int32_t>(turn_size))(mt))];

  for (size_t tc_idx = 0; tc_idx < turn_clus_size; tc_idx++) {
    std::vector<int64_t> cumulative_dist_squared_vec(turn_size);
    for (size_t t_idx = 0; t_idx < turn_size; t_idx++) {
      int32_t dist =
          calc_dist_between_turn_hists(chosen_hist, turn_histograms[t_idx]);
      if (dist < min_dist_by_turn_idx[t_idx]) {
        min_dist_by_turn_idx[t_idx] = dist;
        turn_clustering[t_idx] = tc_idx;
      }
      cumulative_dist_squared_vec[t_idx] =
          squared(min_dist_by_turn_idx[t_idx]) +
          (t_idx == 0 ? 0 : cumulative_dist_squared_vec[t_idx - 1]);
    }

    if (tc_idx == turn_clus_size - 1) {
      return turn_clustering;
    }

    // choose next histogram randomly with probability
    // propotional to distance squared
    int64_t random_no = std::uniform_int_distribution<int64_t>(
        0, cumulative_dist_squared_vec[turn_size - 1] - 1)(mt);

    auto it = std::upper_bound(cumulative_dist_squared_vec.begin(),
                               cumulative_dist_squared_vec.end(), random_no);
    auto chosen_idx = it - cumulative_dist_squared_vec.begin();
    chosen_hist = turn_histograms[chosen_idx];
  }
}
}  // namespace poker
