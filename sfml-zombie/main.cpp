#include "stdafx.h"

int main()
{
    FRAMEWORK.Init(1280, 720, "Pong!!");
    FRAMEWORK.Do();
    FRAMEWORK.Release();

    return 0;
}