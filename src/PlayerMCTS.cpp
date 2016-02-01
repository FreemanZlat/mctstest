#include "PlayerMCTS.h"

#include "Game.h"

#include <cstdlib>
#include <cmath>

#include <cstdio>

PlayerMCTS::PlayerMCTS(int iterations) :
        iterations(iterations)
{
}

PlayerMCTS::~PlayerMCTS()
{
}

PlayerMCTS::Node::Node(Node *parent, Game *game, int move) :
        parent(parent),
        moves(game->moves_get()),
        game(game->clone()),
        move(move),
        visits(0),
        endgame(false),
        score(0)
{
    this->wins[0] = 0;
    this->wins[1] = 0;
}

PlayerMCTS::Node::~Node()
{
    delete game;
}

int PlayerMCTS::move(Game *game)
{
    Node *root = new Node(nullptr, game, -1);

    for (int i = 0; i < iterations; ++i)
    {
        Game *game = root->game->clone();
        search(root, game, false);
        delete game;
    }

    int idx = root->game->get_player() ? 0 : 1;
    int move = 0;
    int max_visits = -1;
    for (Node *node : root->children)
    {
        printf("%d : %d-%d / %d\n", node->move, node->wins[idx], node->wins[1 - idx], node->visits);
        if (node->visits > max_visits)
        {
            max_visits = node->visits;
            move = node->move;
        }
    }

    kill_tree(root);

    return move;
}

int PlayerMCTS::search(Node *node, Game *game, bool expand)
{
    bool current_player = game->get_player();
    int player_idx = current_player ? 0 : 1;
    int result = 0;

    if (game->is_win(!current_player))
    {
        result = -1;
        node->endgame = true;
        node->score = -1;
    }
    else if (expand)
    {
        std::vector<int> moves = game->moves_get();
        while (moves.size() > 0)
        {
            game->move_do(moves[rand() % moves.size()]);
            if (game->is_win(!game->get_player()))
            {
                result = current_player == !game->get_player() ? 1 : -1;
                break;
            }
            moves = game->moves_get();
        }
    }
    else if (node->moves.size() > 0 || node->children.size() > 0)
    {
        Node *next = nullptr;
        bool next_expand = false;

        if (node->moves.size() > 0)
        {
            int random_move = rand() % node->moves.size();
            int move = node->moves[random_move];

            node->moves[random_move] = node->moves.back();
            node->moves.pop_back();

            game->move_do(move);

            next = new Node(node, game, move);
            node->children.push_back(next);

            next_expand = true;
        }
        else
        {
            float max_value = -100500.0f;
            for (Node *child : node->children)
            {
                float value = (float) child->wins[player_idx] / (float) child->visits
                        + sqrtf(2.0f * logf((float) node->visits) / (float) child->visits);
                if (child->endgame)
                {
                    if (child->score < 0)
                    {
                        max_value = 100500.0f;
                        next = child;

                        node->endgame = true;
                        node->score = 1;
                        break;
                    }
                    if (child->score > 0)
                    {
                        value = -100.0f;
                    }
                }
                if (value > max_value)
                {
                    max_value = value;
                    next = child;
                }
            }

            if (max_value < 0.0f)
            {
                node->endgame = true;
                node->score = -1;
            }

            game->move_do(next->move);
        }

        result = -search(next, game, next_expand);
    }
    else
    {
        node->endgame = true;
    }

    if (result > 0)
        node->wins[player_idx]++;
    if (result < 0)
        node->wins[1 - player_idx]++;
    node->visits++;

    return result;
}

void PlayerMCTS::kill_tree(Node *node)
{
    for (Node *child : node->children)
        kill_tree(child);
    delete node;
}
