#include "GameXO_5.h"

#include "Utils.h"

#include <cstdio>

std::vector<std::bitset<GAME_XO_5_SIZE_2>> GameXO_5::check_lines;
std::vector<std::vector<uint32_t>> GameXO_5::win_check;
std::vector<std::bitset<GAME_XO_5_SIZE_2>> GameXO_5::new_moves;

GameXO_5::_init GameXO_5::_initializer;

GameXO_5::_init::_init()
{
    win_check.resize(GAME_XO_5_SIZE_2);
    new_moves.resize(GAME_XO_5_SIZE_2);

    for (int16_t x = 0; x < GAME_XO_5_SIZE; ++x)
        for (int16_t y = 0; y < GAME_XO_5_SIZE; ++y)
        {
            if (x <= GAME_XO_5_SIZE - 5)
            {
                std::bitset<GAME_XO_5_SIZE_2> buf;
                for (int16_t i = 0; i < 5; ++i)
                {
                    buf.set((x + i) * GAME_XO_5_SIZE + y, true);
                    win_check[(x + i) * GAME_XO_5_SIZE + y].push_back(check_lines.size());
                }
                check_lines.push_back(buf);
            }
            if (y <= GAME_XO_5_SIZE - 5)
            {
                std::bitset<GAME_XO_5_SIZE_2> buf;
                for (int16_t i = 0; i < 5; ++i)
                {
                    buf.set(x * GAME_XO_5_SIZE + y + i, true);
                    win_check[x * GAME_XO_5_SIZE + y + i].push_back(check_lines.size());
                }
                check_lines.push_back(buf);
            }
            if (x <= GAME_XO_5_SIZE - 5 && y <= GAME_XO_5_SIZE - 5)
            {
                std::bitset<GAME_XO_5_SIZE_2> buf1, buf2;
                for (int16_t i = 0; i < 5; ++i)
                {
                    buf1.set((x + i) * GAME_XO_5_SIZE + y + i, true);
                    buf2.set((x + i) * GAME_XO_5_SIZE + y - i + 4, true);
                    win_check[(x + i) * GAME_XO_5_SIZE + y + i].push_back(check_lines.size());
                    win_check[(x + i) * GAME_XO_5_SIZE + y - i + 4].push_back(check_lines.size() + 1);
                }
                check_lines.push_back(buf1);
                check_lines.push_back(buf2);
            }

            for (int16_t x1 = -GAME_XO_5_MOVES_WINDOW; x1 <= GAME_XO_5_MOVES_WINDOW; ++x1)
            {
                if (x + x1 < 0 || x + x1 >= GAME_XO_5_SIZE)
                    continue;
                for (int16_t y1 = -GAME_XO_5_MOVES_WINDOW; y1 <= GAME_XO_5_MOVES_WINDOW; ++y1)
                {
                    if (y + y1 < 0 || y + y1 >= GAME_XO_5_SIZE)
                        continue;
                    new_moves[x * GAME_XO_5_SIZE + y].set((x + x1) * GAME_XO_5_SIZE + y + y1, true);
                }
            }
        }
}

GameXO_5::GameXO_5() :
        is_first_player_move(true)
{
    this->board[0].reset();
    this->board[1].reset();
    this->history_moves.reserve(GAME_XO_5_SIZE_2);
    this->history_masks.reserve(GAME_XO_5_SIZE_2 + 1);
    this->history_masks.push_back(new_moves[GAME_XO_5_SIZE_2 / 2]);
}

GameXO_5::~GameXO_5()
{
}

Game* GameXO_5::clone()
{
    GameXO_5 *game = new GameXO_5();
    game->is_first_player_move = this->is_first_player_move;
    game->board[0] = this->board[0];
    game->board[1] = this->board[1];
    game->history_moves = this->history_moves;
    game->history_masks = this->history_masks;
    return game;
}

std::vector<uint32_t> GameXO_5::moves_get(bool sorted)
{
    auto board_mask = this->history_masks.back() & ~(this->board[0] | this->board[1]);

    std::vector<uint32_t> moves;
    moves.reserve(board_mask.count());

    for (uint32_t i = 0; i < GAME_XO_5_SIZE_2; ++i)
        if (board_mask[i])
            moves.push_back(i);

    return moves;
}

void GameXO_5::move_do(const uint32_t move)
{
    if (move < 0 || move >= GAME_XO_5_SIZE_2 || this->board[0][move] || this->board[1][move])
    {
    }

    this->board[this->is_first_player_move ? 0 : 1].set(move, true);
    this->history_moves.push_back(move);
    this->history_masks.push_back(this->history_masks.back() | new_moves[move]);
    this->is_first_player_move = !this->is_first_player_move;
}

void GameXO_5::move_undo(const uint32_t move)
{
    if (move < 0 || move >= GAME_XO_5_SIZE_2 || (!this->board[0][move] && !this->board[1][move])
            || this->history_moves.size() == 0 || this->history_moves.back() != move)
    {
    }

    this->board[this->is_first_player_move ? 1 : 0].set(move, false);
    this->history_moves.pop_back();
    this->history_masks.pop_back();
    this->is_first_player_move = !this->is_first_player_move;
}

bool GameXO_5::move_random(Random *rnd)
{
    auto board_mask = this->history_masks.back() & ~(this->board[0] | this->board[1]);
    uint8_t size = board_mask.count();
    if (size == 0)
        return false;

    uint8_t move_num = rnd->get() % size;
    uint32_t move = 0;

    for (uint32_t i = 0; i < GAME_XO_5_SIZE_2; ++i)
        if (board_mask[i])
            if (move_num-- == 0)
            {
                move = i;
                break;
            }

    this->move_do(move);
    return true;
}

bool GameXO_5::is_win()
{
    if (this->history_moves.size() == 0)
        return false;

    uint32_t move = this->history_moves.back();

    for (uint32_t idx : win_check[move])
        if ((this->board[this->is_first_player_move ? 1 : 0] & check_lines[idx]) == check_lines[idx])
            return true;

    return false;
}

bool GameXO_5::is_single_move()
{
//    if (this->history.size() == 0)
//        return false;
//
//    uint8_t player = this->is_first_player_move ? 0 : 1;
//    uint32_t move = this->history.back();
//
//    for (uint8_t idx : win_check[move])
//    {
//        bool single = ((this->board[player] & check_lines[idx]) | (uint64_t) 1 << move) == check_lines[idx];
//        if (single)
//            return true;
//    }

    return false;
}

bool GameXO_5::get_player()
{
    return this->is_first_player_move;
}

int32_t GameXO_5::eval(uint8_t type)
{
    int32_t resX = 0, resO = 0;
    for (const auto &mask : check_lines)
    {
        auto maskX = this->board[0] & mask;
        auto maskO = this->board[1] & mask;
        uint8_t bitsX = maskX.count();
        uint8_t bitsO = maskO.count();
        if ((bitsX != 0 && bitsO != 0) || (bitsX == 0 && bitsO == 0))
            continue;

        resX += bitsX * bitsX;
        resO += bitsO * bitsO;
    }
    int32_t res = resX - resO;
    return this->is_first_player_move ? res : -res;
}

void GameXO_5::print()
{
    for (uint32_t i = 0; i < GAME_XO_5_SIZE_2; ++i)
    {
        if (this->board[0][i])
            printf("X");
        else if (this->board[1][i])
            printf("0");
        else
            printf(".");
        if ((i + 1) % GAME_XO_5_SIZE == 0)
            printf("\n");
    }
}
