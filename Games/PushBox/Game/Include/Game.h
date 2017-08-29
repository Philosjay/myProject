#ifndef GAME_H_
#define GAME_H_

#include "World.h"
#include "SceneNode.h"

#include <time.h>

class Game {

public:
	Game();

	void			run();
private:
	enum Status
	{
		menu,
		start,
		report,
		quit,
	};
	struct DrawPoint
	{
		int x;
		int y;
	};
	DrawPoint		mDrawPoint;
	clock_t			t1;
	clock_t			t2;
	void			Initialize();
	void			HandlePlayerInput();
	void			HandleStatus();
	void			setCursorPosition(int x, int y);
	void			restart();
	void			setCursorVisible(bool isVisible);
	int				chooseMap();
	void			showTip();

	SceneNode*		mMenu;
	SceneNode*		mReport;
	SceneNode*		mPlayer;
	World*			mWorld;
	int				mStatus;
	int				mMapCount;
};

#endif // !GAME_H_

