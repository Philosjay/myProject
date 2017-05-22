#ifndef GAME_H_
#define GAME_H_

#include<SFML/Graphics.hpp>
#include"ResourceHolder.hpp"

namespace Textures
{
	enum ID { Plane, Missile, Backgroud };
}

class Game 
{
public:                          
					Game();        
	void			run();
private:        
	void			processEvents();             
	void			update(sf::Time deltaTime);
	void			render();
	void			handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
private:        
	ResourceHolder<sf::Texture, 
				   Textures::ID> textures;
	sf::RenderWindow			 mWindow;       
	sf::Texture				     mTexture;
	sf::Sprite					 mPlayer;
	sf::Sprite					 mBackgroud;
	float			  playerSpeed;
	bool			  mIsMovingUp;
	bool			  mIsMovingDown;
	bool			  mIsMovingLeft;
	bool			  mIsMovingRight;
	sf::Time		  timePerFrame;

};


#endif // !GAME_H_

