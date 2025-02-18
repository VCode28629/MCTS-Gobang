#pragma once

#include <chrono>
#include <vector>

#include "games/games.h"

struct MCTNode {
    Player player;
    double wins;
    int visits;
    std::vector<MCTNode *> children;
    std::vector<Action> actions;
    MCTNode *parent;
    MCTNode(Player p, MCTNode *par = nullptr);
    ~MCTNode();
};

class MCTS {
    MCTNode *root;

   private:
    void think_once();
    MCTNode *selection();
    void expansion(MCTNode *state);
    Player simulation();
    void backup(MCTNode *node, Player winner);

   public:
    Game *game;
    MCTS();
    ~MCTS();
    void think_by_times(int times);
    void think_by_time(std::chrono::duration<double>);
    void think_until_good(double delta);
    Action take_action();
    // void print_winning_rate();
    // void print_visit_times();
};
