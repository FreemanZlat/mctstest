#pragma once

class Game;

class Player
{
 public:
    virtual ~Player() {};
    virtual int move(Game *game) = 0;
};
