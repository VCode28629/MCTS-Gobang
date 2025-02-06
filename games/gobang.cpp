#include "gobang.h"

Gobang::Gobang(int board_size, int win_line) {
    for(int i = 0; i < GOBANG_BOARD_SIZE; ++i) {
        for(int j = 0; j < GOBANG_BOARD_SIZE; ++j) {
            state.board[i][j] = None;
        }
    }
    player = Black;
}

int Gobang::get_step() {
    return actions.size() + 1;
}

std::vector<Action> Gobang::get_legal_moves() {
    std::vector<Action> ret;
    for(int i = 0; i < GOBANG_BOARD_SIZE; ++i) {
        for(int j = 0; j < GOBANG_BOARD_SIZE; ++j) {
            if(state.board[i][j] != None) continue;
            ret.push_back(std::make_pair(i, j));
        }
    }
    return ret;
}

void Gobang::move(Action action) {
    state.board[action.first][action.second] = player;
    actions.push_back(action);
    player = next_player(player);
    Player winner = get_winner(action.first, action.second);
    if(winner == None) return;
    over = true;
    this->winner = winner;
}

void Gobang::undo() {
    state.board[actions.back().first][actions.back().second] = None;
    actions.pop_back();
}

Player Gobang::get_winner(int x, int y) {
    int l = 0;
    int r = 0;
    while(x - l - 1 >= 0 && state.board[x][y] == state.board[x - l - 1][y]) ++l;
    while(x + r + 1 < GOBANG_BOARD_SIZE && state.board[x][y] == state.board[x + r + 1][y]) ++r;
    if(l + 1 + r >= WIN_LINE) return state.board[x][y];
    l = 0;
    r = 0;
    while(y - l - 1 >= 0 && state.board[x][y] == state.board[x][y - l - 1]) ++l;
    while(y + r + 1 < GOBANG_BOARD_SIZE && state.board[x][y] == state.board[x][y + r + 1]) ++r;
    if(l + 1 + r >= WIN_LINE) return state.board[x][y];
    l = 0;
    r = 0;
    while(x - l - 1 >= 0 && y - l - 1 >= 0 && state.board[x][y] == state.board[x - l - 1][y - l - 1]) ++l;
    while(x + r + 1 < GOBANG_BOARD_SIZE && y + r + 1 < GOBANG_BOARD_SIZE && state.board[x][y] == state.board[x + r + 1][y + r + 1]) ++r;
    if(l + 1 + r >= WIN_LINE) return state.board[x][y];
    l = 0;
    r = 0;
    while(x - l - 1 >= 0 && y + l + 1 < GOBANG_BOARD_SIZE && state.board[x][y] == state.board[x - l - 1][y + l + 1]) ++l;
    while(x + r + 1 < GOBANG_BOARD_SIZE && y - r - 1 >= 0 && state.board[x][y] == state.board[x + r + 1][y - r - 1]) ++r;
    if(l + 1 + r >= WIN_LINE) return state.board[x][y];
    return None;
}
