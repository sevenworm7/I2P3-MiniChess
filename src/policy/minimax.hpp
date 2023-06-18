#pragma once
#include <vector>
#include "../state/state.hpp"

#define MAX_DEPTH 3 

class minimax{
public:
  State* state;
  std::vector<minimax*> next_node;

  Move get_move();
  int extend(int depth);
  
  minimax(State* s){ state = s; }
  minimax(){ state = nullptr; }
  ~minimax(){ 
    delete state; state = nullptr;
    while(!next_node.empty()){
      delete next_node.back();
      next_node.pop_back();
    }
  }
};