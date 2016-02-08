#pragma once

#include "Player.h"

#include <vector>

class Random;

class PlayerMCTS : public Player
{
 public:
    PlayerMCTS(uint32_t move_duration_ms, uint32_t iterations_max = 100000000);
    virtual ~PlayerMCTS();

    uint32_t move(Game *game, Random *rnd, bool print_info);

 private:
    uint32_t move_duration_ms;
    uint32_t iterations_max;

    struct Node
    {
        Node *parent;
        std::vector<Node*> children;
        std::vector<uint32_t> moves;
        uint32_t move;
        uint32_t wins[2];
        uint32_t visits;
        bool endgame;
        int8_t score;

        Node(Node *parent, Game *game, uint32_t move);
        ~Node();
    };

    static int8_t search(Node *node, Game *game, Random *rnd, bool expand);
    static void kill_tree(Node *node);
};
