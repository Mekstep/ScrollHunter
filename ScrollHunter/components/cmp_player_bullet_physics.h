#pragma once

#include "cmp_sprite.h"
#include "ecm.h"
#include <Box2D/Dynamics/b2Body.h>

class PlayerBulletPhysicsComponent : public Component 
{
public:

    PlayerBulletPhysicsComponent(Entity* p);

    void update(double dt) override;
    void render() override;
    PlayerBulletPhysicsComponent() = delete;

};
