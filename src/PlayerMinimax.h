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
    struct Params
    {
        uint32_t move_duration_ms;
        uint32_t max_depth;
        bool aborted;
        uint32_t nodes;
        Timer *timer;
    };
    Params params;

    static int32_t search(uint32_t depth, uint32_t ply, Game *game, Params &params);
};
