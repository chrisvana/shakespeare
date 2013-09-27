// Copyright 2013
// Author: Christopher Van Arsdale

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "common/base/init.h"
#include "common/base/flags.h"
#include "common/file/fileutil.h"
#include "common/log/log.h"
#include "shakespeare/file_tokenizer.h"
#include "shakespeare/word_freq_map.h"

using std::cout;
using std::endl;
using std::set;
using std::map;
using std::string;
using std::vector;
using namespace shakespeare;

int main(int argc, char** argv) {
  InitProgram(&argc, &argv);
  set<string> files;
  for (int i = 1; i < argc; ++i) {
    vector<string> current;
    CHECK(file::Glob(argv[i], &current));
    for (const string& it : current) {
      files.insert(it);
    }
  }

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

  for (auto it : frequencies) {
    cout << "File: " << it.first << endl;
    cout << "Count: " << it.second->total_words() << endl;
  }

  // TODO.
  return 0;
}
