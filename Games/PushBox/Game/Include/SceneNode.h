#ifndef MENU_H_
#define MENU_H_

#include "Entity.h"

#include <string>


class SceneNode : public Entity
{
public:

	SceneNode(const char* fileName);
	char*	getParticle();
	void	setDrawPoint(int x, int y);
	void	draw();
	void	loadImageFromFile(const char* fileName );

private:
	struct DrawPoint
	{
		int x;
		int y;
	};
	DrawPoint	mDrawPoint;
	char*		mImage;

	void		setImagePosition(int x, int y);
};

#endif // !MENU_H_

