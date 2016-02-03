#pragma once

#include "Player.h"

#include <vector>

class PlayerAB : public Player
{
 public:
    PlayerAB(int depth);
    virtual ~PlayerAB();

    int move(Game *game, bool print_info);

 private:
    int depth;

    static int search(int depth, int ply, int alpha, int beta, Game *game, int &nodes);
};
