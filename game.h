#pragma once

#include "const.h"
#include<vector>
#include<stack>

enum Player : char {
    Black=0,
    None=1,
    White=2,
};

struct GameState {
    Player board[BOARD_SIZE][BOARD_SIZE];
    bool operator==(const GameState &s) const {
        for(int i = 0; i < BOARD_SIZE; ++i) {
            for(int j = 0; j < BOARD_SIZE; ++j) {
                if(board[i][j] != s.board[i][j]) return false;
            }
        }
        return true;
    }
};

using Action = std::pair<int, int>;

class Go {
    bool pass;
    std::vector<GameState> history;
    // player, pass, over, winner;
    std::stack<std::tuple<Player, bool, bool, Player> > info_history;
    static void try_eat(GameState &state, int x, int y);
    static int count(const GameState &state, int x, int y, bool *colored);
    static const int step[4][2];
    static Player get_winner(const GameState &state);
public:
    GameState state;
    Player player;
    bool over;
    Player winner;
    Go();
    static inline Player next_player(Player p) {
        return static_cast<Player>(Black + White - p);
    }
    bool can_move(Action action);
    static GameState next_state(GameState state, Player player, Action action);
    void move(Action action);
    int get_step() {
        return history.size();
    }
    void rollback() {
        history.pop_back();
        state = history.back();
        info_history.pop();
        std::tie(player, pass, over, winner) = info_history.top();
    }
};