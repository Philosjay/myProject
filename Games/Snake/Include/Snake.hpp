#ifndef BOOK_AIRCRAFT_HPP
#define BOOK_AIRCRAFT_HPP

#include "Entity.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics.hpp>


class Snake : public Entity
{

	public:
							Snake(int velocity = 0,int dtheta = 0, bool isGrowing = true,bool head = false);

							Snake*				add();
							sf::Vector2<float>	getRearPosition();
							sf::Vector2<float>	getFrontPosition();
							virtual int			getCategory();
	private:
		virtual void		drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void		updateCurrent(sf::Time dt);


	private:
		enum Type
		{
			Head,
			Body,
		};

		Type				mType;
		sf::Sprite			mSprite;
		sf::CircleShape		mShape;
		int					nodeCount;
		float				nodeDistance;
		bool				isHead;
		bool				isGrowing;
		Snake*				front;
		Snake*				rear;
		Snake*				next;
		Snake*				parent;
};

#endif // BOOK_AIRCRAFT_HPP
