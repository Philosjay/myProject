#include "../Include/Game.h"

#include <Windows.h>
#include <conio.h>

Game::Game()
{
	//��겻�ɼ�
	setCursorVisible(false);
	//���ڴ�С
	system("mode con cols=100 lines=50");
	
	//ê��λ��
	mDrawPoint.x = 16;
	mDrawPoint.y = 3;

	//�˵���ʼ��
	mMenu = new SceneNode("Images/Menu.txt");
	mReport = new SceneNode("Images/Report.txt");

	mReport->setDrawPoint(0 + mDrawPoint.x, 10 + mDrawPoint.y);
	mMenu->setDrawPoint(mDrawPoint.x, mDrawPoint.y);

	//��ȡ���е�ͼ����
	FILE* fp;
	fopen_s(&fp, "Data/Game.txt", "r");
	fscanf_s(fp, "%d", &mMapCount);
	fclose(fp);
}

void Game::run()
{

	while (mStatus != Status::quit)
	{
		//����ͬ��Ϸ״̬���棺�˵�����ʼ���÷�
		HandleStatus();	
		//�����������
		HandlePlayerInput();
		//����World����
		if (mStatus == Status::start) {
			mWorld->update();
			mWorld->show();
			showTip();
		}
		
	}

}

void Game::Initialize()
{
	mStatus = Status::menu;
}

void Game::HandlePlayerInput()
{
	//switch�⽫������ʼ��
	char input = ' ';
	int num = -1;


	//�ڲ�ͬ��Ϸ״̬�£�������Ĵ���ʽ��ͬ
	switch (mStatus)
	{
		//���˵�
	case Status::menu:
		//��ͼѡ��
		num = chooseMap();
		//ȷ��������������
		if (num <= mMapCount && num > -1) {
			system("cls");
			mWorld = new World(num);
			//����ê��
			mWorld->setDrawPoint(mDrawPoint.x, mDrawPoint.y);
			mPlayer = mWorld->getPlayer();
			mStatus = Status::start;
			t1 = clock();
		}
		break;
		//��Ϸ��
	case Status::start:
		setCursorVisible(false);
		//��ȡ��ʱ����
		input = _getch();
		switch (input)
		{
			//�趨�˶����ƣ��Ƿ��ƶ�����World�ж�
		case 'w':
			mPlayer->setSpeed(0, -1);
			break;
		case 's':
			mPlayer->setSpeed(0, 1);
			break;
		case 'a':
			mPlayer->setSpeed(-1, 0);
			break;
		case 'd':
			mPlayer->setSpeed(1, 0);
			break;
		case 'm':
			restart();
			break;
		}
		break;
	case Status::report:
		while (1)
		{
			input = _getch();
			if (input == ' ') {
				break;
			}
		}
		restart();
		break;
	}
}

void Game::HandleStatus()
{
	switch (mStatus)
	{
	case Status::menu:
		mMenu->draw();
		break;
	case Status::start:
		//����ʤ��
		if (mWorld->isVictory())
		{
			t2 = clock();
			mStatus = Status::report;
			mReport->draw();
			float time = (t2 - t1) ;
			//��ӡ������Ϣ
			setCursorPosition(10 + mDrawPoint.x, 24 + mDrawPoint.y);
			printf("Time Cost : %3.2fs", time / CLOCKS_PER_SEC);
			setCursorPosition(40 + mDrawPoint.x, 24 + mDrawPoint.y);
			printf("Movements : %d ", mWorld->getMovements());
		}
		break;
	case Status::report:
		break;
	}
}

void Game::setCursorPosition(int x, int y)
{
	HANDLE hout;
	COORD coord;
	coord.X = x;
	coord.Y = y;
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hout, coord);
}

void Game::restart()
{
	mStatus = Status::menu;
	system("cls");
	t1 = t2 = 0;
	delete mWorld;
}

void Game::setCursorVisible(bool isVisible)
{
	//���ù���Ƿ�ɼ�
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = isVisible;
	SetConsoleCursorInfo(hOut, &cci);
}

int Game::chooseMap()
{
	setCursorPosition(19 + mDrawPoint.x, 16 + mDrawPoint.y);
	printf("%d ~ %d", 1, mMapCount);
	setCursorPosition(45 + mDrawPoint.x, 16 + mDrawPoint.y);
	setCursorVisible(true);

	int num;
	scanf_s("%d", &num);
	return num;
}

void Game::showTip()
{
	setCursorPosition(mWorld->getBottomPosition_x(), mWorld->getBottomPosition_y()+1);
	printf("Press M to Main Menu.");
}

