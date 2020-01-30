#include "turn_kmeans.h"
#include <cfloat>
#include <numeric>
#include <optional>
#include <thread>
#include "common.h"
#include "mean_histogram.h"
#include "turn_kmeans_plusplus_init.h"

namespace {

// calculate the distance between turn_histogram and mean_histogram and return
// the distance. however, if the distance is proved to be bigger than
// abort_threshold_dist, returns std::nullopt.
std::optional<double> calc_thist_mhist_dist_opt(
    const std::vector<int32_t>& turn_histogram,
    const std::vector<std::pair<int32_t, double>>& mean_histogram,
    const double abort_threshold_dist) {
  double total_dist = 0.0;
  auto itr = mean_histogram.begin();
  int k = 0;
  double remaining_t = 1.0;
  double remaining_m = itr->second;
  while (true) {
    if (remaining_t > remaining_m) {
      remaining_t -= remaining_m;
      total_dist += remaining_m * abs(turn_histogram[k] - itr->first);
      if (total_dist >= abort_threshold_dist) return std::nullopt;
      itr++;
      if (itr == mean_histogram.end()) {
        break;
      }
      remaining_m = itr->second;
    } else {
      remaining_m -= remaining_t;
      total_dist += remaining_t * abs(turn_histogram[k] - itr->first);
      if (total_dist >= abort_threshold_dist) return std::nullopt;
      k++;
      if (k == poker::TURN_HIST_SIZE) {
        break;
      }
      remaining_t = 1.0;
    }
  }
  return total_dist;
}

size_t calc_nearest_cluster_idx(
    const std::vector<int32_t>& turn_hist,
    std::vector<std::vector<std::pair<int32_t, double>>> mean_histograms) {
  double min_dist = DBL_MAX;
  size_t min_dist_idx = 0;
  for (size_t mh_idx = 0; mh_idx < mean_histograms.size(); mh_idx++) {
    auto mean_hist = mean_histograms[mh_idx];
    auto dist_opt = calc_thist_mhist_dist_opt(turn_hist, mean_hist, min_dist);
    if (dist_opt) {
      min_dist = *dist_opt;
      min_dist_idx = mh_idx;
    }
  }
  return min_dist_idx;
}
}  // namespace

namespace poker {
size_t turn_kmeans_once(
    std::vector<size_t>* ptr_turn_clustering,
    const std::vector<std::vector<int32_t>>& turn_histograms,
    size_t turn_clus_size, size_t thread_count) {
  auto mhist_calc = MeanHistogramsCalculator<int32_t>(turn_clus_size);
  for (size_t t_idx = 0; t_idx < turn_histograms.size(); t_idx++) {
    size_t cluster_idx = (*ptr_turn_clustering)[t_idx];
    auto turn_histogram = turn_histograms[t_idx];
    mhist_calc.add(cluster_idx, turn_histogram);
  }
  auto mean_histograms = mhist_calc.calc_mean_histograms(turn_histograms);
  std::vector<std::thread> threads;
  std::vector<size_t> update_cnts(thread_count, 0);
  size_t turn_size = turn_histograms.size();

  for (size_t thread_idx = 0; thread_idx < thread_count; thread_idx++) {
    auto thread = std::thread(
        [&ptr_turn_clustering,                        // will be updated
             & update_cnt = update_cnts[thread_idx],  // will be updated
         &mean_histograms,
         &turn_histograms](size_t turn_idx_begin, size_t turn_idx_end) {
          for (size_t t_idx = turn_idx_begin; t_idx < turn_idx_end; t_idx++) {
            size_t nearest_idx = calc_nearest_cluster_idx(
                turn_histograms[t_idx], mean_histograms);
            if (nearest_idx != ptr_turn_clustering->at(t_idx)) {
              update_cnt++;
              ptr_turn_clustering->at(t_idx) = nearest_idx;
            }
          }
        },
        // specify turn index range that the thread is responsible for
        turn_size * thread_idx / thread_count,
        turn_size * (thread_idx + 1) / thread_count);
    threads.emplace_back(std::move(thread));
  }

  for (auto&& thread : threads) {
    thread.join();
  }
  return std::accumulate(update_cnts.begin(), update_cnts.end(), 0);
}
}  // namespace poker
