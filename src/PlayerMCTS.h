#pragma once

#include "Player.h"

#include <vector>

class PlayerMCTS : public Player
{
 public:
    PlayerMCTS(int iterations);
    virtual ~PlayerMCTS();

    int move(Game *game);

 private:
    int iterations;

    struct Node
    {
        Node *parent;
        std::vector<Node*> children;
        std::vector<int> moves;
        Game *game;
        int move;
        int wins, visits;
        bool child_win;

        Node(Node *parent, Game *game, int move);
        ~Node();
    };

    static void KillTree(Node *node);
};
