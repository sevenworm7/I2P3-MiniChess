#include <cstdlib>
#include <vector>

#include "../state/state.hpp"
#include "./minimax.hpp"

Move Minimax::get_move(State* state){ //present state
  if(!state->legal_actions.size())
    state->get_legal_actions();
  
  Move ans_move;
  int ans_score = 0;
  Minimax* root = new Minimax(state); //point to present state directly
  self_player = root->state->player;
  int tmp_int;

  for(int i = 0; i < root->state->legal_actions.size(); i++){
    Minimax* tmp = new Minimax(root->state->next_state(root->state->legal_actions[i])); //player change
    root->next_node.push_back(tmp);
    root->next_node[i]->state->get_legal_actions();
    tmp_int = root->next_node[i]->select_score(1);
    if(tmp_int > ans_score){
      ans_score = tmp_int;
      ans_move = root->state->legal_actions[i];
    }
  }  

  return ans_move;
}

int Minimax::select_score(int depth){
  if(!this->state->legal_actions.size())
    this->state->get_legal_actions();

  int ans_score = 0;
  int tmp_score;

  for(int i = 0; i < this->state->legal_actions.size(); i++){
    Minimax* tmp = new Minimax(this->state->next_state(this->state->legal_actions[i])); //player change
    this->next_node.push_back(tmp);
    this->next_node[i]->state->get_legal_actions();

    if(depth + 1 < MAX_DEPTH) tmp_score = this->next_node[i]->select_score(depth + 1);
    else tmp_score = this->next_node[i]->state->evaluate();

    if(this->state->player == self_player){ //choose max
      (ans_score == 0 || tmp_score > ans_score) ? ans_score = tmp_score : 1;
    }else{ //choose min
      (ans_score == 0 || tmp_score < ans_score) ? ans_score = tmp_score : 1;
    }
  }

  return ans_score;
}