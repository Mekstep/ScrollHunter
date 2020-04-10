#include "cmp_actor_movement.h"
#include <LevelSystem.h>
#include <engine.h>
#include <iostream>
using namespace sf;
using namespace std;

const Keyboard::Key controls[4] = {
	Keyboard::Left,
	Keyboard::Right,
	Keyboard::Up,
	Keyboard::Down
};


void ActorMovementComponent::update(double dt) {}

ActorMovementComponent::ActorMovementComponent(Entity* p)
    : _speed(100.0f), Component(p) {}

bool ActorMovementComponent::validMove(const sf::Vector2f& pos) {
  return (LevelSystem::getTileAt(pos) != LevelSystem::WALL);
  // return true;
}


void ActorMovementComponent::move(const sf::Vector2f& p) {
  auto pp = _parent->getPosition() + p;
  if (validMove(pp)) {
    _parent->setPosition(pp);
  }
}

void ActorMovementComponent::move(float x, float y) {
  move(Vector2f(x, y));
}


//PLAYER MOVEMENT COMPONENT
PlayerMovementComponent::PlayerMovementComponent(Entity* p)
	: ActorMovementComponent(p) {}

void PlayerMovementComponent::update(double dt) {
	// Move in four directions based on keys
	sf::Vector2f direction(0.0f, 0.0f);

	if (Keyboard::isKeyPressed(controls[0])) {
		direction.x = -1.0f * dt;
	}
	if (Keyboard::isKeyPressed(controls[1])) {
		direction.x = 1.0f * dt;
	}
	if (Keyboard::isKeyPressed(controls[2])) {
		direction.y = -1.0f * dt;
	}
	if (Keyboard::isKeyPressed(controls[3])) {
		direction.y = 1.0f * dt;
	}

	move(direction * _speed);
}
