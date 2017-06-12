#include "Include\Bloom.h"
#include "Include\ResourceIdentifiers.hpp"


Bloom::Bloom(const sf::Texture & texture)
	: mSprite(texture)
	, isActive(true)
	, bloomTime(sf::seconds(2))
	, curTime(sf::seconds(0))
	, clock()
{
}

void Bloom::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
//	update();

//	if (isActive)
	{
		target.draw(mSprite, states);
	}

}

void Bloom::update(sf::Time dt)
{


	curTime += clock.restart();
	if (curTime > bloomTime)
	{
		isActive = false;
	}
}
