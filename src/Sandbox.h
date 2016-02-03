#pragma once

class Game;
class Player;

class Sandbox
{
 public:
    static int play(Game *game, Player *player1, Player *player2, bool print_info = false);
};
