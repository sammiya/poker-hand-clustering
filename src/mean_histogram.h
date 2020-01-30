#ifndef POKER_HAND_CLUSTERING_TURN_MEAN_HISTOGRAM_H_
#define POKER_HAND_CLUSTERING_TURN_MEAN_HISTOGRAM_H_

#include <inttypes.h>
#include <algorithm>
#include <map>
#include <random>
#include <vector>

namespace poker {
template <typename IntType>
class MeanHistogramsCalculator {
  size_t _clus_size;
  // _sum_histograms[cluster_idx][histogram_idx(equity or turn_cluster_idx)] =
  // summed frequency
  std::vector<std::map<size_t, size_t>> _sum_histograms;
  std::vector<size_t> _added_hist_counts;

 public:
  MeanHistogramsCalculator(size_t clus_size)
      : _clus_size(clus_size),
        _sum_histograms(clus_size),
        _added_hist_counts(clus_size) {}

  inline void add(size_t c_idx, const std::vector<IntType>& histogram) {
    _added_hist_counts[c_idx]++;
    for (IntType value : histogram) {
      _sum_histograms[c_idx][static_cast<size_t>(value)]++;
    }
  }

  inline std::vector<std::vector<std::pair<IntType, double>>>
  calc_mean_histograms(
      std::vector<std::vector<IntType>>
          histograms  // to choose randomly from this when empty
      ) const {
    std::vector<std::vector<std::pair<IntType, double>>> mean_histograms(
        _clus_size);

    // prepare random index vector whose all elements are unique
    // for when the cluster is empty
    std::vector<size_t> rand_idxes;
    std::mt19937 mt{std::random_device{}()};

    // if cluster size is enough big
    if (_clus_size < histograms.size() * 0.3) {
      rand_idxes.resize(histograms.size());
      std::iota(rand_idxes.begin(), rand_idxes.end(), 0);
    } else {
      std::uniform_int_distribution<int> distribution(0, _clus_size);
      const size_t size_extra = _clus_size * 1.2;
      while (rand_idxes.size() < _clus_size) {
        while (rand_idxes.size() < size_extra) {
          rand_idxes.emplace_back(distribution(mt));
        }
        std::sort(rand_idxes.begin(), rand_idxes.end());
        rand_idxes.erase(std::unique(rand_idxes.begin(), rand_idxes.end()),
                         rand_idxes.end());
      }
    }
    std::shuffle(rand_idxes.begin(), rand_idxes.end(), mt);
    rand_idxes.resize(_clus_size);
    auto random_idx_it = rand_idxes.begin();

    for (size_t c_idx = 0; c_idx < _clus_size; c_idx++) {
      auto sum_histogram = _sum_histograms[c_idx];
      if (sum_histogram.empty()) {
        auto substitute_hist = histograms[*random_idx_it];
        random_idx_it++;
        for (IntType hist_val : substitute_hist) {
          mean_histograms[c_idx].emplace_back(std::make_pair(hist_val, 1.0));
        }
      } else {
        for (const auto& [hist_idx, frequency] : sum_histogram) {
          mean_histograms[c_idx].emplace_back(std::make_pair(
              hist_idx,
              static_cast<double>(frequency) / _added_hist_counts[c_idx]));
        }
      }
    }
    return mean_histograms;
  }
};
}  // namespace poker

#endif  // POKER_HAND_CLUSTERING_TURN_MEAN_HISTOGRAM_H_
