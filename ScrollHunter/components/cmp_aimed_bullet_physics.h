#pragma once

#include "cmp_sprite.h"
#include "ecm.h"

class AimedBulletComponent : public Component {
protected:

public:
    AimedBulletComponent(Entity* p);
    void update(double dt);
    void render() override;
    AimedBulletComponent() = delete;
};
