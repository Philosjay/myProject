#include "..\Include\Player.h"
#include "../Include/CommandQueue.h"

struct SnakeMover
{
	SnakeMover(bool isIncrease)
		: direction(isIncrease)
	{
	};

	void operator() (Snake& aircraft, sf::Time) const
	{
		aircraft.accelerate(direction);
	}
	bool		direction;

};

Player::Player()
{
	//将按键和动作名称关联
	mKeyBinding[sf::Keyboard::A] = Action::TurnLeft;
	mKeyBinding[sf::Keyboard::D] = Action::TurnRight;

	//将动作名称和实际指令关联
	mActionBinding[TurnLeft].action = derivedAction<Snake>(SnakeMover(true));
	mActionBinding[TurnRight].action = derivedAction<Snake>(SnakeMover(false));

	//将接受命令的对象设置为SnakeHead
	FOREACH(auto& pair, mActionBinding)
		pair.second.category = Category::SnakeHead;
}


void Player::handleRealTimeInput(CommandQueue& commands)
{
	// Traverse all assigned keys and check if they are pressed
	FOREACH(auto pair, mKeyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first))
			commands.push(mActionBinding[pair.second]);
	}
}
