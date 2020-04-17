#pragma once
#include "engine.h"

class Enemies{
public:
	static std::shared_ptr<Entity> makeSkeleton(Scene* scene, const sf::Vector2f& pos);
	static std::shared_ptr<Entity> makeSkeletonArcher(Scene* scene, const sf::Vector2f& pos);
	static std::shared_ptr<Entity> makeSkeletonChief(Scene* scene, const sf::Vector2f& pos);
	Enemies() = delete;
};