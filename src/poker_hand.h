#ifndef POKER_HAND_CLUSTERING_POKER_HAND_H_
#define POKER_HAND_CLUSTERING_POKER_HAND_H_

#include <map>
#include <string>
#include <vector>
#include "../dependencies/hand-isomorphism/src/deck.h"

namespace poker {
constexpr size_t HAND_RANKING_CATEGORIES = 10;
enum HandRankingCategory : uint8_t {
  HighCard = 0,
  OnePair = 1,
  TwoPair = 2,
  ThreeOfAKind = 3,
  Straight = 4,
  Flush = 5,
  FullHouse = 6,
  FourOfAKind = 7,
  StraightFlush = 8,
  RoyalFlush = 9
};

namespace internal {
inline uint_fast32_t calc_ranking(HandRankingCategory category,
                                  const std::vector<uint8_t> &highest_hand) {
  return static_cast<uint_fast32_t>(category << 20ul) |
         deck_get_rank(highest_hand[0]) << 16ul |
         deck_get_rank(highest_hand[1]) << 12ul |
         deck_get_rank(highest_hand[2]) << 8ul |
         deck_get_rank(highest_hand[3]) << 4ul | deck_get_rank(highest_hand[4]);
}
}  // namespace internal

class PokerHand {
 private:
  HandRankingCategory _hand_ranking_category;
  std::vector<uint8_t> _highest_hand;

  void _set_category_and_hand(HandRankingCategory hand_ranking_category,
                              const std::vector<uint8_t> &highest_hand) {
    _hand_ranking_category = hand_ranking_category;
    _highest_hand = highest_hand;
  };

  bool _set_highest_if_flush(const std::vector<uint8_t> &cards_reverse_sorted);
  bool _set_highest_if_straight(
      const std::vector<uint8_t> &cards_reverse_sorted, bool are_same_suit);
  void _set_other_ranking(const std::vector<uint8_t> &cards_reverse_sorted);

 public:
  explicit PokerHand(std::vector<uint8_t> cards);

  inline HandRankingCategory get_ranking_category() const {
    return _hand_ranking_category;
  }

  // return strength value
  inline uint_fast32_t calc_ranking() const {
    return internal::calc_ranking(_hand_ranking_category, _highest_hand);
  }

  inline std::vector<uint8_t> get_highest_hand() const { return _highest_hand; }
};
}  // namespace poker

#endif  // POKER_HAND_CLUSTERING_POKER_HAND_H_
