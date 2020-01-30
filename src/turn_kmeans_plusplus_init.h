#ifndef POKER_HAND_CLUSTERING_TURN_KMEANS_PLUSPLUS_INIT_H_
#define POKER_HAND_CLUSTERING_TURN_KMEANS_PLUSPLUS_INIT_H_

#include <inttypes.h>
#include <vector>

namespace poker {
std::vector<size_t> calc_init_turn_clus_by_kmeans_plusplus(
    const std::vector<std::vector<int32_t>>& turn_histograms,
    size_t turn_clus_size);
}

#endif  // POKER_HAND_CLUSTERING_TURN_KMEANS_PLUSPLUS_INIT_H_
