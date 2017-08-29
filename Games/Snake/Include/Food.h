#ifndef FOOD_H_
#define FOOD_H_

#include "../Include/Entity.hpp"

#include <SFML/Graphics.hpp>

class Food : public Entity
{
public:
	Food();
private:
	virtual void		drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::CircleShape mShape;
};

#endif // !FOOD_H_
#pragma once
