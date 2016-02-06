#pragma once

#include "Player.h"
#include "Utils.h"

#include <vector>

class PlayerMinimax : public Player
{
 public:
    PlayerMinimax(uint32_t move_duration_ms, uint32_t max_depth);
    virtual ~PlayerMinimax();

    uint32_t move(Game *game, bool print_info);

 private:
    uint32_t move_duration_ms;
    uint32_t max_depth;

    static int32_t search(uint32_t depth, uint32_t ply, Game *game, uint32_t &nodes, bool &aborted, Utils::Timer &timer,
                          const uint32_t move_duration_ms);
};
