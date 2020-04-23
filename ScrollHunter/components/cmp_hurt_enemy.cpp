#include "cmp_hurt_enemy.h"
#include "cmp_essence.h"
#include "cmp_spritesheet.h"
#include "LevelSystem.h"
#include <engine.h>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

static SoundBuffer hitBuff;
static Sound whack;

static Texture tex;

int damage;

void EnemyHurtComponent::update(double dt) {

	for (auto pl : _parent->scene->ents.find("enemy"))
	{
		if (length(pl->getPosition() - _parent->getPosition()) < 60.0) {

			pl->setHealth(pl->getHealth() - damage);

			_parent->setForDelete();

			if (pl->getHealth() <= 0)
			{
				//drop essence
				auto e = pl->scene->makeEntity();
				e->setPosition(_parent->getPosition());
				auto sp = e->addComponent<SpriteSheetComponent>(Vector2f(24.f, 24.f));
				tex.loadFromFile("res/essenceOrbSheet.png");
				sp->setSpritesheet(tex);
				sp->setFrameCount(16);
				sp->setFrameTime(0.05f);
				auto essence = e->addComponent<EssenceComponent>();

				if (pl->getType() == "skeleton")
				{
					_parent->scene->ents.find("player")[0]->setScore(_parent->scene->ents.find("player")[0]->getScore() + 10);
				}
				if (pl->getType() == "archer")
				{
					_parent->scene->ents.find("player")[0]->setScore(_parent->scene->ents.find("player")[0]->getScore() + 20);
				}
				if (pl->getType() == "chief")
				{
					_parent->scene->ents.find("player")[0]->setScore(_parent->scene->ents.find("player")[0]->getScore() + 30);
				}

				

				//delete
				pl->setForDelete();

			}
		}
		if (ls::getTileAt(_parent->getPosition()) == ls::WALL)
		{
			_parent->setForDelete();
		}
	}

}

	EnemyHurtComponent::EnemyHurtComponent(Entity * p, int _damage) : Component(p), _enemy(_parent->scene->ents.find("enemy")[0])
	{
		damage = _damage;

		if (!hitBuff.loadFromFile("res/hit.flac"))
		{
			cout << "Couldn't load whack sound!" << endl;
		}
		whack.setBuffer(hitBuff);
	}

