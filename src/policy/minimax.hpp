#pragma once
#include <vector>
#include "../state/state.hpp"

#define MAX_DEPTH 4 //4 is max

class Minimax{
public:
  State* state;
  std::vector<Minimax*> next_node;

  Move get_move();
  int select_score(int depth);
  
  Minimax(State* s){ state = s; }
  Minimax(){ state = nullptr; }
  ~Minimax(){ 
    delete state; state = nullptr;
    while(!next_node.empty()){
      delete next_node.back();
      next_node.pop_back();
    }
  }
};