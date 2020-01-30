#ifndef POKER_HAND_CLUSTERING_TURN_HISTOGRAMS_H_
#define POKER_HAND_CLUSTERING_TURN_HISTOGRAMS_H_

#include <inttypes.h>
#include <vector>
#include "common.h"

namespace poker {
std::vector<std::vector<int32_t>> calc_turn_histograms(
    const std::vector<int32_t>& equity_vec);
}

#endif  // POKER_HAND_CLUSTERING_TURN_HISTOGRAMS_H_
