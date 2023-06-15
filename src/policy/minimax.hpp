#pragma once
#include <vector>
#include "../state/state.hpp"

class Minimax{
public:
  static Move get_move(State *state, int depth);
};

class Tree{
public:
  State state;
  std::vector<Tree*> next_node;
};