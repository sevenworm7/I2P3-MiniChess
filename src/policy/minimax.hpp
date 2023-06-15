#pragma once
#include <vector>
#include "../state/state.hpp"

#define MAX_DEPTH 5

class Minimax{
public:
  State* state;
  std::vector<Minimax*> next_node;

  Move get_move();
  int select_score(int depth);
  
  Minimax(State* s){ state = s; }
  Minimax(){ state = nullptr; }
};