#include "cmp_player_bullet_physics.h"
#include "system_physics.h"
#include <LevelSystem.h>

using namespace std;
using namespace sf;

using namespace Physics;

void PlayerBulletPhysicsComponent::update(double dt)
{
    //Fire bullets forwards
    _parent->setPosition(_parent->getPosition() + Vector2f(500*dt,0));
}

PlayerBulletPhysicsComponent::PlayerBulletPhysicsComponent(Entity* p) : Component(p)
{
}


void PlayerBulletPhysicsComponent::render() {}

