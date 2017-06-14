#ifndef BOOK_AIRCRAFT_HPP
#define BOOK_AIRCRAFT_HPP

#include "ResourceIdentifiers.hpp"
#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"
#include"Projectile.hpp"
#include"SoundHolder.hpp"
#include "Command.hpp"
#include"CommandQueue.hpp"

#include<SFML/Graphics.hpp>



class Aircraft : public Entity
{
public:
	enum Type
	{
		Eagle,
		Raptor,
		Avenger,

		RaptorTroopA,
		RaptorTroopB,
		TypeCount,
	};


public:
	Aircraft(sf::RenderWindow& window, Type type, const TextureHolder& textures, const FontHolder& fonts, SoundHolder& mSounds,bool isPlayer);
	virtual unsigned int	getCategory() const;
	Aircraft::Type			getType() const;
	virtual sf::FloatRect	getBoundingRect() const;
	virtual bool 			isMarkedForRemoval() const;
	bool					isAllied() const;
	float					getMaxSpeed() const;

	void					increaseFireRate();
	void					increaseSpread();
	void					GetMissileORUpgradeFire();
	void					GetHpOrGetAlly();
	void					collectMissiles(unsigned int count);

	void 					fire();
	void					launchMissile();

	void					openEMenu();
	void					openQMenu();

	void					closeMenu();
	void					addPoints(int points);


private:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void 			updateCurrent(sf::Time dt, CommandQueue& commands);
	void					updateMovementPattern(sf::Time dt);
	void					checkProjectileLaunch(sf::Time dt, CommandQueue& commands);

	void					createBullets(SceneNode& node, const TextureHolder& textures) const;
	void					createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;
	void					setAllyVelocity(float x, float y);

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
	bool					isPlayerAircraft;

	int						mFireRateLevel;
	int						mSpreadLevel;
	int						mMissileAmmo;
	int						mPoints;

	Command 				mDropPickupCommand;
	float					mTravelledDistance;
	std::size_t				mDirectionIndex;
	bool					isQMEnuOpened;
	bool					isEMenuOpened;

	sf::Font				mFont;
	sf::Text				mPointsText;
	sf::RenderWindow&       mWindow;

	sf::Sprite				EMenu;
	sf::Sprite				QMenu;
	sf::Texture				EMenuTexture;
	sf::Texture				QMenuTexture;

};

#endif // BOOK_AIRCRAFT_HPP
