#include "..\Include\World.h"
#include <stdio.h>
#include <string>
#include <Windows.h>

World::World(int type)
	: isWin(false)
	, Moves(0)
{
	//设定图形打印锚点位置
	mDrawPoint.x = 0;
	mDrawPoint.y = 0;
	//根据type选择地图进行初始化
	initialize(type);
}

void World::update()
{
	adaptMovement();
	judgeStatus();
}

void World::show()
{
	//打印地图
	map->draw();
	//记录地图底部位置
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(hOut, &bInfo);
	mBottomPosition.x = bInfo.dwCursorPosition.X;
	mBottomPosition.y= bInfo.dwCursorPosition.Y;

	//打印箱子
	for (int i = 0; i < boxCount; i++) {
		box[i]->draw();
	}
	//打印玩家
	player->draw();
}

bool World::isVictory()
{
	return isWin;
}


SceneNode * World::getPlayer()
{
	return player;
}

void World::setDrawPoint(int x, int y)
{
	player->setDrawPoint(x, y);
	map->setDrawPoint(x, y);
	for (int i = 0; i < boxCount; i++) {
		box[i]->setDrawPoint(x, y);
	}
}

int World::getBottomPosition_x()
{
	
	return mBottomPosition.x;
}
int World::getBottomPosition_y()
{

	return mBottomPosition.y;
}

void World::initialize(int num)
{
	

	//根据关卡选择所需文件
	std::string MapFileName;
	std::string DataFileName;
	MapFileName = "Maps/Map" + std::to_string(num) + ".txt";
	DataFileName = "Data/Data" + std::to_string(num) + ".txt";

	map = new SceneNode( MapFileName.c_str() );


	//读取数据
	FILE* fp;
	int postion_x;
	int postion_y;
	fopen_s(&fp, DataFileName.c_str(), "r");
	//读取坐标时将换Data.txt内基本单位算为像素，一个基本单位为7 X 4的像素块
	//玩家
	player = new SceneNode("Images/Player.txt");
	fscanf_s(fp, "%d", &postion_x);
	fscanf_s(fp, "%d", &postion_y);
	player->setPosition(postion_x * 7, postion_y * 4);
	//箱子
	fscanf_s(fp, "%d", &boxCount);
	for (int i = 0; i < boxCount; i++) {
		box.push_back(new SceneNode("Images/Box.txt"));
	}
	for (int i = 0; i < boxCount; i++) {
		fscanf_s(fp, "%d", &postion_x);
		fscanf_s(fp, "%d", &postion_y);
		box[i]->setPosition(postion_x * 7, postion_y * 4);
	}
	//终点位置
	mTerminals = new terminalPosition[boxCount];
	for (int i = 0; i < boxCount; i++) {
		int x;
		int y;
		fscanf_s(fp, "%d", &x);
		fscanf_s(fp, "%d", &y);
		(mTerminals + i)->x = x * 7;
		(mTerminals + i)->y = y * 4;
	}
	fclose(fp);

	//获取像素信息
	mWall = map->getParticle();


}

void World::adaptMovement()
{
	if (!isOutOfBorder(player)) {
		//mBox记录可能被推动的箱子，-1为不推箱子
		int mBox = isPushingBox(player);
		if (mBox != -1) {
			setBoxSpeed(mBox);
			if (!isOutOfBorder(box[mBox])&&!isBoxCollided(mBox)) {
				//玩家和箱子都移动
				box[mBox]->move();
				player->move();

				//运动趋势清零
				player->setSpeed(0, 0);
				box[mBox]->setSpeed(0, 0);
				Moves++;
			}
		}
		else {
			//玩家移动
			player->move();
			//运动趋势清零
			player->setSpeed(0, 0);
			Moves++;
		}

		
	}


}

void World::judgeStatus()
{
	int winCount = 0;
	for (int i = 0; i < boxCount; i++) {
		for (int j = 0; j < boxCount; j++) {
			if (box[i]->getPosition().x == (mTerminals + j)->x
				&& box[i]->getPosition().y == (mTerminals + j)->y) {
				winCount++;
			}
		}
	}
	if (winCount == boxCount) {
		setVictory();
	}

}

void World::pushBox(int mBox)
{
	if (player->isMovingUp()) {
		box[mBox]->setSpeed(0, -1);
	}
	else if (player->isMovingDown()) {
		box[mBox]->setSpeed(0, 1);
	}
	else if (player->isMovingLeft()) {
		box[mBox]->setSpeed(-1, 0);
	}
	else if (player->isMovingRight()) {
		box[mBox]->setSpeed(1, 0);
	}
	box[mBox]->move();
}

void World::setBoxSpeed(int mBox)
{
	if (player->isMovingUp()) {
		box[mBox]->setSpeed(0, -1);
	}
	else if (player->isMovingDown()) {
		box[mBox]->setSpeed(0, 1);
	}
	else if (player->isMovingLeft()) {
		box[mBox]->setSpeed(-1, 0);
	}
	else if (player->isMovingRight()) {
		box[mBox]->setSpeed(1, 0);
	}
}

int World::isPushingBox(SceneNode * item)
{
	//判断是否与箱子相邻，返回箱子下标（不相邻返回-1）
	for (int i = 0; i < boxCount; i++) {
		if (item->isMovingUp()) {
			if (item->getPosition().y - 4 == box[i]->getPosition().y && item->getPosition().x == box[i]->getPosition().x) {
				return i;
			}
		}
		else if (item->isMovingDown()) {
			if (item->getPosition().y + 4 == box[i]->getPosition().y && item->getPosition().x == box[i]->getPosition().x) {
				return i;
			}
		}
		else if (item->isMovingLeft()) {
			if (item->getPosition().x - 7 == box[i]->getPosition().x && item->getPosition().y == box[i]->getPosition().y) {
				return i;
			}
		}
		else if (item->isMovingRight()) {
			if (item->getPosition().x + 7 == box[i]->getPosition().x && item->getPosition().y == box[i]->getPosition().y) {
				return i;
			}
		}
	}
	return -1;
}

bool World::isOutOfBorder(SceneNode * item)
{
	int x = item->getPosition().x;
	int y = item->getPosition().y;

	int line = y ;

	if (item->isMovingUp()) {
		if(*(mWall + (line-4) * map->getSize().x + x )!=' ')
			return true;

	}
	else if (item->isMovingDown()) {
		if (*(mWall + (line+4)* map->getSize().x + x) != ' ')
			return true;
	}
	else if (item->isMovingLeft()) {
		if (*(mWall + (line)* map->getSize().x + x-7) != ' ')
			return true;
	}
	else if (item->isMovingRight()) {
		if (*(mWall + (line)* map->getSize().x + x+7) != ' ')
			return true;
	}
	return false;
}

bool World::isBoxCollided(int mBox)
{
	//跳过mBox本身，检查剩余Box
	for (int i = 0; i < boxCount; i++) {
		if (i == mBox) {
			continue;
		}

		if (player->isMovingUp()) {
			if (box[mBox]->getPosition().y - 4 == box[i]->getPosition().y && box[mBox]->getPosition().x == box[i]->getPosition().x) {
				return true;
			}
		}
		else if (player->isMovingDown()) {
			if (box[mBox]->getPosition().y + 4 == box[i]->getPosition().y && box[mBox]->getPosition().x == box[i]->getPosition().x) {
				return true;
			}
		}
		else if (player->isMovingLeft()) {
			if (box[mBox]->getPosition().x - 7 == box[i]->getPosition().x && box[mBox]->getPosition().y == box[i]->getPosition().y) {
				return true;
			}
		}
		else if (player->isMovingRight()) {
			if (box[mBox]->getPosition().x + 7 == box[i]->getPosition().x && box[mBox]->getPosition().y == box[i]->getPosition().y) {
				return true;
			}
		}
	}

	return false;
}

void World::setVictory()
{
	isWin = true;
}

int World::getMovements()
{
	return Moves;
}
