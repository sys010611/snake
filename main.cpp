#include <ncurses.h>
#include <iostream>
#include "Screen.h"
#include "Game.h"
#include "Item.h"
#include "Gate.h"


#define ROW_SIZE 30
#define COL_SIZE 90 // �׵θ� ũ��

int main()
{
	initscr();
	noecho();
	refresh();
	
	curs_set(0); //Ŀ�� �Ⱥ��̰�
	
	Game game = Game(ROW_SIZE, COL_SIZE); //���� ������Ʈ ����
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
			case 'p': //���ӿ��� Ȱ��ȭ
				game.gameOver = true;
				break;
			case 'n': //�������� Ŭ���� Ȱ��ȭ
				game.stageClear = true;
				break;

			case 'w':
			case KEY_UP:
				if(game.snake.cur_direction == down) //������ �ݴ�� Ʋ�� ���ӿ���
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

		if (game.gameOver) //���ӿ��� -> ,endwin()
			game.gameOverScreen();

	}

		
	endwin();

	return 0;
}