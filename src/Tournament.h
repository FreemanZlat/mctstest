#pragma once

#include "Game.h"
#include "Player.h"

#include <vector>
#include <mutex>

class Tournament
{
 public:
    Tournament(Game *game, std::vector<Player*> players);
    ~Tournament();
    void play(int rounds = 10, int threads = 1, bool print_info = true);
    void print_result();
    static void test(Game *game, Player *player1, Player *player2);
    static int play(Game *game, Player *player1, Player *player2, bool print_info);

 private:
    struct PlayerInfo
    {
        int id;
        int score;
        Player *player;
        std::vector<int> winsX, winsO, loses, games;
        PlayerInfo(int id, Player *player, int size) :
                id(id),
                score(0),
                player(player)
        {
            winsX.resize(size, 0);
            winsO.resize(size, 0);
            loses.resize(size, 0);
            games.resize(size, 0);
        }
        ~PlayerInfo()
        {
            delete player;
        }
    };

    Game *game;
    std::vector<PlayerInfo*> stats;
    std::vector<std::pair<int, int>> games;
    std::mutex mutex1, mutex2;

    void play_games(Game *game, int threads, bool print_info);
    void play_games(Game *game, bool print_info);
    std::pair<int, int> get_game();
    void play_game(Game *game, int i, int j, bool print_info);
    void thread_func(unsigned int seed, Game *game, bool print_info);
};
