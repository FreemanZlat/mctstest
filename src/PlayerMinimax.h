#pragma once

#include "Player.h"

#include <vector>

class PlayerMinimax : public Player
{
 public:
    PlayerMinimax(int depth);
    virtual ~PlayerMinimax();

    int move(Game *game, bool print_info);

 private:
    int depth;

    static int search(int depth, int ply, Game *game, int &nodes);
};
