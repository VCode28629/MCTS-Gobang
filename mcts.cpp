#include "mcts.h"
#include "log.h"
#include "utils.h"
#include<cstdio>
#include<cmath>
#include<fstream>

MCTNode::MCTNode(State s, Player p, MCTNode* par) 
    : state(s), player(p), parent(par), wins(0), visits(0) {}

MCTNode::~MCTNode() {
    for (auto child : children) delete child;
}

json MCT::serialize(MCTNode *node) {

    json ret = json::object();
    ret["state"] = json::array();
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            ret["state"].push_back(static_cast<int>(node->state.board[i][j]));
        }
    }
    ret["player"] = static_cast<int>(node->player);
    ret["wins"] = node->wins;
    ret["visits"] = node->visits;
    ret["children"] = json::array();
    for (const auto& child : node->children) {
        ret["children"].push_back(serialize(child));
    }
    ret["actions"] = json::array();
    for (const auto& action : node->actions) {
        ret["actions"].push_back(action.first * BOARD_SIZE + action.second);
    }
    return ret;
}

void MCT::serialize(const char *file_path) {
    json root_json = serialize(tree_root);
    std::ofstream ofs(file_path);
    ofs << root_json.dump(4);
    ofs.close();
}

void MCT::deserialize(MCTNode *node, json data) {
    // // 反序列化基础字段
    // node->wins = data["wins"].get<double>();
    // node->visits = data["visits"].get<int>();
    // for(auto& action : data["actions"]) {
    //     int a = action.get<int>();
    //     node->actions.push_back(std::make_pair(a / BOARD_SIZE, a % BOARD_SIZE));
    // }
    
    // // 反序列化子节点
    // for (auto& child_data : data["children"]) {
    //     // 创建子节点state
    //     State child_state;
    //     for (int i = 0; i < BOARD_SIZE; ++i) {
    //         auto row = child_data["state"][i];
    //         for (int j = 0; j < BOARD_SIZE; ++j) {
    //             child_state.board[i][j] = static_cast<Player>(row[j].get<int>());
    //         }
    //     }
        
    //     // 创建子节点并添加到children
    //     Player p = static_cast<Player>(child_data["player"].get<int>());
    //     MCTNode* child = new MCTNode(child_state, p, node);
    //     node->children.push_back(child);
        
    //     // 递归反序列化
    //     deserialize(child, child_data);
    // }
}

void MCT::deserialize(const char *file_path) {
    // std::ifstream ifs(file_path);
    // json data = json::parse(ifs);
    
    // // 清空原有树结构
    // delete tree_root;
    
    // // 重建根节点
    // State root_state;
    // for (int i = 0; i < BOARD_SIZE; ++i) {
    //     auto row = data["state"][i];
    //     for (int j = 0; j < BOARD_SIZE; ++j) {
    //         root_state.board[i][j] = static_cast<Player>(row[j].get<int>());
    //     }
    // }
    // Player p = static_cast<Player>(data["player"].get<int>());
    // tree_root = new MCTNode(root_state, p);
    
    // // 递归构建子树
    // deserialize(tree_root, data);
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
    this->tree_root = new MCTNode(s, Black);
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
    this->now_player = Black;
}


MCTNode *MCTS::selection() {
    MCTNode *node = this->now_state;
    while(!node->children.empty()) {
        double best_score = -std::numeric_limits<double>::max();
        MCTNode* best_child = nullptr;
        for (auto child : node->children) {
            if(child->visits == 0) return child;
            double score = (double)child->wins / child->visits + std::sqrt(2 * std::log(node->visits) / child->visits);
            if (score <= best_score) continue;
            best_score = score;
            best_child = child;
        }
        log(Debug, fstring("best score: %lf", best_score));
        if(best_child == nullptr) {
            log(Error, "best_child is nullptr when selection");
            return node;
        }
        node = best_child;
    }
    return node;
}

void MCTS::expansion(MCTNode *node) {
    if (!node->children.empty()) {
        log(Warning, "expansion a node twice");
        return;
    }
    State &s = node->state;
    for(int i = 0; i < BOARD_SIZE; ++i) {
        for(int j = 0; j < BOARD_SIZE; ++j) {
            if(s.board[i][j] != None) continue;
            State new_state = s;
            new_state.board[i][j] = node->player;
            Player nextPlayer = Gobang::next_player(node->player);
            MCTNode* child = new MCTNode(new_state, nextPlayer, node);
            node->children.push_back(child);
            node->actions.push_back(std::make_pair(i, j));
        }
    }
}

Player MCTS::simulation(State state, Player player) {
    Player winner = Gobang::get_winner(state);
    while(winner == None) {
        std::vector<Action> moves;
        for(int i = 0; i < BOARD_SIZE; ++i) {
            for(int j = 0; j < BOARD_SIZE; ++j) {
                if(state.board[i][j] != None) continue;
                moves.push_back(std::make_pair(i, j));
            }
        }
        if(moves.empty()) return None;
        Action move = moves[random(moves.size())];
        state.board[move.first][move.second] = player;
        player = Gobang::next_player(player);
        winner = Gobang::get_winner(state, move.first, move.second);
    }
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
    MCTNode *leaf = this->selection();
    this->expansion(leaf);
    if(!leaf->children.empty()) {
        int index = random(leaf->children.size());
        leaf = leaf->children[index];
    }
    Player winner = this->simulation(leaf->state, leaf->player);
    backup(leaf, winner);
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
    now_player = Gobang::next_player(now_player);
    return ret;
}

void MCTS::print_winning_rate() {
    double p[BOARD_SIZE][BOARD_SIZE];
    for(int i = 0; i < BOARD_SIZE; ++i) {
        for(int j = 0; j < BOARD_SIZE; ++j) {
            p[i][j] = 0;
        }
    }
    for(int i = 0; i < now_state->children.size(); ++i) {
        p[now_state->actions[i].first][now_state->actions[i].second] = -1;
        if(now_state->children[i]->visits == 0) continue;
        p[now_state->actions[i].first][now_state->actions[i].second] = now_state->children[i]->wins / now_state->children[i]->visits;
    }
    for(int i = 0; i < BOARD_SIZE; ++i) {
        for(int j = 0; j < BOARD_SIZE; ++j) {
            printf("%6.1lf ", p[i][j] * 100);
        }
        putchar('\n');
    }
}

void MCTS::serialize() {
    tree->serialize("save.json");
}