#pragma once

#include "Player.h"

#include <vector>

class PlayerMCTS : public Player
{
 public:
    PlayerMCTS(int iterations);
    virtual ~PlayerMCTS();

    int move(Game *game, bool print_info);

 private:
    int iterations;

    struct Node
    {
        Node *parent;
        std::vector<Node*> children;
        std::vector<int> moves;
        int move;
        int wins[2];
        int visits;
        bool endgame;
        int score;

        Node(Node *parent, Game *game, int move);
        ~Node();
    };

    static int search(Node *node, Game *game, bool expand);
    static void kill_tree(Node *node);
};
