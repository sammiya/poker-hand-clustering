# poker-hand-clustering

[![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)
[![CircleCI](https://circleci.com/gh/sammiya/poker-hand-clustering.svg?style=svg)](https://circleci.com/gh/sammiya/poker-hand-clustering)


k-means clustering for poker hands based on earth mover's distance (EMD)

## About
An implementation of hand abstraction algorithm for two-player nolimit Texas Hold’em poker introduced in [Potential-Aware Imperfect-Recall Abstraction with Earth Mover’s Distance in
Imperfect-Information Games](https://www.researchgate.net/publication/287088563_Potential-aware_imperfect-recall_abstraction_with_earth_mover's_distance_in_imperfect-information_games).

## Requirements
- g++ ( >=7.4 )
- make
- cmake (Just for building [googletest](https://github.com/google/googletest))

## Usage

Please don't forget to run `git submodule update --init --recursive` after checking out. Then run the clustering program as follows:

    make
    .bin/poker-hand-clustering

  In `data` folder, `turn_kmeans.csv` and `flop_kmeans.csv` will be generated. Here we represent cluster by hand indexes implemented in [kdub0/hand-isomorphism](https://github.com/kdub0/hand-isomorphism). Thanks.

## CAUTION

This is a rewrite of my old-fashioned code to release, but whether this program works is not yet confirmed enough because of its requirement of computational resource etc. In the future, I will add more tests and run this program with enough computational resource to confirm it.
