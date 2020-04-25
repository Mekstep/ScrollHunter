#include "cmp_enemy_turret.h"
#include "cmp_bullet.h"
#include "cmp_hurt_player.h"
#include "cmp_spritesheet.h"
#include "engine.h"
#include <SFML/Graphics/CircleShape.hpp>
#include "cmp_bullet_physics.h"
#include <LevelSystem.h>
#include "cmp_aimed_bullet_physics.h"
using namespace std;
using namespace sf;

auto walls = ls::findTiles(ls::WALL);
static Texture tex;

static int bossBullet = 1;
static int amount = 0;

void EnemyTurretComponent::update(double dt) 
{
       
            _firetime -= dt;
            if (_firetime <= 0.f) 
            {

                    if (_parent->getPosition().x > _parent->scene->ents.find("player")[0]->getPosition().x - 960 && _parent->getPosition().x < _parent->scene->ents.find("player")[0]->getPosition().x + 960)
                    {
                        fire();

                        if (_parent->getType() == "boss")
                        {
                            _firetime = 0.5f;
                        }
                        else
                        {
                            _firetime = 2.0f;
                        }
                    }                                    
            }

  static float angle = 0.f;
  angle += 1.f * dt;

  _parent->setRotation(270.f + sin(angle) * 45.f);
}

void EnemyTurretComponent::fire() const {
  auto bullet = _parent->scene->makeEntity();
  bullet->setPosition(_parent->getPosition());  
  bullet->addComponent<HurtComponent>();
  auto s = bullet->addComponent<ShapeComponent>();
  bullet->addTag("bullet");

  /*
  s->setShape<sf::CircleShape>(8.f);
  s->getShape().setFillColor(Color::Red);
  s->getShape().setOrigin(8.f, 8.f);
  */

  

  if (_parent->getType() == "archer")
  {
      bullet->addComponent<AimedBulletComponent>();
	  auto s = bullet->addComponent<SpriteSheetComponent>(Vector2f(50.f, 50.f));
	  tex.loadFromFile("res/archerAttack.png");
	  s->setSpritesheet(tex);
	  s->setFrameCount(8);
	  s->setFrameTime(0.05f);
  }
  else if(_parent->getType() == "chief")
  {
      bullet->addComponent<BulletPhysicsComponent>();
	  auto s = bullet->addComponent<SpriteSheetComponent>(Vector2f(50.f, 50.f));
	  tex.loadFromFile("res/chiefAttack.png");
	  s->setSpritesheet(tex);
	  s->setFrameCount(8);
	  s->setFrameTime(0.05f);
  }
  else if (_parent->getType() == "boss")
  {
      if (bossBullet == 1)
      {
          bullet->addComponent<BulletPhysicsComponent>();
          auto s = bullet->addComponent<SpriteSheetComponent>(Vector2f(50.f, 50.f));
          tex.loadFromFile("res/chiefAttack.png");
          s->setSpritesheet(tex);
          s->setFrameCount(8);
          s->setFrameTime(0.05f);
          amount++;
          
      }
      if (bossBullet == 2)
      {
          bullet->addComponent<AimedBulletComponent>();
          auto s = bullet->addComponent<SpriteSheetComponent>(Vector2f(50.f, 50.f));
          tex.loadFromFile("res/archerAttack.png");
          s->setSpritesheet(tex);
          s->setFrameCount(8);
          s->setFrameTime(0.05f);
          amount++;
          
      }
      if (bossBullet == 3)
      {
          bullet->addComponent<BulletPhysicsComponent>();
          auto s = bullet->addComponent<SpriteSheetComponent>(Vector2f(100.f, 100.f));
          tex.loadFromFile("res/spell3Sheet.png");
          s->setSpritesheet(tex);
          s->setFrameCount(91);
          s->setFrameTime(0.03f);
          bullet->setPosition(bullet->getPosition() + Vector2f(0,-150));
          amount++;
          
      }
      if (bossBullet == 4)
      {
          bullet->addComponent<BulletPhysicsComponent>();
          auto s = bullet->addComponent<SpriteSheetComponent>(Vector2f(50.f, 50.f));
          tex.loadFromFile("res/chiefAttack.png");
          s->setSpritesheet(tex);
          s->setFrameCount(8);
          s->setFrameTime(0.05f);
          amount++;
          
      }
      if (bossBullet == 5)
      {
          bullet->addComponent<BulletPhysicsComponent>();
          auto s = bullet->addComponent<SpriteSheetComponent>(Vector2f(100.f, 100.f));
          tex.loadFromFile("res/spell3Sheet.png");
          s->setSpritesheet(tex);
          s->setFrameCount(91);
          s->setFrameTime(0.03f);
          bullet->setPosition(bullet->getPosition() + Vector2f(0, 150));
          amount++;
          
      }

      

      if (amount == 2)
      {
          bossBullet++;
      }
      if (amount == 3)
      {
          bossBullet++;
      }
      if (amount == 4)
      {
          bossBullet++;
      }
      if (amount == 6)
      {
          bossBullet++;
      }
      if (amount == 7)
      {
          bossBullet = 1;
          amount = 0;
      }
      
      
      

      cout << "AMOUNT: " << amount << endl;
      cout << "BOSSBULLET: " << bossBullet << endl;
  }
  else
  {
	  bullet->addComponent<BulletPhysicsComponent>();
	  auto s = bullet->addComponent<SpriteSheetComponent>(Vector2f(50.f, 50.f));
	  tex.loadFromFile("res/skeletonAttack.png");
	  s->setSpritesheet(tex);
	  s->setFrameCount(8);
	  s->setFrameTime(0.05f);
  }
  
  
}

EnemyTurretComponent::EnemyTurretComponent(Entity* p)
    : Component(p), _firetime(2.f) 
{
    if (_parent->getType() == "boss")
    {
        _firetime = 0.5f;
    }
    else
    {
        _firetime = 2.0f;
    }
}
