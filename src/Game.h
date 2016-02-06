#pragma once

#include "cinttypes"
#include <vector>

class Game
{
 public:
    virtual ~Game() {};
    virtual Game* clone() = 0;
    virtual std::vector<uint32_t> moves_get(bool sorted = false) = 0;
    virtual void move_do(const uint32_t move) = 0;
    virtual void move_undo(const uint32_t move) = 0;
    virtual bool is_win() = 0;
    virtual bool get_player() = 0;
    virtual int32_t eval() = 0;
    virtual void print() = 0;
};
