#include "mcts.h"
#include "log.h"
#include "utils.h"
#include<cstdio>
#include<cmath>
#include<fstream>

MCTNode::MCTNode(Player p, MCTNode* par) : player(p), parent(par), wins(0), visits(0) {}

MCTNode::~MCTNode() {
    for (auto child : children) delete child;
}

MCTNode *MCT::get_root() {
    return this->tree_root;
}

MCT::MCT() {
    State s;
    for(int i = 0; i < BOARD_SIZE; ++i) {
        for(int j = 0; j < BOARD_SIZE; ++j) {
            s.board[i][j] = None;
        }
    }
    this->tree_root = new MCTNode(Black);
}

MCT::~MCT() {
    delete this->tree_root;    
}

MCTS::MCTS() {
    this->tree = new MCT();
}

MCTS::~MCTS() {
    delete this->tree;
}

void MCTS::init_game() {
    this->now_state = this->tree->get_root();
}

MCTNode *MCTS::selection() {
    MCTNode *node = this->now_state;
    while(!node->children.empty()) {
        double best_score = -std::numeric_limits<double>::max();
        int best_child_index = -1;
        for(int i = 0; i < node->children.size(); ++i) {
            auto child = node->children[i];
            if(child->visits == 0) {
                game->move(node->actions[i]);
                return child;
            }
            double score = (double)child->wins / child->visits + std::sqrt(2 * std::log(node->visits) / child->visits);
            if (score <= best_score) continue;
            best_score = score;
            best_child_index = i;
        }
        log(Debug, fstring("best score: %lf", best_score));
        if(best_child_index == -1) {
            log(Error, "best_child is nullptr when selection");
            return node;
        }
        game->move(node->actions[best_child_index]);
        node = node->children[best_child_index];
    }
    return node;
}

void MCTS::expansion(MCTNode *node) {
    if (!node->children.empty()) {
        log(Warning, "expansion a node twice");
        return;
    }
    for(int i = 0; i < BOARD_SIZE; ++i) {
        for(int j = 0; j < BOARD_SIZE; ++j) {
            if(!game->can_move(std::make_pair(i, j))) continue;
            node->children.push_back(new MCTNode(game->next_player(node->player), node));
            node->actions.push_back(std::make_pair(i, j));
        }
    }
    if(game->can_move(std::make_pair(-1, -1))) {
        node->children.push_back(new MCTNode(game->next_player(node->player), node));
        node->actions.push_back(std::make_pair(-1, -1));
    }
}

Player MCTS::simulation() {
    int size = game->get_step();
    while(!game->over) {
        std::vector<Action> moves;
        for(int i = 0; i < BOARD_SIZE; ++i) {
            for(int j = 0; j < BOARD_SIZE; ++j) {
                if(!game->can_move(std::make_pair(i, j))) continue;
                moves.push_back(std::make_pair(i, j));
            }
        }
        if(game->can_move(std::make_pair(-1, -1))) moves.push_back({-1, -1});
        Action move = moves[random(moves.size())];
        game->move(move);
    }
    Player winner = game->winner;
    while(game->get_step() > size) game->rollback();
    return winner;
}

void MCTS::backup(MCTNode *node, Player winner) {
    while(node != nullptr) {
        ++node->visits;
        if(winner == None) node->wins += 0.5;
        else if(winner != node->player) {node->wins += 1;}
        node = node->parent;
    }
}

void MCTS::think_once() {
    int size = game->get_step();
    MCTNode *leaf = this->selection();
    this->expansion(leaf);
    if(!leaf->children.empty()) {
        int index = random(leaf->children.size());
        game->move(leaf->actions[index]);
        leaf = leaf->children[index];
    }
    Player winner = this->simulation();
    backup(leaf, winner);
    while(game->get_step() > size) game->rollback();
}

void MCTS::think_by_times(int times) {
    log(Info, "------start thinking------");
    log(Info, fstring("set thought times: %d", times));
    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 1; i <= times; ++i) {
        this->think_once();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = end - start;
    log(Info, fstring("thought time: %s", durationToString(duration).c_str()));
    log(Info, "------end thinking------");
}

void MCTS::think_by_time(std::chrono::duration<double> limit) {
    log(Info, "------start thinking------");
    log(Info, fstring("set thinking time: %s", durationToString(limit).c_str()));
    auto start = std::chrono::high_resolution_clock::now();
    auto now = start;

    int cnt = 0;
    while(now - start < limit) {
        this->think_once();
        ++cnt;
        now = std::chrono::high_resolution_clock::now();
    }

    auto duration = now - start;
    log(Info, fstring("thought times: %d", cnt));
    log(Info, fstring("thought time: %s", durationToString(duration).c_str()));
    log(Info, "------end thinking------");
}

Action MCTS::take_action() {
    if(now_state->children.empty()) return std::make_pair(-1, -1);
    int index = 0;
    for(int i = 1; i < now_state->children.size(); ++i) {
        if(now_state->children[i]->visits > now_state->children[index]->visits) {
            index = i;
        }
    }
    Action ret = now_state->actions[index];
    now_state = now_state->children[index];
    return ret;
}

void MCTS::print_winning_rate() {
    double p[BOARD_SIZE][BOARD_SIZE];
    for(int i = 0; i < BOARD_SIZE; ++i) {
        for(int j = 0; j < BOARD_SIZE; ++j) {
            p[i][j] = 0;
        }
    }
    double pass_p = -1;
    for(int i = 0; i < now_state->children.size(); ++i) {
        double pro = -1;
        if(now_state->children[i]->visits == 0) continue;
        pro = now_state->children[i]->wins / now_state->children[i]->visits;
        if(now_state->actions[i].first == -1) {
            pass_p = pro;
        } else {
            p[now_state->actions[i].first][now_state->actions[i].second] = pro;
        }
            }
    for(int i = 0; i < BOARD_SIZE; ++i) {
        for(int j = 0; j < BOARD_SIZE; ++j) {
            printf("%6.1lf ", p[i][j] * 100);
        }
        putchar('\n');
    }
    printf("%6.1lf\n", pass_p * 100);
}
