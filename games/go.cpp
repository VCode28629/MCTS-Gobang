#include "go.h"

#include <cstring>
#include <queue>
#include <unordered_set>

#include "utils.h"

const int Go::step[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

Go::Go() {
    for (int i = 0; i < GO_BOARD_SIZE; ++i) {
        for (int j = 0; j < GO_BOARD_SIZE; ++j) {
            state.board[i][j] = None;
        }
    }
    player = Black;
    history.push_back(state);
    pass = false;
    over = false;
    winner = None;
    info_history.push(std::make_tuple(player, pass, over, winner));
}

int Go::count(const GoState &state, int sx, int sy, bool *colored) {
    int black = 0;
    int white = 0;
    int count = 0;
    std::queue<std::pair<int, int> > queue;
    std::unordered_set<std::pair<int, int>, pair_hash> block;
    queue.push({sx, sy});
    block.insert({sx, sy});
    while (!queue.empty()) {
        auto top = queue.front();
        queue.pop();
        int x = top.first;
        int y = top.second;
        colored[x * GO_BOARD_SIZE + y] = true;
        ++count;
        for (int i = 0; i < 4; ++i) {
            int nx = x + step[i][0];
            int ny = y + step[i][1];
            if (nx < 0 || ny < 0 || nx >= GO_BOARD_SIZE || ny >= GO_BOARD_SIZE)
                continue;
            if (state.board[nx][ny] == Black) ++black;
            if (state.board[nx][ny] == White) ++white;
            if (state.board[nx][ny] != state.board[sx][sy]) continue;
            if (block.count({nx, ny})) continue;
            queue.push({nx, ny});
            block.insert({nx, ny});
        }
    }
    if (state.board[sx][sy] == Black) {
        return count;
    } else if (state.board[sx][sy] == White) {
        return -count;
    }
    if (black > 0 && white == 0) return count;
    if (black == 0 && white > 0) return -count;
    return 0;
}

Player Go::get_winner(const GoState &state) {
    bool color[GO_BOARD_SIZE * GO_BOARD_SIZE];
    memset(color, 0, sizeof(color));
    float black_win = -KOMI;
    for (int i = 0; i < GO_BOARD_SIZE * GO_BOARD_SIZE; ++i) {
        if (color[i]) continue;
        black_win += count(state, i / GO_BOARD_SIZE, i % GO_BOARD_SIZE, color);
    }
    if (black_win > 0) return Black;
    if (black_win < 0) return White;
    return None;
}

bool Go::can_move(Action action) {
    if (over) return false;
    if (action.first < 0 || action.second < 0 ||
        action.first >= GO_BOARD_SIZE || action.second >= GO_BOARD_SIZE)
        return true;  // pass
    if (this->state.board[action.first][action.second] != None) return false;
    GoState state = next_state(this->state, player, action);
    if (state.board[action.first][action.second] != player) return false;
    for (auto &s : history) {
        if (s == state) return false;
    }
    return true;
}

void Go::try_eat(GoState &state, int sx, int sy) {
    if (sx < 0 || sy < 0 || sx >= GO_BOARD_SIZE || sy >= GO_BOARD_SIZE) return;
    if (state.board[sx][sy] == None) return;
    std::queue<std::pair<int, int> > queue;
    std::unordered_set<std::pair<int, int>, pair_hash> block;
    queue.push({sx, sy});
    block.insert({sx, sy});
    while (!queue.empty()) {
        auto top = queue.front();
        queue.pop();
        int x = top.first;
        int y = top.second;
        for (int i = 0; i < 4; ++i) {
            int nx = x + step[i][0];
            int ny = y + step[i][1];
            if (nx < 0 || ny < 0 || nx >= GO_BOARD_SIZE || ny >= GO_BOARD_SIZE)
                continue;
            if (state.board[nx][ny] == None) {
                return;
            }
            if (state.board[nx][ny] != state.board[sx][sy]) continue;
            if (block.count({nx, ny})) continue;
            queue.push({nx, ny});
            block.insert({nx, ny});
        }
    }
    for (auto &loc : block) {
        state.board[loc.first][loc.second] = None;
    }
}

GoState Go::next_state(GoState state, Player player, Action action) {
    state.board[action.first][action.second] = player;
    try_eat(state, action.first - 1, action.second);
    try_eat(state, action.first + 1, action.second);
    try_eat(state, action.first, action.second - 1);
    try_eat(state, action.first, action.second + 1);
    try_eat(state, action.first, action.second);
    return state;
}

void Go::move(Action action) {
    if (action.first < 0 || action.second < 0 ||
        action.first >= GO_BOARD_SIZE || action.second >= GO_BOARD_SIZE) {
        if (pass) {
            over = true;
            winner = get_winner(state);
        }
        pass = true;
    } else {
        pass = false;
        state = next_state(state, player, action);
    }
    player = next_player(player);
    history.push_back(state);
    info_history.push(std::make_tuple(player, pass, over, winner));
}

void Go::undo() {
    history.pop_back();
    state = history.back();
    info_history.pop();
    std::tie(player, pass, over, winner) = info_history.top();
}

int Go::get_step() { return history.size(); }

std::vector<Action> Go::get_legal_moves() {
    std::vector<Action> moves;
    for (int i = 0; i < GO_BOARD_SIZE; ++i) {
        for (int j = 0; j < GO_BOARD_SIZE; ++j) {
            if (!can_move(std::make_pair(i, j))) continue;
            moves.push_back(std::make_pair(i, j));
        }
    }
    if (can_move(std::make_pair(-1, -1))) moves.push_back({-1, -1});
    return moves;
}