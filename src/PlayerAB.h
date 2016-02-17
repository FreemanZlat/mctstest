#pragma once

#include "Player.h"

#include <vector>

class Timer;

class PlayerAB : public Player
{
 public:
    PlayerAB(uint32_t move_duration_ms, bool single_extension = false, uint8_t eval_type = 0, uint32_t max_depth = 64);
    virtual ~PlayerAB();

    uint32_t move(Game *game, Random *rnd, bool print_info);

 private:
    struct Params
    {
        uint32_t move_duration_ms;
        uint8_t eval_type;
        bool single_extension;
        uint32_t max_depth;
        bool aborted;
        uint32_t nodes;
        Timer *timer;
    };
    Params params;

    static int32_t search(uint32_t depth, uint32_t ply, int32_t alpha, int32_t beta, Game *game, Params &params);
};
