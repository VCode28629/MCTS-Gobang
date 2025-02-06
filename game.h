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

class Game {
public:
    GameState state;
    Player player;
    bool over;
    Player winner;

    virtual bool can_move(Action action)=0;
    virtual std::vector<Action> get_legal_moves()=0;
    virtual int get_step()=0;
    virtual void move(Action action)=0;
    virtual void undo()=0;
    
    virtual inline Player next_player(Player p) {
        return static_cast<Player>(Black + White - p);
    }
};

class Go : public Game {
    bool pass;
    std::vector<GameState> history;
    // player, pass, over, winner;
    std::stack<std::tuple<Player, bool, bool, Player> > info_history;
    static void try_eat(GameState &state, int x, int y);
    static int count(const GameState &state, int x, int y, bool *colored);
    static const int step[4][2];
    static Player get_winner(const GameState &state);
    static GameState next_state(GameState state, Player player, Action action);
    bool can_move(Action action);
public:
    Go();
    virtual int get_step();
    virtual std::vector<Action> get_legal_moves();
    virtual void move(Action action);
    virtual void undo();
};