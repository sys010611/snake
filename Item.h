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
    char Poision;
    bool isVisible;
    int Gcnt = 0, Pcnt = 0;

    Item(Game& g) : game(g), isVisible(false)
    {
        Apple = 'A';
        Poision = 'V';
    }

    void Generate_item()
    {
        if (isVisible)
            return;    // 이미 아이템이 있는 경우

        srand((unsigned)time(NULL));

        row = (rand() % 17 + 6);
        col = (rand() % 37 + 5);

        // 랜덤 위치에 아이템 추가
        if (Gcnt == 3 && game.getCharAt(row, col) == '0')
        {
            game.addWithPos(row, col, Poision);
            isVisible = true;
            Pcnt++;
        }

        else if (Gcnt < 3 && game.getCharAt(row, col) == '0')
        {
            game.addWithPos(row, col, Apple);
            isVisible = true;
            Gcnt++;
        }
    }

    void Delete_item()
    {
        if (!isVisible)
            return;

        game.addWithPos(row, col, '0');
        isVisible = false;
    }

    void cnt_init()   // 횟수 초기화 함수
    {
        Gcnt = 0;
        Pcnt = 0;
    }
};

// mainWin = newwin(height, width, yMax/2 - 15, xMax/2-45);