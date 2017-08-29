#include "../Include/World.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <time.h>

World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures() 
, mSceneGraph(), 
 mSceneLayers()
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 480.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mScrollSpeed(-50.f)
, mPlayer(nullptr)
, testCircle(20.f)
{
	loadTextures();
	buildScene();


	// Prepare the view
	mWorldView.setCenter(mSpawnPosition);

	time_t mtime = time(NULL);
	srand(int(mtime));
}

void World::update(sf::Time dt)
{
	static int count = 0;
	if (count < 2) {
		feedSnake();
		count++;
	}

	// Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);

	mSceneGraph.update(dt);
	handleCollision();
}

void World::draw()
{
	
	mWindow.setView(mWorldView);

	mWindow.draw(mSceneGraph);
}

Snake* World::getPlayer()
{
	return mPlayer;
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

void World::feedSnake()
{
	std::unique_ptr<Snake> body(mPlayer->add());
	mSceneLayers[Air]->attachChild(std::move(body)); 
}

void World::loadTextures()
{
	mTextures.load(Textures::Desert, "Media/Textures/Desert.png");
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		SceneNode::Ptr layer(new SceneNode());
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
	std::unique_ptr<Snake> leader(new Snake(60,2,true,true));
	mPlayer = leader.get();
	mPlayer->setPosition(0,50);
	mSceneLayers[Air]->attachChild(std::move(leader));

	std::unique_ptr<Food> food(new Food);
	mFood = food.get();
	mFood->setPosition(100, 100);
	mSceneLayers[Air]->attachChild(std::move(food));



}

void World::handleCollision()
{


	float x_food = mFood->getPosition().x;
	float y_food = mFood->getPosition().y;

	float x_snake = mPlayer->getPosition().x;
	float y_snake = mPlayer->getPosition().y;

	if (pow(pow(x_food - x_snake, 2) + pow(y_food - y_snake, 2), 0.5) < 30) {
		mFood->setPosition(rand()%600, rand()%400);
		feedSnake();
	}
}
