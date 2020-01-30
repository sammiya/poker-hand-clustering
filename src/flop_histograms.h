#ifndef POKER_HAND_CLUSTERING_FLOP_HISTOGRAMS_H_
#define POKER_HAND_CLUSTERING_FLOP_HISTOGRAMS_H_
#include <vector>
#include "inttypes.h"

namespace poker {
std::vector<std::vector<size_t>> calc_flop_histograms(
    const std::vector<size_t>& turn_clustering);
}
#endif  // POKER_HAND_CLUSTERING_FLOP_HISTOGRAMS_H_
