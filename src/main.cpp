#include "GameXO.h"
#include "PlayerMCTS.h"
#include "Sandbox.h"

#include <cstdio>
#include <cstdlib>
#include <ctime>

int main()
{
    srand((unsigned) time(nullptr));

    int result = Sandbox::play(new GameXO(), new PlayerMCTS(2000), new PlayerMCTS(2000));
    if (result == 0)
        printf("Draw!\n");
    else if (result == 1)
        printf("Player0\n");
    else
        printf("Player1\n");
    return 0;
}
