#include "game.h"

Player Gobang::get_winner(GameState state) {
    for(int i = 0; i <= BOARD_SIZE - WIN_LINE; ++i) {
        for(int j = 0; j <= BOARD_SIZE - WIN_LINE; ++j) {
            if(state.board[i][j] == None) continue;
            int count = 1;
            for(int t = 1; t < WIN_LINE; ++t) {
                if(state.board[i][j] != state.board[i][j + t]) break;
                ++count;
            }
            if(count == WIN_LINE) return state.board[i][j];
            count = 1;
            for(int t = 1; t < WIN_LINE; ++t) {
                if(state.board[i][j] != state.board[i + t][j]) break;
                ++count;
            }
            if(count == WIN_LINE) return state.board[i][j];
            count = 1;
            for(int t = 1; t < WIN_LINE; ++t) {
                if(state.board[i][j] != state.board[i + t][j + t]) break;
                ++count;
            }
            if(count == WIN_LINE) return state.board[i][j];
        }
    }
    for(int i = 0; i <= BOARD_SIZE - WIN_LINE; ++i) {
        for(int j = WIN_LINE - 1; j < BOARD_SIZE; ++j) {
            if(state.board[i][j] == None) continue;
            int count = 1;
            for(int t = 1; t < WIN_LINE; ++t) {
                if(state.board[i][j] != state.board[i + t][j - t]) break;
                ++count;
            }
            if(count == WIN_LINE) return state.board[i][j];
        }
    }
    return None;
}

Player Gobang::get_winner(GameState state, int x, int y) {
    int l = 0;
    int r = 0;
    while(x - l - 1 >= 0 && state.board[x][y] == state.board[x - l - 1][y]) ++l;
    while(x + r + 1 < BOARD_SIZE && state.board[x][y] == state.board[x + r + 1][y]) ++r;
    if(l + 1 + r >= WIN_LINE) return state.board[x][y];
    l = 0;
    r = 0;
    while(y - l - 1 >= 0 && state.board[x][y] == state.board[x][y - l - 1]) ++l;
    while(y + r + 1 < BOARD_SIZE && state.board[x][y] == state.board[x][y + r + 1]) ++r;
    if(l + 1 + r >= WIN_LINE) return state.board[x][y];
    l = 0;
    r = 0;
    while(x - l - 1 >= 0 && y - l - 1 >= 0 && state.board[x][y] == state.board[x - l - 1][y - l - 1]) ++l;
    while(x + r + 1 < BOARD_SIZE && y + r + 1 < BOARD_SIZE && state.board[x][y] == state.board[x + r + 1][y + r + 1]) ++r;
    if(l + 1 + r >= WIN_LINE) return state.board[x][y];
    l = 0;
    r = 0;
    while(x - l - 1 >= 0 && y + l + 1 < BOARD_SIZE && state.board[x][y] == state.board[x - l - 1][y + l + 1]) ++l;
    while(x + r + 1 < BOARD_SIZE && y - r - 1 >= 0 && state.board[x][y] == state.board[x + r + 1][y - r - 1]) ++r;
    if(l + 1 + r >= WIN_LINE) return state.board[x][y];
    return None;
}
