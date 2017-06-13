#include "Include\Bloom.h"
#include "Include\ResourceIdentifiers.hpp"


Bloom::Bloom(const sf::Texture & texture)
	: mSprite(texture)
	, origin(texture)
	, isActive(true)
	, bloomTime(sf::seconds(10))
	, curTime(sf::seconds(0))
	, clock()
	, frameSize(texture.getSize().x/4, texture.getSize().y/4)
	, timePerFrame(sf::seconds(1.f / 20.f))
	, curFrame(1)
	, frameShift(sf::seconds(0))
	, frameRect(0, 0, frameSize.x, frameSize.y)
{
	
}

void Bloom::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isActive)
	{
		target.draw(mSprite, states);
	}
}

bool Bloom::toRemove()
{
	return !isActive;
}

void Bloom::updateBloom(sf::Time dt)
{
	curTime += dt;
	frameShift += dt;

	if (frameShift > timePerFrame)
	{
		curFrame++;
		frameShift -= timePerFrame;

		if (curFrame / 4 == 1)		//到下一行
		{
			frameRect.left = 0;
			frameRect.top += frameRect.height;
		}
		else
		{
			frameRect.left += frameSize.x;
		}
	}

	mSprite.setTextureRect(frameRect);
	mSprite.setPosition(-100, -100);

	if (curTime > bloomTime)
	{
		isActive = false;
	}
}

