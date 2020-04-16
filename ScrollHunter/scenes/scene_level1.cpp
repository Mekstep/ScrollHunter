#include "scene_level1.h"
#include "../components/cmp_bullet.h"
#include "../components/cmp_enemy_ai.h"
#include "../components/cmp_enemy_turret.h"
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_spritesheet.h"
#include "../scenes/scene_menu.h"
#include "../game.h"
#include "../Player.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

Texture templeTile;

Texture skele1;
Texture skeletArcher1;
Texture skeletChief1;

Texture tempBG;
Sprite tempBGS;


void Level1Scene::Load() {
  cout << " Scene 1 Load" << endl;
  ls::loadLevelFile("res/level_1.txt", 40.0f);
  templeTile.loadFromFile("res/templeTile.png");
  tempBG.loadFromFile("res/tempBG.png");

  auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
  ls::setOffset(Vector2f(0, ho));

  //background
  tempBGS.setTexture(tempBG);
  tempBGS.setScale(1.2, 1.2);


  // Create player
  {
	  player = Player::makePlayer(this, (ls::getTilePosition(ls::findTiles(ls::START)[0])));

    //sprite.loadFromFile("C:/Users/Euan/Desktop/ScrollHunter Build/bin/Debug/res/Mage.png");
	//mage.setTexture(sprite);
	//mage.setTextureRect(sf::IntRect(0, 0, 38, 64));
    //player->addComponent<PhysicsComponent>(true, Vector2f(38.f, 128.f));

	
	//s->setSprite(mage);

 
	//s->getSprite().setOrigin(10.0f, 15.0f);
	/*
	auto s = player->addComponent<ShapeComponent>();
    s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
    s->getShape().setFillColor(Color::Magenta);
    s->getShape().setOrigin(10.f, 15.f);
	*/

    //player->addComponent<PlayerPhysicsComponent>(Vector2f(30.f, 40.f));
  }

  // Create Skeleton
  // *********************************
  {
	  auto skeleton = makeEntity();
	  skeleton->setHealth(100);
	  skeleton->setPosition(ls::getTilePosition(ls::findTiles('k')[0]) +
		  Vector2f(20, 0));

	  auto anim = skeleton->addComponent<SpriteSheetComponent>(Vector2f(160.f, 140.f));
	  skele1.loadFromFile("res/skeletonIdle.png");
	  anim->setSpritesheet(skele1);
	  anim->setFrameCount(8);
	  anim->setFrameTime(0.1f);

	  /* Add ShapeComponent, Red 16.f Circle
	  auto s = skeleton->addComponent<ShapeComponent>();
	  s->setShape<sf::RectangleShape>(Vector2f(30.f, 50.f));
	  skele.loadFromFile("res/skeleton.png");
	  s->getShape().setTexture(&skele);
	  s->getShape().setOrigin(10.f, 25.f);
	  */

	  //auto turret = makeEntity();
	  ///turret->setPosition(skeleton->getPosition());
	  //auto t = turret->addComponent<ShapeComponent>();
	  //t->setShape<sf::CircleShape>(15.0f, 3);
	  //t->getShape().setFillColor(Color::Red);
	  //t->getShape().setOrigin(16.f, 16.f);

	  // Add EnemyAIComponent
	  skeleton->addComponent<EnemyAIComponent>();
	  //turret->addComponent<EnemyTurretComponent>();
	  //turret->addComponent<EnemyAIComponent>();
  }
  // *********************************


  // Create Skeleton Chief
  // *********************************
  {
	  auto skeleChief = makeEntity();
	  skeleChief->setHealth(100);
	  skeleChief->setPosition(ls::getTilePosition(ls::findTiles('c')[0]) +
		  Vector2f(20, 0));

	  auto anim = skeleChief->addComponent<SpriteSheetComponent>(Vector2f(160.f, 180.f));
	  skeletChief1.loadFromFile("res/skeletonChiefIdle.png");
	  anim->setSpritesheet(skeletChief1);
	  anim->setFrameCount(8);
	  anim->setFrameTime(0.1f);

	  /* Add ShapeComponent, Red 16.f Circle
	  auto s = skeleChief->addComponent<ShapeComponent>();
	  s->setShape<sf::RectangleShape>(Vector2f(30.f, 50.f));

	  skeletChief.loadFromFile("res/skeletonchief.png");
	  s->getShape().setTexture(&skeletChief);
	  s->getShape().setOrigin(10.f, 25.f);
	  */

	  //auto turret = makeEntity();
	  //turret->setPosition(skeleChief->getPosition());
	  //auto t = turret->addComponent<ShapeComponent>();
	  //t->setShape<sf::CircleShape>(15.0f, 3);
	  //t->getShape().setFillColor(Color::Red);
	  //t->getShape().setOrigin(16.f, 16.f);

	  // Add EnemyAIComponent
	  skeleChief->addComponent<EnemyAIComponent>();
	  //turret->addComponent<EnemyTurretComponent>();
	  //turret->addComponent<EnemyAIComponent>();
  }
  // *********************************


  // Create Skeleton Archer
  // *********************************
  {
	  auto skeleArcher = makeEntity();
	  skeleArcher->setHealth(100);
	  skeleArcher->setPosition(ls::getTilePosition(ls::findTiles('a')[0]) +
		  Vector2f(20, 0));

	  auto anim = skeleArcher->addComponent<SpriteSheetComponent>(Vector2f(130.f, 140.f));
	  skeletArcher1.loadFromFile("res/skeletonArcherIdle.png");
	  anim->setSpritesheet(skeletArcher1);
	  anim->setFrameCount(8);
	  anim->setFrameTime(0.1f);

	  /* Add ShapeComponent, Red 16.f Circle
	  auto s = skeleArcher->addComponent<ShapeComponent>();
	  s->setShape<sf::RectangleShape>(Vector2f(30.f, 50.f));

	  skeletArcher.loadFromFile("res/skeletonarcher.png");
	  s->getShape().setTexture(&skeletArcher);
	  s->getShape().setOrigin(10.f, 25.f);
	  */
	  //auto turret = makeEntity();
	  //turret->setPosition(skeleArcher->getPosition());
	  //auto t = turret->addComponent<ShapeComponent>();
	  //t->setShape<sf::CircleShape>(15.0f, 3);
	  //t->getShape().setFillColor(Color::Red);
	  //t->getShape().setOrigin(16.f, 16.f);

	  // Add EnemyAIComponent
	  skeleArcher->addComponent<EnemyAIComponent>();
	  //turret->addComponent<EnemyTurretComponent>();
	  //turret->addComponent<EnemyAIComponent>();
  }
  // *********************************


  // Add physics colliders to level tiles.
  {
    auto walls = ls::findTiles(ls::WALL);
    for (auto w : walls) {
      auto pos = ls::getTilePosition(w);
      pos += Vector2f(20.f, 20.f); //offset to center
      auto e = makeEntity();
      e->setPosition(pos);
      e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
	  auto s = e->addComponent<SpriteSheetComponent>(Vector2f(40.f, 40.f));
	  s->setSpritesheet(templeTile);
    }
  }

  //Simulate long loading times
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  cout << " Scene 1 Load Done" << endl;

  setLoaded(true);
}

void Level1Scene::UnLoad() {
  cout << "Scene 1 Unload" << endl;
  player.reset();
  ls::unload();
  Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {

  if (ls::getTileAt(player->getPosition()) == ls::END) {
    Engine::ChangeScene((Scene*)&level2);
  }
  Scene::Update(dt);
}

void Level1Scene::Render() {
	Engine::GetWindow().draw(tempBGS);
	ls::render(Engine::GetWindow());
	Scene::Render();
}
