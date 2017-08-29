#include "../Include/Snake.hpp"
#include "../Include/ResourceHolder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

Snake::Snake(int velocity, int dtheta, bool isGrowing, bool head)
	: mShape(20.f)
	, front(NULL)
	, rear(NULL)
	, next(NULL)
	, parent(NULL)
	, Entity(velocity, dtheta)
	, isHead(head)
	, isGrowing(isGrowing)
{
	sf::FloatRect bounds = mShape.getLocalBounds();
	mShape.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	if (isHead) {
		rear = this;
		mShape.setOutlineColor(sf::Color::Black);
		mShape.setRadius(15);
		mShape.setOutlineThickness(8);
	}
	else {
		mShape.setOutlineColor(sf::Color::Black);
		mShape.setOutlineThickness(3);
	}


}

Snake* Snake::add()
{
	Snake* mbody(new Snake);

		rear->next = mbody;
		rear = mbody;

	
	mbody->setPosition(getRearPosition());
	isGrowing = true;

	return mbody;
	


}

sf::Vector2<float> Snake::getRearPosition()
{
	if (!mTrace.empty()) {
		return mTrace.front();
	}
	else {
		return sf::Vector2<float>(0, 0);
	}
}

sf::Vector2<float> Snake::getFrontPosition()
{
	return mTrace.back();
}

int Snake::getCategory()
{
	if (mType == Type::Head) {
		return Category::SnakeHead;
	}
	else if (mType == Type::Body) {
		return Category::SnakeBody;
	}
}

void Snake::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mShape, states);
}

void Snake::updateCurrent(sf::Time dt)
{
	//Î²²¿²»¼ÇÂ¼¹ì¼£
	if (next != NULL) {
		mTrace.push(getPosition());
		if (mTrace.size() > 40) {
			isGrowing = false;
			mTrace.pop();
		}
		next->setPosition(getRearPosition());
	}

	move(mVelocity * cos(theta) * dt.asSeconds(), -mVelocity * sin(theta) * dt.asSeconds());
	
}
