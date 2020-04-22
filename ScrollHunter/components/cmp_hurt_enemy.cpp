#include "cmp_hurt_enemy.h"
#include "cmp_essence.h"
#include "LevelSystem.h"
#include <engine.h>
#include <SFML/Audio.hpp>
#include "../score.h"

using namespace std;
using namespace sf;

static SoundBuffer hitBuff;
static Sound whack;

int damage;

Score score;

void EnemyHurtComponent::update(double dt) {

	for (auto pl : _parent->scene->ents.find("enemy"))
	{
		if (length(pl->getPosition() - _parent->getPosition()) < 60.0) {

			pl->setHealth(pl->getHealth() - damage);			

			_parent->setForDelete();

			
			if (pl->getHealth() <= 0)
			{
				//drop essence
				auto e = pl->scene->makeEntity();
				e->setPosition(_parent->getPosition());
				auto s = e->addComponent<ShapeComponent>();
				s->setShape<sf::RectangleShape>(Vector2f(25.0f, 25.0f));
				s->getShape().setFillColor(Color::Blue);
				s->getShape().setOrigin(8.f, 8.f);
				auto essence = e->addComponent<EssenceComponent>();

				if (pl->getType() == "skeleton")
				{
					score.setScore(score.getScore() + 10);
				}
				if (pl->getType() == "archer")
				{
					score.setScore(score.getScore() + 20);
				}
				if (pl->getType() == "chief")
				{
					score.setScore(score.getScore() + 30);
				}

				cout << "Score: " << score.getScore() << endl;

				//delete
				pl->setForDelete();

			}
		}
		if (ls::getTileAt(_parent->getPosition()) == ls::WALL)
		{
			_parent->setForDelete();
		}
	}

}

	EnemyHurtComponent::EnemyHurtComponent(Entity * p, int _damage) : Component(p), _enemy(_parent->scene->ents.find("enemy")[0])
	{
		damage = _damage;

		if (!hitBuff.loadFromFile("res/hit.flac"))
		{
			cout << "Couldn't load whack sound!" << endl;
		}
		whack.setBuffer(hitBuff);
	}

