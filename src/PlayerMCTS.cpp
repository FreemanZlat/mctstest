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
        wins(0),
        visits(0),
        child_win(false)
{
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
        Node *current_node = root;
        Game *current_game = game->clone();
        bool current_player = game->get_player();

        bool is_draw = true;
        bool win_player = true;

        while (current_node->moves.size() == 0 && current_node->children.size() > 0)
        {
            Node *next_node = nullptr;
            float max_value = -100500.0f;
            for (Node *node : current_node->children)
            {
                float value = (float) node->wins / (float) node->visits
                        + sqrtf(2.0f * logf((float) current_node->visits) / (float) node->visits);
                if (node->child_win)
                {
                    if (current_player == current_game->get_player())
                    {
                        value = -100.0f;
                    }
                    else
                    {
                        is_draw = false;
                        win_player = !current_game->get_player();
                        next_node = node;
                        break;
                    }
                }
                if (value > max_value)
                {
                    max_value = value;
                    next_node = node;
                }
            }

            current_node = next_node;
            current_game->move_do(current_node->move);

            if (!is_draw)
                break;

            if (current_game->is_win(!current_game->get_player()))
            {
                is_draw = false;
                win_player = !current_game->get_player();
                break;
            }
        }

        while (is_draw)
        {
            if (current_node->moves.size() > 0)
            {
                int random_move = rand() % current_node->moves.size();
                int move = current_node->moves[random_move];

                current_node->moves[random_move] = current_node->moves.back();
                current_node->moves.pop_back();

                current_game->move_do(move);

                Node *child = new Node(current_node, current_game, move);
                current_node->children.push_back(child);
                current_node = child;

                if (current_game->is_win(!current_game->get_player()))
                {
                    current_node->moves.clear();
                    is_draw = false;
                    win_player = !current_game->get_player();
                    current_node->parent->child_win = true;
                    break;
                }
            }

            std::vector<int> moves = current_game->moves_get();
            while (moves.size() > 0)
            {
                current_game->move_do(moves[rand() % moves.size()]);
                if (current_game->is_win(!current_game->get_player()))
                {
                    is_draw = false;
                    win_player = !current_game->get_player();
                    break;
                }
                moves = current_game->moves_get();
            }

            break;
        }

        delete current_game;

        bool win = !is_draw && (win_player == current_player);
        while (current_node != nullptr)
        {
            current_node->visits++;
            current_node->wins += win ? 1 : 0;
            current_node = current_node->parent;
        }
    }

    int move = 0;
    int max_visits = -1;
    for (Node *node : root->children)
    {
        float value = (float) node->wins / (float) node->visits
                + sqrtf(2.0f * logf((float) root->visits) / (float) node->visits);
        printf("%d : %d/%d (%.4f)\n", node->move, node->wins, node->visits, value);
        if (node->visits > max_visits)
        {
            max_visits = node->visits;
            move = node->move;
        }
    }

    KillTree(root);

    return move;
}

void PlayerMCTS::KillTree(Node *node)
{
    for (Node *child : node->children)
        KillTree(child);
    delete node;
}
