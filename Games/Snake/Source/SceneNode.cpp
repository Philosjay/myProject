#include "../Include/SceneNode.hpp"
#include "../Include/Foreach.hpp"
#include "../Include/Command.h"

#include <algorithm>
#include <cassert>


SceneNode::SceneNode(Category::Type mCategory)
: mChildren()
, mParent(nullptr)
, mDefaultCategory(mCategory)
{
}

void SceneNode::attachChild(Ptr child)
{
	child->mParent = this;
	mChildren.push_back(std::move(child));
}


SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
	auto found = std::find_if(mChildren.begin(), mChildren.end(), [&] (Ptr& p) { return p.get() == &node; });
	assert(found != mChildren.end());

	Ptr result = std::move(*found);
	result->mParent = nullptr;
	mChildren.erase(found);
	return result;
}

void SceneNode::update(sf::Time dt)
{
	updateCurrent(dt);
	updateChildren(dt);
}

void SceneNode::updateCurrent(sf::Time)
{
	// Do nothing by default
}

void SceneNode::updateChildren(sf::Time dt)
{
	FOREACH(Ptr& child, mChildren)
		child->update(dt);
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Apply transform of current node
	states.transform *= getTransform();

	// Draw node and children with changed transform
	drawCurrent(target, states);
	drawChildren(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const
{
	// Do nothing by default
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	FOREACH(const Ptr& child, mChildren)
		child->draw(target, states);
}

int SceneNode::getCategory()
{
	return mDefaultCategory;
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;

	for (const SceneNode* node = this; node != nullptr; node = node->mParent)
		transform = node->getTransform() * transform;

	return transform;
}
void SceneNode::onCommand(const Command& command, sf::Time dt)
{
	// Command current node, if category matches
	//&��ʾת���ɶ����������룬��Category�еĶ�����ö�ٶ�Ӧ����֤Command������ֻ��Ӧһ�ֶ���
	if (command.category & getCategory())
		command.action(*this,dt);

	// Command children
	FOREACH(Ptr& child, mChildren)
		child->onCommand(command, dt);
}