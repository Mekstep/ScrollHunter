#include "cmp_player_physics.h"
#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include "cmp_actor_movement.h"
#include "cmp_spritesheet.h"
#include "cmp_bullet.h"
#include "cmp_bullet_physics.h"
#include "cmp_player_bullet_physics.h"
#include <LevelSystem.h>
#include <engine.h>
#include <iostream>
#include "cmp_hurt_enemy.h"

using namespace std;
using namespace sf;
using namespace Physics;

shared_ptr<Entity> bullet;
shared_ptr<Entity> bullet2;
shared_ptr<Entity> bullet3;

//ability cooldowns start at max value so you can cast right away
float qCooldown = 5.f;
float qCooldownMax = 5.f;
float wCooldown = 30.f;
float wCooldownMax = 30.f;
float eCooldown = 30.f;
float eCooldownMax = 30.f;

Texture qAttack;

bool PlayerPhysicsComponent::isGrounded() const {
  auto touch = getTouching();
  const auto& pos = _body->GetPosition();
  const float halfPlrHeigt = _size.y * .5f;
  const float halfPlrWidth = _size.x * .52f;
  b2WorldManifold manifold;
  for (const auto& contact : touch) {
    contact->GetWorldManifold(&manifold);
    const int numPoints = contact->GetManifold()->pointCount;
    bool onTop = numPoints > 0;
    // If all contacts are below the player.
    for (int j = 0; j < numPoints; j++) {
      onTop &= (manifold.points[j].y < pos.y - halfPlrHeigt);
    }
    if (onTop) {
      return true;
    }
  }

  return false;
}

void PlayerPhysicsComponent::update(double dt) {

  const auto pos = _parent->getPosition();

  setVelocity(Vector2f(0.f, 0.f));

  if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Down)) {
	  if (Keyboard::isKeyPressed(Keyboard::Right))
	  {
		  if (getVelocity().x < _maxVelocity.x)
		  {
			  impulse({ 10.f, 0 });
		  }
	  }
	  if (Keyboard::isKeyPressed(Keyboard::Left))
	  {
		  if (getVelocity().x > -_maxVelocity.x)
		  {
			  impulse({ -10.f, 0 });
		  }
	  }
	  if (Keyboard::isKeyPressed(Keyboard::Up))
	  {
		  if (getVelocity().y > -_maxVelocity.y)
		  {
			  impulse({ 0, -10.f });
		  }
	  }
	  if (Keyboard::isKeyPressed(Keyboard::Down))
	  {
		  if (getVelocity().y < _maxVelocity.y)
		  {
			  impulse({ 0, 10.f });
		  }
	  }
  }
  else
  {
	  // Dampen X axis movement
	  dampen({ 0.9f, 1.0f });
  }


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

	  bullet = _parent->scene->makeEntity();
	  bullet->setPosition(_parent->getPosition() + Vector2f(30.f, 0.f));
	  bullet->addComponent<BulletComponent>();
	  bullet->addComponent<EnemyHurtComponent>(20);
	  auto s = bullet->addComponent<SpriteSheetComponent>(Vector2f(24.f, 24.f));
	  qAttack.loadFromFile("res/attackOrbSheet.png");
	  s->setSpritesheet(qAttack);
	  s->setFrameCount(16);
	  s->setFrameTime(0.03f);
	  auto p = bullet->addComponent<PlayerBulletPhysicsComponent>(true, Vector2f(24.f, 24.f));

	  qCooldown = 0.f;
  }



  //W Ability
  if (Keyboard::isKeyPressed(Keyboard::W) && wCooldown >= wCooldownMax)
  {
	  bullet2 = _parent->scene->makeEntity();
	  bullet2->setPosition(_parent->getPosition() + Vector2f(30.f, 0.f));
	  //bullet->addComponent<HurtComponent>();
	  bullet2->addComponent<BulletComponent>();
	  bullet2->addComponent<EnemyHurtComponent>(10);
	  auto s = bullet2->addComponent<ShapeComponent>();

	  s->setShape<sf::RectangleShape>(Vector2f(25.0f, 25.0f));
	  s->getShape().setFillColor(Color::Blue);
	  s->getShape().setOrigin(8.f, 8.f);

	  auto p = bullet2->addComponent<PlayerBulletPhysicsComponent>(true, Vector2f(50.f, 50.f));
	  wCooldown = 0.f;
  }


  //E ability
  if (Keyboard::isKeyPressed(Keyboard::E) && eCooldown >= eCooldownMax)
  {
	  bullet3 = _parent->scene->makeEntity();
	  bullet3->setPosition(_parent->getPosition() + Vector2f(30.f, 0.f));
	  bullet3->addComponent<EnemyHurtComponent>(100);
	  bullet3->addComponent<BulletComponent>();
	  auto s = bullet3->addComponent<ShapeComponent>();

	  s->setShape<sf::CircleShape>(40.f);
	  s->getShape().setFillColor(Color::Green);
	  s->getShape().setOrigin(8.f, 8.f);

	  auto p = bullet3->addComponent<PlayerBulletPhysicsComponent>(true, Vector2f(40.f, 40.f));

	  eCooldown = 0.f;
  }


  // Clamp velocity.
  auto v = getVelocity();
  v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
  v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
  setVelocity(v);

  PhysicsComponent::update(dt);
}

PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p, const Vector2f& size) : PhysicsComponent(p, true, size) 
{
  _size = sv2_to_bv2(size, true);
  _maxVelocity = Vector2f(200.f, 400.f);
  _groundspeed = 30.f;
  _grounded = false;
  _body->SetSleepingAllowed(false);
  _body->SetFixedRotation(true);
  //Bullet items have higher-res collision detection
  _body->SetBullet(true);
}
