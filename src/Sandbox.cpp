#include "Sandbox.h"

#include "Game.h"
#include "Player.h"

int Sandbox::play(Game *game, Player *player1, Player *player2)
{
    std::vector<Player*> players = { player1, player2 };
    int player = 0;

    while (true)
    {
        std::vector<int> moves = game->moves_get();
        if (moves.size() == 0)
            break;

        int move = players[player]->move(game);
        game->move_do(move);

        if (game->is_win(player == 0))
            return 1 + player;

        player = 1 - player;
    }

    return 0;
}
