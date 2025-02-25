#include <cstdio>
#include <cassert>
#include <algorithm>
#include <vector>
#include <set>
#include <chrono>
#include <random>
#include <phevaluator/phevaluator.h>
#include "gtest/gtest.h"
#include "kev/kev_eval.h"

using namespace phevaluator;

static int percentage(long long numerator, long long denominator) {
  return numerator * 100 / denominator;
}

static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
static std::default_random_engine generator(seed);
static std::uniform_int_distribution<int> distribution(0, 51);

static std::vector<int> RandomCardSample(int size) {
  std::vector<int> ret;
  std::set<int> sample;

  while (sample.size() < size) {
    int number = distribution(generator);
    if (sample.find(number) == sample.end()) {
      ret.push_back(number);
      sample.insert(number);
    }
  }

  return ret;
}

static short IterateKevEval(int a, int b, int c, int d, int e,
                            int f, int g, int h, int i) {
  short best = 20000;

  int board[10][3] = {
    {a, b, c},
    {a, b, d},
    {a, b, e},
    {a, c, d},
    {a, c, e},
    {a, d, e},
    {b, c, d},
    {b, c, e},
    {b, d, e},
    {c, d, e},
  };
  int hole[6][2] = {
    {f, g},
    {f, h},
    {f, i},
    {g, h},
    {g, i},
    {h, i},
  };

  for (int j = 0; j < 10; j++) {
    for (int k = 0; k < 6; k++) {
      best = std::min(kev_eval_5cards(board[j][0], board[j][1], board[j][2],
            hole[k][0], hole[k][1]), best);
    }
  }

  return best;
}

TEST(EvaluationTest, TestPlo4Cards) {
  int progress = 0;
  const long long total = 100 * 1000 * 1000;

  std::printf("Start testing Plo4 cards\n");

  for (long long count = 0; count < total; count++) {
    std::vector<int> sample = RandomCardSample(9);
  
    int ph_eval = EvaluatePlo4Cards(sample[0],
                                     sample[1],
                                     sample[2],
                                     sample[3],
                                     sample[4],
                                     sample[5],
                                     sample[6],
                                     sample[7],
                                     sample[8]).value();

    int kev_eval = IterateKevEval(sample[0],
                                  sample[1],
                                  sample[2],
                                  sample[3],
                                  sample[4],
                                  sample[5],
                                  sample[6],
                                  sample[7],
                                  sample[8]);

    EXPECT_EQ(ph_eval, kev_eval) << "Cards are: "
      << sample[0] << ", " << sample[1] << ", " << sample[2] << ", " << sample[3] << ", "
      << sample[4] << ", " << sample[5] << ", " << sample[6] << ", " << sample[7] << ", "
      << sample[8] << ", ";

    if (percentage(count, total) > progress) {
      progress = percentage(count, total);
      if (progress % 10 == 0) {
        std::printf("Test progress: %d%%\n", progress);
      }
    }
  }

  std::printf("Complete testing Plo4 cards\n");
  std::printf("Tested %lld random hands in total\n", total);
}

