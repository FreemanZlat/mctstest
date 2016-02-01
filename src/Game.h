#pragma once

#include <vector>

class Game
{
 public:
    virtual ~Game() {};
    virtual Game* clone() = 0;
    virtual std::vector<int> moves_get() = 0;
    virtual void move_do(const int move) = 0;
    virtual bool is_win(const bool first_player) = 0;
    virtual bool get_player() = 0;
    virtual int eval() = 0;
};
