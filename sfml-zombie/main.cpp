#include "stdafx.h"

int main()
{
    FRAMEWORK.Init(1280, 720, "Zombie Arena");
    FRAMEWORK.Do();
    FRAMEWORK.Release();

    return 0;
}