#include "cmp_enemy_contact.h"
#include <engine.h>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

static SoundBuffer hitBuff;
static Sound whack;

static int damage;

float dmgTime = 0.8f;

void ContactComponent::update(double dt) {

	for (auto pl : _parent->scene->ents.find("enemy"))
	{
		if (length(_parent->getPosition() - pl->getPosition()) < 70.0) {
		
			dmgTime -= dt;
		
			if (dmgTime <= 0.f) 
			{
				_parent->setHealth(_parent->getHealth() - damage);
				cout << _parent->getHealth() << endl;
		
				dmgTime = 0.8f;
			}			
					
			if (_parent->getHealth() <= 0)
			{
				//delete
				_parent->setForDelete();		
			}
		}
	}

}

	ContactComponent::ContactComponent(Entity * p, int _damage) : Component(p), _enemy(_parent->scene->ents.find("player")[0])
	{
		damage = _damage;

		if (!hitBuff.loadFromFile("res/hit.flac"))
		{
			cout << "Couldn't load whack sound!" << endl;
		}
		whack.setBuffer(hitBuff);
	}

