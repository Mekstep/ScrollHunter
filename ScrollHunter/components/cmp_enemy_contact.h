#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class ContactComponent : public Component {
protected:
  std::weak_ptr<Entity> _enemy;
  int _damage;

public:
  void update(double dt) override;
  void render() override {}
  explicit ContactComponent(Entity* p, int _damage);
  ContactComponent() = delete;
};
