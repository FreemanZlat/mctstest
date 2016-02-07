#pragma once

#include "Player.h"
#include "Utils.h"

#include <vector>

class PlayerAB : public Player
{
 public:
    PlayerAB(uint32_t move_duration_ms, bool single_extension, uint32_t max_depth = 64);
    virtual ~PlayerAB();

    uint32_t move(Game *game, bool print_info);

 private:
    uint32_t move_duration_ms;
    bool single_extension;
    uint32_t max_depth;

    static int32_t search(uint32_t depth, uint32_t ply, int32_t alpha, int32_t beta, Game *game, uint32_t &nodes,
                          bool &aborted, Utils::Timer &timer, const uint32_t move_duration_ms, bool single_extension);
};
