#ifndef POKER_HAND_CLUSTERING_COMMON_H_
#define POKER_HAND_CLUSTERING_COMMON_H_

extern "C" {
#include <stdbool.h>
#include "../dependencies/hand-isomorphism/src/deck.h"
#include "../dependencies/hand-isomorphism/src/hand_index.h"
}
#include <inttypes.h>

namespace poker {
enum Ranks : uint8_t {
  Two = 0,
  Three = 1,
  Four = 2,
  Five = 3,
  Six = 4,
  Seven = 5,
  Eight = 6,
  Nine = 7,
  Ten = 8,
  Jack = 9,
  Queen = 10,
  King = 11,
  Ace = 12
};

enum Suits : uint8_t { Club = 0, Diamond = 1, Heart = 2, Spade = 3 };

constexpr size_t TURN_HIST_SIZE = CARDS - 6;  // six cards are faced up
constexpr size_t FLOP_HIST_SIZE = CARDS - 5;  // five cards are faced up
}  // namespace poker

#endif /* POKER_HAND_CLUSTERING_COMMON_H_ */
