// Copyright 2013
// Author: Christopher Van Arsdale
//
// Example usage:
// ./freq_compare shakespeare/works/*/*.txt

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
#include "shakespeare/clusterer.h"
#include "shakespeare/file_tokenizer.h"
#include "shakespeare/word_freq_map.h"

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

int main(int argc, char** argv) {
  InitProgram(&argc, &argv);

  // Figure out our input files.
  set<string> files;
  for (int i = 1; i < argc; ++i) {
    vector<string> current;
    CHECK(file::Glob(argv[i], &current));
    files.insert(current.begin(), current.end());
  }

  // Figure out word frequencies.
  map<string, WordFreqMap*> frequencies;
  ValueDeleter del(&frequencies);
  for (const string& filename : files) {
    WordFreqMap* freq = new WordFreqMap();

    vector<string> line;
    FileTokenizer tokenizer(filename);
    while (tokenizer.ReadLine(&line)) {
      for (const string& word : line) {
        freq->add_word(word);
      }
    }

    frequencies[filename] = freq;    
  }

  // Initialize our clusters.
  Clusterer clusterer;
  for (auto it : frequencies) {
    const string& primary = it.first;
    const WordFreqMap* freq = it.second;
    for (auto it2 : frequencies) {
      const string& secondary = it2.first;
      if (secondary <= primary) { continue; }
      clusterer.AddPair(primary, secondary, freq->Distance(*it2.second));
    }
  }

  // Run the clusterer.
  vector<vector<string> > clusters;
  clusterer.Cluster(FLAGS_num_clusters, FLAGS_max_cluster_merge, &clusters);
  for (auto it : clusters) {
    cout << "CLUSTER" << endl;
    for (auto it2 : it) {
      cout << it2 << endl;
    }
  }

  return 0;
}
