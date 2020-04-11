#include "cmp_hurt_player.h"
#include <engine.h>

using namespace std;
using namespace sf;

void HurtComponent::update(double dt) {
  if (auto pl = _player.lock()) {
    if (length(pl->getPosition() - _parent->getPosition()) < 25.0) {

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
    : Component(p), _player(_parent->scene->ents.find("player")[0]) {}
