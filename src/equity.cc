#include "equity.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include "common.h"
#include "poker_hand.h"

namespace {
struct River {
  uint_fast32_t ranking;
  size_t index;
  card_t hole_1;
  card_t hole_2;

  River(uint_fast32_t ranking, size_t index, card_t hole_1, card_t hole_2)
      : ranking(ranking), index(index), hole_1(hole_1), hole_2(hole_2) {}
};
}  // namespace

namespace poker {
std::vector<int32_t> calculate_equity() {
  hand_indexer_t indexer_7, indexer_5, indexer_2_5;

  hand_indexer_init(1, (const uint8_t[]){7}, &indexer_7);
  hand_indexer_init(1, (const uint8_t[]){5}, &indexer_5);
  hand_indexer_init(2, (const uint8_t[]){2, 5}, &indexer_2_5);

  const size_t index_size_7 = hand_indexer_size(&indexer_7, 0);
  const size_t index_size_5 = hand_indexer_size(&indexer_5, 0);
  const size_t index_size_2_5 = hand_indexer_size(&indexer_2_5, 1);

  std::vector<uint_fast32_t> rankings(index_size_7);
  std::vector<uint8_t> hand_category_nos(index_size_7);

  for (size_t i = 0; i < index_size_7; ++i) {
    uint8_t cards_7[7];
    hand_unindex(&indexer_7, indexer_7.rounds - 1, i, cards_7);
    auto poker_hand = PokerHand(std::vector<uint8_t>(cards_7, cards_7 + 7));
    rankings[i] = poker_hand.calc_ranking();
  }

  uint8_t cards_5[5];
  std::vector<int8_t> deck_sorted(52);
  std::iota(deck_sorted.begin(), deck_sorted.end(), 0);

  std::vector<int32_t> equity_vec(index_size_2_5);  // vector to return

  for (size_t i = 0; i < index_size_5; ++i) {
    hand_unindex(&indexer_5, indexer_5.rounds - 1, i, cards_5);
    std::sort(cards_5, cards_5 + 5);
    std::vector<uint8_t> deck_remaining;
    std::set_difference(deck_sorted.begin(), deck_sorted.end(),
                        std::begin(cards_5), std::end(cards_5),
                        std::inserter(deck_remaining, deck_remaining.end()));

    std::map<uint_fast32_t, std::vector<River>> river_dtos_by_ranking;

    for (size_t i_hole_1 = 0; i_hole_1 < 47; ++i_hole_1) {
      for (size_t i_hole_2 = i_hole_1 + 1; i_hole_2 < 47; ++i_hole_2) {
        auto hole_1 = deck_remaining[i_hole_1];
        auto hole_2 = deck_remaining[i_hole_2];
        uint8_t cards_7[] = {deck_remaining[i_hole_1],
                             deck_remaining[i_hole_2],
                             cards_5[0],
                             cards_5[1],
                             cards_5[2],
                             cards_5[3],
                             cards_5[4]};
        auto index_7 = hand_index_last(&indexer_7, cards_7);
        auto ranking = rankings[index_7];
        auto index_2_5 = hand_index_last(&indexer_2_5, cards_7);
        river_dtos_by_ranking[ranking].emplace_back(
            River(ranking, index_2_5, hole_1, hole_2));
      }
    }
    long inferior_count = 0;
    std::vector<long> inferior_count_by_card(CARDS);
    for (const auto& [ranking, river_dtos] : river_dtos_by_ranking) {
      long equal_count = river_dtos.size();
      std::vector<long> equal_count_by_card(CARDS);
      for (const auto& river_dto : river_dtos) {
        ++equal_count_by_card[river_dto.hole_1];
        ++equal_count_by_card[river_dto.hole_2];
      }
      for (const auto& river_dto : river_dtos) {
        auto win_count = inferior_count -
                         inferior_count_by_card[river_dto.hole_1] -
                         inferior_count_by_card[river_dto.hole_2];
        auto tie_count = equal_count - equal_count_by_card[river_dto.hole_1] -
                         equal_count_by_card[river_dto.hole_2] + 1;
        equity_vec[river_dto.index] = win_count * 2 + tie_count;
      }
      std::transform(inferior_count_by_card.begin(),
                     inferior_count_by_card.end(), equal_count_by_card.begin(),
                     inferior_count_by_card.begin(), std::plus<>());
      inferior_count += equal_count;
    }
  }

  hand_indexer_free(&indexer_7);
  hand_indexer_free(&indexer_5);
  hand_indexer_free(&indexer_2_5);
  return equity_vec;
}
}  // namespace poker
