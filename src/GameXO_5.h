#pragma once

#include "Game.h"

#include <bitset>

#define GAME_XO_5_SIZE      15
#define GAME_XO_5_SIZE_2    GAME_XO_5_SIZE*GAME_XO_5_SIZE
#define GAME_XO_5_MOVES_WINDOW 2

class GameXO_5 : public Game
{
 public:
    GameXO_5();
    virtual ~GameXO_5();

    Game* clone();
    std::vector<uint32_t> moves_get(bool sorted);
    void move_do(const uint32_t move);
    void move_undo(const uint32_t move);
    bool move_random(Random *rnd);
    bool is_win();
    bool is_single_move();
    bool get_player();
    int32_t eval(uint8_t type);
    void print();

 private:
    bool is_first_player_move;
    std::bitset<GAME_XO_5_SIZE_2> board[2];
    std::vector<uint32_t> history_moves;
    std::vector<std::bitset<GAME_XO_5_SIZE_2>> history_masks;
    static std::vector<std::bitset<GAME_XO_5_SIZE_2>> check_lines;
    static std::vector<std::vector<uint32_t>> win_check;
    static std::vector<std::bitset<GAME_XO_5_SIZE_2>> new_moves;
    static struct _init
    {
        _init();
    } _initializer;
};
