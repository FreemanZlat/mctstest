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

    Tournament tournament(new GameXO_4x4x4(), { new PlayerMCTS(5000), new PlayerMinimax(5000), new PlayerAB(5000) });
    tournament.play(1000, 2, true);

//    Tournament::test(new GameXO_4x4x4(), new PlayerMCTS(10000), new PlayerAB(10000));

    return 0;
}
