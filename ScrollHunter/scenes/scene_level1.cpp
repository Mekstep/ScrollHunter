#include "scene_level1.h"
#include "scene_level2.h"
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
#include "../Enemies.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

const int screenWidth = 1920;
const int screenHeight = 1080;

View scene1view;
View scene1view2;
View scene1view3;

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

  //Set Viewports for scrolling screen
  scene1view.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene1view2.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene1view3.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene1view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));

  //auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
 // ls::setOffset(Vector2f(0, ho));

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
	  auto skeleton = Enemies::makeSkeleton(this, ls::getTilePosition(ls::findTiles('k')[0]) + Vector2f(20, 0));
  }
  // *********************************


  // Create Skeleton Chief
  // *********************************
  {
	  auto skeletonChief = Enemies::makeSkeletonChief(this, ls::getTilePosition(ls::findTiles('c')[0]) + Vector2f(20, 0));
  }
  // *********************************


  // Create Skeleton Archer
  // *********************************
  {
	  auto skeletonArcher = Enemies::makeSkeletonArcher(this, ls::getTilePosition(ls::findTiles('a')[0]) + Vector2f(20, 0));
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
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
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

	Engine::GetWindow().setView(scene1view2);

	Engine::GetWindow().setView(scene1view);

	ls::render(Engine::GetWindow());

	Engine::GetWindow().setView(scene1view3);

	Scene::Render();
}
