#include <iostream>
#include <sstream>
#include <cstdint>
#include <cmath>
#include <algorithm>

#include "./state.hpp"
#include "../config.hpp"

enum Chess{ // 0 2 6 7 8 20 100
  empty = 0, pawn = 1, rook = 2, knight = 3, bishop = 4, queen = 5, king = 6
}; //add

int n_player_attack_xy(int player, int x, int y, char (*board)[BOARD_H][BOARD_W]){
  const int div = 3;
  int value = 0;

  for(int i = 0; i < BOARD_H; i++){
    for(int j = 0; j < BOARD_W; j++){
      switch((int)board[player][i][j]){
        case pawn:
          if((player == 0 && i-x==1 && std::abs(j-y)==1)
          || (player == 1 && x-i==1 && std::abs(y-j)==1))
            value += 8; //eating possibility
          break;
        case rook: 
          if(i-x==0 || j-y==0){
            int xx = x-i!=0 ? (x-i)/std::abs(x-i) : 0;
            int yy = y-j!=0 ? (y-j)/std::abs(y-j) : 0;
            for(int k = 1; ; k++){
              if(i + xx*k == x && j + yy*k == y){
                value += 7;
                break;
              }else if(board[1-player][i + xx*k][j + yy*k] != empty 
              || board[player][i + xx*k][j + yy*k] != empty) break;
            }
          }
          break;
        case knight: 
          if((std::abs(i-x)==1 && std::abs(j-y)==2)
          || (std::abs(i-x)==2 && std::abs(j-y)==1))
            value += 7;
          break;
        case bishop: 
          if(std::abs(i-x) == std::abs(j-y)){
            int xx = (x-i)/std::abs(x-i);
            int yy = (y-j)/std::abs(y-j);
            for(int k = 1; ; k++){
              if(i + xx*k == x && j + yy*k == y){
                value += 7;
                break;
              }else if(board[1-player][i + xx*k][j + yy*k] != empty 
              || board[player][i + xx*k][j + yy*k] != empty) break;
            }
          }
          break;
        case queen: 
          if(std::abs(i-x) == std::abs(j-y) || std::abs(i-x)==0 || std::abs(j-y)==0){
            int xx = x-i!=0 ? (x-i)/std::abs(x-i) : 0;
            int yy = y-j!=0 ? (y-j)/std::abs(y-j) : 0;
            for(int k = 1; ; k++){
              if(i + xx*k == x && j + yy*k == y){
                value += 5;
                break;
              }else if(board[1-player][i + xx*k][j + yy*k] != empty 
              || board[player][i + xx*k][j + yy*k] != empty) break;
            }
          }
          break;
        case king: 
          if(std::abs(i-x)<=1 && std::abs(j-y)<=1) value += 4;
          break;
        default: 
          break;
      }
    }
  }
  return value / div;
}

/**
 * @brief evaluate the state
 * 
 * @return int 
 */
int State::evaluate(){ //state value function
  // [TODO] design your own evaluation function
  const int self_mul = 10;
  const int oppo_mul = 9;
  const int attacked_poss_mul = 1;

  int value = 0;
  int opponent = 1 - player;
  int self_chess_value; //positive
  int oppo_chess_value, attacked_poss_value; //negative
  
  for(int i = 0; i < BOARD_H; i++){
    for(int j = 0; j < BOARD_W; j++){
      switch((int)board.board[player][i][j]){ //self chess value determine
        case pawn: self_chess_value = 1 + (player == 1 ? (1 + i) : (BOARD_H - i)) / 1.9; break; //2
        case rook: self_chess_value = 11; break; //6
        case knight: self_chess_value = 13; break; //7
        case bishop: self_chess_value = 15; break; //8
        case queen: self_chess_value = 30; break; //20
        case king: self_chess_value = 666666; break; //100
        default: self_chess_value = 0; break;
      }
      switch((int)board.board[opponent][i][j]){ //opponent chess value determine
        case pawn: oppo_chess_value = 1 + (opponent == 1 ? (1 + i) : (BOARD_H - i)) / 1.9; break;
        case rook: oppo_chess_value = 11; break;
        case knight: oppo_chess_value = 13; break;
        case bishop: oppo_chess_value = 15; break;
        case queen: oppo_chess_value = 30; break;
        case king: oppo_chess_value = 666666; break;
        default: oppo_chess_value = 0; break;
      }
      if(self_chess_value != 0)
        attacked_poss_value = self_chess_value * n_player_attack_xy(opponent, i, j, board.board); //attacked possibility
      else attacked_poss_value = 0;

      value += ((self_chess_value * self_mul) //sum
        - (oppo_chess_value * oppo_mul + attacked_poss_value * attacked_poss_mul));
    }
  }

  return value;
}


/**
 * @brief return next state after the move
 * 
 * @param move 
 * @return State* 
 */
State* State::next_state(Move move){
  Board next = this->board;
  Point from = move.first, to = move.second;
  
  int8_t moved = next.board[this->player][from.first][from.second];
  //promotion for pawn
  if(moved == 1 && (to.first==BOARD_H-1 || to.first==0)){
    moved = 5;
  }
  if(next.board[1-this->player][to.first][to.second]){
    next.board[1-this->player][to.first][to.second] = 0;
  }
  
  next.board[this->player][from.first][from.second] = 0;
  next.board[this->player][to.first][to.second] = moved;
  
  State* next_state = new State(next, 1-this->player); //change the player
  
  if(this->game_state != WIN)
    next_state->get_legal_actions();
  return next_state;
}


static const int move_table_rook_bishop[8][7][2] = {
  {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}},
  {{0, -1}, {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6}, {0, -7}},
  {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}},
  {{-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0}},
  {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}},
  {{1, -1}, {2, -2}, {3, -3}, {4, -4}, {5, -5}, {6, -6}, {7, -7}},
  {{-1, 1}, {-2, 2}, {-3, 3}, {-4, 4}, {-5, 5}, {-6, 6}, {-7, 7}},
  {{-1, -1}, {-2, -2}, {-3, -3}, {-4, -4}, {-5, -5}, {-6, -6}, {-7, -7}},
};
static const int move_table_knight[8][2] = {
  {1, 2}, {1, -2},
  {-1, 2}, {-1, -2},
  {2, 1}, {2, -1},
  {-2, 1}, {-2, -1},
};
static const int move_table_king[8][2] = {
  {1, 0}, {0, 1}, {-1, 0}, {0, -1}, 
  {1, 1}, {1, -1}, {-1, 1}, {-1, -1},
};


/**
 * @brief get all legal actions of now state
 * 
 */
void State::get_legal_actions(){
  // [Optional]
  // This method is not very efficient
  // You can redesign it
  this->game_state = NONE;
  std::vector<Move> all_actions;
  auto self_board = this->board.board[this->player];
  auto oppn_board = this->board.board[1 - this->player];
  
  int now_piece, oppn_piece;
  for(int i=0; i<BOARD_H; i+=1){
    for(int j=0; j<BOARD_W; j+=1){
      if((now_piece=self_board[i][j])){
        // std::cout << this->player << "," << now_piece << ' ';
        switch (now_piece){
          case 1: //pawn
            if(this->player && i<BOARD_H-1){
              //black
              if(!oppn_board[i+1][j] && !self_board[i+1][j])
                all_actions.push_back(Move(Point(i, j), Point(i+1, j)));
              if(j<BOARD_W-1 && (oppn_piece=oppn_board[i+1][j+1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i+1, j+1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
              if(j>0 && (oppn_piece=oppn_board[i+1][j-1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i+1, j-1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
            }else if(!this->player && i>0){
              //white
              if(!oppn_board[i-1][j] && !self_board[i-1][j])
                all_actions.push_back(Move(Point(i, j), Point(i-1, j)));
              if(j<BOARD_W-1 && (oppn_piece=oppn_board[i-1][j+1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i-1, j+1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
              if(j>0 && (oppn_piece=oppn_board[i-1][j-1])>0){
                all_actions.push_back(Move(Point(i, j), Point(i-1, j-1)));
                if(oppn_piece==6){
                  this->game_state = WIN;
                  this->legal_actions = all_actions;
                  return;
                }
              }
            }
            break;
          
          case 2: //rook
          case 4: //bishop
          case 5: //queen
            int st, end;
            switch (now_piece){
              case 2: st=0; end=4; break; //rook
              case 4: st=4; end=8; break; //bishop
              case 5: st=0; end=8; break; //queen
              default: st=0; end=-1;
            }
            for(int part=st; part<end; part+=1){
              auto move_list = move_table_rook_bishop[part];
              for(int k=0; k<std::max(BOARD_H, BOARD_W); k+=1){
                int p[2] = {move_list[k][0] + i, move_list[k][1] + j};
                
                if(p[0]>=BOARD_H || p[0]<0 || p[1]>=BOARD_W || p[1]<0) break;
                now_piece = self_board[p[0]][p[1]];
                if(now_piece) break;
                
                all_actions.push_back(Move(Point(i, j), Point(p[0], p[1])));
                
                oppn_piece = oppn_board[p[0]][p[1]];
                if(oppn_piece){
                  if(oppn_piece==6){
                    this->game_state = WIN;
                    this->legal_actions = all_actions;
                    return;
                  }else
                    break;
                };
              }
            }
            break;
          
          case 3: //knight
            for(auto move: move_table_knight){
              int x = move[0] + i;
              int y = move[1] + j;
              
              if(x>=BOARD_H || x<0 || y>=BOARD_W || y<0) continue;
              now_piece = self_board[x][y];
              if(now_piece) continue;
              all_actions.push_back(Move(Point(i, j), Point(x, y)));
              
              oppn_piece = oppn_board[x][y];
              if(oppn_piece==6){
                this->game_state = WIN;
                this->legal_actions = all_actions;
                return;
              }
            }
            break;
          
          case 6: //king
            for(auto move: move_table_king){
              int p[2] = {move[0] + i, move[1] + j};
              
              if(p[0]>=BOARD_H || p[0]<0 || p[1]>=BOARD_W || p[1]<0) continue;
              now_piece = self_board[p[0]][p[1]];
              if(now_piece) continue;
              
              all_actions.push_back(Move(Point(i, j), Point(p[0], p[1])));
              
              oppn_piece = oppn_board[p[0]][p[1]];
              if(oppn_piece==6){
                this->game_state = WIN;
                this->legal_actions = all_actions;
                return;
              }
            }
            break;
        }
      }
    }
  }
  std::cout << "\n";
  this->legal_actions = all_actions;
}


const char piece_table[2][7][5] = {
  {" ", "♙", "♖", "♘", "♗", "♕", "♔"},
  {" ", "♟", "♜", "♞", "♝", "♛", "♚"}
};
/**
 * @brief encode the output for command line output
 * 
 * @return std::string 
 */
std::string State::encode_output(){
  std::stringstream ss;
  int now_piece;
  for(int i=0; i<BOARD_H; i+=1){
    for(int j=0; j<BOARD_W; j+=1){
      if((now_piece = this->board.board[0][i][j])){
        ss << std::string(piece_table[0][now_piece]);
      }else if((now_piece = this->board.board[1][i][j])){
        ss << std::string(piece_table[1][now_piece]);
      }else{
        ss << " ";
      }
      ss << " ";
    }
    ss << "\n";
  }
  return ss.str();
}


/**
 * @brief encode the state to the format for player
 * 
 * @return std::string 
 */
std::string State::encode_state(){
  std::stringstream ss;
  ss << this->player;
  ss << "\n";
  for(int pl=0; pl<2; pl+=1){
    for(int i=0; i<BOARD_H; i+=1){
      for(int j=0; j<BOARD_W; j+=1){
        ss << int(this->board.board[pl][i][j]);
        ss << " ";
      }
      ss << "\n";
    }
    ss << "\n";
  }
  return ss.str();
}