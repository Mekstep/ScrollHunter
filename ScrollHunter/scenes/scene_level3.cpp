#include "scene_level3.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_player_physics.h"
#include "../game.h"
#include "../components/cmp_bullet.h"
#include "../Player.h"
#include "../EnemyFactory.h"
#include "../components/cmp_spritesheet.h"
#include <LevelSystem.h>
#include <iostream>
using namespace std;
using namespace sf;

const int screenWidth = 1920;
const int screenHeight = 1080;

View scene3view;
View scene3view2;
View scene3view3;

Sprite bckSprites3[6];
Texture bckTextures3[6];

Texture templeTile;

static shared_ptr<Entity> player;

void Level3Scene::Load() {
  cout << "Scene 3 Load" << endl;
  ls::loadLevelFile("res/level_3.txt", 40.0f);
  templeTile.loadFromFile("res/templeTile.png");

  //Set Viewports for scrolling screen
  scene3view.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene3view2.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene3view3.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene3view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));

  //Background
// ***************************************************************************
  for (int i = 0; i < 6; i++)
  {
	  if (!bckTextures3[i].loadFromFile("res/sky" + to_string(i + 1) + ".png"))
	  {
		  cout << "Couldn't load Background" + to_string(i + 1) + "!" << endl;
	  }

	  bckSprites3[i].setTexture(bckTextures3[i]);
  }
  // ***************************************************************************

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
		  auto s = e->addComponent<SpriteSheetComponent>(Vector2f(40.f, 40.f));
		  s->setSpritesheet(templeTile);
	  }
    // *********************************
  }

  //Simulate long loading times
  //std::this_thread::sleep_for(std::chrono::milliseconds(2000));

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
	//scroll screen as player reaches middle
	//*****************************************************
	Vector2f position(0, 0);

	position.x = player->getPosition().x + 10 - (1920 / 2);
	position.y = player->getPosition().y + 10 - (1080 / 2);

	if (position.x < 0)
	{
		position.x = 0;
	}
	if (position.y < 0)
	{
		position.y = 0;
	}

	scene3view.reset(FloatRect(position.x, 0, screenWidth, screenHeight));
	//*****************************************************


	//Background Speeds
	//***********************************************************
	if (Keyboard::isKeyPressed(Keyboard::Right) && position.x > 0)
	{
		bckSprites3[0].move(Vector2f(-350 * dt, 0));
		bckSprites3[1].move(Vector2f(-300 * dt, 0));
		bckSprites3[2].move(Vector2f(-250 * dt, 0));
		bckSprites3[3].move(Vector2f(-200 * dt, 0));
		bckSprites3[4].move(Vector2f(-150 * dt, 0));
		bckSprites3[5].move(Vector2f(-100 * dt, 0));
		bckSprites3[6].move(Vector2f(-50 * dt, 0));
	}
	if (Keyboard::isKeyPressed(Keyboard::Left) && position.x > 0)
	{
		bckSprites3[0].move(Vector2f(350 * dt, 0));
		bckSprites3[1].move(Vector2f(300 * dt, 0));
		bckSprites3[2].move(Vector2f(250 * dt, 0));
		bckSprites3[3].move(Vector2f(200 * dt, 0));
		bckSprites3[4].move(Vector2f(150 * dt, 0));
		bckSprites3[5].move(Vector2f(100 * dt, 0));
		bckSprites3[6].move(Vector2f(50 * dt, 0));
	}
	//***********************************************************

  const auto pp = player->getPosition();
  if (ls::getTileAt(pp) == ls::END) 
  {
	  scene3view.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene3view2.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene3view3.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene3view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
	  Engine::ChangeScene((Scene*)&level1);
  } 
  else if (!player->isAlive()) 
  {
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

  Scene::Update(dt);
  
}

void Level3Scene::Render() {
	Engine::GetWindow().setView(scene3view2);

	for (int i = 5; i > -1; i--)
	{
		Engine::GetWindow().draw(bckSprites3[i]);
	}

	Engine::GetWindow().setView(scene3view);

	ls::render(Engine::GetWindow());

	Engine::GetWindow().setView(scene3view3);

	Engine::GetWindow().setView(scene3view);

	Scene::Render();
}
