#pragma once
#include <vector>
#include "../state/state.hpp"

#define MAX_DEPTH 3 //max 3

class submission{
public:
  State* state;
  std::vector<submission*> next_node;

  Move get_move(std::ofstream& fout);
  int extend(int depth);
  
  submission(State* s){ state = s; }
  submission(){ state = nullptr; }
  ~submission(){ 
    delete state; state = nullptr;
    while(!next_node.empty()){
      delete next_node.back();
      next_node.pop_back();
    }
  }
};