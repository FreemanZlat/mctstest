#include "PlayerAB.h"

#include "Game.h"
#include "Utils.h"

#include <algorithm>
#include <cstdio>

PlayerAB::PlayerAB(int move_duration_ms, int max_depth) :
        move_duration_ms(move_duration_ms),
        max_depth(max_depth)
{
}

PlayerAB::~PlayerAB()
{
}

int PlayerAB::move(Game *game, bool print_info)
{
    Utils::Timer timer;

    int result = 0;
    int result_score = 0;

    std::vector<int> moves = game->moves_get(false);
    std::vector<std::pair<int, int>> moves_scores;
    for (int move : moves)
        moves_scores.push_back(std::make_pair(move, 0));

    int nodes = 1;
    bool aborted = false;
    for (int d = 0; d < this->max_depth; ++d)
    {
        int max = -100000;
        for (auto &move : moves_scores)
        {
            game->move_do(move.first);
            move.second = -search(d, 0, -100000, -max, game, nodes, aborted, timer, this->move_duration_ms);
            game->move_undo(move.first);

            if (aborted)
                break;

            if (print_info)
                printf("Depth:%d move:%d : %d\n", d + 1, move.first, move.second);

            if (move.second > max)
            {
                max = move.second;
                result = move.first;
                result_score = max;
            }
        }

        if (aborted)
            break;

        std::sort(moves_scores.begin(), moves_scores.end(), [](std::pair<int, int> a, std::pair<int, int> b)
        {
            return b.second < a.second;
        });
    }

    if (print_info)
        printf("Move=%d (%d)  nodes=%d\n", result, result_score, nodes);

    return result;
}

int PlayerAB::search(int depth, int ply, int alpha, int beta, Game *game, int &nodes, bool &aborted,
                     Utils::Timer &timer, const int move_duration_ms)
{
    if (nodes++ % 100000 && timer.get() >= move_duration_ms)
    {
        aborted = true;
        return 0;
    }

    if (game->is_win())
        return -(10000 - ply);
    if (depth == 0)
        return game->eval();

    std::vector<int> moves = game->moves_get(false);
    if (moves.size() == 0)
        return 0;

    int max = -100000;
    int result = 0;
    for (int move : moves)
    {
        game->move_do(move);
        int value = -search(depth - 1, ply + 1, -beta, -alpha, game, nodes, aborted, timer, move_duration_ms);
        game->move_undo(move);

        if (aborted)
            return 0;

        if (value > max)
        {
            max = value;
            result = move;
        }

        if (max > alpha)
            alpha = max;
        if (alpha > beta)
            return alpha;
    }

    return max;
}
