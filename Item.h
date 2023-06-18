#include <iostream>
#include <time.h>
#include "Screen.h"

#pragma once
class Item
{
public:
    Game& game;
    int row, col;
    char Apple;
    char Poison;
    int Gcnt = 0, Pcnt = 0;
    int itemPeriod;

    Item(Game& g) : game(g), itemPeriod(0)
    {
        Apple = 'A';
        Poison = 'P';
    }

    void itemManage()
    {
        Generate_item();
        itemPeriod++;
        if (itemPeriod < 100)
            return;
        Delete_item();
        itemPeriod = 0;
        cnt_init();
        return;
    }

    void Generate_item()
    {
        srand((unsigned)time(NULL));
        if ((Gcnt + Pcnt) < 3)   // Apple 2개, P 1개 생성
        {
            do
            {
                row = (rand() % 17 + 6);
                col = (rand() % 37 + 5);
            } while (game.getCharAt(row, col) != '0');

            if (Gcnt == 0 || Gcnt == 1)
            {
                game.addWithPos(row, col, Apple);
                Gcnt++;
            }
            else
            {
                game.addWithPos(row, col, Poison);
                Pcnt++;
            }
        }
    }

    void Delete_item()
    {
        int flag = 0;
        for (int i = 0; i < 23; i++)
        {
            for (int j = 0; j < 42; j++)
            {
                if (flag == 2)
                    break;
                else
                {
                    if (game.getCharAt(i, j) == 'A' || game.getCharAt(i, j) == 'P')
                    {
                        game.addWithPos(i, j, '0');
                        flag++;
                    }
                }
            }
        }
    }

    void cnt_init()   // 횟수 초기화 함수
    {
        Gcnt = 1;
        Pcnt = 0;
    }

};