#include "GameXO.h"
#include "PlayerMCTS.h"
#include "Sandbox.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>

int main()
{
    srand((unsigned) time(nullptr));

    int result = Sandbox::play(new GameXO(), new PlayerMCTS(1000), new PlayerMCTS(200));
    if (result == 0)
        printf("Draw!\n");
    else if (result == 1)
        printf("Player0 win!!\n");
    else
        printf("Player1 win!!\n");
    return 0;
}
