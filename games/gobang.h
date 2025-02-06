#pragma once

#include <cstdio>

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
    Player get_winner(int x, int y);
    // player, over, winner;
    std::stack<std::tuple<Player, bool, Player> > info_history;

   public:
    Gobang();
    virtual int get_step();
    virtual std::vector<Action> get_legal_moves();
    virtual void move(Action action);
    virtual void undo();
    virtual void print_board() {
        printf("  ");
        for (int i = 0; i < GOBANG_BOARD_SIZE; ++i) {
            printf(" %02d", i);
        }
        putchar('\n');
        for (int i = 0; i < GOBANG_BOARD_SIZE; ++i) {
            printf("%02d", i);
            for (int j = 0; j < GOBANG_BOARD_SIZE; ++j) {
                if (state.board[i][j] == None) {
                    printf("  .");
                } else if (state.board[i][j] == Black) {
                    printf("  X");
                } else if (state.board[i][j] == White) {
                    printf("  O");
                }
            }
            putchar('\n');
        }
    }
};