#pragma once

#include "gameBase.h"

const int GOBANG_BOARD_SIZE = 15;
const int WIN_LINE = 5;

struct GobangState {
    Player board[GOBANG_BOARD_SIZE][GOBANG_BOARD_SIZE];
    bool operator==(const GobangState &s) const {
        for (int i = 0; i < GOBANG_BOARD_SIZE; ++i) {
            for (int j = 0; j < GOBANG_BOARD_SIZE; ++j) {
                if (board[i][j] != s.board[i][j]) return false;
            }
        }
        return true;
    }
};

class Gobang : public Game {
    GobangState state;
    std::vector<Action> actions;
    Player Gobang::get_winner(int x, int y);

   public:
    Gobang(int board_size = 15, int win_line = 5);
    virtual int get_step();
    virtual std::vector<Action> get_legal_moves();
    virtual void move(Action action);
    virtual void undo();
};