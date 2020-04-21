#include "cmp_familiar.h"
#include "cmp_bullet.h"
#include "cmp_hurt_player.h"
#include "cmp_hurt_enemy.h"
#include "engine.h"
#include <SFML/Graphics/CircleShape.hpp>
#include "cmp_player_bullet_physics.h"
#include <LevelSystem.h>
using namespace std;
using namespace sf;

static auto walls = ls::findTiles(ls::WALL);

void FamiliarComponent::update(double dt) 
{
  _firetime -= dt;
  if (_firetime <= 0.f) {
    fire();
    _firetime = 1.f;
  }

}

void FamiliarComponent::fire() const {
  auto bullet = _parent->scene->makeEntity();
  bullet->setPosition(_parent->getPosition());
  bullet->addComponent<EnemyHurtComponent>(10);
  auto s = bullet->addComponent<ShapeComponent>();

  s->setShape<sf::CircleShape>(8.f);
  s->getShape().setFillColor(Color::Blue);
  s->getShape().setOrigin(8.f, 8.f);

  

  auto p = bullet->addComponent<PlayerBulletPhysicsComponent>(true, Vector2f(8.f, 8.f));
  
  
}

FamiliarComponent::FamiliarComponent(Entity* p)
    : Component(p), _firetime(2.f) {}
