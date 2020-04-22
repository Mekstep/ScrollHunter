#include "cmp_enemy_ai.h"

using namespace std;
using namespace sf;

void EnemyAIComponent::update(double dt) 
{
  if (!validMove(_parent->getPosition() + Vector2f(0,80)) || !validMove(_parent->getPosition() - Vector2f(0, 60))) {
    _direction *= -1.f;
  }

  move(_direction * (float)(dt * _speed));
  ActorMovementComponent::update(dt);
}

EnemyAIComponent::EnemyAIComponent(Entity* p) : ActorMovementComponent(p) {
  _direction = Vector2f(0, 1.0f);
  _speed = 100.0f;
}
