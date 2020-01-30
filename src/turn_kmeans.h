#ifndef POKER_HAND_CLUSTERING_TURN_KMEANS_H_
#define POKER_HAND_CLUSTERING_TURN_KMEANS_H_

#include <inttypes.h>
#include <vector>

namespace poker {
size_t turn_kmeans_once(
    std::vector<size_t>* ptr_turn_clustering,
    const std::vector<std::vector<int32_t>>& turn_histograms,
    size_t turn_clus_size, size_t thread_count);
}

#endif  // POKER_HAND_CLUSTERING_TURN_KMEANS_H_
