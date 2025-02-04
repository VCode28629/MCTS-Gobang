#include<cstdio>
#include"game.h"
#include"mcts.h"
#include <exception>
#include <iostream>
#include "utils.h"
#include "log.h"

MCTS mcts;

void print_board(GameState s, int x=-1, int y=-1) {
    printf("  ");
    for(int i = 0; i < BOARD_SIZE; ++i) {
        printf(" %02d", i);
    }
    putchar('\n');
    for(int i = 0; i < BOARD_SIZE; ++i) {
        printf("%02d", i);
        for(int j = 0; j < BOARD_SIZE; ++j) {
            // if(x == i && y == j) printf("\033[31m");
            if(s.board[i][j] == None) {
                printf("  .");
            } else if(s.board[i][j] == Black) {
                printf("  X");
            } else if(s.board[i][j] == White) {
                printf("  O");
            }
            // if(x == i && y == j) printf("\033[0m");
        }
        putchar('\n');
    }
}

void game() {
    Go game;
    mcts.init_game();
    mcts.game = &game;
    Player winner = None;

    int step = 0;
    print_board(game.state);
    while(winner == None) {
        log(Info, fstring("step: %d", game.get_step()));
        mcts.think_by_time(std::chrono::seconds(19));
        mcts.print_winning_rate();
        Action action = mcts.take_action();
        step += 1;
        printf("Move %d: %s %02d %02d\n", step, game.player == Black ? "Black" : "White", action.first, action.second);
        game.move(action);
        print_board(game.state, action.first, action.second);
        if(game.over) {
            winner = game.winner;
            break;
        }
    }
    if(winner = Black) {
        puts("winner: Black");
    } else if(winner = White) {
        puts("winner: White");
    } else {
        puts("Balanced");
    }
}


int main() {
    while(true) {
        try {
            game();
        } catch(const std::exception& e) {
            // 处理异常
            std::cerr << "Catched exception: " << e.what() << '\n';
        }
    }
}