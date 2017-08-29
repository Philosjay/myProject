#include "../Include/Game.h"

#include <Windows.h>
#include <conio.h>

Game::Game()
{
	//光标不可见
	setCursorVisible(false);
	//窗口大小
	system("mode con cols=100 lines=50");
	
	//锚点位置
	mDrawPoint.x = 16;
	mDrawPoint.y = 3;

	//菜单初始化
	mMenu = new SceneNode("Images/Menu.txt");
	mReport = new SceneNode("Images/Report.txt");

	mReport->setDrawPoint(0 + mDrawPoint.x, 10 + mDrawPoint.y);
	mMenu->setDrawPoint(mDrawPoint.x, mDrawPoint.y);

	//读取现有地图数量
	FILE* fp;
	fopen_s(&fp, "Data/Game.txt", "r");
	fscanf_s(fp, "%d", &mMapCount);
	fclose(fp);
}

void Game::run()
{

	while (mStatus != Status::quit)
	{
		//处理不同游戏状态界面：菜单，开始，得分
		HandleStatus();	
		//处理玩家输入
		HandlePlayerInput();
		//更新World数据
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
	//switch外将变量初始化
	char input = ' ';
	int num = -1;


	//在不同游戏状态下，对输入的处理方式不同
	switch (mStatus)
	{
		//主菜单
	case Status::menu:
		//地图选择
		num = chooseMap();
		//确保输入在区间内
		if (num <= mMapCount && num > -1) {
			system("cls");
			mWorld = new World(num);
			//设置锚点
			mWorld->setDrawPoint(mDrawPoint.x, mDrawPoint.y);
			mPlayer = mWorld->getPlayer();
			mStatus = Status::start;
			t1 = clock();
		}
		break;
		//游戏中
	case Status::start:
		setCursorVisible(false);
		//获取即时输入
		input = _getch();
		switch (input)
		{
			//设定运动趋势，是否移动交给World判断
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
		//处理胜利
		if (mWorld->isVictory())
		{
			t2 = clock();
			mStatus = Status::report;
			mReport->draw();
			float time = (t2 - t1) ;
			//打印报告信息
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
	//设置光标是否可见
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

