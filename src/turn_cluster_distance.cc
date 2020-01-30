#include "turn_cluster_distance.h"
#include <map>
#include "common.h"
#include "mean_histogram.h"

using std::map;
using std::pair;
using std::vector;

namespace {
double calc_mean_hist_distance(const vector<pair<int32_t, double>>& hist1,
                               vector<pair<int32_t, double>> hist2) {
  auto itr1 = hist1.begin();
  auto itr2 = hist2.begin();

  double remaining1 = itr1->second;
  double remaining2 = itr2->second;

  double total_dist = 0.0;
  while (true) {
    if (remaining1 > remaining2) {
      remaining1 -= remaining2;
      total_dist += remaining2 * abs(itr1->first - itr2->first);
      if (++itr2 == hist2.end()) {
        return total_dist;
      }
      remaining2 = itr2->second;
    } else {
      remaining2 -= remaining1;
      total_dist += remaining1 * abs(itr1->first - itr2->first);
      if (++itr1 == hist1.end()) {
        return total_dist;
      }
      remaining1 = itr1->second;
    }
  }
}
}  // namespace

namespace poker {
std::vector<std::vector<double>> calc_turn_cluster_distance(
    const std::vector<std::vector<int32_t>>& turn_histograms,
    const std::vector<size_t>& turn_clustering) {
  size_t turn_size = turn_histograms.size();
  size_t turn_clus_size = turn_clustering.size();

  vector<map<int32_t, int32_t>> sum_histograms(turn_clus_size);
  vector<size_t> mean_histogram_sizes(turn_clus_size, 0);

  auto mhist_calc = MeanHistogramsCalculator<int32_t>(turn_clus_size);
  for (size_t t_idx = 0; t_idx < turn_size; t_idx++) {
    size_t cluster_idx = turn_clustering[t_idx];
    auto turn_histogram = turn_histograms[t_idx];
    mhist_calc.add(cluster_idx, turn_histogram);
  }
  auto mean_histograms = mhist_calc.calc_mean_histograms(turn_histograms);

  vector<vector<double>> turn_cluster_distance(turn_clus_size);
  for (size_t idx1 = 0; idx1 < turn_clus_size; idx1++) {
    turn_cluster_distance.resize(idx1 + 1);
    auto mean1 = mean_histograms[idx1];
    for (size_t idx2 = 0; idx2 < idx1; idx2++) {
      auto mean2 = mean_histograms[idx2];
      turn_cluster_distance[idx1][idx2] = calc_mean_hist_distance(mean1, mean2);
    }
    turn_cluster_distance[idx1][idx1] = 0.0;
  }

  return turn_cluster_distance;
}
}  // namespace poker
