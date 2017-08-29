#ifndef WORLD_H_
#define WORLD_H_

#include "SceneNode.h"

#include <vector>

class World {
public:
	World(int type);
	
	void			update();
	void			show();
	bool			isVictory();
	SceneNode*		getPlayer();
	void			setDrawPoint(int x, int y);
	int		getBottomPosition_x();
	int		getBottomPosition_y();
	int							getMovements();
private:
	void						initialize(int type);
	void						adaptMovement();
	void						judgeStatus();
	void						pushBox(int mBox);
	void						setBoxSpeed(int mBox);
	int							isPushingBox(SceneNode * item);
	bool						isOutOfBorder(SceneNode* item);
	bool						isBoxCollided(int mBox);
	void						setVictory();

	char*						mWall;
	int							boxCount;
	int							Moves;
	struct terminalPosition
	{
		int x;
		int y;
	};
	struct Border
	{
		int Up;
		int Down;
		int Left;
		int	Right;
	};
	struct Position
	{
		int x;
		int y;
	};
	Position					mDrawPoint;
	Position					mBottomPosition;
	terminalPosition*			mTerminals;
	bool						isWin;
	SceneNode*					map;
	std::vector<SceneNode*>		box;
	SceneNode*					player;
};

#endif // !WORLD_H_
