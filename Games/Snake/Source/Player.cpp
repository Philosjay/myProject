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
	//�������Ͷ������ƹ���
	mKeyBinding[sf::Keyboard::A] = Action::TurnLeft;
	mKeyBinding[sf::Keyboard::D] = Action::TurnRight;

	//���������ƺ�ʵ��ָ�����
	mActionBinding[TurnLeft].action = derivedAction<Snake>(SnakeMover(true));
	mActionBinding[TurnRight].action = derivedAction<Snake>(SnakeMover(false));

	//����������Ķ�������ΪSnakeHead
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
