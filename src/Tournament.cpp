#include "Tournament.h"

#include "Game.h"
#include "Player.h"
#include "Utils.h"

#include <algorithm>
#include <cstdio>
#include <thread>

Tournament::Tournament(Game *game, std::vector<Player*> players) :
        game(game)
{
    for (uint8_t i = 0; i < players.size(); ++i)
        this->stats.push_back(new PlayerInfo(i, players[i], players.size()));
}

Tournament::~Tournament()
{
    delete game;
    for (int i = 0; i < this->stats.size(); ++i)
        delete this->stats[i];
}

void Tournament::play(uint32_t rounds, uint8_t threads, bool print_info)
{
    for (uint8_t i = 0; i < threads; ++i)
        this->rands.push_back(new Random());

    for (uint32_t round = 0; round < rounds; ++round)
    {
        if (print_info)
            printf("Round %d:\n", round + 1);

        for (uint8_t i = 0; i < this->stats.size(); ++i)
            for (uint8_t j = 0; j < this->stats.size(); ++j)
                this->games.push_back(std::make_pair(i, j));

        this->play_games(threads, print_info);

        std::sort(this->stats.begin(), this->stats.end(), [](PlayerInfo *a, PlayerInfo *b)
        {
            return b->score < a->score;
        });

        if (print_info)
        {
            this->print_result();
            printf("\n");
        }
    }

    for (uint8_t i = 0; i < threads; ++i)
        delete this->rands[i];

    if (print_info)
        printf("done!\n");
}

void Tournament::print_result()
{
    for (uint8_t i = 0; i < this->stats.size(); ++i)
    {
        printf("%d: %7d   | ", this->stats[i]->id, this->stats[i]->score);
        for (uint8_t j = 0; j < this->stats.size(); ++j)
        {
            uint32_t winsX = this->stats[i]->winsX[this->stats[j]->id];
            uint32_t winsO = this->stats[i]->winsO[this->stats[j]->id];
            uint32_t wins = winsX + winsO;
            uint32_t loses = this->stats[i]->loses[this->stats[j]->id];
            uint32_t games = this->stats[i]->games[this->stats[j]->id];
            printf(" %d(%d/%d)-%d / %d |", wins, winsX, winsO, loses, games);
        }
        printf("\n");
    }
}

void Tournament::test(Game *game, Player *player1, Player *player2)
{
    Random rnd;
    uint8_t result = play(&rnd, game, player1, player2, true);

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

Tournament::PlayerInfo::PlayerInfo(uint8_t id, Player *player, uint32_t size) :
        id(id),
        score(0),
        player(player)
{
    winsX.resize(size, 0);
    winsO.resize(size, 0);
    loses.resize(size, 0);
    games.resize(size, 0);
}

Tournament::PlayerInfo::~PlayerInfo()
{
    delete player;
}

void Tournament::play_games(uint8_t threads, bool print_info)
{
    std::vector<std::thread*> threads_pool;
    for (uint8_t i = 0; i < threads; ++i)
        threads_pool.push_back(new std::thread(&Tournament::thread_func, this, i, print_info));

    for (auto thread : threads_pool)
    {
        thread->join();
        delete thread;
    }
}

void Tournament::play_games(Random *rnd, bool print_info)
{
    while (true)
    {
        bool got = false;
        auto players = this->get_game(got);
        if (!got)
            break;

        this->play_game(rnd, players.first, players.second, print_info);
    }
}

std::pair<uint8_t, uint8_t> Tournament::get_game(bool &got)
{
    auto result = std::make_pair(0, 0);
    got = false;

    this->mutex1.lock();

    if (this->games.size() > 0)
    {
        result = games.back();
        got = true;
        games.pop_back();
    }

    this->mutex1.unlock();

    return result;
}

void Tournament::play_game(Random *rnd, uint8_t i, uint8_t j, bool print_info)
{
    Game *current_game = this->game->clone();

    uint8_t result = play(rnd, current_game, this->stats[i]->player, this->stats[j]->player, false);

    this->mutex2.lock();

    if (print_info)
        printf("Game %d x %d : %d\n", this->stats[i]->id, this->stats[j]->id, result);

    if (result == 1)
    {
        if (i != j)
            this->stats[i]->score++;
        this->stats[i]->winsX[this->stats[j]->id]++;
        this->stats[j]->loses[this->stats[i]->id]++;
    }

    if (result == 2)
    {
        if (i != j)
            this->stats[j]->score++;
        this->stats[j]->winsO[this->stats[i]->id]++;
        this->stats[i]->loses[this->stats[j]->id]++;
    }

    this->stats[i]->games[this->stats[j]->id]++;
    this->stats[j]->games[this->stats[i]->id]++;

    this->mutex2.unlock();

    delete current_game;
}

void Tournament::thread_func(uint8_t thread_id, bool print_info)
{
    this->play_games(this->rands[thread_id], print_info);
}

uint8_t Tournament::play(Random *rnd, Game *game, Player *player1, Player *player2, bool print_info)
{
    std::vector<Player*> players = { player1, player2 };
    uint8_t player = 0;

    while (true)
    {
        std::vector<uint32_t> moves = game->moves_get();
        if (moves.size() == 0)
            break;

        if (print_info)
            printf("Player%d:\n", player);

        uint32_t move = players[player]->move(game, rnd, print_info);
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
