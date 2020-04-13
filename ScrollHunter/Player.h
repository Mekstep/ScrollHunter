#pragma once
#include "engine.h"

class Player{
public:
	static std::shared_ptr<Entity> makePlayer(Scene* scene, const sf::Vector2f& pos);
	Player() = delete;
};