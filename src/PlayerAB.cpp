#include "PlayerAB.h"

#include "Game.h"

#include <cstdio>

PlayerAB::PlayerAB(int depth) :
        depth(depth)
{
}

PlayerAB::~PlayerAB()
{
}

int PlayerAB::move(Game *game, bool print_info)
{
    int result = 0;

    std::vector<int> moves = game->moves_get(false);
    int max = -100000;
    int nodes = 1;
    for (int move : moves)
    {
        game->move_do(move);
        int value = -search(depth - 1, 0, -100000, -max, game, nodes);
        game->move_undo(move);

        if (print_info)
            printf("%d : %d\n", move, value);

        if (value > max)
        {
            max = value;
            result = move;
        }
    }

    if (print_info)
        printf("Move=%d (%d)  nodes=%d\n", result, max, nodes);

    return result;
}

int PlayerAB::search(int depth, int ply, int alpha, int beta, Game *game, int &nodes)
{
    nodes++;

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
        int value = -search(depth - 1, ply + 1, -beta, -alpha, game, nodes);
        game->move_undo(move);

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
