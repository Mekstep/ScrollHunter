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
#include "components/cmp_enemy_contact.h"


using namespace sf;
using namespace std;

static Texture texture;
static Texture cloud;

//create player
std::shared_ptr<Entity> Player::makePlayer(Scene* scene, const Vector2f& pos) {

	auto player = scene->makeEntity();
	player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	player->addTag("player");
	player->addComponent<PlayerPhysicsComponent>(Vector2f(60.f, 130.f));
	player->setHealth(100);
	player->setEssence(100);
	player->setScore(0);
	player->addComponent<ContactComponent>(10);

	//cloud
	auto cloudP = player->addComponent<SpriteSheetComponent>(Vector2f(152.f, 256.f));
	cloud.loadFromFile("res/cloud.png");
	cloudP->setSpritesheet(cloud);
	cloudP->setFrameCount(1);

	//add spritesheet with dimension of each frame
	auto anim = player->addComponent<SpriteSheetComponent>(Vector2f(60.f, 130.f));
	texture.loadFromFile("res/idleSheet.png");
	anim->setSpritesheet(texture);
	anim->setFrameCount(10);
	anim->setFrameTime(0.1f);





	return player;
}