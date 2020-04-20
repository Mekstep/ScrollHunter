#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class EnemyHurtComponent : public Component {
protected:
  std::weak_ptr<Entity> _enemy;
  int _damage;

public:
  void update(double dt) override;
  void render() override {}
  explicit EnemyHurtComponent(Entity* p, int damage);
  EnemyHurtComponent() = delete;
};
