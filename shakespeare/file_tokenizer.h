// Copyright 2013
// Author: Christopher Van Arsdale

#ifndef _SHAKESPEARE_FILE_TOKENIZER_H__
#define _SHAKESPEARE_FILE_TOKENIZER_H__

#include <memory>
#include <string>
#include <vector>

namespace file {
class FileLineReader;
}

namespace shakespeare {

class FileTokenizer {
 public:
  FileTokenizer(const std::string& file);
  ~FileTokenizer();

  bool ReadLine(std::vector<std::string>* line);

 private:
  std::unique_ptr<file::FileLineReader> reader_;
};

}  // namespace shakespeare

#endif  // _SHAKESPEARE_FILE_TOKENIZER_H__
