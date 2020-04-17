#include "cmp_hurt_player.h"
#include <engine.h>
#include <SFML/Audio.hpp>


using namespace std;
using namespace sf;

SoundBuffer hitBuff;
Sound whack;

void HurtComponent::update(double dt) {
  if (auto pl = _player.lock()) {
    if (length(pl->getPosition() - _parent->getPosition()) < 25.0) {

        whack.play();

        pl->setHealth(pl->getHealth()-10);

        _parent->setForDelete();

        cout << pl->getHealth() << endl;

        if (pl->getHealth() <= 0)
        {
            pl->setForDelete();
        }

      //pl->setForDelete();
      //_parent->setForDelete();
    }

  }
}

HurtComponent::HurtComponent(Entity* p)
    : Component(p), _player(_parent->scene->ents.find("player")[0]) 
{
    if (!hitBuff.loadFromFile("res/hit.flac"))
    {
        cout << "Couldn't load whack sound!" << endl;
    }
    whack.setBuffer(hitBuff);
}
