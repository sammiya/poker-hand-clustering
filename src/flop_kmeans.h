#ifndef POKER_HAND_CLUSTERING_FLOP_KMEANS_H_
#define POKER_HAND_CLUSTERING_FLOP_KMEANS_H_

#include <inttypes.h>
#include <vector>

namespace poker {
size_t flop_kmeans_once(
    std::vector<size_t>* ptr_flop_clustering,
    const std::vector<std::vector<size_t>>& flop_histograms,
    const std::vector<std::vector<double>>& turn_clus_distances,
    size_t flop_clus_size, size_t thread_count);
}
#endif  // POKER_HAND_CLUSTERING_FLOP_KMEANS_H_
