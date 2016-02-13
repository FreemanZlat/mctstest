#pragma once

#include "Game.h"

class GameXO : public Game
{
 public:
    GameXO();
    virtual ~GameXO();

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
    std::vector<uint8_t> board;
    static std::vector<uint8_t> win_check;
    static std::vector<int8_t> eval_pst;
};
