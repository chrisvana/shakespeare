// Copyright 2013
// Author: Christopher Van Arsdale

#ifndef _SHAKESPEARE_WORD_FREQ_MAP_H__
#define _SHAKESPEARE_WORD_FREQ_MAP_H__

#include "common/log/log.h"

#include <algorithm>
#include <map>
#include <string>
#include "common/util/stl.h"

namespace shakespeare {

class WordFreqMap {
 public:
  WordFreqMap() : total_words_(0) {}
  ~WordFreqMap() {}

  // Accessors
  int total_words() const { return total_words_; }
  int unique_words() const { return counts_.size(); }
  int count_for_word(const std::string& word) const {
    return FindWithDefault(counts_, word, 0);
  }
  double freq_for_word(const std::string& word) const {
    return (static_cast<double>(count_for_word(word)) /
            std::max(1, total_words_));
  }
  const std::map<std::string, int>& raw_counts() const { return counts_; }

  // Mutators
  void add_word(const std::string& word) {
    ++total_words_;
    counts_[word]++;
  }

 private:
  int total_words_;
  std::map<std::string, int> counts_;
};

}  // namespace shakespeare

#endif  // _SHAKESPEARE_WORD_FREQ_MAP_H__
