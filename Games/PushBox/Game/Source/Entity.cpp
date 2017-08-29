#include "../Include/Entity.h"

Entity::Entity()
{
}

void Entity::setPosition(int x, int y)
{
	position.x = x;
	position.y = y;
}

void Entity::setSize(int x, int y)
{
	size.x = x;
	size.y = y;
}

void Entity::setSpeed(int x, int y)
{
	speed.x = x;
	speed.y = y;

}

void Entity::move()
{
	position.x += speed.x * 7;
	position.y += speed.y * 4;

}

void Entity::move(int x, int y)
{
	position.x += x;
	position.y += y;
}

void Entity::draw()
{
	//do nothing by default
}

bool Entity::isMovingUp()
{
	return speed.y == -1;
}

bool Entity::isMovingDown()
{
	return speed.y == 1;
}

bool Entity::isMovingRight()
{
	return speed.x == 1;
}

bool Entity::isMovingLeft()
{
	return speed.x == -1;
}

Position Entity::getPosition()
{
	return position;
}

Size Entity::getSize()
{
	return size;
}

