#include "cmp_hurt_player.h"
#include "LevelSystem.h"
#include <engine.h>
#include <SFML/Audio.hpp>


using namespace std;
using namespace sf;

static SoundBuffer hitBuff;
static Sound whack;

void HurtComponent::update(double dt) 
{
    if (auto pl = _player.lock()) {
        //If player health is less than 0, delete them
        if (pl->getHealth() <= 0)
        {
            pl->setForDelete();
        }

        //If bullet hits player
        if (length(pl->getPosition() - _parent->getPosition()) < 60.0) {

            whack.play();

            //If shield is on do not remove health otherwise remove health
	      	if (pl->getShield() == true)
	      	{
	      		pl->setHealth(pl->getHealth());
	      	}
	      	else
	      	{
	      		pl->setHealth(pl->getHealth() - 10);
	      	}
            
              //Delete bullet
            _parent->setForDelete();
         
        }

        //Delete bullets as they hit walls
        if (ls::getTileAt(_parent->getPosition()) == ls::WALL)
        {
            _parent->setForDelete();
        }

    }
}

HurtComponent::HurtComponent(Entity* p)
    : Component(p), _player(_parent->scene->ents.find("player")[0]) 
{
    if (!hitBuff.loadFromFile("res/sounds/hit.wav"))
    {
        cout << "Couldn't load whack sound!" << endl;
    }
    whack.setBuffer(hitBuff);
}
