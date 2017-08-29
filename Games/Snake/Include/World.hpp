#ifndef BOOK_WORLD_HPP
#define BOOK_WORLD_HPP

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "Snake.hpp"
#include "Food.h"
#include "CommandQueue.h"

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
		Snake*								getPlayer();
		CommandQueue&						getCommandQueue();
		void								feedSnake();

	private:
		void								loadTextures();
		void								buildScene();
		void								handleCollision();


	private:
		enum Layer
		{
			Background,
			Air,
			LayerCount
		};


	private:
		sf::RenderWindow&					mWindow;
		sf::View							mWorldView;
		TextureHolder						mTextures;

		SceneNode							mSceneGraph;
		CommandQueue						mCommandQueue;
		std::array<SceneNode*, LayerCount>	mSceneLayers;

		sf::FloatRect						mWorldBounds;
		sf::Vector2f						mSpawnPosition;
		float								mScrollSpeed;
		Snake*								mPlayer;
		Food*								mFood;
		sf::CircleShape						testCircle;
};

#endif // BOOK_WORLD_HPP
