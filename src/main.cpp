#include "GameXO.h"
#include "GameXO_4x4x4.h"
#include "GameXO_5.h"
#include "PlayerAB.h"
#include "PlayerMinimax.h"
#include "PlayerMCTS.h"
#include "Tournament.h"

int main()
{
    Tournament tournament(
            new GameXO_4x4x4(), { new PlayerMCTS(5000), new PlayerMinimax(5000), new PlayerAB(5000, false, 1),
                    new PlayerAB(5000, true, 1), new PlayerAB(5000, false), new PlayerAB(5000, true) });
    tournament.play(1000, 4, true);

//    Tournament::test(new GameXO_4x4x4(), new PlayerMCTS(1000), new PlayerAB(1000));

    return 0;
}
