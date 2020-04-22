#include "cmp_boss_ai.h"

using namespace std;
using namespace sf;

void BossAIComponent::update(double dt) {
  auto mov = _direction * (float)(dt * _speed);
  mov.y += _direction.y * 200.f;
  if (!validMove(_parent->getPosition() + mov)) {
    _direction *= -1.f;
  }

  move(_direction * (float)(dt * _speed));
  ActorMovementComponent::update(dt);
}

BossAIComponent::BossAIComponent(Entity* p) : ActorMovementComponent(p) {
  _direction = Vector2f(0, 1.0f);
  _speed = 100.0f;
}
