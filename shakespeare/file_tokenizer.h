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

class Word {
 public:
  Word() {}
  explicit Word(const std::string& word) {
    Set(word);
  }
  ~Word() {}

  const std::string& raw() const { return raw_; }
  void Set(const std::string& raw) { raw_ = raw; }

 private:
  std::string raw_;
};

class Line {
 public:
  Line() {}
  ~Line() {}

  // Accessors
  int num_words() const { return words_.size(); }
  const Word& word(int i) const { return words_[i]; }
  std::vector<Word>::const_iterator begin() const { return words_.begin(); }
  std::vector<Word>::const_iterator end() const { return words_.end(); }

  // Mutators
  void AddWord(const Word& word) { words_.push_back(word); }

 private:
  std::vector<Word> words_;
};

class FileTokenizer {
 public:
  FileTokenizer(const std::string& file);
  ~FileTokenizer();

  bool ReadLine(Line* line);

 private:
  std::unique_ptr<file::FileLineReader> reader_;
};

}  // namespace shakespeare

#endif  // _SHAKESPEARE_FILE_TOKENIZER_H__
