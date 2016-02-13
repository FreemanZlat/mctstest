#include "GameXO_4x4x4.h"

#include <cstdio>

std::vector<uint64_t> GameXO_4x4x4::check_lines;
std::vector<std::vector<uint8_t>> GameXO_4x4x4::win_check;
std::vector<int8_t> GameXO_4x4x4::eval_pst = { 12, 11, 11, 12, 11, 10, 10, 11, 11, 10, 10, 11, 12, 11, 11, 12, 11, 10,
        10, 11, 10, 12, 12, 10, 10, 12, 12, 10, 11, 10, 10, 11, 11, 10, 10, 11, 10, 12, 12, 10, 10, 12, 12, 10, 11, 10,
        10, 11, 12, 11, 11, 12, 11, 10, 10, 11, 11, 10, 10, 11, 12, 11, 11, 12 };
GameXO_4x4x4::_init GameXO_4x4x4::_initializer;

GameXO_4x4x4::_init::_init()
{
    win_check.resize(64);

    for (uint8_t x = 0; x < 4; ++x)
        for (uint8_t y = 0; y < 4; ++y)
        {
            uint64_t line1 = 0, line2 = 0, line3 = 0;
            for (uint8_t z = 0; z < 4; ++z)
            {
                line1 |= (uint64_t) 1 << (x * 16 + y * 4 + z);
                line2 |= (uint64_t) 1 << (x * 16 + z * 4 + y);
                line3 |= (uint64_t) 1 << (z * 16 + x * 4 + y);
                win_check[x * 16 + y * 4 + z].push_back(check_lines.size());
                win_check[x * 16 + z * 4 + y].push_back(check_lines.size() + 1);
                win_check[z * 16 + x * 4 + y].push_back(check_lines.size() + 2);
            }
            check_lines.push_back(line1);
            check_lines.push_back(line2);
            check_lines.push_back(line3);
        }

    for (uint8_t x = 0; x < 4; ++x)
    {
        uint64_t diag1a = 0, diag1b = 0, diag2a = 0, diag2b = 0, diag3a = 0, diag3b = 0;
        for (uint8_t y = 0; y < 4; ++y)
        {
            diag1a |= (uint64_t) 1 << (x * 16 + y * 4 + y);
            diag1b |= (uint64_t) 1 << (x * 16 + y * 4 + (3 - y));
            diag2a |= (uint64_t) 1 << (y * 16 + x * 4 + y);
            diag2b |= (uint64_t) 1 << (y * 16 + x * 4 + (3 - y));
            diag3a |= (uint64_t) 1 << (y * 16 + y * 4 + x);
            diag3b |= (uint64_t) 1 << (y * 16 + (3 - y) * 4 + x);
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

    uint64_t diag1 = 0, diag2 = 0, diag3 = 0, diag4 = 0;
    for (uint8_t x = 0; x < 4; ++x)
    {
        diag1 |= (uint64_t) 1 << (x * 16 + x * 4 + x);
        diag2 |= (uint64_t) 1 << (x * 16 + x * 4 + (3 - x));
        diag3 |= (uint64_t) 1 << (x * 16 + (3 - x) * 4 + x);
        diag4 |= (uint64_t) 1 << (x * 16 + (3 - x) * 4 + (3 - x));
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
    this->board[0] = 0;
    this->board[1] = 0;
    this->history.reserve(64);
}

GameXO_4x4x4::~GameXO_4x4x4()
{
}

Game* GameXO_4x4x4::clone()
{
    GameXO_4x4x4 *game = new GameXO_4x4x4();
    game->is_first_player_move = this->is_first_player_move;
    game->board[0] = this->board[0];
    game->board[1] = this->board[1];
    game->history = this->history;
    return game;
}

std::vector<uint32_t> GameXO_4x4x4::moves_get(bool sorted)
{
    uint64_t board_mask = this->board[0] | this->board[1];

    std::vector<uint32_t> moves;
    moves.reserve(64);

    uint64_t mask = 1;
    for (uint8_t i = 0; i < 64; ++i)
    {
        if ((board_mask & mask) == 0)
            moves.push_back(i);
        mask <<= 1;
    }
    return moves;
}

void GameXO_4x4x4::move_do(const uint32_t move)
{
    uint64_t move_mask = (uint64_t) 1 << move;
    if (move < 0 || move > 63 || ((this->board[0] | this->board[1]) & move_mask))
    {
    }

    this->board[this->is_first_player_move ? 0 : 1] |= move_mask;
    this->history.push_back(move);
    this->is_first_player_move = !this->is_first_player_move;
}

void GameXO_4x4x4::move_undo(const uint32_t move)
{
    uint64_t move_mask = (uint64_t) 1 << move;
    if (move < 0 || move > 63 || ((this->board[0] | this->board[1]) & move_mask) == 0 || this->history.size() == 0
            || this->history.back() != move)
    {
    }

    this->board[this->is_first_player_move ? 1 : 0] &= ~move_mask;
    this->history.pop_back();
    this->is_first_player_move = !this->is_first_player_move;
}

bool GameXO_4x4x4::is_win()
{
    if (this->history.size() == 0)
        return false;

    uint8_t player = this->is_first_player_move ? 1 : 0;
    uint32_t move = this->history.back();

    for (uint8_t idx : win_check[move])
    {
        bool win = (this->board[player] & check_lines[idx]) == check_lines[idx];
        if (win)
            return true;
    }

    return false;
}

bool GameXO_4x4x4::is_single_move()
{
    if (this->history.size() == 0)
        return false;

    uint8_t player = this->is_first_player_move ? 0 : 1;
    uint32_t move = this->history.back();

    for (uint8_t idx : win_check[move])
    {
        bool single = ((this->board[player] & check_lines[idx]) | (uint64_t) 1 << move) == check_lines[idx];
        if (single)
            return true;
    }

    return false;
}

bool GameXO_4x4x4::get_player()
{
    return this->is_first_player_move;
}

int32_t GameXO_4x4x4::eval(uint8_t type)
{
    int32_t resX = 0, resO = 0;
    if (type == 0)
    {
        uint64_t mask = 1;
        for (uint8_t i = 0; i < 64; ++i)
        {
            if (this->board[0] & mask)
                resX += eval_pst[i];
            else if (this->board[1] & mask)
                resO += eval_pst[i];
            mask <<= 1;
        }
    }
    else
    {
        for (uint64_t mask : check_lines)
        {
            uint64_t maskX = this->board[0] & mask;
            uint64_t maskO = this->board[1] & mask;
            if ((maskX != 0 && maskO != 0) || (maskX == 0 && maskO == 0))
                continue;
            uint8_t bitsX = bits_count(maskX);
            uint8_t bitsO = bits_count(maskO);
            resX += bitsX * bitsX;
            resO += bitsO * bitsO;
        }
    }
    int32_t res = resX - resO;
    return this->is_first_player_move ? res : -res;
}

void GameXO_4x4x4::print()
{
    for (uint8_t y = 0; y < 4; ++y)
    {
        for (uint8_t x = 0; x < 4; ++x)
        {
            for (uint8_t z = 0; z < 4; ++z)
            {
                uint8_t pos = x * 16 + y * 4 + z;
                uint64_t mask = (uint64_t) 1 << pos;
                if (this->board[0] & mask)
                    printf("X");
                else if (this->board[1] & mask)
                    printf("0");
                else
                    printf(".");
            }
            printf(" ");
        }
        printf("\n");
    }
}

uint8_t GameXO_4x4x4::bits_count(uint64_t mask)
{
    uint8_t count = 0;
    while (mask != 0)
    {
        count++;
        mask &= mask - 1;
    }
    return count;
}
