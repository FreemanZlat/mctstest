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

    Tournament tournament(new GameXO_4x4x4(), { new PlayerMCTS(5000), new PlayerMCTS(10000), new PlayerAB(3),
                                  new PlayerAB(4) });
    tournament.play(10, 4, true);

//    Tournament::test(new GameXO_4x4x4(), new PlayerAB(5), new PlayerMinimax(4));

    return 0;
}
