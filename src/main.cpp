#include "GameXO.h"
#include "GameXO_4x4x4.h"
#include "PlayerMCTS.h"
#include "Sandbox.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>

int main()
{
    srand((unsigned) time(nullptr));

    int result = Sandbox::play(new GameXO_4x4x4(), new PlayerMCTS(100000), new PlayerMCTS(100000));
    if (result == 0)
        printf("Draw!\n");
    else if (result == 1)
        printf("Player0 win!!\n");
    else
        printf("Player1 win!!\n");
    return 0;
}
