#ifndef BOOK_WORLD_HPP
#define BOOK_WORLD_HPP

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "Aircraft.hpp"
#include "Command.hpp"
#include "CommandQueue.hpp"
#include"SoundHolder.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>

// Forward declaration
namespace sf
{
	class RenderWindow;
}

class World : private sf::NonCopyable
{
public:
	explicit							World(sf::RenderWindow& window);
	void								update(sf::Time dt);
	void								draw();

	CommandQueue&						getCommandQueue();

	bool 								hasAlivePlayer() const;
	bool 								hasPlayerReachedEnd() const;


private:
	void								loadTextures();
	void								adaptPlayerPosition();
	void								handleCollisions();
	void								randomEvents(sf::Time dt);
	void								randomEnemys(sf::Time dt);

	void								buildScene();
	void								addEnemies();
	void								addEnemy(Aircraft::Type type, float relX, float relY);
	void								spawnEnemies();
	void								destroyEntitiesOutsideView();
	void								guideMissiles();
	sf::FloatRect						getViewBounds() const;
	sf::FloatRect						getBattlefieldBounds() const;


private:
	enum Layer
	{
		Background,
		Air,
		LayerCount
	};

	struct SpawnPoint
	{
		SpawnPoint(Aircraft::Type type, float x, float y)
			: type(type)
			, x(x)
			, y(y)
		{
		}

		Aircraft::Type type;
		float x;
		float y;
	};

	struct RandomEvents
	{
		RandomEvents(sf::Time RandomEnemyInterval, sf::Time RandomPickupInterval, sf::Time RandomEventsCountdown)
			: RandomEnemyInterval(RandomEnemyInterval)
			, RandomPickupInterval(RandomPickupInterval)
			, RandomEventsCountdown(RandomEventsCountdown)
		{
		}

		sf::Time RandomEnemyInterval;
		sf::Time RandomPickupInterval;

		sf::Time RandomEventsCountdown;
	};


private:
	sf::RenderWindow&					mWindow;
	sf::View							mWorldView;
	sf::Vector2f						mViewCenter;
	TextureHolder						mTextures;
	FontHolder&							mFonts;
	SoundHolder							mSounds;

	SceneNode							mSceneGraph;
	std::array<SceneNode*, LayerCount>	mSceneLayers;
	CommandQueue						mCommandQueue;

	sf::FloatRect						mWorldBounds;
	sf::Vector2f						mSpawnPosition;
	float								mScrollSpeed;
	Aircraft*							mPlayerAircraft;
	RandomEvents						mRandomEvents;


	std::vector<SpawnPoint>				mEnemySpawnPoints;
	std::vector<Aircraft*>				mActiveEnemies;
};
#endif // BOOK_WORLD_HPP
