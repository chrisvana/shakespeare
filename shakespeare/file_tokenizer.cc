// Copyright 2013
// Author: Christopher Van Arsdale

#include <string>
#include <vector>
#include "common/log/log.h"
#include "common/file/linereader.h"
#include "common/strings/strutil.h"
#include "shakespeare/file_tokenizer.h"
#include "third_party/boost/boost/tokenizer.hpp"

using std::string;
using std::vector;

namespace shakespeare {

FileTokenizer::FileTokenizer(const string& file)
    : reader_(new file::FileLineReader(file)) {
}

FileTokenizer::~FileTokenizer() {
}

bool FileTokenizer::ReadLine(vector<string>* line) {
  line->clear();
  if (reader_->Done()) {
    return false;
  }
  boost::tokenizer<> tokenize(reader_->line());
  for (const string& raw : tokenize) {
    // Will screw up on UTF8, but oh well.
    line->push_back(strings::LowerString(raw));
  }
  reader_->Next();
  return true;
}

}  // namespace shakespeare
