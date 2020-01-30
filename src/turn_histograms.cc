#include "turn_histograms.h"
#include <algorithm>
#include "common.h"

namespace poker {
std::vector<std::vector<int32_t>> calc_turn_histograms(
    const std::vector<int32_t>& equity_vec) {
  hand_indexer_t turn_indexer{};
  hand_indexer_t river_indexer{};
  hand_indexer_init(2, (const uint8_t[]){2, 4}, &turn_indexer);
  hand_indexer_init(2, (const uint8_t[]){2, 5}, &river_indexer);
  const size_t turn_size = hand_indexer_size(&turn_indexer, 1);

  std::vector<std::vector<int32_t>> turn_histograms(
      turn_size, std::vector<int32_t>(TURN_HIST_SIZE));

  uint8_t cards[7];

  for (hand_index_t index = 0; index < turn_size; index++) {
    hand_unindex(&turn_indexer, turn_indexer.rounds - 1, index, cards);
    size_t i = 0;
    for (card_t j = 0; j < 52; j++) {
      auto it_end = std::prev(std::end(cards));
      if (std::find(std::begin(cards), it_end, j) != it_end) continue;

      cards[6] = j;
      turn_histograms[index][i] =
          equity_vec[hand_index_last(&river_indexer, cards)];
      i++;
    }
    std::sort(turn_histograms[index].begin(), turn_histograms[index].end());
  }

  hand_indexer_free(&turn_indexer);
  hand_indexer_free(&river_indexer);
  return turn_histograms;
}
}  // namespace poker
