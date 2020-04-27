#include "cmp_aimed_bullet_physics.h"
#include "engine.h"

Vector2f bPos;
Vector2f pPos;

void AimedBulletComponent::update(double dt)
{     
    //For every bullet, fire in their given direction
    for (auto bul : _parent->scene->ents.find("bullet"))
    {
        bul->setPosition(bul->getPosition() + bul->getDirection() * (float)dt * 100.f);
    }           
}

AimedBulletComponent::AimedBulletComponent(Entity* p) : Component(p)
{
  //Player Pos
  auto pl = _parent->scene->ents.find("player");
  pPos = pl[0]->getPosition();

  //Bullet pos
  bPos = _parent->getPosition();

  //Normalize difference and set as bullet direction
  _parent->setDirection(normalize(pPos - bPos));
 
}

void AimedBulletComponent::render() {}

