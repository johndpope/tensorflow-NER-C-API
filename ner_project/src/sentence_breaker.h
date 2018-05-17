#ifndef SENTENCE_BREAKER_H_
#define SENTENCE_BREAKER_H_
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "basic_string_util.h"
class SentenceBreaker {
 public:
  SentenceBreaker(int maxLen);
  bool breakSentences(const UnicodeStr& text,
                      std::vector<UnicodeStr>* lines);

 private:
  static char*  kInlineMarks[];
  static char* kBreakMarks[];

  bool is_inline_mark(UnicodeCharT uch) ;
  bool is_break_mark(UnicodeCharT uch) ;

  std::unordered_map<UnicodeCharT, UnicodeCharT> inline_marks_;
  std::unordered_set<UnicodeCharT> break_marks_;
  std::unordered_set<UnicodeCharT> inline_marks_set_;
  int max_len_;
};
#endif

