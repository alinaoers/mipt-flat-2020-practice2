#ifndef EARLEY_HPP
#define EARLEY_HPP

#include <set>
#include <vector>
#include <string>
#include <exception>
const int ConfPrefixLength = 3;

class IncorrectInput : public std::exception {
public:
  const char* what() const noexcept;
};

struct Configuration {
  char from_;
  std::string to_;
  size_t pos_;
  size_t start_pos_;
  
  Configuration();
  
  Configuration(char symb, const std::string& state, size_t pos = 0, size_t start_pos = 0);
  
  Configuration(const std::string& input, size_t size, size_t cur_start);
  
  Configuration(const Configuration& other);
  
  char& NextLetter();
};

bool operator==(const Configuration& first, const Configuration& second);
bool operator<(const Configuration& first, const Configuration& second);

using Grammar = std::vector<Configuration>;

class Algo {
  std::vector<std::set<Configuration>> result_;
  Grammar g_;
public:
  Algo();
  Algo(const Grammar& g);
  
  bool HasWord(const std::string& word);
  
  void Scan(const std::string& word, size_t idx);
  
  void Predict(size_t idx);
  
  void Complete(size_t idx);
  
  std::set<Configuration> GetData(size_t idx) const;
};

std::vector<Configuration> ParseInput(std::string input);
bool NotTerm(char symb);
bool FinishedConf(const Configuration& conf);

#endif /* EARLEY_HPP */
