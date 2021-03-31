#include "Earley.hpp"

const char* IncorrectInput::what() const noexcept {
  return "Incorrect Grammar Input\n";
}

Configuration::Configuration() = default;
  
Configuration::Configuration(char symb, const std::string& state, size_t pos, size_t start_pos) : from_(symb), to_(state), pos_(pos), start_pos_(start_pos) {}
  
Configuration::Configuration(const std::string& input, size_t size, size_t cur_start) : from_(input[cur_start]), pos_(0), start_pos_(0) {
  if (input[cur_start + 1] != '-' || input[cur_start + 2] != '>') {
    throw IncorrectInput();
  }
  for (int i = ConfPrefixLength; i < size; ++i) {
    if (!isalpha(input[cur_start + i])) {
      throw IncorrectInput();
    }
  }
  if ((size - ConfPrefixLength) == 1 && input[cur_start + ConfPrefixLength] == 'e') {
    to_ = "";
  } else {
    to_.resize(size - ConfPrefixLength);
    std::copy(input.begin() + cur_start + ConfPrefixLength, input.begin() + cur_start + size, to_.begin());
  }
}
  
Configuration::Configuration(const Configuration& other) : from_(other.from_), pos_(other.pos_), start_pos_(other.start_pos_) {
  to_.resize(other.to_.size());
  std::copy(other.to_.begin(), other.to_.end(), to_.begin());
}
  
char& Configuration::NextLetter() {
  return to_[pos_];
}

bool operator==(const Configuration& first, const Configuration& second) {
  return (first.from_ == second.from_ && first.to_ == second.to_ && first.pos_ == second.pos_ && first.start_pos_ == second.start_pos_);
}

bool operator<(const Configuration& first, const Configuration& second) {
  return (first.from_ < second.from_ || (first.from_ == second.from_ && first.to_ < second.to_) || (first.from_ == second.from_ && first.to_ == second.to_ && first.pos_ < second.pos_) || (first.from_ == second.from_ && first.to_ == second.to_ && first.pos_ == second.pos_ && first.start_pos_ < second.start_pos_));
}

bool NotTerm(char symb) {
  return ((symb - 'A') >= 0 && (symb - 'Z') <= 0);
}

bool FinishedConf(const Configuration& conf) {
  Configuration finished('A', "S", 1, 0);
  return (conf.from_ == finished.from_ && conf.to_ == finished.to_ && conf.pos_ == finished.pos_ && conf.start_pos_ == finished.start_pos_);
}

Algo::Algo() = default;
Algo::Algo(const Grammar& g) : result_(1), g_(g) {
  result_[0].insert(Configuration('A', "S", 0, 0));
}
  
bool Algo::HasWord(const std::string& word) {
  for (int i = 0; i <= word.size(); ++i) {
    Scan(word, i);
    bool updated = true;
    while (updated) {
      size_t old_size = result_[i].size();
      Complete(i);
      Predict(i);
      updated = (old_size != result_[i].size());
    }
  }

  for (auto& conf : result_[word.size()]) {
    if (FinishedConf(conf)) {
      return true;
    }
  }
  return false;
}
  
void Algo::Scan(const std::string& word, size_t idx) {
  if (idx == 0) return;
  result_.resize(result_.size() + 1);
  for (auto conf : result_[idx - 1]) {
    if (word[idx - 1] == conf.NextLetter()) {
      result_[idx].insert(Configuration(conf.from_, conf.to_, conf.pos_ + 1, conf.start_pos_));
    }
  }
}
  
void Algo::Predict(size_t idx) {
  for (auto conf : result_[idx]) {
    char next_symb = conf.NextLetter();
    if (NotTerm(next_symb)) {
      for (auto start_conf : g_) {
        if (start_conf.from_ == next_symb) {
          result_[idx].insert(Configuration(start_conf.from_, start_conf.to_, 0, idx));
        }
      }
    }
  }
}
  
void Algo::Complete(size_t idx) {
  for (auto conf : result_[idx]) {
    if (conf.pos_ == conf.to_.size()) {
      for (auto up_conf : result_[conf.start_pos_]) {
        if (up_conf.NextLetter() == conf.from_) {
          result_[idx].insert(Configuration(up_conf.from_, up_conf.to_, up_conf.pos_ + 1, up_conf.start_pos_));
        }
      }
    }
  }
}

std::set<Configuration> Algo::GetData(size_t idx) const {
  return result_[idx];
}

std::vector<Configuration> ParseInput(std::string input) {
  std::vector<Configuration> res;
  res.push_back(Configuration('A', "S", 0, 0));
  size_t cur_start = 0;
  while (true) {
    auto pos = std::find(input.begin() + cur_start, input.end(), ' ');
    res.push_back(Configuration(input, pos - input.begin() - cur_start, cur_start));
    cur_start = pos - input.begin() + 1;
    if (input[cur_start - 1] == '\0') {
      return res;
    }
  }
}
