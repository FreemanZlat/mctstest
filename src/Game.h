#pragma once

#include <cinttypes>
#include <vector>

class Random;

class Game
{
 public:
    virtual ~Game() {};
    virtual Game* clone() = 0;
    virtual std::vector<uint32_t> moves_get(bool sorted = false) = 0;
    virtual void move_do(const uint32_t move) = 0;
    virtual void move_undo(const uint32_t move) = 0;
    virtual bool move_random(Random *rnd) = 0;
    virtual bool is_win() = 0;
    virtual bool is_single_move() = 0;
    virtual bool get_player() = 0;
    virtual int32_t eval(uint8_t type = 0) = 0;
    virtual void print() = 0;
};
