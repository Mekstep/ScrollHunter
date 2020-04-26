#include "cmp_familiar.h"
#include "cmp_bullet.h"
#include "cmp_hurt_player.h"
#include "cmp_hurt_enemy.h"
#include "cmp_spritesheet.h"
#include "engine.h"
#include <SFML/Graphics/CircleShape.hpp>
#include "cmp_player_bullet_physics.h"
#include <LevelSystem.h>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

static auto walls = ls::findTiles(ls::WALL);

static Texture tex;

//sounds
static Sound cast;
static SoundBuffer castB;

void FamiliarComponent::update(double dt) 
{
  _firetime -= dt;
  if (_firetime <= 0.f) {
    fire();
    _firetime = 0.5f;
  }

}

void FamiliarComponent::fire() const {

	cast.stop();
	cast.play();
	auto bullet = _parent->scene->makeEntity();
	bullet->setPosition(_parent->getPosition());
	bullet->addComponent<EnemyHurtComponent>(10);
	auto s = bullet->addComponent<SpriteSheetComponent>(Vector2f(55.f, 55.f));
	tex.loadFromFile("res/attackOrbSheet.png");
	s->setSpritesheet(tex);
	s->setFrameCount(8);
	s->setFrameTime(0.05f);



	auto p = bullet->addComponent<PlayerBulletPhysicsComponent>(true, Vector2f(30.f, 30.f));
  
  
}

FamiliarComponent::FamiliarComponent(Entity* p)
    : Component(p), _firetime(0.5f) {
	castB.loadFromFile("res/sounds/shootPlayer.wav");
	cast.setBuffer(castB);
}
