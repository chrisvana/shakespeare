// Copyright 2013
// Author: Christopher Van Arsdale

#include <cmath>
#include <string>
#include <set>
#include "common/base/flags.h"
#include "shakespeare/word_freq_map.h"

DEFINE_double(word_freq_minimum, 0.0003,
              "Minimum frequency to use in fingerprint.");

DEFINE_double(word_freq_maximum, 0.006,
              "Maximum frequency to use in fingerprint.");

using std::set;
using std::string;

namespace shakespeare {

double WordFreqMap::Distance(const WordFreqMap& other) const {
  double total = 0;

  set<string> processed;
  for (auto it : raw_counts()) {
    const string& word = it.first;
    double freq1 = freq_for_word(word);
    double freq2 = other.freq_for_word(word);
    if (freq1 >= FLAGS_word_freq_minimum &&
        freq1 <= FLAGS_word_freq_maximum) {
      processed.insert(word);
      total += (freq1 - freq2) * (freq1 - freq2);
    }
  }

  for (auto it : other.raw_counts()) {
    const string& word = it.first;
    double freq1 = freq_for_word(word);
    double freq2 = other.freq_for_word(word);
    if (freq2 >= FLAGS_word_freq_minimum &&
        freq2 <= FLAGS_word_freq_maximum &&
        processed.insert(word).second) {
      total += (freq1 - freq2) * (freq1 - freq2);
    }
  }

  return sqrt(total);
}

}  // namespace shakespeare
