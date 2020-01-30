#ifndef POKER_HAND_CLUSTERING_FLOP_KMEANS_PLUSPLUS_INIT_H_
#define POKER_HAND_CLUSTERING_FLOP_KMEANS_PLUSPLUS_INIT_H_

#include <inttypes.h>
#include <vector>

namespace poker {
std::vector<size_t> calc_init_flop_clus_by_kmeans_plusplus(
    const std::vector<std::vector<size_t>>& flop_histograms,
    const std::vector<std::vector<double>> turn_clus_distances,
    size_t flop_clus_size);
}

#endif  // POKER_HAND_CLUSTERING_FLOP_KMEANS_PLUSPLUS_INIT_H_
