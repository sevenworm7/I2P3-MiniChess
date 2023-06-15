#include <cstdlib>
#include <vector>

#include "../state/state.hpp"
#include "./minimax.hpp"

static int self_player;

Move Minimax::get_move(){ //present state
  if(!this->state->legal_actions.size())
    this->state->get_legal_actions();
  
  Move ans_move;
  int ans_score = 0;
  self_player = this->state->player;
  int tmp_int;
  int size = (int)this->state->legal_actions.size();

  for(int i = 0; i < size; i++){
    Minimax* tmp = new Minimax(this->state->next_state(this->state->legal_actions[i])); //player change
    this->next_node.push_back(tmp);
    this->next_node[i]->state->get_legal_actions();
    tmp_int = this->next_node[i]->select_score(1);
    if(tmp_int == 0 || tmp_int > ans_score){
      ans_score = tmp_int;
      ans_move = this->state->legal_actions[i];
    }
  }  

  return ans_move;
}

int Minimax::select_score(int depth){
  if(!this->state->legal_actions.size())
    this->state->get_legal_actions();

  int ans_score = 0;
  int tmp_score;
  int size = (int)this->state->legal_actions.size();

  for(int i = 0; i < size; i++){
    Minimax* tmp = new Minimax(this->state->next_state(this->state->legal_actions[i])); //player change
    this->next_node.push_back(tmp);
    this->next_node[i]->state->get_legal_actions();

    if(depth + 1 < MAX_DEPTH) tmp_score = this->next_node[i]->select_score(depth + 1);
    else tmp_score = this->next_node[i]->state->evaluate(); //evaluate for opponent //is next state player

    //since it evaluate for the reversing player
    if(this->state->player == self_player){ //choose min
      (ans_score == 0 || tmp_score > ans_score) ? ans_score = tmp_score : 1;
    }else{ //choose max
      (ans_score == 0 || tmp_score < ans_score) ? ans_score = tmp_score : 1;
    }
  }

  return ans_score;
}