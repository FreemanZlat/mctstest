#pragma once

#include "Player.h"

class PlayerMCTS : public Player
{
 public:
    PlayerMCTS(int iterations);
    virtual ~PlayerMCTS();

    int move(Game *game);
};
