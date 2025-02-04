#include<cstdio>
#include"game.h"
#include"mcts.h"
#include <exception>
#include <iostream>

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
    GameState s;
    Player player = Black;
    for(int i = 0; i < BOARD_SIZE; ++i) {
        for(int j = 0; j < BOARD_SIZE; ++j) {
            s.board[i][j] = None;
        }
    }
    mcts.init_game();
    Player winner = None;
    int step = 0;
    print_board(s);
    while(winner == None) {
        mcts.think_by_time(std::chrono::seconds(5));
        mcts.print_winning_rate();
        Action action = mcts.take_action();
        s.board[action.first][action.second] = player;
        step += 1;
        printf("Move %d: %s %02d %02d\n", step, player == Black ? "Black" : "White", action.first, action.second);
        player = Gobang::next_player(player);
        winner = Gobang::get_winner(s, action.first, action.second);
        print_board(s, action.first, action.second);
        if(step == BOARD_SIZE * BOARD_SIZE) {
            printf("Balanced\n");
            mcts.serialize();
            return;
        }
    }
    printf("winner: %s\n", winner == Black ? "Black" : "White");
    mcts.serialize();
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