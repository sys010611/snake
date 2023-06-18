#include <ncurses.h>
#include <iostream>
#include "Screen.h"
#include "Game.h"
#include "Item.h"
#include "Gate.h"


#define ROW_SIZE 30
#define COL_SIZE 90 // 테두리 크기

int main()
{
	initscr();
	noecho();
	refresh();
	
	curs_set(0); //커서 안보이게
	
	Game game = Game(ROW_SIZE, COL_SIZE); //게임 오브젝트 생성
	Item item = Item(game);
	Gate gate = Gate(game);

	game.updateScreen();

	
	while (true)
	{
		if(!game.stageClear && !game.gameClear)
			game.second++;

		item.itemManage();
		game.updateScore();
		
		if(!game.gameOver && !game.gameClear)
			game.updateSnake();

		gate.generateGate();

		chtype input = game.getInput();
		switch (input)
		{
			case 'p': //게임오버 활성화
				game.gameOver = true;
				break;
			case 'n': //스테이지 클리어 활성화
				game.stageClear = true;
				break;

			case 'w':
			case KEY_UP:
				if(game.snake.cur_direction == down) //방향을 반대로 틀시 게임오버
					game.gameOver = true;
				game.snake.setDirection(up);
				break;
			case 'a':
			case KEY_LEFT:
				if (game.snake.cur_direction == right)
					game.gameOver = true;
				game.snake.setDirection(left);
				break;
			case 's':
			case KEY_DOWN:
				if (game.snake.cur_direction == up)
					game.gameOver = true;
				game.snake.setDirection(down);
				break;
			case 'd':
			case KEY_RIGHT:
				if (game.snake.cur_direction == left)
					game.gameOver = true;
				game.snake.setDirection(right);
				break;
		}

		if (game.gameOver) //게임오버 -> ,endwin()
			game.gameOverScreen();

	}

		
	endwin();

	return 0;
}