#include <iostream>
#include <time.h>

#pragma once
class Gate
{
public:
    Game& game;
    int gatePeriod; //게이트 생성 주기

    Gate(Game& g): game(g), gatePeriod(0) {};

    void generateGate() //게이트 생성
    {
        gatePeriod++;
        if (gatePeriod < 10) //게이트 생성 주기: 10초
            return;

        srand(time(NULL));
        int gateCount = 0; //게이트는 한번에 한 쌍으로 생성

        removeGate(); //이미 있었던 게이트는 제거
        while (gateCount < 2)
        {
            int y = rand() % 19 + 5;
            int x = rand() % 40 + 4;

            if (game.getCharAt(y, x) == '1')
            {
                game.addWithPos(y, x, '7');
                gateCount++;
            }
        }
        gatePeriod = 0;
        return;
    }

    void removeGate() //현재 맵 상의 게이트 제거
    {
        for (int i = 0; i < 25; i++)
        {
            for (int j = 0; j < 45; j++)
            {
                if (game.getCharAt(i, j) == '7')
                    game.addWithPos(i, j, '1');
            }
        }
    }
};