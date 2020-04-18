#include "cmp_actor_movement.h"
#include "cmp_spritesheet.h"
#include "cmp_bullet.h"
#include "cmp_bullet_physics.h"
#include "cmp_player_bullet_physics.h"
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

Texture qAttack;
std::shared_ptr<Entity> forceShield;
bool shieldOn = false;

//ability cooldowns start at max value so you can cast right away
float qCooldown = 50.f;
float qCooldownMax = 50.f;
float wCooldown = 300.f;
float wCooldownMax = 300.f;
float eCooldown = 300.f;
float eCooldownMax = 300.f;



void ActorMovementComponent::update(double dt) {}

ActorMovementComponent::ActorMovementComponent(Entity* p)
    : _speed(500.0f), Component(p) {}

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
	

	//Movement
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


	if (Keyboard::isKeyPressed(Keyboard::Num1))
	{
		auto shield = _parent->scene->makeEntity();
		auto s = shield->addComponent<ShapeComponent>();
		s->setShape<sf::CircleShape>(40.f);
		s->getShape().setFillColor(Color::Blue);
		s->getShape().setOrigin(40.f, 40.f);
		forceShield = shield;
		shieldOn == true;
	}

	if (shieldOn == true)
	{
		forceShield->setPosition(_parent->getPosition());
	}


	move(direction * _speed);


	//ABILITIES
	//Q ability cooldown
	if (qCooldown < qCooldownMax)
	{
		qCooldown += 0.05f;
	}

	//W ability cooldown
	if (wCooldown < wCooldownMax)
	{
		wCooldown += 0.05f;
	}

	//E ability cooldown
	if (eCooldown < eCooldownMax)
	{
		eCooldown += 0.05f;
	}


	//Q Ability
	if (Keyboard::isKeyPressed(Keyboard::Q) && qCooldown >= qCooldownMax)
	{
		/*
		auto anim = _parent->addComponent<SpriteSheetComponent>(Vector2f(240.f, 162.f));
		mage2.loadFromFile("res/attackSheet.png");
		anim->setSpritesheet(mage2);
		anim->setFrameCount(17);
		anim->setFrameTime(0.1f);
		*/
		
		auto bullet = _parent->scene->makeEntity();
		bullet->setPosition(_parent->getPosition());
		//bullet->addComponent<HurtComponent>();
		bullet->addComponent<BulletComponent>();
		auto s = bullet->addComponent<SpriteSheetComponent>(Vector2f(24.f, 24.f));
		qAttack.loadFromFile("res/attackOrbSheet.png");
		s->setSpritesheet(qAttack);
		s->setFrameCount(16);
		s->setFrameTime(0.03f);
		auto p = bullet->addComponent<PlayerBulletPhysicsComponent>(true, Vector2f(24.f, 24.f));

		/*
		auto p = bullet->addComponent<PhysicsComponent>(true, Vector2f(24.f, 24.f));
		p->setRestitution(.4f);
		p->setFriction(.005f);  
		p->impulse(sf::rotate(Vector2f(60.f, 20.f), -_parent->getRotation()));		
		*/

		qCooldown = 0.f;
	}

	//W Ability
	if (Keyboard::isKeyPressed(Keyboard::W) && wCooldown >= wCooldownMax)
	{
		auto bullet = _parent->scene->makeEntity();
		bullet->setPosition(_parent->getPosition());
		//bullet->addComponent<HurtComponent>();
		bullet->addComponent<BulletComponent>();
		auto s = bullet->addComponent<ShapeComponent>();

		s->setShape<sf::RectangleShape>(Vector2f(50.0f, 50.0f));
		s->getShape().setFillColor(Color::Blue);
		s->getShape().setOrigin(8.f, 8.f);

		auto p = bullet->addComponent<PlayerBulletPhysicsComponent>(true, Vector2f(50.f, 50.f));

		/*
		auto p = bullet->addComponent<PhysicsComponent>(true, Vector2f(30.f, 30.f));
		p->setRestitution(.4f);
		p->setFriction(.005f);
		p->impulse(sf::rotate(Vector2f(20.f, 20.f), -_parent->getRotation()));
		*/

		//wCooldown = 0.f;
	}


	//E ability
	if (Keyboard::isKeyPressed(Keyboard::E) && eCooldown >= eCooldownMax)
	{
		auto bullet = _parent->scene->makeEntity();
		bullet->setPosition(_parent->getPosition());
		//bullet->addComponent<HurtComponent>();
		bullet->addComponent<BulletComponent>();
		auto s = bullet->addComponent<ShapeComponent>();

		s->setShape<sf::CircleShape>(40.f);
		s->getShape().setFillColor(Color::Green);
		s->getShape().setOrigin(8.f, 8.f);

		auto p = bullet->addComponent<PlayerBulletPhysicsComponent>(true, Vector2f(40.f, 40.f));

		/*
		auto p = bullet->addComponent<PhysicsComponent>(true, Vector2f(40.f, 40.f));
		p->setRestitution(.4f);
		p->setFriction(.005f);
		p->impulse(sf::rotate(Vector2f(60.f, 20.f), -_parent->getRotation()));
		*/

		eCooldown = 0.f;
	}
}
