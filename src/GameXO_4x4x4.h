#pragma once

#include "Game.h"

class GameXO_4x4x4 : public Game
{
 public:
    GameXO_4x4x4();
    virtual ~GameXO_4x4x4();

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
    std::vector<unsigned char> history;
    static std::vector<std::vector<unsigned char>> check_lines;
    static std::vector<std::vector<int>> win_check;
    static struct _init
    {
        _init();
    } _initializer;
};
