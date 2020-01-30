#ifndef POKER_HAND_CLUSTERING_TURN_CLUSTER_DISTANCE_H_
#define POKER_HAND_CLUSTERING_TURN_CLUSTER_DISTANCE_H_

#include <inttypes.h>
#include <vector>

namespace poker {
std::vector<std::vector<double>> turn_cluster_distance(
    const std::vector<std::vector<int32_t>>& turn_histograms,
    const std::vector<size_t>& turn_clustering);
}

#endif  // POKER_HAND_CLUSTERING_TURN_CLUSTER_DISTANCE_H_
