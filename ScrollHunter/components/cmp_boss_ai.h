#pragma once

#include "cmp_actor_movement.h"

class BossAIComponent : public ActorMovementComponent {
protected:
  sf::Vector2f _direction;

public:
  void update(double dt) override;

  explicit BossAIComponent(Entity* p);

  BossAIComponent() = delete;
};
