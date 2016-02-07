#pragma once

#include "Game.h"

class GameXO_4x4x4 : public Game
{
 public:
    GameXO_4x4x4();
    virtual ~GameXO_4x4x4();

    Game* clone();
    std::vector<uint32_t> moves_get(bool sorted);
    void move_do(const uint32_t move);
    void move_undo(const uint32_t move);
    bool is_win();
    bool is_single_move();
    bool get_player();
    int32_t eval();
    void print();

 private:
    bool is_first_player_move;
    uint64_t _board[2];
    std::vector<uint8_t> history;
    static std::vector<uint64_t> check_lines;
    static std::vector<std::vector<uint8_t>> win_check;
    static std::vector<int8_t> eval_pst;
    static struct _init
    {
        _init();
    } _initializer;
};
