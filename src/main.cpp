#include "Game.h"
#include "GameXO.h"
#include "GameXO_4x4x4.h"
#include "Player.h"
#include "PlayerAB.h"
#include "PlayerMinimax.h"
#include "PlayerMCTS.h"
#include "Sandbox.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>

struct Plr
{
    int id;
    int score;
    Player *player;
    std::vector<int> winsX, winsO, loses, games;
    Plr(int id, Player *player, int size) :
            id(id),
            score(0),
            player(player)
    {
        winsX.resize(size, 0);
        winsO.resize(size, 0);
        loses.resize(size, 0);
        games.resize(size, 0);
    }
    ~Plr()
    {
        delete player;
    }
};

static void tournament()
{
    Game *game = new GameXO_4x4x4();

    const std::vector<Player*> players = { new PlayerMCTS(10000), new PlayerMCTS(50000), new PlayerAB(3), new PlayerAB(
            4), new PlayerAB(5) };
    std::vector<Plr*> plrs;
    for (int i = 0; i < players.size(); ++i)
        plrs.push_back(new Plr(i, players[i], players.size()));

    while (true)
    {
        for (int i = 0; i < plrs.size(); ++i)
            for (int j = 0; j < plrs.size(); ++j)
            {
                Game *current_game = game->clone();

                int result = Sandbox::play(current_game, plrs[i]->player, plrs[j]->player, false);

                printf("Game %d x %d : %d\n", plrs[i]->id, plrs[j]->id, result);

                if (result == 1)
                {
                    if (i != j)
                        plrs[i]->score++;
                    plrs[i]->winsX[plrs[j]->id]++;
                    plrs[j]->loses[plrs[i]->id]++;
                }

                if (result == 2)
                {
                    if (i != j)
                        plrs[j]->score++;
                    plrs[j]->winsO[plrs[i]->id]++;
                    plrs[i]->loses[plrs[j]->id]++;
                }

                plrs[i]->games[plrs[j]->id]++;
                plrs[j]->games[plrs[i]->id]++;

                delete current_game;
            }

        std::sort(plrs.begin(), plrs.end(), [](Plr *a, Plr *b)
        {
            return b->score < a->score;
        });

        for (int i = 0; i < plrs.size(); ++i)
        {
            printf("%d: %7d   | ", plrs[i]->id, plrs[i]->score);
            for (int j = 0; j < plrs.size(); ++j)
            {
                int winsX = plrs[i]->winsX[plrs[j]->id];
                int winsO = plrs[i]->winsO[plrs[j]->id];
                int wins = winsX + winsO;
                int loses = plrs[i]->loses[plrs[j]->id];
                int games = plrs[i]->games[plrs[j]->id];
                printf(" %d(%d/%d)-%d / %d |", wins, winsX, winsO, loses, games);
            }
            printf("\n");
        }

        printf("\n");
    }

    for (int i = 0; i < plrs.size(); ++i)
        delete plrs[i];
}

static void test()
{
    Game *game = new GameXO_4x4x4();
    Player *player2 = new PlayerAB(5);
    Player *player1 = new PlayerMinimax(4);

    int result = Sandbox::play(game, player1, player2, true);

    delete game;
    delete player1;
    delete player2;

    if (result == 0)
        printf("Draw!\n");
    else if (result == 1)
        printf("Player0 win!!\n");
    else
        printf("Player1 win!!\n");
}

int main()
{
    srand((unsigned) time(nullptr));

//    test();
    tournament();

    return 0;
}
