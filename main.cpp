#include <cstdio>
#include <exception>
#include <iostream>

#include "games/games.h"
#include "log.h"
#include "mcts.h"
#include "utils.h"

void game() {
    MCTS mcts;
    Gobang game;
    mcts.game = &game;
    Player winner = None;

    int step = 0;
    game.print_board();
    while (winner == None) {
        log(Info, fstring("step: %d", game.get_step()));
        mcts.think_until_good(0.001);
        // mcts.print_winning_rate();
        // mcts.print_visit_times();
        Action action = mcts.take_action();
        step += 1;
        printf("Move %d: %s %02d %02d\n", step,
               game.player == Black ? "Black" : "White", action.first,
               action.second);
        game.move(action);
        game.print_board();
        if (game.over) {
            winner = game.winner;
            break;
        }
    }
    if (winner == Black) {
        puts("winner: Black");
    } else if (winner == White) {
        puts("winner: White");
    } else {
        puts("Balanced");
    }
}

int main() {
    while (true) {
        try {
            game();
        } catch (const std::exception &e) {
            // 处理异常
            std::cerr << "Catched exception: " << e.what() << '\n';
        }
    }
}