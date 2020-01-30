#include "common.h"
#include "gtest/gtest.h"
#include "poker_hand.h"

namespace poker {
namespace {
uint8_t make_card(card_t suit, card_t rank) {
  return static_cast<uint8_t>(deck_make_card(suit, rank));
}
}  // namespace

#define CHECK_HAND(hand, ranking, highest)             \
  {                                                    \
    EXPECT_EQ(hand.get_ranking_category(), (ranking)); \
    EXPECT_EQ(hand.get_highest_hand(), highest);       \
  }

TEST(PokerHandTest, CheckConstructor) {
  CHECK_HAND(PokerHand({make_card(Suits::Club, Ranks::Two),
                        make_card(Suits::Club, Ranks::Three),
                        make_card(Suits::Club, Ranks::Four),
                        make_card(Suits::Diamond, Ranks::Six),
                        make_card(Suits::Diamond, Ranks::Seven),
                        make_card(Suits::Diamond, Ranks::Eight),
                        make_card(Suits::Diamond, Ranks::Nine)}),
             HandRankingCategory::HighCard,
             (std::vector{make_card(Suits::Diamond, Ranks::Nine),
                          make_card(Suits::Diamond, Ranks::Eight),
                          make_card(Suits::Diamond, Ranks::Seven),
                          make_card(Suits::Diamond, Ranks::Six),
                          make_card(Suits::Club, Ranks::Four)}));

  CHECK_HAND(PokerHand({make_card(Suits::Club, Ranks::Two),
                        make_card(Suits::Club, Ranks::Three),
                        make_card(Suits::Club, Ranks::Four),
                        make_card(Suits::Diamond, Ranks::Seven),
                        make_card(Suits::Diamond, Ranks::Eight),
                        make_card(Suits::Diamond, Ranks::Nine),
                        make_card(Suits::Diamond, Ranks::Four)}),
             HandRankingCategory::OnePair,
             (std::vector{make_card(Suits::Diamond, Ranks::Four),
                          make_card(Suits::Club, Ranks::Four),
                          make_card(Suits::Diamond, Ranks::Nine),
                          make_card(Suits::Diamond, Ranks::Eight),
                          make_card(Suits::Diamond, Ranks::Seven)}));

  CHECK_HAND(PokerHand({make_card(Suits::Club, Ranks::Two),
                        make_card(Suits::Club, Ranks::Eight),
                        make_card(Suits::Spade, Ranks::Four),
                        make_card(Suits::Diamond, Ranks::Seven),
                        make_card(Suits::Diamond, Ranks::Eight),
                        make_card(Suits::Diamond, Ranks::Nine),
                        make_card(Suits::Heart, Ranks::Four)}),
             HandRankingCategory::TwoPair,
             (std::vector{make_card(Suits::Diamond, Ranks::Eight),
                          make_card(Suits::Club, Ranks::Eight),
                          make_card(Suits::Spade, Ranks::Four),
                          make_card(Suits::Heart, Ranks::Four),
                          make_card(Suits::Diamond, Ranks::Nine)}));

  CHECK_HAND(PokerHand({make_card(Suits::Spade, Ranks::Four),
                        make_card(Suits::Club, Ranks::Three),
                        make_card(Suits::Club, Ranks::Four),
                        make_card(Suits::Diamond, Ranks::Seven),
                        make_card(Suits::Diamond, Ranks::Eight),
                        make_card(Suits::Diamond, Ranks::Nine),
                        make_card(Suits::Diamond, Ranks::Four)}),
             HandRankingCategory::ThreeOfAKind,
             (std::vector{make_card(Suits::Spade, Ranks::Four),
                          make_card(Suits::Diamond, Ranks::Four),
                          make_card(Suits::Club, Ranks::Four),
                          make_card(Suits::Diamond, Ranks::Nine),
                          make_card(Suits::Diamond, Ranks::Eight)}));

  CHECK_HAND(PokerHand({make_card(Suits::Heart, Ranks::Eight),
                        make_card(Suits::Spade, Ranks::Eight),
                        make_card(Suits::Club, Ranks::Four),
                        make_card(Suits::Diamond, Ranks::Five),
                        make_card(Suits::Diamond, Ranks::Six),
                        make_card(Suits::Diamond, Ranks::Seven),
                        make_card(Suits::Diamond, Ranks::Eight)}),
             HandRankingCategory::Straight,
             (std::vector{make_card(Suits::Spade, Ranks::Eight),
                          make_card(Suits::Diamond, Ranks::Seven),
                          make_card(Suits::Diamond, Ranks::Six),
                          make_card(Suits::Diamond, Ranks::Five),
                          make_card(Suits::Club, Ranks::Four)}));

  // flush of 2d, 5d, 6d,8d, 9d (and straight of 4,5,6,7,8)
  CHECK_HAND(PokerHand({make_card(Suits::Diamond, Ranks::Two),
                        make_card(Suits::Club, Ranks::Four),
                        make_card(Suits::Diamond, Ranks::Five),
                        make_card(Suits::Diamond, Ranks::Six),
                        make_card(Suits::Heart, Ranks::Seven),
                        make_card(Suits::Diamond, Ranks::Eight),
                        make_card(Suits::Diamond, Ranks::Nine)}),
             HandRankingCategory::Flush,
             (std::vector{make_card(Suits::Diamond, Ranks::Nine),
                          make_card(Suits::Diamond, Ranks::Eight),
                          make_card(Suits::Diamond, Ranks::Six),
                          make_card(Suits::Diamond, Ranks::Five),
                          make_card(Suits::Diamond, Ranks::Two)}));

  CHECK_HAND(PokerHand({make_card(Suits::Diamond, Ranks::Two),
                        make_card(Suits::Club, Ranks::Two),
                        make_card(Suits::Heart, Ranks::Two),
                        make_card(Suits::Diamond, Ranks::Three),
                        make_card(Suits::Heart, Ranks::Three),
                        make_card(Suits::Diamond, Ranks::Eight),
                        make_card(Suits::Diamond, Ranks::Nine)}),
             HandRankingCategory::FullHouse,
             (std::vector{make_card(Suits::Heart, Ranks::Two),
                          make_card(Suits::Diamond, Ranks::Two),
                          make_card(Suits::Club, Ranks::Two),
                          make_card(Suits::Heart, Ranks::Three),
                          make_card(Suits::Diamond, Ranks::Three)}));

  CHECK_HAND(PokerHand({make_card(Suits::Diamond, Ranks::Jack),
                        make_card(Suits::Club, Ranks::Jack),
                        make_card(Suits::Heart, Ranks::Jack),
                        make_card(Suits::Spade, Ranks::Jack),
                        make_card(Suits::Club, Ranks::King),
                        make_card(Suits::Heart, Ranks::King),
                        make_card(Suits::Spade, Ranks::Ace)}),
             HandRankingCategory::FourOfAKind,
             (std::vector{make_card(Suits::Spade, Ranks::Jack),
                          make_card(Suits::Heart, Ranks::Jack),
                          make_card(Suits::Diamond, Ranks::Jack),
                          make_card(Suits::Club, Ranks::Jack),
                          make_card(Suits::Spade, Ranks::Ace)}));

  CHECK_HAND(PokerHand({make_card(Suits::Club, Ranks::Eight),
                        make_card(Suits::Club, Ranks::Nine),
                        make_card(Suits::Club, Ranks::Ten),
                        make_card(Suits::Club, Ranks::Jack),
                        make_card(Suits::Club, Ranks::Queen),
                        make_card(Suits::Diamond, Ranks::King),
                        make_card(Suits::Diamond, Ranks::Ace)}),
             HandRankingCategory::StraightFlush,
             (std::vector{make_card(Suits::Club, Ranks::Queen),
                          make_card(Suits::Club, Ranks::Jack),
                          make_card(Suits::Club, Ranks::Ten),
                          make_card(Suits::Club, Ranks::Nine),
                          make_card(Suits::Club, Ranks::Eight)}));

  CHECK_HAND(PokerHand({make_card(Suits::Club, Ranks::Eight),
                        make_card(Suits::Club, Ranks::Nine),
                        make_card(Suits::Club, Ranks::Ten),
                        make_card(Suits::Club, Ranks::Jack),
                        make_card(Suits::Club, Ranks::Queen),
                        make_card(Suits::Club, Ranks::King),
                        make_card(Suits::Club, Ranks::Ace)}),
             HandRankingCategory::RoyalFlush,
             (std::vector{make_card(Suits::Club, Ranks::Ace),
                          make_card(Suits::Club, Ranks::King),
                          make_card(Suits::Club, Ranks::Queen),
                          make_card(Suits::Club, Ranks::Jack),
                          make_card(Suits::Club, Ranks::Ten)}));

  CHECK_HAND(PokerHand({make_card(Suits::Club, Ranks::Ace),
                        make_card(Suits::Club, Ranks::Two),
                        make_card(Suits::Club, Ranks::Three),
                        make_card(Suits::Club, Ranks::Four),
                        make_card(Suits::Club, Ranks::Five),
                        make_card(Suits::Heart, Ranks::Six),
                        make_card(Suits::Heart, Ranks::Seven)}),
             HandRankingCategory::StraightFlush,
             (std::vector{make_card(Suits::Club, Ranks::Five),
                          make_card(Suits::Club, Ranks::Four),
                          make_card(Suits::Club, Ranks::Three),
                          make_card(Suits::Club, Ranks::Two),
                          make_card(Suits::Club, Ranks::Ace)}));

  CHECK_HAND(PokerHand({make_card(Suits::Club, Ranks::King),
                        make_card(Suits::Club, Ranks::Ace),
                        make_card(Suits::Club, Ranks::Two),
                        make_card(Suits::Club, Ranks::Three),
                        make_card(Suits::Club, Ranks::Four),
                        make_card(Suits::Heart, Ranks::Five),
                        make_card(Suits::Heart, Ranks::Six)}),
             HandRankingCategory::Flush,
             (std::vector{make_card(Suits::Club, Ranks::Ace),
                          make_card(Suits::Club, Ranks::King),
                          make_card(Suits::Club, Ranks::Four),
                          make_card(Suits::Club, Ranks::Three),
                          make_card(Suits::Club, Ranks::Two)}));
}

TEST(PokerHandTest, CalculateStrength) {
  auto ranking = internal::calc_ranking(
      HandRankingCategory::TwoPair,
      std::vector{make_card(Suits::Diamond, Ranks::Eight),
                  make_card(Suits::Club, Ranks::Eight),
                  make_card(Suits::Spade, Ranks::Four),
                  make_card(Suits::Heart, Ranks::Four),
                  make_card(Suits::Diamond, Ranks::Nine)});
  uint_fast32_t mask = 0xf;
  EXPECT_EQ(ranking >> 20ul,
            static_cast<uint_fast32_t>(HandRankingCategory::TwoPair));
  EXPECT_EQ((ranking >> 16ul) & mask, Ranks::Eight);
  EXPECT_EQ((ranking >> 12ul) & mask, Ranks::Eight);
  EXPECT_EQ((ranking >> 8ul) & mask, Ranks::Four);
  EXPECT_EQ((ranking >> 4ul) & mask, Ranks::Four);
  EXPECT_EQ((ranking >> 0ul) & mask, Ranks::Nine);
}
}  // namespace poker
