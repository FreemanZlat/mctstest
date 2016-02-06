#pragma once

#include <cinttypes>

class Game;

class Player
{
 public:
    virtual ~Player() {};
    virtual uint32_t move(Game *game, bool print_info = false) = 0;
};
