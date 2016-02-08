#include "PlayerMCTS.h"

#include "Game.h"
#include "Utils.h"

#include <cmath>
#include <cstdio>

PlayerMCTS::PlayerMCTS(uint32_t move_duration_ms, uint32_t iterations_max) :
        move_duration_ms(move_duration_ms),
        iterations_max(iterations_max)
{
}

PlayerMCTS::~PlayerMCTS()
{
}

PlayerMCTS::Node::Node(Node *parent, Game *game, uint32_t move) :
        parent(parent),
        moves(game->moves_get()),
        move(move),
        visits(0),
        endgame(false),
        score(0)
{
    this->children.reserve(moves.size());
    this->wins[0] = 0;
    this->wins[1] = 0;
}

PlayerMCTS::Node::~Node()
{
}

uint32_t PlayerMCTS::move(Game *game, Random *rnd, bool print_info)
{
    Timer timer;

    Node *root = new Node(nullptr, game, -1);

    for (uint32_t i = 0; i < this->iterations_max; ++i)
    {
        Game *root_game = game->clone();
        root->endgame = false;
        root->score = 0;
        search(root, root_game, rnd, false);
        delete root_game;

        if (timer.get() >= this->move_duration_ms)
            break;
    }

    uint8_t idx = game->get_player() ? 0 : 1;
    uint32_t move = 0;
    Node *move_node = nullptr;
    uint32_t max_visits = 0;
    uint32_t sum_visits = 0;
    for (Node *node : root->children)
    {
        if (print_info)
            printf("%3d : %d-%d-%d / %d\n", node->move, node->wins[idx], node->wins[1 - idx],
                   node->visits - node->wins[idx] - node->wins[1 - idx], node->visits);
        sum_visits += node->visits;
        if (node->visits > max_visits)
        {
            max_visits = node->visits;
            move = node->move;
            move_node = node;
        }
    }

    if (print_info)
        printf("Move=%d  %d-%d-%d / %d (%d)\n", move, move_node->wins[idx], move_node->wins[1 - idx],
               move_node->visits - move_node->wins[idx] - move_node->wins[1 - idx], move_node->visits, sum_visits);

    kill_tree(root);

    return move;
}

int8_t PlayerMCTS::search(Node *node, Game *game, Random *rnd, bool expand)
{
    bool current_player = game->get_player();
    uint8_t player_idx = current_player ? 0 : 1;
    int8_t result = 0;

    if (node->endgame)
    {
        result = node->score;
    }
    else if (game->is_win())
    {
        result = -1;
        node->endgame = true;
        node->score = -1;
    }
    else if (expand)
    {
        std::vector<uint32_t> moves = game->moves_get();
        while (moves.size() > 0)
        {
            game->move_do(moves[rnd->get() % moves.size()]);
            if (game->is_win())
            {
                result = current_player != game->get_player() ? 1 : -1;
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
            uint32_t random_move = rnd->get() % node->moves.size();
            uint32_t move = node->moves[random_move];

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
                        value -= 100000.0f;
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

        result = -search(next, game, rnd, next_expand);
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
