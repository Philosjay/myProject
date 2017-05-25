#ifndef GAME_H_
#define GAME_H_

#include<SFML/Graphics.hpp>
#include"ResourceHolder.hpp"
#include"Include\Aircraft.hpp"
#include"SceneNode.hpp"
#include"World.hpp"
#include"Player.hpp"

class Game 
{
public:                          
					Game();        
	void			run();
	void			processInput();
private:        
	void			processEvents();             
	void			update(sf::Time deltaTime);
	void					updateStatistics(sf::Time elapsedTime);
	void			render();
private:     
	
	sf::RenderWindow			 mWindow;     
	World						 mWorld;
	Player					mPlayer;
	sf::Font				mFont;
	sf::Text				mStatisticsText;
	sf::Time				mStatisticsUpdateTime;
	std::size_t				mStatisticsNumFrames;
	sf::Time		  TimePerFrame;
	

	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingLeft;
	bool mIsMovingRight;
};


#endif // !GAME_H_

