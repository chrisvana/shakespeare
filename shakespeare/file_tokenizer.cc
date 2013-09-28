// Copyright 2013
// Author: Christopher Van Arsdale

#include <string>
#include "common/log/log.h"
#include "common/file/linereader.h"
#include "common/strings/strutil.h"
#include "shakespeare/file_tokenizer.h"
#include "third_party/boost/boost/tokenizer.hpp"

using std::string;

namespace shakespeare {

FileTokenizer::FileTokenizer(const string& file)
    : reader_(new file::FileLineReader(file)) {
}

FileTokenizer::~FileTokenizer() {
}

bool FileTokenizer::ReadLine(Line* line) {
  if (reader_->Done()) {
    return false;
  }
  boost::tokenizer<> tokenize(reader_->line());
  for (const string& raw : tokenize) {
    // Will screw up on UTF8, but oh well.
    line->AddWord(Word(strings::LowerString(raw)));
  }
  reader_->Next();
  return true;
}

}  // namespace shakespeare
