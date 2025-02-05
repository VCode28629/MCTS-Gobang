#pragma once

#include "game.h"
#include "json.hpp"

#include<vector>
#include<chrono>

using json = nlohmann::json;

using State = GameState;

struct MCTNode {
    Player player;
    double wins;
    int visits;
    std::vector<MCTNode*> children;
    std::vector<Action> actions;
    MCTNode* parent;
    MCTNode(Player p, MCTNode* par = nullptr);
    ~MCTNode();
};

class MCT {
    MCTNode *tree_root;
public:
    MCT();
    ~MCT();
    inline MCTNode *get_root();
};

class MCTS {
    MCT* tree;
    MCTNode *now_state;
private:
    void think_once();
    MCTNode *selection();
    void expansion(MCTNode *state);
    Player simulation();
    void backup(MCTNode *node, Player winner);
public:
    Go *game;
    MCTS();
    ~MCTS();
    void init_game();
    void think_by_times(int times);
    void think_by_time(std::chrono::duration<double>);
    Action take_action();
    void print_winning_rate();
    void print_visit_times();
};
