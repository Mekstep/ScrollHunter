#include "cmp_bullet_physics.h"
#include "engine.h"


void BulletPhysicsComponent::update(double dt) 
{       
    //Fire bullet in straight line towards left
    _parent->setPosition(_parent->getPosition() + Vector2f(-500 * dt, 0));      
}

BulletPhysicsComponent::BulletPhysicsComponent(Entity* p) : Component(p)
{
}

void BulletPhysicsComponent::render() {}
