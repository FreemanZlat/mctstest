#pragma once

#include "Game.h"

class GameXO : public Game
{
 public:
    GameXO();
    virtual ~GameXO();

    Game* clone();
    std::vector<int> moves_get(bool sorted);
    void move_do(const int move);
    void move_undo(const int move);
    bool is_win();
    bool get_player();
    int eval();
    void print();

 private:
    bool is_first_player_move;
    std::vector<unsigned char> board;
    static std::vector<unsigned char> win_check;
    static std::vector<int> eval_pst;
};
