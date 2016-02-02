#include "GameXO_4x4x4.h"

#include <cstdio>

std::vector<std::vector<unsigned char>> GameXO_4x4x4::check_lines;
std::vector<std::vector<int>> GameXO_4x4x4::win_check;
GameXO_4x4x4::_init GameXO_4x4x4::_initializer;

GameXO_4x4x4::_init::_init()
{
    win_check.resize(64);

    for (int x = 0; x < 4; ++x)
        for (int y = 0; y < 4; ++y)
        {
            std::vector<unsigned char> line1, line2, line3;
            for (int z = 0; z < 4; ++z)
            {
                line1.push_back(x * 16 + y * 4 + z);
                line2.push_back(x * 16 + z * 4 + y);
                line3.push_back(z * 16 + x * 4 + y);
                win_check[x * 16 + y * 4 + z].push_back(check_lines.size());
                win_check[x * 16 + z * 4 + y].push_back(check_lines.size() + 1);
                win_check[z * 16 + x * 4 + y].push_back(check_lines.size() + 2);
            }
            check_lines.push_back(line1);
            check_lines.push_back(line2);
            check_lines.push_back(line3);
        }

    for (int x = 0; x < 4; ++x)
    {
        std::vector<unsigned char> diag1a, diag1b, diag2a, diag2b, diag3a, diag3b;
        for (int y = 0; y < 4; ++y)
        {
            diag1a.push_back(x * 16 + y * 4 + y);
            diag1b.push_back(x * 16 + y * 4 + (3 - y));
            diag2a.push_back(y * 16 + x * 4 + y);
            diag2b.push_back(y * 16 + x * 4 + (3 - y));
            diag3a.push_back(y * 16 + y * 4 + x);
            diag3b.push_back(y * 16 + (3 - y) * 4 + x);
            win_check[x * 16 + y * 4 + y].push_back(check_lines.size());
            win_check[x * 16 + y * 4 + (3 - y)].push_back(check_lines.size() + 1);
            win_check[y * 16 + x * 4 + y].push_back(check_lines.size() + 2);
            win_check[y * 16 + x * 4 + (3 - y)].push_back(check_lines.size() + 3);
            win_check[y * 16 + y * 4 + x].push_back(check_lines.size() + 4);
            win_check[y * 16 + (3 - y) * 4 + x].push_back(check_lines.size() + 5);
        }
        check_lines.push_back(diag1a);
        check_lines.push_back(diag1b);
        check_lines.push_back(diag2a);
        check_lines.push_back(diag2b);
        check_lines.push_back(diag3a);
        check_lines.push_back(diag3b);
    }

    std::vector<unsigned char> diag1, diag2, diag3, diag4;
    for (int x = 0; x < 4; ++x)
    {
        diag1.push_back(x * 16 + x * 4 + x);
        diag2.push_back(x * 16 + x * 4 + (3 - x));
        diag3.push_back(x * 16 + (3 - x) * 4 + x);
        diag4.push_back(x * 16 + (3 - x) * 4 + (3 - x));
        win_check[x * 16 + x * 4 + x].push_back(check_lines.size());
        win_check[x * 16 + x * 4 + (3 - x)].push_back(check_lines.size() + 1);
        win_check[x * 16 + (3 - x) * 4 + x].push_back(check_lines.size() + 2);
        win_check[x * 16 + (3 - x) * 4 + (3 - x)].push_back(check_lines.size() + 3);
    }
    check_lines.push_back(diag1);
    check_lines.push_back(diag2);
    check_lines.push_back(diag3);
    check_lines.push_back(diag4);
}

GameXO_4x4x4::GameXO_4x4x4() :
        is_first_player_move(true)
{
    this->board.resize(64, 0);
}

GameXO_4x4x4::~GameXO_4x4x4()
{
}

Game* GameXO_4x4x4::clone()
{
    GameXO_4x4x4 *game = new GameXO_4x4x4();
    game->is_first_player_move = this->is_first_player_move;
    game->board = this->board;
    game->history = this->history;
    return game;
}

std::vector<int> GameXO_4x4x4::moves_get(bool sorted)
{
    std::vector<int> moves;
    for (int i = 0; i < 64; ++i)
        if (this->board[i] == 0)
            moves.push_back(i);
    return moves;
}

void GameXO_4x4x4::move_do(const int move)
{
    if (move < 0 || move > 63 || this->board[move] != 0)
    {
    }

    this->board[move] = this->is_first_player_move ? 1 : 2;
    this->history.push_back(move);
    this->is_first_player_move = !is_first_player_move;
}

void GameXO_4x4x4::move_undo(const int move)
{
    if (move < 0 || move > 63 || this->board[move] == 0 || this->history.size() == 0 || this->history.back() != move)
    {
    }

    this->board[move] = 0;
    this->history.pop_back();
    this->is_first_player_move = !is_first_player_move;
}

bool GameXO_4x4x4::is_win()
{
    if (this->history.size() == 0)
        return false;

    int player = !this->is_first_player_move ? 1 : 2;
    int move = this->history.back();

    for (int idx : win_check[move])
    {
        bool win = true;
        for (auto pos : check_lines[idx])
            if (this->board[pos] != player)
            {
                win = false;
                break;
            }
        if (win)
            return true;
    }

    return false;
}

bool GameXO_4x4x4::get_player()
{
    return this->is_first_player_move;
}

int GameXO_4x4x4::eval()
{
    return 0;
}

void GameXO_4x4x4::print()
{
    for (int y = 0; y < 4; ++y)
    {
        for (int x = 0; x < 4; ++x)
        {
            for (int z = 0; z < 4; ++z)
                printf("%c", ".X0"[this->board[x * 16 + y * 4 + z]]);
            printf(" ");
        }
        printf("\n");
    }
}
