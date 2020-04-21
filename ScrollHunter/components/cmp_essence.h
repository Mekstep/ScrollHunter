#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class EssenceComponent : public Component {
protected:
  std::weak_ptr<Entity> _player;

public:
  void update(double dt) override;
  void render() override {}
  explicit EssenceComponent(Entity* p);
  EssenceComponent() = delete;
};
