#include "cmp_hurt_enemy.h"
#include <engine.h>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

static SoundBuffer hitBuff;
static Sound whack;

int damage;

void EnemyHurtComponent::update(double dt) {

	for (auto pl : _parent->scene->ents.find("enemy"))
	{
		if (length(pl->getPosition() - _parent->getPosition()) < 60.0) {

			pl->setHealth(pl->getHealth() - 10);

			_parent->setForDelete();

			cout << pl->getHealth() << endl;

			if (pl->getHealth() <= 0)
			{
				pl->setForDelete();
			}


		}
	}

}

	EnemyHurtComponent::EnemyHurtComponent(Entity * p, int damage) : Component(p), _enemy(_parent->scene->ents.find("enemy")[0])
	{
		if (!hitBuff.loadFromFile("res/hit.flac"))
		{
			cout << "Couldn't load whack sound!" << endl;
		}
		whack.setBuffer(hitBuff);
	}

