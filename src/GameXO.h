#pragma once

#include "Game.h"

class GameXO : public Game
{
 public:
    GameXO();
    virtual ~GameXO();

    Game* clone();
    std::vector<int> moves_get();
    void move_do(const int move);
    bool is_win(const bool first_player);
    bool get_player();
    int eval();

 private:
    bool is_first_player_move;
    std::vector<unsigned char> board;
    static std::vector<unsigned char> win_check;
};
