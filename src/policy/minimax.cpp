#include <cstdlib>
#include <vector>

#include "../state/state.hpp"
#include "./minimax.hpp"

static int self_player; //let all evaluater be the self_player

Move minimax::get_move(){ //present state
  int ans_score = 0;
  Move ans_move;
  int tmp_score;
  self_player = this->state->player;

  if(this->state->legal_actions.empty())
    this->state->get_legal_actions();
  for(int i = 0; i < (int)this->state->legal_actions.size(); i++){
    minimax* target = new minimax(this->state->next_state(this->state->legal_actions[i]));
    this->next_node.push_back(target);
    if(this->next_node[i]->state->legal_actions.empty())
      this->next_node[i]->state->get_legal_actions();
    if(this->next_node[i]->state->game_state == WIN) continue; //opponent will win, neglect this choice
    tmp_score = this->next_node[i]->extend(1); //switch to function 2
    if(tmp_score > ans_score || ans_score == 0){
        ans_score = tmp_score;
        ans_move = this->state->legal_actions[i];
    }
  }
  while(!this->next_node.empty()){ //delete
    delete this->next_node.back();
    this->next_node.pop_back();
  } //doesnt delete this node
  return ans_move;
}

int minimax::extend(int depth){
  int ans_score = 0, tmp_score;

  if(this->state->legal_actions.empty())
    this->state->get_legal_actions();
  for(int i = 0; i < (int)this->state->legal_actions.size(); i++){
    minimax* target = new minimax(this->state->next_state(this->state->legal_actions[i]));
    this->next_node.push_back(target);
    if(this->next_node[i]->state->legal_actions.empty())
      this->next_node[i]->state->get_legal_actions();
    if(this->next_node[i]->state->game_state == WIN) continue; //opponent will win, neglect this choice
    if(depth >= MAX_DEPTH && this->next_node[i]->state->player == self_player) //control that evaluater will be self player
      tmp_score = this->next_node[i]->state->evaluate();
    else tmp_score = this->next_node[i]->extend(depth + 1);
    if(this->state->player == self_player){ //result
      if(tmp_score > ans_score || ans_score == 0) ans_score = tmp_score;
    }else{
      if(tmp_score < ans_score || ans_score == 0) ans_score = tmp_score;
    }
  }
  while(!this->next_node.empty()){ //delete
    delete this->next_node.back();
    this->next_node.pop_back();
  } //doesnt delete this node
  return ans_score;
}