#include "..\Include\Food.h"

Food::Food()
	: Entity()
	, mShape(15.f)
{
	mShape.setFillColor(sf::Color::Red);
	sf::FloatRect bounds = mShape.getLocalBounds();
	mShape.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Food::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mShape, states);
}