#ifndef BOOK_ENTITY_HPP
#define BOOK_ENTITY_HPP

#include "SceneNode.hpp"



class Entity : public SceneNode
{
	public:
		Entity(int velocity = 0, int dtheta = 0);
		void				setVelocity(int theta, float velocity);
		void				accelerate(bool isIncrease);
		std::queue<sf::Vector2<float>>		mTrace;
		float				mVelocity;
		float				theta;
		float				dtheta;
	private:
		virtual void		updateCurrent(sf::Time dt);


	private:
		


};

#endif // BOOK_ENTITY_HPP
