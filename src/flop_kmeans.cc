#include "flop_kmeans.h"
#include <cfloat>
#include <thread>
#include "flop_kmeans_util.h"
#include "mean_histogram.h"

using std::pair;
using std::vector;

namespace {
size_t calc_nearest_cluster_idx(
    const std::vector<size_t>& flop_hist,
    std::vector<std::vector<std::pair<size_t, double>>> mean_histograms,
    const std::vector<std::vector<std::pair<double, size_t>>>
        sorted_distances_with_clusters) {
  double min_dist = DBL_MAX;
  size_t min_dist_idx = 0;
  for (size_t mh_idx = 0; mh_idx < mean_histograms.size(); mh_idx++) {
    auto mean_map =
        poker::convert_means_to_unordered_map(mean_histograms[mh_idx]);
    auto dist_opt = poker::calc_flop_hist_mean_dist_opt(
        flop_hist, mean_map, sorted_distances_with_clusters, min_dist);
    if (dist_opt) {
      min_dist = *dist_opt;
      min_dist_idx = mh_idx;
    }
  }
  return min_dist_idx;
}

}  // namespace

namespace poker {
size_t flop_kmeans_once(
    std::vector<size_t>* ptr_flop_clustering,
    const std::vector<std::vector<size_t>>& flop_histograms,
    const std::vector<std::vector<double>>& turn_clus_distances,
    size_t flop_clus_size, size_t thread_count) {
  size_t flop_size = flop_histograms.size();

  auto mhist_calc = MeanHistogramsCalculator<size_t>(flop_clus_size);
  for (size_t f_idx = 0; f_idx < flop_histograms.size(); f_idx++) {
    size_t cluster_idx = (*ptr_flop_clustering)[f_idx];
    mhist_calc.add(cluster_idx, flop_histograms[f_idx]);
  }
  auto mean_histograms = mhist_calc.calc_mean_histograms(flop_histograms);
  std::vector<std::thread> threads;
  std::vector<size_t> update_cnts(thread_count, 0);

  for (size_t thread_idx = 0; thread_idx < thread_count; thread_idx++) {
    auto thread = std::thread(
        [&ptr_flop_clustering,                        // will be updated
             & update_cnt = update_cnts[thread_idx],  // will be updated
         &mean_histograms, &flop_histograms,
         &turn_clus_distances](size_t flop_idx_begin, size_t flop_idx_end) {
          for (size_t f_idx = flop_idx_begin; f_idx < flop_idx_end; f_idx++) {
            std::vector<std::pair<size_t, double>> mean =
                mean_histograms[f_idx];
            vector<vector<pair<double, size_t>>> sorted_dists_with_clusters =
                calc_dist_clus_idx_pairs_sorted_by_dist(mean,
                                                        turn_clus_distances);
            size_t nearest_idx = calc_nearest_cluster_idx(
                flop_histograms[f_idx], mean_histograms,
                sorted_dists_with_clusters);
            if (nearest_idx != (*ptr_flop_clustering)[f_idx]) {
              update_cnt++;
              (*ptr_flop_clustering)[f_idx] = nearest_idx;
            }
          }
        },
        // specify flop index range that the thread is responsible for
        flop_size * thread_idx / thread_count,
        flop_size * (thread_idx + 1) / thread_count);
    threads.emplace_back(std::move(thread));
  }

  for (auto&& thread : threads) {
    thread.join();
  }
  return std::accumulate(update_cnts.begin(), update_cnts.end(), 0);
}

}  // namespace poker
