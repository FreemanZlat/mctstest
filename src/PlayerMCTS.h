#pragma once

#include "Player.h"

#include <vector>

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

        void set(Node *parent, Game *game, uint32_t move);
    };

    class NodesPool
    {
     public:
        NodesPool();
        Node* get();
     private:
        std::vector<std::vector<Node>> pool;
        uint32_t idx;
    };

    static int8_t search(Node *node, Game *game, Random *rnd, bool expand, NodesPool &nodes);
    static void kill_tree(Node *node);
};
