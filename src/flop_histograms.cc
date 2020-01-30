#include "flop_histograms.h"
#include <algorithm>
#include "common.h"
using std::vector;

namespace poker {
std::vector<std::vector<size_t>> calc_flop_histograms(
    const std::vector<int32_t>& turn_clustering) {
  uint8_t cards[6];
  hand_indexer_t flop_indexer{};
  hand_indexer_t turn_indexer{};
  hand_indexer_init(2, (const uint8_t[]){2, 3}, &flop_indexer);
  hand_indexer_init(2, (const uint8_t[]){2, 4}, &turn_indexer);
  const size_t flop_size = hand_indexer_size(&flop_indexer, 1);

  vector<vector<size_t>> flop_histograms(flop_size,
                                         vector<size_t>(FLOP_HIST_SIZE));
  for (hand_index_t flop_idx = 0; flop_idx < flop_size; flop_idx++) {
    hand_unindex(&flop_indexer, flop_indexer.rounds - 1, flop_idx, cards);
    size_t flop_hist_idx = 0;
    for (card_t j = 0; j < 52; j++) {
      auto it_end = std::prev(std::end(cards));
      if (std::find(std::begin(cards), it_end, j) != it_end) continue;

      cards[5] = j;

      flop_histograms[flop_idx][flop_hist_idx] =
          turn_clustering[hand_index_last(&turn_indexer, cards)];
      flop_hist_idx++;
    }
    std::sort(flop_histograms[flop_idx].begin(),
              flop_histograms[flop_idx].end());
  }
  hand_indexer_free(&flop_indexer);
  hand_indexer_free(&turn_indexer);

  return flop_histograms;
}
}  // namespace poker
