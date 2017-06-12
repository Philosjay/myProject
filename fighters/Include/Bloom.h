#ifndef BLOOM_H_
#define BLOOM_H_

#include "SceneNode.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

class Bloom : public SceneNode
{
public:
	Bloom(const sf::Texture& texture);
	void	update(sf::Time dt);
private:
	void	drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const ;		//!!!!!!!!!在调用 mWindow.draw(SceneNode) 时，此函数实现绘图!!!!!!!!!
	void	drawCurrent(sf::RenderTarget& target, sf::RenderStates states);


	sf::Sprite	mSprite;
	bool isActive;
	sf::Time	bloomTime;
	sf::Time	curTime;
	sf::Clock	clock;

};


#endif // !BLOOM_H_
#pragma once
