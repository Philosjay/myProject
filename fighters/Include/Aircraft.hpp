#ifndef BOOK_AIRCRAFT_HPP
#define BOOK_AIRCRAFT_HPP

#include "ResourceIdentifiers.hpp"
#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"
#include"Projectile.hpp"
#include"SoundHolder.hpp"
#include "Command.hpp"
#include"CommandQueue.hpp"


#include <SFML/Graphics/Sprite.hpp>



class Aircraft : public Entity
{
public:
	enum Type
	{
		Eagle,
		Raptor,
		Avenger,
		TypeCount
	};


public:
	Aircraft(Type type, const TextureHolder& textures, const FontHolder& fonts, SoundHolder& mSounds);

	virtual unsigned int	getCategory() const;
	virtual sf::FloatRect	getBoundingRect() const;
	virtual bool 			isMarkedForRemoval() const;
	bool					isAllied() const;
	float					getMaxSpeed() const;

	void					increaseFireRate();
	void					increaseSpread();
	void					collectMissiles(unsigned int count);

	void 					fire();
	void					launchMissile();


private:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void 			updateCurrent(sf::Time dt, CommandQueue& commands);
	void					updateMovementPattern(sf::Time dt);
	void					checkProjectileLaunch(sf::Time dt, CommandQueue& commands);

	void					createBullets(SceneNode& node, const TextureHolder& textures) const;
	void					createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;


private:
	Type					mType;
	sf::Sprite				mSprite;
	Command 				mFireCommand;
	Command					mMissileCommand;
	SoundHolder&			mSounds;

	sf::Time				mFireCountdown;
	bool 					mIsFiring;
	bool					mIsLaunchingMissile;
	bool 					mIsMarkedForRemoval;

	int						mFireRateLevel;
	int						mSpreadLevel;
	int						mMissileAmmo;

	Command 				mDropPickupCommand;
	float					mTravelledDistance;
	std::size_t				mDirectionIndex;

};

#endif // BOOK_AIRCRAFT_HPP
