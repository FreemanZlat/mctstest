#pragma once

#include "Player.h"
#include "Utils.h"

#include <vector>

class PlayerMinimax : public Player
{
 public:
    PlayerMinimax(int move_duration_ms, int max_depth);
    virtual ~PlayerMinimax();

    int move(Game *game, bool print_info);

 private:
    int move_duration_ms;
    int max_depth;

    static int search(int depth, int ply, Game *game, int &nodes, bool &aborted, Utils::Timer &timer,
                      const int move_duration_ms);
};
