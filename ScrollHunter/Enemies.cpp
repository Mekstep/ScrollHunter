#include "ecm.h"
#include "Enemies.h"
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

Texture skele;
Texture skeletArcher;
Texture skeletChief;

//create skeleton
std::shared_ptr<Entity> Enemies::makeSkeleton(Scene* scene, const Vector2f& pos) {

	auto skeleton = scene->makeEntity();
	skeleton->setHealth(100);
	skeleton->setPosition(pos);

	// Add ShapeComponent, Red 16.f Circle
	auto anim = skeleton->addComponent<SpriteSheetComponent>(Vector2f(160.f, 140.f));
	skele.loadFromFile("res/skeletonIdle.png");
	anim->setSpritesheet(skele);
	anim->setFrameCount(8);
	anim->setFrameTime(0.1f);

	auto turret = scene->makeEntity();
	turret->setPosition(skeleton->getPosition());
	auto t = turret->addComponent<ShapeComponent>();
	t->setShape<sf::CircleShape>(15.0f, 3);
	t->getShape().setFillColor(Color::Transparent);
	t->getShape().setOrigin(16.f, 16.f);

	// Add EnemyAIComponent
	skeleton->addComponent<EnemyAIComponent>();
	turret->addComponent<EnemyTurretComponent>();
	turret->addComponent<EnemyAIComponent>();


	return skeleton;
}

//create skeleton archer
std::shared_ptr<Entity> Enemies::makeSkeletonArcher(Scene* scene, const Vector2f& pos) {

	auto skeleArcher = scene->makeEntity();
	skeleArcher->setHealth(100);
	skeleArcher->setPosition(pos);

	// Add ShapeComponent, Red 16.f Circle
	auto anim = skeleArcher->addComponent<SpriteSheetComponent>(Vector2f(130.f, 140.f));
	skeletArcher.loadFromFile("res/skeletonArcherIdle.png");
	anim->setSpritesheet(skeletArcher);
	anim->setFrameCount(8);
	anim->setFrameTime(0.1f);

	auto turret = scene->makeEntity();
	turret->setPosition(skeleArcher->getPosition());
	//auto t = turret->addComponent<ShapeComponent>();
	//t->setShape<sf::CircleShape>(15.0f, 3);
	//t->getShape().setFillColor(Color::Red);
	//t->getShape().setOrigin(16.f, 16.f);

	// Add EnemyAIComponent
	skeleArcher->addComponent<EnemyAIComponent>();
	//turret->addComponent<EnemyTurretComponent>();
	//turret->addComponent<EnemyAIComponent>();


	return skeleArcher;
}

//create skeleton chief
std::shared_ptr<Entity> Enemies::makeSkeletonChief(Scene* scene, const Vector2f& pos) {

	auto skeleChief = scene->makeEntity();
	skeleChief->setHealth(100);
	skeleChief->setPosition(pos);

	// Add ShapeComponent, Red 16.f Circle
	auto anim = skeleChief->addComponent<SpriteSheetComponent>(Vector2f(160.f, 180.f));
	skeletChief.loadFromFile("res/skeletonChiefIdle.png");
	anim->setSpritesheet(skeletChief);
	anim->setFrameCount(8);
	anim->setFrameTime(0.1f);

	auto turret = scene->makeEntity();
	turret->setPosition(skeleChief->getPosition());
	//auto t = turret->addComponent<ShapeComponent>();
	//t->setShape<sf::CircleShape>(15.0f, 3);
	//t->getShape().setFillColor(Color::Red);
	//t->getShape().setOrigin(16.f, 16.f);

	// Add EnemyAIComponent
	skeleChief->addComponent<EnemyAIComponent>();
	//turret->addComponent<EnemyTurretComponent>();
	//turret->addComponent<EnemyAIComponent>();


	return skeleChief;
}