#pragma once

#include "cmp_sprite.h"
#include "ecm.h"
#include <Box2D/Dynamics/b2Body.h>

class BulletPhysicsComponent : public Component {
protected:
  b2Body* _body;
  const bool _dynamic;
  b2Fixture* _fixture;

public:
  BulletPhysicsComponent(Entity* p, bool dyn, const sf::Vector2f& size);

  b2Fixture* const getFixture() const;
  bool isTouching(const BulletPhysicsComponent& pc) const;
  bool isTouching(const BulletPhysicsComponent& pc, b2Contact const* bc) const;
  std::vector<const b2Contact*> getTouching() const;
  const sf::Vector2f getVelocity() const;
  void update(double dt) override;
  void render() override;
  void setVelocity(const sf::Vector2f& v);
  ~BulletPhysicsComponent() override;
};
