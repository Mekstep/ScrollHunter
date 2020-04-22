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

//hud and hud background
static Texture HUD;
static Texture HUDbg;
static Sprite HUDs;
static Sprite HUDbgs;

//health bar
static Sprite hpBarS;
static Texture hpBarT;

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
	  if (!bckTextures3[i].loadFromFile("res/scene3/sky" + to_string(i + 1) + ".png"))
	  {
		  cout << "Couldn't load Background" + to_string(i + 1) + "!" << endl;
	  }

	  bckSprites3[i].setTexture(bckTextures3[i]);
  }
  // ***************************************************************************
// Create player
  //**************************************************************************************
  {
	  player = Player::makePlayer(this, (ls::getTilePosition(ls::findTiles(ls::START)[0])));
  }
  //**************************************************************************************

 // Create Skeletons
  // *****************************************************************
  {
	  auto skeletons = ls::findTiles('k');
	  for (auto s : skeletons)
	  {
		  auto pos = ls::getTilePosition(s);
		  auto skeleton = Enemies::makeSkeleton(this);
		  skeleton->setPosition(pos);
	  }

  }
  // *****************************************************************


  // Create Skeleton Chiefs
  // *********************************************************************
  {
	  auto skeletonChiefs = ls::findTiles('c');
	  for (auto s : skeletonChiefs)
	  {
		  auto pos = ls::getTilePosition(s);
		  auto skeletonChief = Enemies::makeSkeletonChief(this);
		  skeletonChief->setPosition(pos);
	  }
  }
  // *********************************************************************


  // Create Skeleton Archers
  // ***********************************************************************
  {
	  auto skeletonArchers = ls::findTiles('a');
	  for (auto s : skeletonArchers)
	  {
		  auto pos = ls::getTilePosition(s);
		  auto skeletonArcher = Enemies::makeSkeletonArcher(this);
		  skeletonArcher->setPosition(pos);
	  }
  }
  // ***********************************************************************
  // Add physics colliders to level tiles.
  {
	  // *********************************
	  auto walls = ls::findTiles(ls::WALL);
	  for (auto w : walls) {
		  auto pos = ls::getTilePosition(w);
		  pos += Vector2f(20.0f, 20.0f); // offset to centre
		  auto e = makeEntity();
		  e->setPosition(pos);
		  e->addComponent<PhysicsComponent>(false, Vector2f(40.0f, 40.0f));
		  e->addTag("wall");
		  auto s = e->addComponent<SpriteSheetComponent>(Vector2f(40.f, 40.f));
		  s->setSpritesheet(templeTile);
	  }
	  // *********************************

	  //HP Bar & HUD
	  //***********************************************
	  {
		  hpBarT.loadFromFile("res/hp.png");
		  hpBarS.setTexture(hpBarT);
		  hpBarS.setScale(player->getHealth() / 10, 1);
		  HUD.loadFromFile("res/HUD.png");
		  HUDs.setTexture(HUD);
		  HUDbg.loadFromFile("res/HUDbg.png");
		  HUDbgs.setTexture(HUDbg);
	  }
	  //***********************************************
  }

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

	hpBarS.setScale(player->getHealth() / 10, 1);

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
	}
	if (Keyboard::isKeyPressed(Keyboard::Left) && position.x > 0)
	{
		bckSprites3[0].move(Vector2f(350 * dt, 0));
		bckSprites3[1].move(Vector2f(300 * dt, 0));
		bckSprites3[2].move(Vector2f(250 * dt, 0));
		bckSprites3[3].move(Vector2f(200 * dt, 0));
		bckSprites3[4].move(Vector2f(150 * dt, 0));
		bckSprites3[5].move(Vector2f(100 * dt, 0));
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

	//draw hud
	Engine::GetWindow().draw(HUDbgs);
	Engine::GetWindow().draw(hpBarS);
	Engine::GetWindow().draw(HUDs);

	Engine::GetWindow().setView(scene3view);

	Scene::Render();
}
