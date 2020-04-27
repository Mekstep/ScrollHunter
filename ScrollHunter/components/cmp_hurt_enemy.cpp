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

//Damage enemies by damage set
void EnemyHurtComponent::update(double dt) {

	for (auto en : _parent->scene->ents.find("enemy"))
	{
		if (en->getType() == "boss")
		{
			//If bullet hits boss
			if (length(en->getPosition() - _parent->getPosition()) < 160.0) {
				whack.stop();
				whack.play();

				//Remove health by damage amount
				en->setHealth(en->getHealth() - damage);

				//Delete bullet
				_parent->setForDelete();

				//If boss health is below 0, delete and drop essence
				if (en->getHealth() <= 0)
				{
					whack.stop();
					die.stop();
					die.play();
					//drop essence
					auto e = en->scene->makeEntity();
					e->setPosition(_parent->getPosition());
					auto sp = e->addComponent<SpriteSheetComponent>(Vector2f(24.f, 24.f));
					tex.loadFromFile("res/essenceOrbSheet.png");
					sp->setSpritesheet(tex);
					sp->setFrameCount(16);
					sp->setFrameTime(0.05f);
					auto essence = e->addComponent<EssenceComponent>();

					//Add boss score to player
					_parent->scene->ents.find("player")[0]->setScore(_parent->scene->ents.find("player")[0]->getScore() + 150);					

					en->setAlive(false);

					//delete
					en->setForDelete();

				}
			}
		}
		else
		{
			//If bullet hits normal enemy
			if (length(en->getPosition() - _parent->getPosition()) < 60.0) {

				whack.stop();
				whack.play();

				//Remove enemy health by damage amount
				en->setHealth(en->getHealth() - damage);

				//Delete bullet
				_parent->setForDelete();

				//If enemy health is below 0, drop essence and add score
				if (en->getHealth() <= 0)
				{
					whack.stop();
					die.stop();
					die.play();
					//drop essence
					auto e = en->scene->makeEntity();
					e->setPosition(_parent->getPosition());
					auto sp = e->addComponent<SpriteSheetComponent>(Vector2f(24.f, 24.f));
					tex.loadFromFile("res/essenceOrbSheet.png");
					sp->setSpritesheet(tex);
					sp->setFrameCount(16);
					sp->setFrameTime(0.05f);
					auto essence = e->addComponent<EssenceComponent>();

					//Add to players score by enemy type
					if (en->getType() == "skeleton")
					{
						_parent->scene->ents.find("player")[0]->setScore(_parent->scene->ents.find("player")[0]->getScore() + 10);
					}
					if (en->getType() == "archer")
					{
						_parent->scene->ents.find("player")[0]->setScore(_parent->scene->ents.find("player")[0]->getScore() + 20);
					}
					if (en->getType() == "chief")
					{
						_parent->scene->ents.find("player")[0]->setScore(_parent->scene->ents.find("player")[0]->getScore() + 30);
					}
					if (en->getType() == "boss")
					{
						_parent->scene->ents.find("player")[0]->setScore(_parent->scene->ents.find("player")[0]->getScore() + 150);
					}

					en->setAlive(false);

					//delete
					en->setForDelete();

				}
			}
		}

		//Delete bullets as they hit walls
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

