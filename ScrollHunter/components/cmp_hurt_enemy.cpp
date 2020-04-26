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

static SoundBuffer deathBuff;
static Sound die;

static Texture tex;

int damage;

void EnemyHurtComponent::update(double dt) {

	for (auto pl : _parent->scene->ents.find("enemy"))
	{
		if (pl->getType() == "boss")
		{
			if (length(pl->getPosition() - _parent->getPosition()) < 160.0) {
				whack.stop();
				whack.play();
				pl->setHealth(pl->getHealth() - damage);

				_parent->setForDelete();

				if (pl->getHealth() <= 0)
				{
					whack.stop();
					die.stop();
					die.play();
					//drop essence
					auto e = pl->scene->makeEntity();
					e->setPosition(_parent->getPosition());
					auto sp = e->addComponent<SpriteSheetComponent>(Vector2f(24.f, 24.f));
					tex.loadFromFile("res/essenceOrbSheet.png");
					sp->setSpritesheet(tex);
					sp->setFrameCount(16);
					sp->setFrameTime(0.05f);
					auto essence = e->addComponent<EssenceComponent>();

					_parent->scene->ents.find("player")[0]->setScore(_parent->scene->ents.find("player")[0]->getScore() + 150);					

					pl->setAlive(false);

					//delete
					pl->setForDelete();

				}
			}
		}
		else
		{
			if (length(pl->getPosition() - _parent->getPosition()) < 60.0) {

				whack.stop();
				whack.play();
				pl->setHealth(pl->getHealth() - damage);

				_parent->setForDelete();

				if (pl->getHealth() <= 0)
				{
					whack.stop();
					die.stop();
					die.play();
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
					if (pl->getType() == "boss")
					{
						_parent->scene->ents.find("player")[0]->setScore(_parent->scene->ents.find("player")[0]->getScore() + 150);
					}

					pl->setAlive(false);

					//delete
					pl->setForDelete();

				}
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

		if (!hitBuff.loadFromFile("res/sounds/hit.wav"))
		{
			cout << "Couldn't load whack sound!" << endl;
		}
		whack.setBuffer(hitBuff);

		if (!deathBuff.loadFromFile("res/sounds/die.wav"))
		{
			cout << "Couldn't load death sound!" << endl;
		}
		die.setBuffer(deathBuff);
	}

