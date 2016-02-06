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
    void play(uint32_t rounds = 10, uint8_t threads = 1, bool print_info = true);
    void print_result();
    static void test(Game *game, Player *player1, Player *player2);
    static uint8_t play(Game *game, Player *player1, Player *player2, bool print_info);

 private:
    struct PlayerInfo
    {
        uint8_t id;
        uint32_t score;
        Player *player;
        std::vector<uint32_t> winsX, winsO, loses, games;
        PlayerInfo(uint8_t id, Player *player, uint32_t size) :
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
    std::vector<std::pair<uint8_t, uint8_t>> games;
    std::mutex mutex1, mutex2;

    void play_games(Game *game, uint8_t threads, bool print_info);
    void play_games(Game *game, bool print_info);
    std::pair<uint8_t, uint8_t> get_game(bool &got);
    void play_game(Game *game, uint8_t i, uint8_t j, bool print_info);
    void thread_func(uint32_t seed, Game *game, bool print_info);
};
