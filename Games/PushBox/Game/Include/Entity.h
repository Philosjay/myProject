#ifndef ENTITY_H_
#define ENTUTY_H_


typedef struct
{
	int x;
	int y;
} Position;
typedef struct
{
	int x;
	int y;
} Speed;
typedef struct
{
	int x;
	int y;
} Size;

class Entity
{
private:
	Position	position;
	Size		size;
	Speed		speed;
public:
	Entity();
	void			move();
	void			move(int x, int y);
	void			setPosition(int x, int y);
	void			setSize(int x, int y);
	void			setSpeed(int x, int y);
	virtual void	draw();
	bool			isMovingUp();
	bool			isMovingDown();
	bool			isMovingRight();
	bool			isMovingLeft();
	Position		getPosition();
	Size			getSize();
};

#endif // !ENTITY_H_

