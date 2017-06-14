#ifndef GAME_H_
#define GAME_H_

#include<SFML/Graphics.hpp>
#include"ResourceHolder.hpp"
#include"Include\Aircraft.hpp"
#include"SceneNode.hpp"
#include"World.hpp"
#include"Player.hpp"
#include"MusicPlayer.h"

class Game 
{
public:                          
					Game();        
	void			run();
	void			processInput();
private:        
	void			processEvents();             
	void			update(sf::Time deltaTime);
	void			updateStatistics(sf::Time elapsedTime);
	void			render();
private:     
	bool						isPlayerAlive;
	sf::RenderWindow			mWindow;     
	World						mWorld;
	MusicPlayer					mMusics;
	Player						mPlayer;
	sf::Font					mFont;
	sf::Text					mStatisticsText;
	sf::Time					mStatisticsUpdateTime;
	std::size_t					mStatisticsNumFrames;
	sf::Time					TimePerFrame;


	int							myScore;
	sf::Time					myTime;


};


#endif // !GAME_H_

