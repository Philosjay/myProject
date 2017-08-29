#include "../Include/Entity.hpp"

#include <math.h>


Entity::Entity(int velocity, int dtheta)
	:mVelocity(velocity)
	, dtheta(dtheta)
{
}

void Entity::setVelocity(int theta,float velocity)
{
	this->theta = theta;
	mVelocity = velocity;
}

void Entity::accelerate(bool isIncrease)
{
	if (isIncrease) {
		theta += dtheta * 0.018;
	}
	else {
		theta -= dtheta * 0.018;
	}
}

void Entity::updateCurrent(sf::Time dt)
{	
	move(mVelocity * cos( theta ) * dt.asSeconds(), -mVelocity * sin( theta ) * dt.asSeconds() );
}