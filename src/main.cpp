#include "GameXO.h"
#include "PlayerMCTS.h"
#include "Sandbox.h"

#include <cstdio>

int main()
{
    int result = Sandbox::play(new GameXO(), new PlayerMCTS(10), new PlayerMCTS(200));
    if (result == 0)
        printf("Draw!\n");
    else if (result == 1)
        printf("PlayerMCTS(10)\n");
    else
        printf("PlayerMCTS(200)\n");
    return 0;
}
