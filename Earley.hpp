#ifndef EARLEY_HPP
#define EARLEY_HPP

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
  int pos_;
  int start_pos_;
  
  Configuration();
  
  Configuration(char symb, const std::string& state, int pos = 0, int start_pos = 0);
  
  Configuration(const std::string& input, int size, int cur_start);
  
  Configuration(const Configuration& other);
  
  char NextLetter() const;
};

bool operator==(const Configuration& first, const Configuration& second);

using Grammar = std::vector<Configuration>;

struct State {
  Configuration conf;
  bool used_for_predict;
  bool used_for_complete;
  
  State();
  State(const Configuration& c, bool used_p = false, bool used_c = false);
};

class Algo {
  std::vector<std::vector<State>> result_;
  Grammar g_;
public:
  Algo();
  Algo(const Grammar& g);
  
  bool HasWord(const std::string& word);
  
  void Scan(const std::string& word, int idx);
  
  void Predict(int idx);
  
  void Complete(int idx);
  
  std::vector<State> GetData(int idx) const;
};

bool operator==(const State& first, const State& second);

std::vector<Configuration> ParseInput(std::string input);
bool NotTerm(char symb);
bool FinishedConf(const Configuration& conf);

#endif /* EARLEY_HPP */
