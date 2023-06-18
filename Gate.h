#include <iostream>
#include <time.h>

#pragma once
class Gate
{
public:
    Game& game;
    int gatePeriod; //����Ʈ ���� �ֱ�

    Gate(Game& g): game(g), gatePeriod(0) {};

    void generateGate() //����Ʈ ����
    {
        if(game.usingGate) //����Ʈ�� ������̶�� ����
            return;

        gatePeriod++;
        if (gatePeriod < 60) //����Ʈ ���� �ֱ�
            return;

        srand(time(NULL));
        int gateCount = 0; //����Ʈ�� �ѹ��� �� ������ ����

        removeGate(); //�̹� �־��� ����Ʈ�� ����
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

    void removeGate() //���� �� ���� ����Ʈ ����
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