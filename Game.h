#include <ncurses.h>
#include <time.h>
#include "Screen.h"
#include "Snake.h"
#include "Score.cpp"

#pragma once
class Game //GameManager�� ����
{
public:
	Screen screen; //Screen ��ü
	Snake snake; //snake ��ü
	bool gameOver; //���ӿ��� ����
	bool gameClear; //��ü ���� Ŭ���� ����
	bool stageClear; // �������� Ŭ���� ����
	int mapNumber; //�� ��ȣ
	bool usingGate; //���� ����Ʈ�� ���������
	static int inGateLength; //����Ʈ�� ������� ��ü�� ����
	int currSize; //���� �� ����
	int second = 0;
	Score score;

	//�̼� ��ҵ� (�ε���: n�� ��������)
	int missionLength[4] = {5,5,5,5};
	int missionPlus[4] = {1,1,2,1};
	int missionMinus[4] = {1,1,1,1};
	int missionGate[4] = {1,2,2,3};

	Game(int height, int width)
	{
		screen = Screen(height, width);
		screen.init(); //�� ������ clear, ���
		gameOver = false;
		stageClear = false;
		gameClear = false;
		usingGate = false;
		mapNumber = 0; //���� �� �� ��ȣ = 0
		toNextStage(); //���� �� ���		
	}

	void initializeSnake()
	{
		while(!snake.prev_pieces.empty()) //���� ������ũ ������ �����
			snake.prev_pieces.pop();

		snake.setDirection(down);

		SnakePiece next = SnakePiece(6, 5);
		addSnakePiece(next);
		snake.addPiece(next);

		next = snake.nextHead();
		addSnakePiece(next);
		snake.addPiece(next);

		next = snake.nextHead();
		addSnakePiece(next);
		snake.addPiece(next);

		snake.setDirection(right);

		next = snake.nextHead();
		addSnakePiece(next);
		snake.addPiece(next);
	}

	void updateSnake()
	{
		if(stageClear)
			return;
		//snake update
		currSize = snake.prev_pieces.size(); //������ũ ���� ����
		if (currSize <= 3)
			gameOver = true; //���� ���̰� 3 ����-> ���ӿ���

		if (missionLength[mapNumber - 1] <= currSize)
			score.lengthCheck = 1; //���� ���̰� �̼� ���� �̻� -> �̼� �޼�

		removeSnakePiece(snake.prev_pieces.front());
		snake.removePiece(); //�� ������ ���̸� ���̴� �ڵ�

		SnakePiece next = snake.nextHead();

		if (getCharAt(next.y, next.x) == 'A')
		{
			// apple������ GItem �ϳ� ������Ű��, �̼� �����ߴ��� üũ
			score.GItem++;
			if (missionPlus[mapNumber-1] <= score.GItem)
				score.GCheck = 1;

			// ������ũ ���� ����
			SnakePiece next = snake.nextHead();
			addSnakePiece(next);
			snake.addPiece(next);
			next = snake.nextHead();
			addSnakePiece(next);
			snake.addPiece(next);
			return;
		}
		else if (getCharAt(next.y, next.x) == 'P')
		{
			SnakePiece next = snake.nextHead();
			addSnakePiece(next);
			snake.addPiece(next);

			// poison������ PItem �ϳ� ������Ű��, �̼� �����ߴ��� üũ
			score.PItem++;
			if (missionMinus[mapNumber-1] <= score.PItem)
				score.PCheck = 1;

			// ������ũ ���� ����  Error
			removeSnakePiece(snake.prev_pieces.front());
			snake.removePiece();
			return;
			// ������ũ ���� 2 �����̸� ���� ����
		}

		if (getCharAt(next.y, next.x) == '7') //����Ʈ�� ���������� ó��
		{
			score.gatePass++;
			if (missionGate[mapNumber-1] <= score.gatePass)
				score.gateCheck = 1;

			inGateLength = currSize; //����Ʈ�� ������� ��ü�� ����
			enterGate(next.y, next.x);
			return;
		}
		inGateLength--;
		if(inGateLength <= 0) //����Ʈ�� �� ����ߴٸ� �̿� �� flag off
			usingGate = false;
		

		addSnakePiece(next);
		snake.addPiece(next); // �տ� SnakePiece next = snake.nextHead(); ���� �ؼ� �� ���� ���̸� �ø��� �ڵ�
	}

	void updateScore()
	{
		Sclear();
		if (stageClear) // �������� Ŭ���� ȭ�� ǥ��
		{
			screen.loadMap(5);
			chtype input = getInput();
			if (input == 't')
			{
				toNextStage();
				return;
			}
		}

		makeScore(score);
		wrefresh(screen.scoreBoardWin);

		char lc = score.lengthCheck == 1 ? 'v' : ' ';
		char gc = score.GCheck == 1 ? 'v' : ' ';
		char pc = score.PCheck == 1 ? 'v' : ' ';
		char gtc = score.gateCheck == 1 ? 'v' : ' ';

		screen.addWithPosAndWin(3, 9, lc, screen.missionWin);
		screen.addWithPosAndWin(4, 9, gc, screen.missionWin);
		screen.addWithPosAndWin(5, 9, pc, screen.missionWin);
		screen.addWithPosAndWin(6, 9, gtc, screen.missionWin);

		screen.refresh();
	}

	void enterGate(int currY, int currX) //����Ʈ ����
	{
		usingGate = true; //�̿� �� flag on

		int otherGateY, otherGateX;
		for (int i = 0; i < 25; i++)
		{
			for (int j = 0; j < 45; j++)
			{
				if (getCharAt(i, j) == '7' && (i != currY || j != currX)) //�Ǵٸ� ����Ʈ �ϳ� ã��
				{
					otherGateY = i, otherGateX = j; break;
				}
			}
		}
		int nextY = otherGateY, nextX = otherGateX;
		chtype nextPosMap;

		switch (snake.cur_direction)
		{
			case up:
				nextPosMap = getCharAt(otherGateY - 1, otherGateX);
				if (nextPosMap != '1' && nextPosMap != '2' && nextPosMap != ' ') //���� ���� �״�� �����Ѱ��
					nextY -= 1;
				else if (nextPosMap == '1' || nextPosMap == '2' || nextPosMap == ' ') //������ �����ִ� ��� / �� ���� ���
				{
					snake.cur_direction = right; //������ ���� �õ�
					enterGate(currY, currX); //��� ȣ��
					return;
				}
				break;
			case down:
				nextPosMap = getCharAt(otherGateY + 1, otherGateX);
				if (nextPosMap != '1' && nextPosMap != '2' && nextPosMap != ' ') //���� ���� �״�� �����Ѱ��
					nextY += 1;
				else if (nextPosMap == '1' || nextPosMap == '2' || nextPosMap == ' ') //������ �����ִ� ��� / �� ���� ���
				{
					snake.cur_direction = left; //���� ���� �õ�
					enterGate(currY, currX);
					return;
				}
				break;
			case right:	
				nextPosMap = getCharAt(otherGateY, otherGateX+1);
				if (nextPosMap != '1' && nextPosMap != '2' && nextPosMap != ' ') //���� ���� �״�� �����Ѱ��
					nextX += 1;
				else if (nextPosMap == '1' || nextPosMap == '2' || nextPosMap == ' ') //������ �����ִ� ��� / �� ���� ���
				{
					snake.cur_direction = down; //�Ʒ� ���� �õ�
					enterGate(currY, currX);
					return;
				}
				break;
			case left:
				nextPosMap = getCharAt(otherGateY, otherGateX - 1);
				if (nextPosMap != '1' && nextPosMap != '2' && nextPosMap != ' ') //���� ���� �״�� �����Ѱ��
					nextX -= 1;
				else if (nextPosMap == '1' || nextPosMap == '2' || nextPosMap == ' ') //������ �����ִ� ��� / �� ���� ���
				{
					snake.cur_direction = up; //�� ���� �õ�
					enterGate(currY, currX);
					return;
				}
				break;
		}


		SnakePiece next = SnakePiece(nextY, nextX);
		addSnakePiece(next);
		snake.addPiece(next);
		return;
	}

	void Sclear()
	{
		if (score.lengthCheck && score.GCheck && score.PCheck && score.gateCheck)
		{
			stageClear = true;
			score.GItem = 0;
			score.PItem = 0;
			score.gatePass = 0;

			score.lengthCheck = 0;
			score.GCheck = 0;
			score.PCheck = 0;
			score.gateCheck = 0;
		}
	}

	void toNextStage() //���� ����������
	{
		mvwprintw(screen.scoreBoardWin, 7, 8, "   ", second);
		second = 0;
		stageClear = false;
		if (mapNumber == 4)
		{
			gameClear = true; 
			gameClearScreen();
			return;
		}
		screen.loadMap(mapNumber); //�� �ε�
		makeMission(mapNumber++);
		makeScore(score);
		mvwprintw(screen.mainWin, 1, 2, "Stage"); //�������� ǥ��
		mvwaddch(screen.mainWin, 1, 8, mapNumber+'0');
		initializeSnake(); //������ũ �ʱ�ȭ
	}

	void gameOverScreen()
	{
		screen.loadMap(-1);
		screen.refresh();
	}

	void gameClearScreen()
	{
		screen.loadMap(4);
		screen.refresh();
	}

	void makeMission(int mapNumber) //�̼� ���� 
	{
		screen.addWithPosAndWin(3, 6, (missionLength[mapNumber] + '0'), screen.missionWin);
		screen.addWithPosAndWin(4, 6, missionPlus[mapNumber] + '0', screen.missionWin);
		screen.addWithPosAndWin(5, 6, missionMinus[mapNumber] + '0', screen.missionWin);
		screen.addWithPosAndWin(6, 6, missionGate[mapNumber] + '0', screen.missionWin);
		screen.refresh();
	}

	void makeScore(Score s)   // ���� ���ھ� 
	{
		screen.addWithPosAndWin(3, 6, currSize + '0', screen.scoreBoardWin);
		screen.addWithPosAndWin(4, 6, score.GItem + '0', screen.scoreBoardWin);
		screen.addWithPosAndWin(5, 6, score.PItem + '0', screen.scoreBoardWin);
		screen.addWithPosAndWin(6, 6, score.gatePass + '0', screen.scoreBoardWin);
		mvwprintw(screen.scoreBoardWin, 7, 8, "%d", second);

	}

	void updateScreen() //ȭ�� ���
	{
		screen.refresh();
	}

	chtype getInput() //�Է� �ޱ�
	{
		chtype input = screen.getInput();
		return input;
	}

	void addWithPos(int y, int x, chtype ch) //��ǥ ���� �� ���� ���
	{
		screen.addWithPos(y, x, ch);
	}

	void addSnakePiece(SnakePiece& p)
	{
		if(getCharAt(p.y,p.x) == '1' || getCharAt(p.y,p.x) == '2') //���� �ε��� �� ���ӿ���
			gameOver = true;
		else if (getCharAt(p.y, p.x) == '4') // �ڱ� ���� �ε��� �� ���ӿ���
			gameOver = true;

		addWithPos(p.y, p.x, '4'); //���ٸ��� ���� �� �״�� ���� ��ġ�� 4 ���
	}

	void removeSnakePiece(SnakePiece& p)
	{
		addWithPos(p.y, p.x, '0');
	}

	chtype getCharAt(int y, int x) //�ش� ��ǥ�� ���� ���� �б�
	{
		return screen.getCharAt(y, x);
	}
};

int Game::inGateLength = 0;