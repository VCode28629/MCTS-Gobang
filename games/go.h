#pragma once

#include <cstdio>

#include "gameBase.h"

const int GO_BOARD_SIZE = 15;
const float KOMI = 7.5;

struct GoState {
    Player board[GO_BOARD_SIZE][GO_BOARD_SIZE];
    bool operator==(const GoState &s) const {
        for (int i = 0; i < GO_BOARD_SIZE; ++i) {
            for (int j = 0; j < GO_BOARD_SIZE; ++j) {
                if (board[i][j] != s.board[i][j]) return false;
            }
        }
        return true;
    }
};

class Go : public Game {
    GoState state;
    bool pass;
    std::vector<GoState> history;
    // player, pass, over, winner;
    std::stack<std::tuple<Player, bool, bool, Player> > info_history;
    void try_eat(GoState &state, int x, int y);
    int count(const GoState &state, int x, int y, bool *colored);
    Player get_winner(const GoState &state);
    GoState next_state(GoState state, Player player, Action action);
    bool can_move(Action action);
    static const int step[4][2];

   public:
    Go();
    virtual int get_step();
    virtual std::vector<Action> get_legal_moves();
    virtual void move(Action action);
    virtual void undo();
    virtual void print_board() {
        printf("  ");
        for (int i = 0; i < GO_BOARD_SIZE; ++i) {
            printf(" %02d", i);
        }
        putchar('\n');
        for (int i = 0; i < GO_BOARD_SIZE; ++i) {
            printf("%02d", i);
            for (int j = 0; j < GO_BOARD_SIZE; ++j) {
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
        if (pass) puts("passed");
    }
};