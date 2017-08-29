#ifndef PLAYER_H_
#define PLAYER_H_

#include "Command.h"
#include "Snake.hpp"
#include "Foreach.hpp"

#include <SFML/Window/Event.hpp>
#include <map>

class CommandQueue;

class Player {
public:
	enum Action
	{
		TurnLeft,
		TurnRight,
	};
				Player();
	void		handleRealTimeInput(CommandQueue& commands);

private:

	bool									playerAlive;
	std::map<sf::Keyboard::Key, Action>		mKeyBinding;
	std::map<Action, Command>				mActionBinding;
};


#endif // PLAYER_H_

