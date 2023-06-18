#include <iostream>
#include <fstream>

#include "../config.hpp"
#include "../state/state.hpp"
#include "../policy/alpha_beta.hpp"

State* s;

void read_board(std::ifstream& fin) {
  Board board;
  int player;
  fin >> player;

  for (int pl=0; pl<2; pl++) {
    for (int i=0; i<BOARD_H; i++) {
      for (int j=0; j<BOARD_W; j++) {
        int c; fin >> c;
        // std::cout << c << " ";
        board.board[pl][i][j] = c;
      }
      // std::cout << std::endl;
    }
  }
  s = new State(board, player);
  s->get_legal_actions();
}

void write_valid_spot(std::ofstream& fout) {
  alpha_beta* root = new alpha_beta(s, 0, 0);
  auto move = root->get_move();
  fout << move.first.first << " " << move.first.second << " "\
       << move.second.first << " " << move.second.second << std::endl;
  fout.flush();
}

int main(int, char** argv) {
  srand(RANDOM_SEED);
  std::ifstream fin(argv[1]);
  std::ofstream fout(argv[2]);

  read_board(fin);
  write_valid_spot(fout);

  fin.close();
  fout.close();
  return 0;
}
