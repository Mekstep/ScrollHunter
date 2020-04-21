#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class FamiliarComponent : public Component {
protected:
  void fire() const;
  float _firetime;

public:
  void update(double dt) override;
  void render() override {}
  explicit FamiliarComponent(Entity* p);
  FamiliarComponent() = delete;
};
