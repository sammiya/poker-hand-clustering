#include "poker_hand.h"
#include <algorithm>
#include <functional>
#include "common.h"

using std::map;
using std::vector;

namespace poker {
PokerHand::PokerHand(vector<uint8_t> cards) {
  vector<uint8_t> cards_reverse_sorted = std::move(cards);
  std::sort(cards_reverse_sorted.begin(), cards_reverse_sorted.end(),
            std::greater<>());
  if (_set_highest_if_flush(cards_reverse_sorted)) {
    return;
  }
  if (_set_highest_if_straight(cards_reverse_sorted, false)) {
    return;
  }
  _set_other_ranking(cards_reverse_sorted);
}

bool PokerHand::_set_highest_if_flush(
    const vector<uint8_t> &cards_reverse_sorted) {
  vector<vector<uint8_t>> suit_to_cards(SUITS);
  for (uint8_t card : cards_reverse_sorted) {
    auto suit = deck_get_suit(card);
    suit_to_cards[suit].emplace_back(card);
  }
  for (size_t suit = 0; suit < SUITS; ++suit) {
    auto flush_cards = suit_to_cards[suit];
    auto size = flush_cards.size();
    if (size < 5) {
      continue;
    }
    if (!_set_highest_if_straight(flush_cards, true)) {
      vector<uint8_t> highest_hand(flush_cards.begin(),
                                   flush_cards.begin() + 5);
      _set_category_and_hand(HandRankingCategory::Flush, highest_hand);
    }
    return true;
  }
  return false;
}

bool PokerHand::_set_highest_if_straight(
    const vector<uint8_t> &cards_reverse_sorted, bool are_same_suit) {
  auto category = are_same_suit ? HandRankingCategory::StraightFlush
                                : HandRankingCategory::Straight;

  vector<uint8_t> highest_hand;
  uint8_t rank_previous = 0;
  for (uint8_t card : cards_reverse_sorted) {
    auto rank = deck_get_rank(card);
    if (rank == rank_previous) {
      continue;
    }
    if (rank + 1 == rank_previous) {
      highest_hand.emplace_back(card);
      if (highest_hand.size() == 4 && rank == Ranks::Two) {
        auto strongest = cards_reverse_sorted[0];
        if (deck_get_rank(strongest) != Ranks::Ace) {
          return false;
        }
        highest_hand.emplace_back(strongest);
      }
      if (highest_hand.size() == 5) {
        if (are_same_suit && deck_get_rank(highest_hand[0]) == Ranks::Ace) {
          category = HandRankingCategory::RoyalFlush;
        }
        _set_category_and_hand(category, highest_hand);
        return true;
      }
    } else {
      highest_hand.clear();
      highest_hand.emplace_back(card);
    }
    rank_previous = rank;
  }
  return false;
}

void PokerHand::_set_other_ranking(
    const vector<uint8_t> &cards_reverse_sorted) {
  map<uint8_t, vector<uint8_t>> cards_by_rank;
  for (const auto &card : cards_reverse_sorted) {
    cards_by_rank[deck_get_rank(card)].emplace_back(card);
  }
  vector<std::pair<size_t, uint8_t>> card_count_rank_pairs;
  for (const auto &[rank, cards] : cards_by_rank) {
    card_count_rank_pairs.emplace_back(std::make_pair(cards.size(), rank));
  };
  std::sort(card_count_rank_pairs.begin(), card_count_rank_pairs.end(),
            std::greater<>());

  vector<uint8_t> cards_rearranged;
  size_t two_count = 0;
  size_t one_count = 0;
  size_t three_count = 0;
  size_t four_count = 0;
  for (const auto &[card_count, rank] : card_count_rank_pairs) {
    auto same_rank_cards = cards_by_rank.at(rank);
    std::copy(same_rank_cards.begin(), same_rank_cards.end(),
              std::back_inserter(cards_rearranged));
    switch (card_count) {
      case 1:
        ++one_count;
        break;
      case 2:
        ++two_count;
        break;
      case 3:
        ++three_count;
        break;
      case 4:
        ++four_count;
        break;
    }
  }

  HandRankingCategory hand_ranking_category;
  if (four_count == 1) {
    hand_ranking_category = HandRankingCategory::FourOfAKind;
  } else if (three_count > 0) {
    hand_ranking_category = one_count == 4 ? HandRankingCategory::ThreeOfAKind
                                           : HandRankingCategory::FullHouse;
  } else if (two_count >= 2) {
    hand_ranking_category = HandRankingCategory::TwoPair;
  } else if (two_count == 1) {
    hand_ranking_category = HandRankingCategory::OnePair;
  } else {
    hand_ranking_category = HandRankingCategory::HighCard;
  }

  vector<uint8_t> highest_hand(cards_rearranged.begin(),
                               cards_rearranged.begin() + 5);
  if (one_count == 1 && two_count > 0) {
    highest_hand[4] = std::max(
        {cards_rearranged[4], cards_rearranged[5], cards_rearranged[6]});
  }
  _set_category_and_hand(hand_ranking_category, highest_hand);
}
}  // namespace poker
