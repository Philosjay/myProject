#ifndef BOOK_SCENENODE_HPP
#define BOOK_SCENENODE_HPP

#include "Category.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <SFML/Graphics/Transformable.hpp>

#include <queue>
#include <memory>

struct Command;
class CommandQueue;

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
	public:
		typedef std::unique_ptr<SceneNode> Ptr;


	public:
								SceneNode(Category::Type mCategory = Category::None);

		void					attachChild(Ptr child);
		Ptr						detachChild(const SceneNode& node);
		void					onCommand(const Command& command, sf::Time dt);		//执行对应指令
		void					update(sf::Time dt);

		virtual int				getCategory();
		sf::Vector2f			getWorldPosition() const;
		sf::Transform			getWorldTransform() const;


	private:
		virtual void			updateCurrent(sf::Time dt);
		void					updateChildren(sf::Time dt);

		virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const;
		virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
		void					drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		std::vector<Ptr>		mChildren;
		SceneNode*				mParent;
		Category::Type			mDefaultCategory;
};

#endif // BOOK_SCENENODE_HPP
