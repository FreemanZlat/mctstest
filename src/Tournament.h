#pragma once

#include <vector>
#include <mutex>

class Game;
class Player;
class Random;

class Tournament
{
 public:
    Tournament(Game *game, std::vector<Player*> players);
    ~Tournament();
    void play(uint32_t rounds = 10, uint8_t threads = 1, bool print_info = true);
    void print_result();
    static void test(Game *game, Player *player1, Player *player2);

 private:
    struct PlayerInfo
    {
        uint8_t id;
        uint32_t score;
        Player *player;
        std::vector<uint32_t> winsX, winsO, loses, games;
        PlayerInfo(uint8_t id, Player *player, uint32_t size);
        ~PlayerInfo();
    };

    Game *game;
    std::vector<PlayerInfo*> stats;
    std::vector<std::pair<uint8_t, uint8_t>> games;
    std::vector<Random*> rands;
    std::mutex mutex1, mutex2;

    void play_games(uint8_t threads, bool print_info);
    void play_games(Random *rnd, bool print_info);
    std::pair<uint8_t, uint8_t> get_game(bool &got);
    void play_game(Random *rnd, uint8_t i, uint8_t j, bool print_info);
    void thread_func(uint8_t thread_id, bool print_info);
    static uint8_t play(Random *rnd, Game *game, Player *player1, Player *player2, bool print_info);
};
