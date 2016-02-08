#pragma once

#include "Player.h"
#include "Utils.h"

#include <vector>

class Random;
class Timer;

class PlayerMinimax : public Player
{
 public:
    PlayerMinimax(uint32_t move_duration_ms, uint32_t max_depth = 64);
    virtual ~PlayerMinimax();

    uint32_t move(Game *game, Random *rnd, bool print_info);

 private:
    uint32_t move_duration_ms;
    uint32_t max_depth;

    static int32_t search(uint32_t depth, uint32_t ply, Game *game, uint32_t &nodes, bool &aborted, Timer &timer,
                          const uint32_t move_duration_ms);
};
