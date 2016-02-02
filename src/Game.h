#pragma once

#include <vector>

class Game
{
 public:
    virtual ~Game() {};
    virtual Game* clone() = 0;
    virtual std::vector<int> moves_get(bool sorted = false) = 0;
    virtual void move_do(const int move) = 0;
    virtual void move_undo(const int move) = 0;
    virtual bool is_win() = 0;
    virtual bool get_player() = 0;
    virtual int eval() = 0;
    virtual void print() = 0;
};
