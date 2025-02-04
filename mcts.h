#pragma once

#include "game.h"
#include "json.hpp"

#include<vector>
#include<chrono>

using json = nlohmann::json;

using State = GameState;
using Action = std::pair<int, int>;

struct MCTNode {
    State state;
    Player player;
    double wins;
    int visits;
    std::vector<MCTNode*> children;
    std::vector<Action> actions;
    MCTNode* parent;
    MCTNode(State s, Player p, MCTNode* par = nullptr);
    ~MCTNode();
};

class MCT {
    MCTNode *tree_root;
    json serialize(MCTNode *node);
    void deserialize(MCTNode *node, json data);
public:
    MCT();
    ~MCT();
    inline MCTNode *get_root();
    void serialize(const char *file_path);
    void deserialize(const char *file_path);
};

class MCTS {
    MCT* tree;
    MCTNode *now_state;
    Player now_player;
private:
    void think_once();
    MCTNode *selection();
    void expansion(MCTNode *state);
    Player simulation(State state, Player player);
    void backup(MCTNode *node, Player winner);
public:
    MCTS();
    ~MCTS();
    void init_game();
    void think_by_times(int times);
    void think_by_time(std::chrono::duration<double>);
    Action take_action();
    void serialize();
    void print_winning_rate();
};
