#include "cmp_hurt_player.h"
#include "LevelSystem.h"
#include <engine.h>
#include <SFML/Audio.hpp>


using namespace std;
using namespace sf;

static SoundBuffer hitBuff;
static Sound whack;

void HurtComponent::update(double dt) {
  if (auto pl = _player.lock()) {
    if (length(pl->getPosition() - _parent->getPosition()) < 60.0) {

        whack.play();

		if (pl->getShield() == true)
		{
			pl->setHealth(pl->getHealth());
		}
		else
		{
			pl->setHealth(pl->getHealth() - 10);
		}
        

        _parent->setForDelete();

        cout << pl->getHealth() << endl;

        if (pl->getHealth() <= 0)
        {
            pl->setForDelete();
        }
    }

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
