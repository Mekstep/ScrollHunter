#include "ecm.h"
#include "EnemyFactory.h"
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
#include "components/cmp_bullet_physics.h"


using namespace sf;
using namespace std;

Texture skele;
Texture skeletArcher;
Texture skeletChief;

//create skeleton
std::shared_ptr<Entity> Enemies::makeSkeleton(Scene* scene) {

	auto skeleton = scene->makeEntity();
	skeleton->setHealth(100);
	skeleton->setPosition(ls::getTilePosition(ls::findTiles('k')[0]) + Vector2f(20, 0));
	skeleton->addTag("enemy");
	skeleton->setAimer(false);

	// Add ShapeComponent, Red 16.f Circle
	auto anim = skeleton->addComponent<SpriteSheetComponent>(Vector2f(160.f, 140.f));
	skele.loadFromFile("res/skeletonIdle.png");
	anim->setSpritesheet(skele);
	anim->setFrameCount(8);
	anim->setFrameTime(0.1f);

	//auto turret = scene->makeEntity();
	//turret->setPosition(skeleton->getPosition());
	//auto t = turret->addComponent<ShapeComponent>();
	//t->setShape<sf::CircleShape>(15.0f, 3);
	//t->getShape().setFillColor(Color::Red);
	//t->getShape().setOrigin(16.f, 16.f);

	// Add EnemyAIComponent
	skeleton->addComponent<EnemyAIComponent>();
	skeleton->addComponent<EnemyTurretComponent>();
	//turret->addComponent<EnemyAIComponent>();


	return skeleton;
}

//create skeleton archer
std::shared_ptr<Entity> Enemies::makeSkeletonArcher(Scene* scene) {

	auto skeleArcher = scene->makeEntity();
	skeleArcher->setHealth(100);
	skeleArcher->setPosition(ls::getTilePosition(ls::findTiles('a')[0]) + Vector2f(20, 0));
	skeleArcher->addTag("enemy");
	skeleArcher->setAimer(true);

	// Add ShapeComponent, Red 16.f Circle
	auto anim = skeleArcher->addComponent<SpriteSheetComponent>(Vector2f(130.f, 140.f));
	skeletArcher.loadFromFile("res/skeletonArcherIdle.png");
	anim->setSpritesheet(skeletArcher);
	anim->setFrameCount(8);
	anim->setFrameTime(0.1f);

	//auto turret = scene->makeEntity();
	//turret->setPosition(skeleArcher->getPosition());
	//auto t = turret->addComponent<ShapeComponent>();
	//t->setShape<sf::CircleShape>(15.0f, 3);
	//t->getShape().setFillColor(Color::Red);
	//t->getShape().setOrigin(16.f, 16.f);

	// Add EnemyAIComponent
	skeleArcher->addComponent<EnemyAIComponent>();
	skeleArcher->addComponent<EnemyTurretComponent>();
	//turret->addComponent<EnemyAIComponent>();


	return skeleArcher;
}

//create skeleton chief
std::shared_ptr<Entity> Enemies::makeSkeletonChief(Scene* scene) {



	auto skeleChief = scene->makeEntity();
	skeleChief->setHealth(100);
	skeleChief->setPosition(ls::getTilePosition(ls::findTiles('c')[0]) + Vector2f(20, 0));
	skeleChief->addTag("enemy");
	skeleChief->setAimer(false);

	// Add ShapeComponent, Red 16.f Circle
	auto anim = skeleChief->addComponent<SpriteSheetComponent>(Vector2f(160.f, 180.f));
	skeletChief.loadFromFile("res/skeletonChiefIdle.png");
	anim->setSpritesheet(skeletChief);
	anim->setFrameCount(8);
	anim->setFrameTime(0.1f);

	//auto turret = scene->makeEntity();
	//turret->setPosition(skeleChief->getPosition());
	//auto t = turret->addComponent<ShapeComponent>();
	//t->setShape<sf::CircleShape>(15.0f, 3);
	//t->getShape().setFillColor(Color::Red);
	//t->getShape().setOrigin(16.f, 16.f);

	// Add EnemyAIComponent
	skeleChief->addComponent<EnemyAIComponent>();
	skeleChief->addComponent<EnemyTurretComponent>();
	//turret->addComponent<EnemyAIComponent>();


	return skeleChief;
}

