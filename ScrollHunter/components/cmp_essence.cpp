#include "cmp_essence.h"
#include "LevelSystem.h"
#include <engine.h>
#include <SFML/Audio.hpp>


using namespace std;
using namespace sf;

static SoundBuffer pickupBuff;
static Sound pickup;

void EssenceComponent::update(double dt) {
  if (auto pl = _player.lock()) {
    if (length(pl->getPosition() - _parent->getPosition()) < 60.0) {
		pickup.stop();
		pickup.play();

		if (pl->getEssence() < 100)
		{
			pl->setEssence(pl->getEssence() + 20);
			pl->setScore(pl->getScore() + 5);

			cout << "PLAYER PICKED UP ESSENCE" << endl;
		}

		if (pl->getEssence() > 100)
		{
			pl->setEssence(100);
		}

        _parent->setForDelete();
    }

    if (ls::getTileAt(_parent->getPosition()) == ls::WALL)
    {
        _parent->setForDelete();
    }

  }
}

EssenceComponent::EssenceComponent(Entity* p)
    : Component(p), _player(_parent->scene->ents.find("player")[0]) 
{
	if (!pickupBuff.loadFromFile("res/sounds/pickup.wav"))
	{
		cout << "Couldn't load pickup sound!" << endl;
	}
	pickup.setBuffer(pickupBuff);
}
