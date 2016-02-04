#pragma once

#include "Player.h"
#include "Utils.h"

#include <vector>

class PlayerAB : public Player
{
 public:
    PlayerAB(int move_duration_ms, int max_depth);
    virtual ~PlayerAB();

    int move(Game *game, bool print_info);

 private:
    int move_duration_ms;
    int max_depth;

    static int search(int depth, int ply, int alpha, int beta, Game *game, int &nodes, bool &aborted,
                      Utils::Timer &timer, const int move_duration_ms);
};
