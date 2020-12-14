#include <gtest/gtest.h>
#include "Earley.hpp"

TEST(UsualTest1, Predict) {
  std::vector<Configuration> confs = ParseInput("S->aS S->T T->b");
  Algo algo(confs);
  algo.Predict(0);
  std::vector<State> except_states;
  except_states.push_back(State(Configuration('A', "S", 0, 0), true, false));
  except_states.push_back(State(Configuration('S', "aS", 0, 0), false, false));
  except_states.push_back(State(Configuration('S', "T", 0, 0), false, false));
  std::vector<State> new_states = algo.GetData(0);
  ASSERT_EQ(new_states, except_states);
}

TEST(UsualTest2, Scan) {
  std::vector<Configuration> confs = ParseInput("S->aS S->T T->b");
  std::string word = "ab";
  Algo algo(confs);
  algo.Predict(0);
  algo.Scan(word, 1);
  std::vector<State> new_states = algo.GetData(1);
  std::vector<State> except_states;
  except_states.push_back(State(Configuration('S', "aS", 1, 0), false, false));
  ASSERT_EQ(new_states, except_states);
}

TEST(UsualTest3, Complete) {
  std::vector<Configuration> confs = ParseInput("S->aS S->b");
  std::string word = "ab";
  Algo algo(confs);
  algo.Predict(0);
  algo.Scan(word, 1);
  algo.Predict(1);
  algo.Scan(word, 2);
  algo.Complete(2);
  std::vector<State> new_states = algo.GetData(2);
  std::vector<State> except_states;
  except_states.push_back(State(Configuration('S', "b", 1, 1), false, true));
  except_states.push_back(State(Configuration('S', "aS", 2, 0), false, false));
  ASSERT_EQ(new_states, except_states);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
