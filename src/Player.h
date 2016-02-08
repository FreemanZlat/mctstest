#pragma once

#include <cinttypes>

class Game;
class Random;

class Player
{
 public:
    virtual ~Player()
    {
    }
    ;
    virtual uint32_t move(Game *game, Random *rnd, bool print_info = false) = 0;
};
