#pragma once
#include <ncurses.h>
#include <queue>

enum Direction {
    up,
    down,
    left,
    right
};

class SnakePiece  
{
public:
    int x, y;

    SnakePiece(int y, int x) {
        this->x = x;
        this->y = y;
    }
};


class Snake { // SnakePiece가 모인 Snake
public:
    Direction cur_direction;

    std::queue<SnakePiece> prev_pieces;
    
    Snake(){ // 시작 방향 아래
        cur_direction = down;
    }
    
    void addPiece(SnakePiece piece) {// 길이 + 1
        prev_pieces.push(piece);
    }
    
    void removePiece(){// 길이 - 1
        prev_pieces.pop();
    }
    
    SnakePiece tail() {// 꼬리 반환 함수
        return prev_pieces.front();
    }
    
    SnakePiece head() {// 머리 반환 함수
        return prev_pieces.back();
    }
    
    void setDirection(Direction d) {// 위치 설정
        cur_direction = d;
    }
    
    SnakePiece nextHead() {// 다음 머리 위치
        int y = head().y;
        int x = head().x;
        
        switch(cur_direction) {
            case down:
                y++;
                break;
            case up:
                y--;
                break;
            case right:
                x++;
                break;
            case left:
                x--;
                break;
        }
        
        return SnakePiece(y, x);
    }
    
    
};
