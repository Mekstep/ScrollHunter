#include "cmp_essence.h"
#include "LevelSystem.h"
#include <engine.h>
#include <SFML/Audio.hpp>


using namespace std;
using namespace sf;

void EssenceComponent::update(double dt) {
  if (auto pl = _player.lock()) {
    if (length(pl->getPosition() - _parent->getPosition()) < 60.0) {

		if (pl->getEssence() <= 100)
		{
			pl->setEssence(pl->getEssence() + 20);

			cout << "PLAYER PICKED UP ESSENCE" << endl;
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
}
