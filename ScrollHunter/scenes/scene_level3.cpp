#include "scene_level3.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_player_physics.h"
#include "../game.h"
#include "../components/cmp_bullet.h"
#include "../Player.h"
#include "../Enemies.h"
#include <LevelSystem.h>
#include <iostream>
using namespace std;
using namespace sf;

const int screenWidth = 1920;
const int screenHeight = 1080;

View scene3view;
View scene3view2;
View scene3view3;

static shared_ptr<Entity> player;

void Level3Scene::Load() {
  cout << "Scene 3 Load" << endl;
  ls::loadLevelFile("res/level_3.txt", 40.0f);

  //Set Viewports for scrolling screen
  scene3view.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene3view2.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene3view3.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene3view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));

  //auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
  //ls::setOffset(Vector2f(0, ho));

  // Create player
  {
    // *********************************
	  player = Player::makePlayer(this, (ls::getTilePosition(ls::findTiles(ls::START)[0])));
    // *********************************
  }

  // Add physics colliders to level tiles.
  {
    // *********************************
	  auto walls = ls::findTiles(ls::WALL);
	  for (auto w : walls) {
		  auto pos = ls::getTilePosition(w);
		  pos += Vector2f(20.f, 20.f); //offset to center
		  auto e = makeEntity();
		  e->setPosition(pos);
		  e->addComponent<PhysicsComponent>(false, Vector2f(40.f, 40.f));
	  }
    // *********************************
  }

  //Simulate long loading times
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  cout << " Scene 3 Load Done" << endl;
  setLoaded(true);
}

void Level3Scene::UnLoad() {
  cout << "Scene 3 UnLoad" << endl;
  player.reset();
  ls::unload();
  Scene::UnLoad();
}



void Level3Scene::Update(const double& dt) {
  Scene::Update(dt);
  const auto pp = player->getPosition();
  if (ls::getTileAt(pp) == ls::END) {
    Engine::ChangeScene((Scene*)&level1);
  } else if (!player->isAlive()) {
    Engine::ChangeScene((Scene*)&level3);
  }

  static float rocktime = 0.0f;
  rocktime -= dt;

  if (rocktime <= 0.f){
    rocktime  = 5.f;
    auto rock = makeEntity();
    rock->setPosition(ls::getTilePosition(ls::findTiles('r')[0]) +
                      Vector2f(0, 40) );
    rock->addComponent<BulletComponent>(30.f);
    auto s = rock->addComponent<ShapeComponent>();
    s->setShape<sf::CircleShape>(40.f);
    s->getShape().setFillColor(Color::Cyan);
    s->getShape().setOrigin(40.f, 40.f);
    auto p = rock->addComponent<PhysicsComponent>(true, Vector2f(75.f, 75.f));
    p->setRestitution(.4f);
    p->setFriction(.0001f);
    p->impulse(Vector2f(-3.f, 0));
    p->setMass(1000000000.f);
  }
  
}

void Level3Scene::Render() {
	Engine::GetWindow().setView(scene3view2);

	Engine::GetWindow().setView(scene3view);

	ls::render(Engine::GetWindow());

	Engine::GetWindow().setView(scene3view3);

	Scene::Render();
}
