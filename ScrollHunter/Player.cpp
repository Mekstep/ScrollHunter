#include "ecm.h"
#include "Player.h"
#include "components/cmp_spritesheet.h"
#include "components/cmp_enemy_ai.h"
#include "components/cmp_actor_movement.h"
#include "components/cmp_sprite.h"
#include "components/cmp_hurt_player.h"
#include "system_resources.h"
#include "components/cmp_physics.h"
#include "components/cmp_player_physics.h"
#include "components/cmp_enemy_turret.h"
#include "components/cmp_bullet.h"
#include <LevelSystem.h>
#include <SFML/Graphics/Transformable.hpp>


using namespace sf;
using namespace std;

Texture texture;

//create player
std::shared_ptr<Entity> Player::makePlayer(Scene* scene, const Vector2f& pos) {

	auto player = scene->makeEntity();
	player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	player->addTag("player");
	player->addComponent<PlayerPhysicsComponent>(Vector2f(60.f, 130.f));
	player->setHealth(100);

	/*
	auto s = player->addComponent<ShapeComponent>();
	s->setShape<sf::RectangleShape>(Vector2f(38.f, 64.f));
	mage.loadFromFile("res/mage.png");
	s->getShape().setTexture(&mage);
	s->getShape().setOrigin(10.f, 15.f);
	*/

	//add spritesheet with dimension of each frame
	auto anim = player->addComponent<SpriteSheetComponent>(Vector2f(60.f, 130.f));
	texture.loadFromFile("res/idleSheet.png");
	anim->setSpritesheet(texture);
	anim->setFrameCount(10);
	anim->setFrameTime(0.1f);


	return player;
}