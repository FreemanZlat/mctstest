#include "Sandbox.h"

#include "Game.h"
#include "Player.h"

#include <cstdio>

int Sandbox::play(Game *game, Player *player1, Player *player2, bool print_info)
{
    std::vector<Player*> players = { player1, player2 };
    int player = 0;

    while (true)
    {
        std::vector<int> moves = game->moves_get();
        if (moves.size() == 0)
            break;

        if (print_info)
            printf("Player%d:\n", player);

        int move = players[player]->move(game, print_info);
        game->move_do(move);

        if (print_info)
        {
            game->print();
            printf("\n");
        }

        if (game->is_win())
            return 1 + player;

        player = 1 - player;
    }

    return 0;
}
