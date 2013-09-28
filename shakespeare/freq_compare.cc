// Copyright 2013
// Author: Christopher Van Arsdale

#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>
#include "common/base/init.h"
#include "common/base/flags.h"
#include "common/file/fileutil.h"
#include "common/log/log.h"
#include "shakespeare/file_tokenizer.h"
#include "shakespeare/word_freq_map.h"

DEFINE_double(word_freq_minimum, 0.0003,
              "Minimum frequency to use in fingerprint.");

DEFINE_double(word_freq_maximum, 0.006,
              "Maximum frequency to use in fingerprint.");

DEFINE_int32(num_clusters, 3,
             "Number of clusters to compute.");

DEFINE_double(max_cluster_merge, 0.031,
              "Maximum distance to merge two clusters");


using std::cout;
using std::endl;
using std::set;
using std::map;
using std::string;
using std::tuple;
using std::vector;
using namespace shakespeare;

namespace {
double ComputeDist(const WordFreqMap& freq_map1,
                   const WordFreqMap& freq_map2) {
  double total = 0;

  set<string> processed;
  for (auto it : freq_map1.raw_counts()) {
    const string& word = it.first;
    double freq1 = freq_map1.freq_for_word(word);
    double freq2 = freq_map2.freq_for_word(word);
    if (freq1 >= FLAGS_word_freq_minimum &&
        freq1 <= FLAGS_word_freq_maximum) {
      processed.insert(word);
      total += (freq1 - freq2) * (freq1 - freq2);
    }
  }

  for (auto it : freq_map2.raw_counts()) {
    const string& word = it.first;
    double freq1 = freq_map1.freq_for_word(word);
    double freq2 = freq_map2.freq_for_word(word);
    if (freq2 >= FLAGS_word_freq_minimum &&
        freq2 <= FLAGS_word_freq_maximum &&
        processed.insert(word).second) {
      total += (freq1 - freq2) * (freq1 - freq2);
    }
  }

  return sqrt(total);
}

}  // anonymous namespace

struct Pair {
  Pair() {}
  Pair(const string& f, const string& s) : first(f>s ? s : f),
                                           second(f>s ? f : s) {}
  string first, second;
  bool operator==(const Pair& other) const {
    return first == other.first && second == other.second;
  }
  bool operator<(const Pair& other) const {
    return (first < other.first ||
            (first == other.first && second < other.second));
  }
};
struct ComparePair {
  bool operator()(const Pair& a, const Pair& b) const {
    return a < b;
  }
};

int main(int argc, char** argv) {
  InitProgram(&argc, &argv);

  // Figure out our input files.
  set<string> files;
  for (int i = 1; i < argc; ++i) {
    vector<string> current;
    CHECK(file::Glob(argv[i], &current));
    for (const string& it : current) {
      files.insert(it);
    }
  }

  // Figure out word frequencies.
  map<string, WordFreqMap*> frequencies;
  ValueDeleter del(&frequencies);
  for (const string& filename : files) {
    WordFreqMap* freq = new WordFreqMap();
    frequencies[filename] = freq;
    FileTokenizer tokenizer(filename);
    while (true) {
      Line line;
      if (!tokenizer.ReadLine(&line)) {
        break;
      }
      for (const Word& word : line) {
        freq->add_word(word.raw());
      }
    }
  }

  map<string, vector<string> > clusters;
  map<Pair, double, ComparePair> distance;

  // Initial distance map
  for (auto it : frequencies) {
    const string& primary = it.first;
    const WordFreqMap* freq = it.second;
    clusters[primary].push_back(primary);
    for (auto it2 : frequencies) {
      const string& secondary = it2.first;
      if (secondary <= primary) { continue; }
      const WordFreqMap* freq2 = it2.second;
      distance[Pair(primary, secondary)] = ComputeDist(*freq, *freq2);
    }
  }

  // Clustering time (... note that this is not coded optimally ....)!
  while (clusters.size() > FLAGS_num_clusters) {
    // Figure out closest distance
    double min_dist = 10000000;
    Pair min_tuple;
    for (auto it : distance) {
      if (it.second < min_dist) {
        min_dist = it.second;
        min_tuple = it.first;
      }
    }
    if (min_dist > FLAGS_max_cluster_merge) {
      break;
    }
    LOG(INFO) << "MERGING: " << min_tuple.first << " " << min_tuple.second
              << " (" << min_dist << ") ";

    // Figure out new distances for everyone.
    int first_size =  clusters[min_tuple.first].size();
    int second_size = clusters[min_tuple.second].size();
    set<Pair> to_erase;
    for (auto& it : distance) {
      if (it.first.first == min_tuple.first ||
          it.first.second == min_tuple.first) {
        const string& other = (it.first.first == min_tuple.first ?
                               it.first.second : it.first.first);
        double other_dist = distance[Pair(min_tuple.second, other)];
        double distance = ((it.second * first_size +
                            other_dist * second_size) /
                           (first_size + second_size));
        it.second = distance;
      }
      if (it.first.first == min_tuple.second ||
          it.first.second == min_tuple.second) {
        to_erase.insert(it.first);
      }
    }

    // Erase everyone's connection to the dead cluster.
    for (auto it : to_erase) {
      distance.erase(it);
    }

    // Merge finally.
    for (auto it : clusters[min_tuple.second]) {
      clusters[min_tuple.first].push_back(it);
    }
    CHECK_EQ(1, clusters.erase(min_tuple.second));
  }

  for (auto it : clusters) {
    cout << "CLUSTER" << endl;
    for (auto it2 : it.second) {
      cout << it2 << endl;
    }
  }

  // TODO.
  return 0;
}
