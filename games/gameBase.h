#pragma once

#include <stack>
#include <vector>

enum Player : char {
    Black = 0,
    None = 1,
    White = 2,
};

using Action = std::pair<int, int>;

class Game {
   public:
    Player player;
    bool over;
    Player winner;

    virtual std::vector<Action> get_legal_moves() = 0;
    virtual int get_step() = 0;
    virtual void move(Action action) = 0;
    virtual void undo() = 0;

    virtual inline Player next_player(Player p) {
        return static_cast<Player>(Black + White - p);
    }
};
