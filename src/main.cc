#include <sys/stat.h>
#include <fstream>
#include "common.h"
#include "equity.h"
#include "flop_histograms.h"
#include "flop_kmeans.h"
#include "flop_kmeans_plusplus_init.h"
#include "turn_cluster_distance.h"
#include "turn_histograms.h"
#include "turn_kmeans.h"
#include "turn_kmeans_plusplus_init.h"

int main() {
  const size_t turn_clus_size = 5000;
  const size_t flop_clus_size = 1000;
  const size_t thread_count = 4;
  const size_t max_try_count = 200;

  mkdir("data", 0755);
  std::ofstream ofs;

  std::vector<int32_t> equity_vec = poker::calculate_equity();
  ofs.open("data/equity.csv");
  for (int32_t equity : equity_vec) {
    ofs << equity << std::endl;
  }
  ofs.close();

  auto turn_histograms = poker::calc_turn_histograms(equity_vec);
  auto turn_clustering = poker::calc_init_turn_clus_by_kmeans_plusplus(
      turn_histograms, turn_clus_size);

  for (size_t itr_cnt = 0;; itr_cnt++) {
    size_t update_cnt = poker::turn_kmeans_once(
        &turn_clustering, turn_histograms, turn_clus_size, thread_count);
    printf("[k-means for turn] iteration %ld: updated %ld clusters", itr_cnt,
           update_cnt);
    if (update_cnt == 0 || itr_cnt == max_try_count) {
      ofs.open("turn_kmeans.csv");
      for (size_t cluster_no : turn_clustering) {
        ofs << cluster_no << std::endl;
      }
      ofs.close();
    }
  }

  auto turn_cluster_distances =
      poker::turn_cluster_distance(turn_histograms, turn_clustering);
  auto flop_histograms = poker::calc_flop_histograms(turn_clustering);
  auto flop_clustering = poker::calc_init_flop_clus_by_kmeans_plusplus(
      flop_histograms, turn_cluster_distances, flop_clus_size);

  for (size_t itr_cnt = 0;; itr_cnt++) {
    size_t update_cnt = poker::flop_kmeans_once(
        &flop_clustering, flop_histograms, turn_cluster_distances,
        flop_clus_size, thread_count);
    printf("[k-means for flop] iteration %ld: updated %ld clusters", itr_cnt,
           update_cnt);
    if (update_cnt == 0 || itr_cnt == max_try_count) {
      ofs.open("flop_kmeans.csv");
      for (size_t cluster_no : flop_clustering) {
        ofs << cluster_no << std::endl;
      }
      ofs.close();
    }
  }
  return 0;
}
