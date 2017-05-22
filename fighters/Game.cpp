#include"Game.h"
#include<SFML/Graphics/Text.hpp>
#include<SFML/Graphics/Font.hpp>



Game::Game() : mWindow(sf::VideoMode(640, 480), "F I G H T E R S")
{
	
	textures.load(Textures::Plane, "Media/Textures/Eagle.png");
	mPlayer.setTexture(textures.get(Textures::Plane));
	textures.load(Textures::Backgroud, "Media/Textures/Desert.png");
	mBackgroud.setTexture(textures.get(Textures::Backgroud));

	mPlayer.setPosition(200.f, 200.f);
 
	mIsMovingDown = mIsMovingUp = mIsMovingLeft = mIsMovingRight = false;
	playerSpeed = 100.f;
	timePerFrame = sf::seconds(1.f / 60.f);




}
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceUpdate = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		processEvents();
		timeSinceUpdate += clock.restart();
		while (timeSinceUpdate > timePerFrame)
		{
			timeSinceUpdate -= timePerFrame;
			update(timePerFrame);
		}		
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			mWindow.close();
			break;
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);                
			break;
		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;
		}
	}
}
void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::W)        
		mIsMovingUp = isPressed;    
	else if (key == sf::Keyboard::S)        
		mIsMovingDown = isPressed;    
	else if (key == sf::Keyboard::A)        
		mIsMovingLeft = isPressed;    
	else if (key == sf::Keyboard::D)        
		mIsMovingRight = isPressed;
}

void Game::update(sf::Time deltaTime)
{
	sf::Vector2f movement(0.f, 0.f);    
	if (mIsMovingUp)        
		movement.y -= playerSpeed;    
	if (mIsMovingDown)       
		movement.y += playerSpeed;
	if (mIsMovingLeft)        
		movement.x -= playerSpeed;
	if (mIsMovingRight)       
		movement.x += playerSpeed;
	mPlayer.move(deltaTime.asSeconds() * movement);
}
void Game::render()
{
	mWindow.clear();
	mWindow.draw(mBackgroud);
	mWindow.draw(mPlayer);

	mWindow.display();
}