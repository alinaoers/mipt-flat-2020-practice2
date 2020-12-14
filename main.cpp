#include <iostream>
#include <string>
#include <exception>
#include "Earley.hpp"

int main() {
  std::string input;
  std::string word;
  std::getline(std::cin, input, '\n');
  std::cin >> word;
  try {
    Grammar grammar = ParseInput(input);
    Algo algo(grammar);
    std::cout << (algo.HasWord(word) ? "YES" : "NO") << '\n';
  } catch (std::exception& exc) {
    std::cout << exc.what();
  }

  return 0;
}

//S->T S->aS T->Ub T->bTc U->cVc U->cUb V->a V->bcV
//aabcacbc
//S->bTba S->a T->cSa T->a S->Ua S->c U->bS U->e
//bcbaaaba
//S->TuT->uu
//uuu
//S->cSS S->Tc S->acS S->bU T->Ub U->a
//cabcacba
//S->aSS S->b
//abb
