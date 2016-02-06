#include "GameXO.h"
#include "GameXO_4x4x4.h"
#include "PlayerAB.h"
#include "PlayerMinimax.h"
#include "PlayerMCTS.h"
#include "Tournament.h"

#include <cstdlib>
#include <ctime>

int main()
{
    srand((unsigned) time(nullptr));

    Tournament tournament(new GameXO_4x4x4(), { new PlayerMCTS(500, 10000000), new PlayerMCTS(1000, 10000000),
                                  new PlayerMinimax(500, 64), new PlayerMinimax(1000, 64), new PlayerAB(500, 64),
                                  new PlayerAB(1000, 64) });
    tournament.play(1000, 4, true);

//    Tournament::test(new GameXO_4x4x4(), new PlayerMCTS(1000, 10000000), new PlayerAB(1000, 64));

    return 0;
}
