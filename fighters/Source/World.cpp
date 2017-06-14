#include "Include\World.hpp"
#include"Include\Projectile.hpp"
#include "Include\Foreach.hpp"
#include <SFML/Graphics/RenderWindow.hpp>


#include <algorithm>
#include <cmath>
#include <limits>

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2);

World::World(sf::RenderWindow& window, int* mScore, sf::Time* mTime, bool& isPlayerAlive)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mFonts(mFonts)
	, mSounds()
	, mTextures()
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 8000.f)
	, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
	, mViewCenter(mSpawnPosition)
	, mScrollSpeed(-50.f)
	, mPlayerAircraft(nullptr)
	, mEnemySpawnPoints()
	, mAllies()
	, mActiveEnemies()
	, mRandomEvents(sf::seconds(5), sf::seconds(10), sf::seconds(0))
	, mScore(mScore)
	, mTime(mTime)
	, mPlayer(mPlayerAircraft)
	, mBloomNode()
	, mBloom(NULL)
	, isPlayerAlive(isPlayerAlive)
{
	loadTextures();
	buildScene();

	// Prepare the view
	mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time dt)
{

//	mPlayerAircraft->resetMenu();

	// Scroll the world, reset player velocity
	mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
	mViewCenter.y -= mScrollSpeed * dt.asSeconds();
	randomEvents(dt);

	mPlayerAircraft->setVelocity(0.f, 0.f);

	

	// Setup commands to destroy entities, and guide missiles
	destroyEntitiesOutsideView();
	guideMissiles();

	// Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);

	//adjust the velocity of  diagonal movements 
	sf::Vector2f velocity = mPlayerAircraft->getVelocity();
	if (velocity.x != 0.f && velocity.y != 0.f)        
		mPlayerAircraft->setVelocity(velocity / std::sqrt(2.f));
	mPlayerAircraft->accelerate(0.f, mScrollSpeed);

	// Collision detection and response (may destroy entities)
	handleCollisions();

	// Remove all destroyed entities, create new ones
	mSceneGraph.removeWrecks();
	spawnEnemies();

	// Regular update step, adapt position (correct if outside view)
	mSceneGraph.update(dt, mCommandQueue);	//此函数实现飞机的位移，调用了entity的  virtual void		updateCurrent(sf::Time dt, CommandQueue& commands);

	adaptPlayerPosition();
	mSounds.emptyQueue();


	mBloomNode.update(dt);
//	mBloomNode.removeBlooms();



	
}

void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);

	mWindow.draw(mBloomNode);

}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

bool World::hasAlivePlayer() const
{
	return !mPlayerAircraft->isMarkedForRemoval();
}

bool World::hasPlayerReachedEnd() const
{
	return !mWorldBounds.contains(mPlayerAircraft->getPosition());
}

void World::loadTextures()
{
	mTextures.load(Textures::Eagle, "Media/Textures/Eagle.png");
	mTextures.load(Textures::Raptor, "Media/Textures/Raptor.png");
	mTextures.load(Textures::Avenger, "Media/Textures/Avenger.png");
	mTextures.load(Textures::Desert, "Media/Textures/Desert.png");

	mTextures.load(Textures::Bullet, "Media/Textures/Bullet.png");
	mTextures.load(Textures::Missile, "Media/Textures/Missile.png");

	mTextures.load(Textures::HealthRefill, "Media/Textures/HealthRefill.png");
	mTextures.load(Textures::MissileRefill, "Media/Textures/MissileRefill.png");
	mTextures.load(Textures::FireSpread, "Media/Textures/FireSpread.png");
	mTextures.load(Textures::FireRate, "Media/Textures/FireRate.png");

	mTextures.load(Textures::Explosion, "Media/Textures/Explosion.png");
	bm.setTexture(mTextures.get(Textures::Explosion));
}

void World::adaptPlayerPosition()
{
	// Keep player's position inside the screen bounds, at least borderDistance units from the border
	sf::FloatRect viewBounds = getViewBounds();
	const float borderDistance = 40.f;

	sf::Vector2f position = mPlayerAircraft->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	mPlayerAircraft->setPosition(position);
}



bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
	unsigned int category1 = colliders.first->getCategory();
	unsigned int category2 = colliders.second->getCategory();

	// Make sure first pair entry has category type1 and second has type2
	if (type1 & category1 && type2 & category2)
	{
		return true;
	}
	else if (type1 & category2 && type2 & category1)
	{
		std::swap(colliders.first, colliders.second);
		return true;
	}
	else
	{
		return false;
	}
}


void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		Category::Type category = (i == Air) ? Category::SceneAirLayer : Category::None;

		SceneNode::Ptr layer(new SceneNode(category));
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	// Prepare the tiled background
	sf::Texture& texture = mTextures.get(Textures::Desert);
	sf::IntRect textureRect(mWorldBounds);
	texture.setRepeated(true);

	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	// Add player's aircraft
	std::unique_ptr<Aircraft> player(new Aircraft(mWindow,Aircraft::Eagle, mTextures, mFonts,mSounds,true));
	mPlayerAircraft = player.get();
	mPlayerAircraft->setPosition(mSpawnPosition);
	mSceneLayers[Air]->attachChild(std::move(player));

	// Add enemy aircraft
	addEnemies();

}

void World::addEnemies()
{
	// Add enemies to the spawn point container
	addEnemy(Aircraft::Raptor, 0.f, 500.f);
	addEnemy(Aircraft::Raptor, 0.f, 700.f);
	addEnemy(Aircraft::Raptor, 0.f, 700.f);
	addEnemy(Aircraft::Raptor, 0.f, 1000.f);
//	addEnemy(Aircraft::Raptor, +100.f, 1500.f);
//	addEnemy(Aircraft::Raptor, -100.f, 1500.f);
//	addEnemy(Aircraft::Avenger, -70.f, 1800.f);
//	addEnemy(Aircraft::Avenger, -70.f, 2000.f);
//	addEnemy(Aircraft::Avenger, 70.f, 1400.f);
//	addEnemy(Aircraft::Avenger, 70.f, 1600.f);

	addTroopA(2000);
	addTroopB(2000);

	addEnemy(Aircraft::Raptor, +300.f, 2000.f);
	addEnemy(Aircraft::Raptor, -300.f, 2000.f);
	addEnemy(Aircraft::Raptor, 0.f, 2100.f);

	addEnemy(Aircraft::Raptor, +200.f, 2300.f);
	addEnemy(Aircraft::Raptor, -200.f, 2300.f);
	addEnemy(Aircraft::Raptor, 0.f, 2500.f);
	addEnemy(Aircraft::Raptor, +400.f, 2700.f);
	addEnemy(Aircraft::Raptor, -400.f, 2700.f);
	addEnemy(Aircraft::Raptor, 0.f, 2900.f);

	addEnemy(Aircraft::Avenger, +70.f, 3500.f);
	addEnemy(Aircraft::Avenger, -70.f, 3500.f);
	addEnemy(Aircraft::Avenger, +140.f, 3600.f);
	addEnemy(Aircraft::Avenger, -140.f, 3600.f);

	// Sort all enemies according to their y value, such that lower enemies are checked first for spawning
	std::sort(mEnemySpawnPoints.begin(), mEnemySpawnPoints.end(), [](SpawnPoint lhs, SpawnPoint rhs)
	{
		return lhs.y < rhs.y;
	});
}

void World::addTroopA()
{
	for (int i = 0; i < 6; i++)
	{
		addEnemy(Aircraft::RaptorTroopA,100+i*30, mViewCenter.y + 500+i*20);
	}
}

void World::addTroopB()
{
	for (int i = 0; i < 6; i++)
	{
		addEnemy(Aircraft::RaptorTroopB, -100 - i * 30, mViewCenter.y + 500 + i * 20);
	}
}

void World::addTroopA(float y)
{
	for (int i = 0; i < 6; i++)
	{
		addEnemy(Aircraft::RaptorTroopA, 100 + i * 30, y + i * 20);
	}
}

void World::addTroopB(float y)
{
	for (int i = 0; i < 6; i++)
	{
		addEnemy(Aircraft::RaptorTroopB, -100 - i * 30, y + i * 20);
	}
}

void World::addEnemy(Aircraft::Type type, float relX, float relY)
{
	SpawnPoint spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y - relY);
	mEnemySpawnPoints.push_back(spawn);
}

void World::spawnEnemies()
{
	// Spawn all enemies entering the view area (including distance) this frame
	while (!mEnemySpawnPoints.empty()
		&& mEnemySpawnPoints.back().y > getBattlefieldBounds().top)
	{
		SpawnPoint spawn = mEnemySpawnPoints.back();

		std::unique_ptr<Aircraft> enemy(new Aircraft(mWindow,spawn.type, mTextures, mFonts,mSounds,false));
		enemy->setPosition(spawn.x, spawn.y);
		enemy->setRotation(180.f);

		mSceneLayers[Air]->attachChild(std::move(enemy));

		// Enemy is spawned, remove from the list to spawn
		mEnemySpawnPoints.pop_back();
	}
}

void World::destroyEntitiesOutsideView()
{
	Command command;
	command.category = Category::Projectile | Category::EnemyAircraft;
	command.action = derivedAction<Entity>([this](Entity& e, sf::Time)
	{
		if (!getBattlefieldBounds().intersects(e.getBoundingRect()))
			e.destroy();
	});

	mCommandQueue.push(command);
}

void World::guideMissiles()
{
	// Setup command that stores all enemies in mActiveEnemies
	Command enemyCollector;
	enemyCollector.category = Category::EnemyAircraft;
	enemyCollector.action = derivedAction<Aircraft>([this](Aircraft& enemy, sf::Time)
	{
		if (!enemy.isDestroyed())
			mActiveEnemies.push_back(&enemy);
	});

	// Setup command that guides all missiles to the enemy which is currently closest to the player
	Command missileGuider;
	missileGuider.category = Category::AlliedProjectile;
	missileGuider.action = derivedAction<Projectile>([this](Projectile& missile, sf::Time)
	{
		// Ignore unguided bullets
		if (!missile.isGuided())
			return;

		float minDistance = std::numeric_limits<float>::max();
		Aircraft* closestEnemy = nullptr;

		// Find closest enemy
		FOREACH(Aircraft* enemy, mActiveEnemies)
		{
			float enemyDistance = distance(missile, *enemy);

			if (enemyDistance < minDistance)
			{
				closestEnemy = enemy;
				minDistance = enemyDistance;
			}
		}

		if (closestEnemy)
			missile.guideTowards(closestEnemy->getWorldPosition());
	});

	// Push commands, reset active enemies
	mCommandQueue.push(enemyCollector);
	mCommandQueue.push(missileGuider);
	mActiveEnemies.clear();
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
	// Return view bounds + some area at top, where enemies spawn
	sf::FloatRect bounds = getViewBounds();
	bounds.top -= 100.f;
	bounds.height += 100.f;

	return bounds;
}

void World::handleCollisions()
{
	std::set<SceneNode::Pair> collisionPairs;
	mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

	FOREACH(SceneNode::Pair pair, collisionPairs)		//飞机碰撞
	{
		if (matchesCategories(pair, Category::PlayerAircraft, Category::EnemyAircraft))
		{
			auto& player = static_cast<Aircraft&>(*pair.first);
			auto& enemy = static_cast<Aircraft&>(*pair.second);

			// Collision: Player damage = enemy's remaining HP
			player.damage(enemy.getHitpoints());
			enemy.destroy();

			updateScore(enemy);

			if (player.getHitpoints() <= 0)
			{
				addBloom(player.getPosition().x, player.getPosition().y);
			}
			
		}
/*
		else if (matchesCategories(pair, Category::PlayerAircraft, Category::Pickup))
		{
			auto& player = static_cast<Aircraft&>(*pair.first);
			auto& pickup = static_cast<Pickup&>(*pair.second);

			// Apply pickup effect to player, destroy projectile
			pickup.apply(player);
			pickup.destroy();
		}
*/
		else if (matchesCategories(pair, Category::EnemyAircraft, Category::AlliedProjectile)
			|| matchesCategories(pair, Category::PlayerAircraft, Category::EnemyProjectile))
		{
			auto& aircraft = static_cast<Aircraft&>(*pair.first);
			auto& projectile = static_cast<Projectile&>(*pair.second);

			// Apply projectile damage to aircraft, destroy projectile
			aircraft.damage(projectile.getDamage());
			projectile.destroy();
			mSounds.play(SoundEffect::Explosion1);
			

			updateScore(aircraft);
		}
	}
	if (mPlayerAircraft->getHitpoints() <= 0 && isPlayerAlive)
	{
		isPlayerAlive = false;
		addBloom(mPlayerAircraft->getPosition().x, mPlayerAircraft->getPosition().y);
	}
}

void World::randomEvents(sf::Time dt)
{
	randomEnemys(dt);
}

void World::randomEnemys(sf::Time dt)
{
	mRandomEvents.RandomEventsCountdown += dt;
	if (mRandomEvents.RandomEventsCountdown > mRandomEvents.RandomEnemyInterval)
	{
		if (rand() % 3 == 1)
		{
			addEnemy(Aircraft::Avenger, rand() % 600 - 300, mViewCenter.y + 500);
		}
		else
		{
			addEnemy(Aircraft::Raptor, rand() % 600 - 300, mViewCenter.y + 500);
		}
		

		mRandomEvents.RandomEventsCountdown -= mRandomEvents.RandomEnemyInterval;
	}
}

void World::updateScore(Aircraft& mAircraft)
{
	if (!mAircraft.isAllied() && mAircraft.isDestroyed())
	{
		switch (mAircraft.getType())
		{
		case Aircraft::Type::Raptor:
			*mScore += 50;
			mPlayerAircraft->addPoints(1);
			break;
		case Aircraft::Type::Avenger:
			*mScore += 100;
			mPlayerAircraft->addPoints(4);
			break;
		case Aircraft::Type::Eagle:
			*mScore += 150;
			mPlayerAircraft->addPoints(7);
			break;
		}
		addBloom(mAircraft.getPosition().x, mAircraft.getPosition().y);
	}

}

void World::addAlly()
{
/*
	static int Allies = 0;

	if (*mTime > sf::seconds(5)&&Allies<1)
	{
		std::unique_ptr<Aircraft> t;
		std::unique_ptr<Aircraft> Ally(new Aircraft(Aircraft::Type::Eagle, mTextures, mFonts, mSounds));
		Ally->setPosition(-50, -50);
		mPlayerAircraft->attachChild(std::move(Ally));
		t.reset();
//		mAllies[Allies].reset(t);
		Allies++;

	}
*/

	
		
}

void World :: addBloom(float x,float y)
{
	sf::Texture& texture = mTextures.get(Textures::Explosion);
	std::unique_ptr<Bloom> mbloom(new Bloom(texture));
	mbloom->setPosition(x, y);

/*
	sf::Texture& texture = mTextures.get(Textures::Explosion);
	std::unique_ptr<SpriteNode> mbloom(new SpriteNode(texture));
	mbloom->setPosition(x,y);
*/

	mBloomNode.attachChild(std::move(mbloom));



}