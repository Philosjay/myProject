#include "Include/Aircraft.hpp"
#include "Include/ResourceHolder.hpp"
#include "Include\Datatables.hpp"
#include"Include\utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace
{
	const std::vector<AircraftData> Table = initializeAircraftData();
}

Aircraft::Aircraft(sf::RenderWindow& window, Type type, const TextureHolder& textures, const FontHolder& fonts,SoundHolder& mSounds, bool isPlayer)
	: Entity(Table[type].hitpoints)
	, mType(type)
	, mSprite(textures.get(Table[type].texture))
	, mSounds(mSounds)
	, mFireCommand()
	, mMissileCommand()
	, mFireCountdown(sf::Time::Zero)
	, mIsFiring(false)
	, mIsLaunchingMissile(false)
	, mIsMarkedForRemoval(false)
	, mFireRateLevel(1)
	, mSpreadLevel(1)
	, mMissileAmmo(2)
	, mDropPickupCommand()
	, mTravelledDistance(0.f)
	, mDirectionIndex(0)
	, mPoints(100)
	, isQMEnuOpened(false)
	, isEMenuOpened(false)
	, mWindow(window)
	, isPlayerAircraft(isPlayer)
{
	centerOrigin(mSprite);

	mFireCommand.category = Category::SceneAirLayer;
	mFireCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createBullets(node, textures);
	};

	mMissileCommand.category = Category::SceneAirLayer;
	mMissileCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createProjectile(node, Projectile::Missile, 0.f, 0.5f, textures);
	};
	if (isPlayerAircraft)
	{
		mFont.loadFromFile("Media/Sansation.ttf");
		mPointsText.setFont(mFont);
		mPointsText.setCharacterSize(15);

		EMenuTexture.loadFromFile("Media/Textures/EMenu.png");
		EMenu.setTexture(EMenuTexture);

		QMenuTexture.loadFromFile("Media/Textures/QMenu.png");
		QMenu.setTexture(QMenuTexture);
	}

}

void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isPlayerAircraft&&(isEMenuOpened||isQMEnuOpened))
	{
		if(isEMenuOpened)mWindow.draw(EMenu);
		else mWindow.draw(QMenu);
		
		mWindow.draw(mPointsText);
	}

	target.draw(mSprite, states);
}

void Aircraft::updateCurrent(sf::Time dt, CommandQueue& commands)
{

	// Entity has been destroyed: Possibly drop pickup, mark for removal
	if (isDestroyed())
	{
//		checkPickupDrop(commands);

		mIsMarkedForRemoval = true;
		return;
	}

	// Check if bullets or missiles are fired
	checkProjectileLaunch(dt, commands);

	// Update enemy movement pattern; apply velocity
	updateMovementPattern(dt);
	Entity::updateCurrent(dt, commands);

	// Update texts
//	updateTexts();
	if (isPlayerAircraft)
	{
		sf::Color color;
		mPointsText.setPosition(getPosition().x-60, getPosition().y+60);
		mPointsText.setString("HP:"+toString(getHitpoints())+ "      Points:" + toString(mPoints)+"\n"+"        Missiles:"+toString(mMissileAmmo));
		
		
		mPointsText.setFillColor(color.Red);

		EMenu.setPosition(getPosition().x+10 , getPosition().y-100);
		QMenu.setPosition(getPosition().x-200, getPosition().y - 100);
	}

}

unsigned int Aircraft::getCategory() const
{
	if (isAllied())
		return Category::PlayerAircraft;
	else
		return Category::EnemyAircraft;
}

Aircraft::Type Aircraft::getType() const
{
	switch (mType)
	{
	case Type::Raptor:
		return Type::Raptor;
	case Type::Avenger:
		return Type::Avenger;
	case Type::Eagle:
		return Type::Eagle;
	}
}

sf::FloatRect Aircraft::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Aircraft::isMarkedForRemoval() const
{
	return mIsMarkedForRemoval;
}

bool Aircraft::isAllied() const
{
	return mType == Eagle;
}

float Aircraft::getMaxSpeed() const
{
	return Table[mType].speed;
}

void Aircraft::increaseFireRate()
{
	if (mPoints - 7 >= 0)
	{
		if (mFireRateLevel < 10)
			++mFireRateLevel;
		mPoints -= 7;
	}

}

void Aircraft::increaseSpread()
{
	if (mSpreadLevel < 3)
		++mSpreadLevel;
}

void Aircraft::collectMissiles(unsigned int count)
{
	mMissileAmmo += count;
}

void Aircraft::fire()
{
	// Only ships with fire interval != 0 are able to fire
	if (Table[mType].fireInterval != sf::Time::Zero)
		mIsFiring = true;
}

void Aircraft::launchMissile()
{
	if (mMissileAmmo > 0)
	{
		mIsLaunchingMissile = true;
		--mMissileAmmo;
	}
}

void Aircraft::updateMovementPattern(sf::Time dt)
{
	// Enemy airplane: Movement pattern
	const std::vector<Direction>& directions = Table[mType].directions;
	if (!directions.empty())
	{
		// Moved long enough in current direction: Change direction
		if (mTravelledDistance > directions[mDirectionIndex].distance)
		{
			mDirectionIndex = (mDirectionIndex + 1) % directions.size();
			mTravelledDistance = 0.f;
		}

		// Compute velocity from direction
		float radians = toRadian(directions[mDirectionIndex].angle + 90.f);
		float vx = getMaxSpeed() * std::cos(radians);
		float vy = getMaxSpeed() * std::sin(radians);

		setVelocity(vx, vy);

		mTravelledDistance += getMaxSpeed() * dt.asSeconds();
	}
}
/*
void Aircraft::checkPickupDrop(CommandQueue& commands)
{
if (!isAllied() && randomInt(3) == 0)
commands.push(mDropPickupCommand);
}
*/
void Aircraft::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	// Enemies try to fire all the time
	if (!isAllied())
	{
		fire();
	}
		

	// Check for automatic gunfire, allow only in intervals
	if (mIsFiring && mFireCountdown <= sf::Time::Zero)
	{
		// Interval expired: We can fire a new bullet
		commands.push(mFireCommand);
		mFireCountdown += Table[mType].fireInterval / (mFireRateLevel + 1.f);
		mIsFiring = false;
		if (!isAllied())
		{
			mSounds.play(SoundEffect::EnemyGunfire);
		}
		else
		{
			mSounds.play(SoundEffect::AlliedGunfire);
		}
		
	}
	else if (mFireCountdown > sf::Time::Zero)
	{
		// Interval not expired: Decrease it further
		mFireCountdown -= dt;
		mIsFiring = false;
	}

	// Check for missile launch
	if (mIsLaunchingMissile)
	{
		commands.push(mMissileCommand);
		mIsLaunchingMissile = false;
		mSounds.play(SoundEffect::LaunchMissile);

	}
}

void Aircraft::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	Projectile::Type type = isAllied() ? Projectile::AlliedBullet : Projectile::EnemyBullet;

	switch (mSpreadLevel)
	{
	case 1:
		createProjectile(node, type, 0.0f, 0.5f, textures);
		break;

	case 2:
		createProjectile(node, type, -0.33f, 0.33f, textures);
		createProjectile(node, type, +0.33f, 0.33f, textures);
		break;

	case 3:
		createProjectile(node, type, -0.5f, 0.33f, textures);
		createProjectile(node, type, 0.0f, 0.5f, textures);
		createProjectile(node, type, +0.5f, 0.33f, textures);
		break;
	}
}

void Aircraft::createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

	sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
	sf::Vector2f velocity(0, projectile->getMaxSpeed());

	float sign = isAllied() ? -1.f : +1.f;
	projectile->setPosition(getWorldPosition() + offset * sign);
	projectile->setVelocity(velocity * sign);
	node.attachChild(std::move(projectile));
}

void Aircraft::setAllyVelocity(float x,float y)
{
//	this->mch
}

void Aircraft::GetMissileORUpgradeFire()
{
	if (isEMenuOpened)
	{
		if (mPoints - 2 >= 0)
		{
			mPoints -= 2;
			mMissileAmmo++;
			mSounds.play(SoundEffect::Purchase);
		}
	}
	else if (isQMEnuOpened)
	{
		if (mPoints - 15 >= 0)
		{
			mPoints -= 15;
			increaseFireRate();
			mSounds.play(SoundEffect::Upgrade);
		}
	}
}

void Aircraft::GetHpOrGetAlly()
{
	if (isEMenuOpened)
	{
		if (mPoints - 5 >= 0)
		{
			repair(20);
			mPoints -= 5;
			mSounds.play(SoundEffect::Purchase);
		}
	}
	else if (isQMEnuOpened)
	{
		if (mPoints - 15 >= 0)
		{
			mPoints -= 10;
			increaseSpread();
			mSounds.play(SoundEffect::Upgrade);
		}
	}
}

void Aircraft::openEMenu()
{
	if(!isQMEnuOpened)  isEMenuOpened = true;
}

void Aircraft::openQMenu()
{
	if(!isEMenuOpened)	isQMEnuOpened = true;
}

void Aircraft::closeMenu()
{
	isQMEnuOpened = false;
	isEMenuOpened = false;
}

void Aircraft::addPoints(int points)
{
	mPoints += points;
}
