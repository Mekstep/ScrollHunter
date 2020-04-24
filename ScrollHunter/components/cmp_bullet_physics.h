#pragma once

#include "cmp_sprite.h"
#include "ecm.h"

class BulletPhysicsComponent : public Component {
public:
  BulletPhysicsComponent(Entity* p);

  void update(double dt) override;
  void render() override;
  BulletPhysicsComponent() = delete;
};
