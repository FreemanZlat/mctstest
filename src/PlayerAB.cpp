#include "PlayerAB.h"

#include "Game.h"
#include "Utils.h"

#include <algorithm>
#include <cstdio>

PlayerAB::PlayerAB(uint32_t move_duration_ms, uint8_t eval_type, bool single_extension, uint32_t max_depth)
{
    this->params.move_duration_ms = move_duration_ms;
    this->params.eval_type = eval_type;
    this->params.single_extension = single_extension;
    this->params.max_depth = max_depth;
    this->params.aborted = false;
    this->params.nodes = 0;
    this->params.timer = new Timer();
}

PlayerAB::~PlayerAB()
{
    delete this->params.timer;
}

uint32_t PlayerAB::move(Game *game, Random *rnd, bool print_info)
{
    this->params.timer->start();

    uint32_t result = 0;
    int32_t result_score = 0;

    std::vector<uint32_t> moves = game->moves_get(false);
    std::vector<std::pair<uint32_t, int32_t>> moves_scores;
    for (uint32_t move : moves)
        moves_scores.push_back(std::make_pair(move, rnd->get() % 1000));

    this->params.nodes = 1;
    this->params.aborted = false;
    for (uint32_t d = 0; d < this->params.max_depth; ++d)
    {
        std::sort(moves_scores.begin(), moves_scores.end(),
                  [](std::pair<uint32_t, int32_t> a, std::pair<uint32_t, int32_t> b)
                  {
                      return b.second < a.second;
                  });

        int32_t max = -100000;
        for (auto &move : moves_scores)
        {
            game->move_do(move.first);
            move.second = -search(d, 0, -100000, -max, game, this->params);
            game->move_undo(move.first);

            if (this->params.aborted)
                break;

            if (print_info && this->params.nodes > 200000)
                printf("Depth:%d move:%d : %d\n", d + 1, move.first, move.second);

            if (move.second > max)
            {
                max = move.second;
                result = move.first;
                result_score = max;
            }
        }

        if (this->params.aborted)
            break;
    }

    if (print_info)
        printf("Move=%d (%d)  nodes=%d\n", result, result_score, this->params.nodes);

    return result;
}

int32_t PlayerAB::search(uint32_t depth, uint32_t ply, int32_t alpha, int32_t beta, Game *game, Params &params)
{
    if ((++params.nodes % 50000) == 0 && params.timer->get() >= params.move_duration_ms)
    {
        params.aborted = true;
        return 0;
    }

    if (game->is_win())
        return -(10000 - ply);

    if (params.single_extension && game->is_single_move())
        depth++;

    if (depth == 0)
        return game->eval(params.eval_type);

    std::vector<uint32_t> moves = game->moves_get(false);
    if (moves.size() == 0)
        return 0;

    int32_t max = -100000;
    for (uint32_t move : moves)
    {
        game->move_do(move);
        int32_t value = -search(depth - 1, ply + 1, -beta, -alpha, game, params);
        game->move_undo(move);

        if (params.aborted)
            return 0;

        if (value > max)
            max = value;

        if (max > alpha)
            alpha = max;
        if (alpha > beta)
            return alpha;
    }

    return max;
}
