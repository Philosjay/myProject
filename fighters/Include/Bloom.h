#ifndef BLOOM_H_
#define BLOOM_H_

#include "SceneNode.hpp"
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

class Bloom : public SceneNode
{
public:
	Bloom(const sf::Texture& texture);
	void		updateBloom(sf::Time dt);
	bool		toRemove();
private:
	void						initialize();
	void						drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const ;		//!!!!!!!!!�ڵ��� mWindow.draw(SceneNode) ʱ���˺���ʵ�ֻ�ͼ!!!!!!!!!
	typedef						std::array<sf::RenderTexture, 2> RenderTextureArray;
	std::vector<sf::Texture>	bloomTextures;

	sf::Sprite	mSprite;
	sf::Texture origin;
	bool		isActive;
	sf::Time	bloomTime;
	sf::Time	curTime;
	sf::Clock	clock;
	sf::Vector2f frameSize;
	sf::Time	timePerFrame;
	sf::Time	frameShift;
	int			curFrame;
	sf::IntRect frameRect;

};


#endif // !BLOOM_H_
#pragma once
