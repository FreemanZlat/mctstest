#include "GameXO.h"

#include <cstdio>

std::vector<unsigned char> GameXO::win_check =
        { 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 3, 6, 1, 4, 7, 2, 5, 8, 0, 4, 8, 2, 4, 6 };

GameXO::GameXO() :
        is_first_player_move(true)
{
    this->board.resize(9, 0);
}

GameXO::~GameXO()
{
}

Game* GameXO::clone()
{
    GameXO *game = new GameXO();
    game->is_first_player_move = this->is_first_player_move;
    game->board = this->board;
    return game;
}

std::vector<int> GameXO::moves_get()
{
    std::vector<int> moves;
    for (int i = 0; i < 9; ++i)
        if (this->board[i] == 0)
            moves.push_back(i);
    return moves;
}

void GameXO::move_do(const int move)
{
    if (move < 0 || move > 8 || this->board[move] != 0)
    {
    }

    this->board[move] = this->is_first_player_move ? 1 : 2;
    this->is_first_player_move = !is_first_player_move;
}

bool GameXO::is_win(const bool first_player)
{
    int player = first_player ? 1 : 2;
    for (int i = 0; i < 8; ++i)
        if (this->board[win_check[i * 3]] == player && this->board[win_check[i * 3 + 1]] == player
                && this->board[win_check[i * 3 + 2]] == player)
            return true;
    return false;
}

bool GameXO::get_player()
{
    return this->is_first_player_move;
}

int GameXO::eval()
{
    return 0;
}

void GameXO::print()
{
    for (int i = 0; i < 9; ++i)
    {
        printf("%c", ".X0"[this->board[i]]);
        if ((i + 1) % 3 == 0)
            printf("\n");
    }
}
