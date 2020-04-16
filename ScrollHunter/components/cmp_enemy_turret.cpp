#include "cmp_enemy_turret.h"
#include "cmp_bullet.h"
#include "cmp_hurt_player.h"
#include "engine.h"
#include <SFML/Graphics/CircleShape.hpp>
#include "cmp_bullet_phsics.h"
#include <LevelSystem.h>
using namespace std;
using namespace sf;

auto walls = ls::findTiles(ls::WALL);

void EnemyTurretComponent::update(double dt) 
{
  _firetime -= dt;
  if (_firetime <= 0.f) {
    fire();
    _firetime = 1.f;
  }
  static float angle = 0.f;
  angle += 1.f * dt;

  _parent->setRotation(270.f + sin(angle) * 45.f);
}

void EnemyTurretComponent::fire() const {
  auto bullet = _parent->scene->makeEntity();
  bullet->setPosition(_parent->getPosition());
  bullet->addComponent<HurtComponent>();
  bullet->addComponent<BulletComponent>();
  auto s = bullet->addComponent<ShapeComponent>();

  s->setShape<sf::CircleShape>(8.f);
  s->getShape().setFillColor(Color::Red);
  s->getShape().setOrigin(8.f, 8.f);

  

  auto p = bullet->addComponent<BulletPhysicsComponent>(true, Vector2f(8.f, 8.f));
  
  
}

EnemyTurretComponent::EnemyTurretComponent(Entity* p)
    : Component(p), _firetime(2.f) {}
