#include "mcts.h"

#include <cmath>
#include <cstdio>
#include <fstream>

#include "log.h"
#include "utils.h"

MCTNode::MCTNode(Player p, MCTNode *par)
    : player(p), parent(par), wins(0), visits(0) {}

MCTNode::~MCTNode() {
    for (auto child : children)
        if (child != nullptr) delete child;
}

MCTS::MCTS() { this->root = new MCTNode(Black, nullptr); }

MCTS::~MCTS() { delete this->root; }

MCTNode *MCTS::selection() {
    MCTNode *node = this->root;
    while (!node->children.empty()) {
        double best_score = -std::numeric_limits<double>::max();
        int best_child_index = -1;
        for (int i = 0; i < node->children.size(); ++i) {
            auto child = node->children[i];
            if (child->visits == 0) {
                game->move(node->actions[i]);
                return child;
            }
            double score =
                (double)child->wins / child->visits +
                std::sqrt(2 * std::log(node->visits) / child->visits);
            if (score <= best_score) continue;
            best_score = score;
            best_child_index = i;
        }
        game->move(node->actions[best_child_index]);
        node = node->children[best_child_index];
    }
    return node;
}

void MCTS::expansion(MCTNode *node) {
    auto moves = game->get_legal_moves();
    for (auto move : moves) {
        node->children.push_back(
            new MCTNode(game->next_player(node->player), node));
        node->actions.push_back(move);
    }
}

Player MCTS::simulation() {
    int size = game->get_step();
    while (!game->over) {
        auto moves = game->get_legal_moves();
        Action move = moves[random(moves.size())];
        game->move(move);
    }
    Player winner = game->winner;
    while (game->get_step() > size) game->undo();
    return winner;
}

void MCTS::backup(MCTNode *node, Player winner) {
    while (node != nullptr) {
        ++node->visits;
        if (winner == None)
            node->wins += 0.5;
        else if (winner != node->player) {
            node->wins += 1;
        }
        node = node->parent;
    }
}

void MCTS::think_once() {
    int size = game->get_step();
    MCTNode *leaf = this->selection();
    this->expansion(leaf);
    if (!leaf->children.empty()) {
        int index = random(leaf->children.size());
        game->move(leaf->actions[index]);
        leaf = leaf->children[index];
    }
    Player winner = this->simulation();
    backup(leaf, winner);
    while (game->get_step() > size) game->undo();
}

void MCTS::think_by_times(int times) {
    log(Info, "------start thinking------");
    log(Info, fstring("set thought times: %d", times));
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 1; i <= times; ++i) {
        this->think_once();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = end - start;
    log(Info, fstring("thought time: %s", durationToString(duration).c_str()));
    log(Info, "------end thinking------");
}

void MCTS::think_by_time(std::chrono::duration<double> limit) {
    log(Info, "------start thinking------");
    log(Info,
        fstring("set thinking time: %s", durationToString(limit).c_str()));
    auto start = std::chrono::high_resolution_clock::now();
    auto now = start;

    int cnt = 0;
    while (now - start < limit) {
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
    if (root == nullptr || root->children.empty())
        return std::make_pair(-1, -1);
    int index = 0;
    for (int i = 1; i < root->children.size(); ++i) {
        if (root->children[i]->visits > root->children[index]->visits) {
            index = i;
        }
    }
    auto selected = root->children[index];
    Action ret = root->actions[index];
    root->children[index] = nullptr;
    delete root;
    root = selected;
    root->parent = nullptr;
    return ret;
}

// void MCTS::print_winning_rate() {
//     double p[BOARD_SIZE][BOARD_SIZE];
//     for(int i = 0; i < BOARD_SIZE; ++i) {
//         for(int j = 0; j < BOARD_SIZE; ++j) {
//             p[i][j] = 0;
//         }
//     }
//     double pass_p = -1;
//     for(int i = 0; i < root->children.size(); ++i) {
//         double pro = -1;
//         if(root->children[i]->visits == 0) continue;
//         pro = root->children[i]->wins / root->children[i]->visits;
//         if(root->actions[i].first == -1) {
//             pass_p = pro;
//         } else {
//             p[root->actions[i].first][root->actions[i].second] = pro;
//         }
//             }
//     for(int i = 0; i < BOARD_SIZE; ++i) {
//         for(int j = 0; j < BOARD_SIZE; ++j) {
//             printf("%6.1lf ", p[i][j] * 100);
//         }
//         putchar('\n');
//     }
//     printf("%6.1lf\n", pass_p * 100);
// }

// void MCTS::print_visit_times() {
//     int p[BOARD_SIZE][BOARD_SIZE];
//     for(int i = 0; i < BOARD_SIZE; ++i) {
//         for(int j = 0; j < BOARD_SIZE; ++j) {
//             p[i][j] = 0;
//         }
//     }
//     int pass_p = -1;
//     for(int i = 0; i < root->children.size(); ++i) {
//         if(root->actions[i].first == -1) {
//             pass_p = root->children[i]->visits;
//         } else {
//             p[root->actions[i].first][root->actions[i].second] =
//             root->children[i]->visits;
//         }
//     }
//     for(int i = 0; i < BOARD_SIZE; ++i) {
//         for(int j = 0; j < BOARD_SIZE; ++j) {
//             printf("%6d ", p[i][j]);
//         }
//         putchar('\n');
//     }
//     printf("%6d\n", pass_p);
// }
