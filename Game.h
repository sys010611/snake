#include <ncurses.h>
#include <time.h>
#include "Screen.h"
#include "Snake.h"
#include "Score.cpp"

#pragma once
class Game //GameManager의 역할
{
public:
	Screen screen; //Screen 객체
	Snake snake; //snake 객체
	bool gameOver; //게임오버 여부
	bool gameClear; //전체 게임 클리어 여부
	bool stageClear; // 스테이지 클리어 여부
	int mapNumber; //맵 번호
	bool usingGate; //현재 게이트를 통과중인지
	static int inGateLength; //게이트를 통과중인 몸체의 길이
	int currSize; //현재 몸 길이
	int second = 0;
	Score score;

	//미션 요소들 (인덱스: n번 스테이지)
	int missionLength[4] = {5,5,5,5};
	int missionPlus[4] = {1,1,2,1};
	int missionMinus[4] = {1,1,1,1};
	int missionGate[4] = {1,2,2,3};

	Game(int height, int width)
	{
		screen = Screen(height, width);
		screen.init(); //각 윈도우 clear, 출력
		gameOver = false;
		stageClear = false;
		gameClear = false;
		usingGate = false;
		mapNumber = 0; //시작 시 맵 번호 = 0
		toNextStage(); //시작 맵 출력		
	}

	void initializeSnake()
	{
		while(!snake.prev_pieces.empty()) //기존 스네이크 조각들 지우기
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
		currSize = snake.prev_pieces.size(); //스네이크 길이 저장
		if (currSize <= 3)
			gameOver = true; //몸의 길이가 3 이하-> 게임오버

		if (missionLength[mapNumber - 1] <= currSize)
			score.lengthCheck = 1; //몸의 길이가 미션 길이 이상 -> 미션 달성

		removeSnakePiece(snake.prev_pieces.front());
		snake.removePiece(); //이 두줄이 길이를 줄이는 코드

		SnakePiece next = snake.nextHead();

		if (getCharAt(next.y, next.x) == 'A')
		{
			// apple먹으면 GItem 하나 증가시키고, 미션 성공했는지 체크
			score.GItem++;
			if (missionPlus[mapNumber-1] <= score.GItem)
				score.GCheck = 1;

			// 스네이크 길이 증가
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

			// poison먹으면 PItem 하나 증가시키고, 미션 성공했는지 체크
			score.PItem++;
			if (missionMinus[mapNumber-1] <= score.PItem)
				score.PCheck = 1;

			// 스네이크 길이 감소  Error
			removeSnakePiece(snake.prev_pieces.front());
			snake.removePiece();
			return;
			// 스네이크 길이 2 이하이면 게임 종료
		}

		if (getCharAt(next.y, next.x) == '7') //게이트를 만났을때의 처리
		{
			score.gatePass++;
			if (missionGate[mapNumber-1] <= score.gatePass)
				score.gateCheck = 1;

			inGateLength = currSize; //게이트를 통과중인 몸체의 길이
			enterGate(next.y, next.x);
			return;
		}
		inGateLength--;
		if(inGateLength <= 0) //게이트를 다 통과했다면 이용 중 flag off
			usingGate = false;
		

		addSnakePiece(next);
		snake.addPiece(next); // 앞에 SnakePiece next = snake.nextHead(); 까지 해서 세 줄이 길이를 늘리는 코드
	}

	void updateScore()
	{
		Sclear();
		if (stageClear) // 스테이지 클리어 화면 표시
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

	void enterGate(int currY, int currX) //게이트 진입
	{
		usingGate = true; //이용 중 flag on

		int otherGateY, otherGateX;
		for (int i = 0; i < 25; i++)
		{
			for (int j = 0; j < 45; j++)
			{
				if (getCharAt(i, j) == '7' && (i != currY || j != currX)) //또다른 게이트 하나 찾기
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
				if (nextPosMap != '1' && nextPosMap != '2' && nextPosMap != ' ') //진행 방향 그대로 가능한경우
					nextY -= 1;
				else if (nextPosMap == '1' || nextPosMap == '2' || nextPosMap == ' ') //벽으로 막혀있는 경우 / 맵 밖인 경우
				{
					snake.cur_direction = right; //오른쪽 방향 시도
					enterGate(currY, currX); //재귀 호출
					return;
				}
				break;
			case down:
				nextPosMap = getCharAt(otherGateY + 1, otherGateX);
				if (nextPosMap != '1' && nextPosMap != '2' && nextPosMap != ' ') //진행 방향 그대로 가능한경우
					nextY += 1;
				else if (nextPosMap == '1' || nextPosMap == '2' || nextPosMap == ' ') //벽으로 막혀있는 경우 / 맵 밖인 경우
				{
					snake.cur_direction = left; //왼쪽 방향 시도
					enterGate(currY, currX);
					return;
				}
				break;
			case right:	
				nextPosMap = getCharAt(otherGateY, otherGateX+1);
				if (nextPosMap != '1' && nextPosMap != '2' && nextPosMap != ' ') //진행 방향 그대로 가능한경우
					nextX += 1;
				else if (nextPosMap == '1' || nextPosMap == '2' || nextPosMap == ' ') //벽으로 막혀있는 경우 / 맵 밖인 경우
				{
					snake.cur_direction = down; //아래 방향 시도
					enterGate(currY, currX);
					return;
				}
				break;
			case left:
				nextPosMap = getCharAt(otherGateY, otherGateX - 1);
				if (nextPosMap != '1' && nextPosMap != '2' && nextPosMap != ' ') //진행 방향 그대로 가능한경우
					nextX -= 1;
				else if (nextPosMap == '1' || nextPosMap == '2' || nextPosMap == ' ') //벽으로 막혀있는 경우 / 맵 밖인 경우
				{
					snake.cur_direction = up; //위 방향 시도
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

	void toNextStage() //다음 스테이지로
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
		screen.loadMap(mapNumber); //맵 로드
		makeMission(mapNumber++);
		makeScore(score);
		mvwprintw(screen.mainWin, 1, 2, "Stage"); //스테이지 표시
		mvwaddch(screen.mainWin, 1, 8, mapNumber+'0');
		initializeSnake(); //스네이크 초기화
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

	void makeMission(int mapNumber) //미션 생성 
	{
		screen.addWithPosAndWin(3, 6, (missionLength[mapNumber] + '0'), screen.missionWin);
		screen.addWithPosAndWin(4, 6, missionPlus[mapNumber] + '0', screen.missionWin);
		screen.addWithPosAndWin(5, 6, missionMinus[mapNumber] + '0', screen.missionWin);
		screen.addWithPosAndWin(6, 6, missionGate[mapNumber] + '0', screen.missionWin);
		screen.refresh();
	}

	void makeScore(Score s)   // 현재 스코어 
	{
		screen.addWithPosAndWin(3, 6, currSize + '0', screen.scoreBoardWin);
		screen.addWithPosAndWin(4, 6, score.GItem + '0', screen.scoreBoardWin);
		screen.addWithPosAndWin(5, 6, score.PItem + '0', screen.scoreBoardWin);
		screen.addWithPosAndWin(6, 6, score.gatePass + '0', screen.scoreBoardWin);
		mvwprintw(screen.scoreBoardWin, 7, 8, "%d", second);

	}

	void updateScreen() //화면 출력
	{
		screen.refresh();
	}

	chtype getInput() //입력 받기
	{
		chtype input = screen.getInput();
		return input;
	}

	void addWithPos(int y, int x, chtype ch) //좌표 지정 후 문자 출력
	{
		screen.addWithPos(y, x, ch);
	}

	void addSnakePiece(SnakePiece& p)
	{
		if(getCharAt(p.y,p.x) == '1' || getCharAt(p.y,p.x) == '2') //벽에 부딪힐 시 게임오버
			gameOver = true;
		else if (getCharAt(p.y, p.x) == '4') // 자기 몸에 부딪힐 시 게임오버
			gameOver = true;

		addWithPos(p.y, p.x, '4'); //별다른일 없을 시 그대로 다음 위치에 4 출력
	}

	void removeSnakePiece(SnakePiece& p)
	{
		addWithPos(p.y, p.x, '0');
	}

	chtype getCharAt(int y, int x) //해당 좌표의 현재 문자 읽기
	{
		return screen.getCharAt(y, x);
	}
};

int Game::inGateLength = 0;