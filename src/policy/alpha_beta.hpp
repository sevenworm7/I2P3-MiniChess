#pragma once
#include <vector>
#include "../state/state.hpp"

#define MAX_DEPTH 4 //max 4 //not stable

class alpha_beta{
public:
  State* state;
  std::vector<alpha_beta*> next_node;
  int alpha, beta;

  Move get_move(std::ofstream& fout);
  int extend(int depth);
  
  alpha_beta(State* s, int a, int b) : state(s), alpha(a), beta(b) {}
  alpha_beta(State* s) : state(s) {}
  alpha_beta() : state(nullptr) {}
  ~alpha_beta(){ 
    delete state; state = nullptr;
    while(!next_node.empty()){
      delete next_node.back();
      next_node.pop_back();
    }
  }
};