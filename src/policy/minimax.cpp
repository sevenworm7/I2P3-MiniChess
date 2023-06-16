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
    if(!this->next_node[i]->state->legal_actions.size())
      this->next_node[i]->state->get_legal_actions();
    tmp_int = this->next_node[i]->select_score(1);

    if(tmp_int > ans_score || ans_score == 0){
      ans_score = tmp_int;
      ans_move = this->state->legal_actions[i];
    }else if(std::abs(tmp_int - ans_score) <= 5 && (int)std::rand()%3 == 0){ //if poss gets close, choose random
      ans_score = tmp_int;
      ans_move = this->state->legal_actions[i];
    }
  }  
  return ans_move;
}

//delete when running
int Minimax::select_score(int depth){
  if(!this->state->legal_actions.size())
    this->state->get_legal_actions();

  int ans_score = 0;
  int tmp_score;
  int size = (int)this->state->legal_actions.size();

  for(int i = 0; i < size; i++){
    //init
    Minimax* tmp = new Minimax(this->state->next_state(this->state->legal_actions[i])); //player change
    this->next_node.push_back(tmp);
    if(!this->next_node[i]->state->legal_actions.size())
      this->next_node[i]->state->get_legal_actions();

    //calculate score
    if(this->next_node[i]->state->game_state == WIN) this->state->player == self_player ? tmp_score = -6666666 : tmp_score = 6666666;
    else if(depth + 1 < MAX_DEPTH) tmp_score = this->next_node[i]->select_score(depth + 1);
    else tmp_score = this->next_node[i]->state->evaluate(); //evaluate for opponent //is next state player

    //choose move //since it evaluate for the reversing player
    if(this->state->player == self_player){ 
      (ans_score == 0 || tmp_score > ans_score) ? ans_score = tmp_score : 1;
    }else{
      (ans_score == 0 || tmp_score < ans_score) ? ans_score = tmp_score : 1;
    }
  }

  return ans_score;
}