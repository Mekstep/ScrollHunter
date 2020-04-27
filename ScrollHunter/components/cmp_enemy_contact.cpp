#include "cmp_enemy_contact.h"
#include <engine.h>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

static SoundBuffer hitBuff;
static Sound whack;

static int damage;

float dmgTime = 0.8f;

static float dist;

//Hurt player as they come into contact with an enemy itself
void ContactComponent::update(double dt) {

	for (auto en : _parent->scene->ents.find("enemy"))
	{
		//If its a boss make the hitbox larger
		if (en->getType() == "boss")
		{
			dist = 240.0f;
		}
		//else normal enemy
		else
		{
			dist = 100.0f;
		}
		//Check distance between enemy and player
		if (length(_parent->getPosition() - en->getPosition()) < dist) {
		
			dmgTime -= dt;
		
			//Damage player every second while contacting enemy
			if (dmgTime <= 0.f) 
			{
				//Remove health
				_parent->setHealth(_parent->getHealth() - damage);
		
				//Reset damage timer
				dmgTime = 0.8f;
			}			
			
			//If player health is less than 0, dead
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

