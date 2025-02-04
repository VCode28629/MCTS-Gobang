#pragma once

#include "const.h"

enum Player : char {
    Black=0,
    None=1,
    White=2,
};

struct GameState {
    Player board[BOARD_SIZE][BOARD_SIZE];
};

class Gobang {
    GameState state;
    Player player;
public:
    static inline Player next_player(Player p) {
        return static_cast<Player>(Black + White - p);
    }
    static Player get_winner(GameState state);
    static Player get_winner(GameState state, int last_x, int last_y);
};