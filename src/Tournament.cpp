#include "Tournament.h"

#include "Sandbox.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <thread>

Tournament::Tournament(Game *game, std::vector<Player*> players) :
        game(game)
{
    for (int i = 0; i < players.size(); ++i)
        this->stats.push_back(new PlayerInfo(i, players[i], players.size()));
}

Tournament::~Tournament()
{
    delete game;
    for (int i = 0; i < this->stats.size(); ++i)
        delete this->stats[i];
}

void Tournament::play(int rounds, int threads, bool print_info)
{
    for (int round = 0; round < rounds; ++round)
    {
        if (print_info)
            printf("Round %d:\n", round + 1);

        for (int i = 0; i < this->stats.size(); ++i)
            for (int j = 0; j < this->stats.size(); ++j)
                this->games.push_back(std::make_pair(i, j));

        this->play_games(game, threads, print_info);

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

    if (print_info)
        printf("done!\n");
}

void Tournament::print_result()
{
    for (int i = 0; i < this->stats.size(); ++i)
    {
        printf("%d: %7d   | ", this->stats[i]->id, this->stats[i]->score);
        for (int j = 0; j < this->stats.size(); ++j)
        {
            int winsX = this->stats[i]->winsX[this->stats[j]->id];
            int winsO = this->stats[i]->winsO[this->stats[j]->id];
            int wins = winsX + winsO;
            int loses = this->stats[i]->loses[this->stats[j]->id];
            int games = this->stats[i]->games[this->stats[j]->id];
            printf(" %d(%d/%d)-%d / %d |", wins, winsX, winsO, loses, games);
        }
        printf("\n");
    }
}

void Tournament::test(Game *game, Player *player1, Player *player2)
{
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

void Tournament::play_games(Game *game, int threads, bool print_info)
{
    std::vector<std::thread*> threads_pool;
    unsigned int current_time = time(nullptr);
    for (int i = 1; i < threads; ++i)
        threads_pool.push_back(new std::thread(&Tournament::thread_func, this, current_time + i, game, print_info));

    this->play_games(game, print_info);

    for (auto thread : threads_pool)
    {
        thread->join();
        delete thread;
    }
}

void Tournament::play_games(Game *game, bool print_info)
{
    while (true)
    {
        auto players = this->get_game();
        if (players.first < 0 || players.second < 0)
            break;

        this->play_game(game, players.first, players.second, print_info);
    }
}

std::pair<int, int> Tournament::get_game()
{
    auto result = std::make_pair(-1, -1);

    this->mutex1.lock();

    if (this->games.size() > 0)
    {
        result = games.back();
        games.pop_back();
    }

    this->mutex1.unlock();

    return result;
}

void Tournament::play_game(Game *game, int i, int j, bool print_info)
{
    Game *current_game = game->clone();

    int result = Sandbox::play(current_game, this->stats[i]->player, this->stats[j]->player, false);

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

void Tournament::thread_func(unsigned int seed, Game *game, bool print_info)
{
    srand(seed);
    this->play_games(game, print_info);
}
