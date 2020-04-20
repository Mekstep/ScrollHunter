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
#include "../EnemyFactory.h"
#include <LevelSystem.h>
#include <iostream>
#include <SFML/Audio.hpp>
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> monst;

const int screenWidth = 1920;
const int screenHeight = 1080;

Sprite bckSprites1[6];
Texture bckTextures1[6];

Texture templeTile1;

Sprite hpBarS1;
Texture hpBarT1;


View scene1view;
View scene1view1;
View scene1view3;

SoundBuffer buffer1;
Sound level1sound;

void Level1Scene::Load() 
{
  cout << "Scene 1 Load" << endl;
  ls::loadLevelFile("res/level_1.txt", 40.0f);
  templeTile1.loadFromFile("res/templeTile.png");

  //Set Viewports for scrolling screen
  scene1view.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene1view1.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene1view3.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene1view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));


  //Background
  // ***************************************************************************
  for (int i = 0; i < 6; i++)
  {
      if (!bckTextures1[i].loadFromFile("res/sky" + to_string(i + 1) + ".png"))
      {
          cout << "Couldn't load Background" + to_string(i + 1) + "!" << endl;
      }
  
      bckSprites1[i].setTexture(bckTextures1[i]);
  }
  // ***************************************************************************

  // Create player
  //**************************************************************************************
  {
	player = Player::makePlayer(this, (ls::getTilePosition(ls::findTiles(ls::START)[0])));
  }
  //**************************************************************************************

  //HP Bar
  //***********************************************
  {
      hpBarT1.loadFromFile("res/hp.png");
      hpBarS1.setTexture(hpBarT1);
      hpBarS1.setScale(player->getHealth() / 10, 1);
  }
  //***********************************************

  //Level Music
  //************************************************
  if (!buffer1.loadFromFile("res/level.ogg"))
  {
      cout << "Couldn't load level music!" << endl;
  }
  level1sound.setBuffer(buffer1);
  level1sound.play();
  level1sound.setLoop(true);
  //************************************************

  // Create Skeleton
  // *****************************************************************
  {
	  auto skeleton = Enemies::makeSkeleton(this); 
  }
  // *****************************************************************
  

  // Create Skeleton Chief
  // *********************************************************************
  {
	  auto skeletonChief = Enemies::makeSkeletonChief(this);
  }
  // *********************************************************************


  // Create Skeleton Archer
  // ***********************************************************************
  {
	  auto skeletonArcher = Enemies::makeSkeletonArcher(this);
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
		  s->setSpritesheet(templeTile1);
	  }
    // *********************************
  }

  //Simulate long loading times
  //std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  cout << " Scene 1 Load Done" << endl;
  setLoaded(true);
}

void Level1Scene::UnLoad() 
{
  cout << "Scene 1 UnLoad" << endl;
  player.reset();
  ls::unload();
  Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) 
{

    hpBarS1.setScale(player->getHealth() / 10, 1);

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

    scene1view.reset(FloatRect(position.x, 0, screenWidth, screenHeight));
    //*****************************************************


    //Background Speeds
    //***********************************************************
    if(Keyboard::isKeyPressed(Keyboard::Right) && position.x > 0)
    {
        bckSprites1[0].move(Vector2f(-350 * dt,0));
        bckSprites1[1].move(Vector2f(-300 * dt, 0));
        bckSprites1[2].move(Vector2f(-250 * dt, 0));
        bckSprites1[3].move(Vector2f(-200 * dt, 0));
        bckSprites1[4].move(Vector2f(-150 * dt, 0));
        bckSprites1[5].move(Vector2f(-100 * dt, 0));
        bckSprites1[6].move(Vector2f(-50 * dt, 0));
    }
    if (Keyboard::isKeyPressed(Keyboard::Left) && position.x > 0)
    {
        bckSprites1[0].move(Vector2f(350 * dt, 0));
        bckSprites1[1].move(Vector2f(300 * dt, 0));
        bckSprites1[2].move(Vector2f(250 * dt, 0));
        bckSprites1[3].move(Vector2f(200 * dt, 0));
        bckSprites1[4].move(Vector2f(150 * dt, 0));
        bckSprites1[5].move(Vector2f(100 * dt, 0));
        bckSprites1[6].move(Vector2f(50 * dt, 0));
    }
    //***********************************************************

  const auto pp = player->getPosition();

  if (ls::getTileAt(pp) == ls::END) 
  {
	  scene1view.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene1view1.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene1view3.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene1view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
	  Engine::ChangeScene((Scene*)&level3);
  } 
  else if (!player->isAlive()) 
  {
    level1sound.stop();
    Engine::ChangeScene((Scene*)&gameOver);
  }

  Scene::Update(dt);
}

void Level1Scene::Render() 
{
  Engine::GetWindow().setView(scene1view1);
    
  for (int i = 5; i > -1; i--)
  {
      Engine::GetWindow().draw(bckSprites1[i]);
  }
  
  Engine::GetWindow().setView(scene1view);

  ls::render(Engine::GetWindow());

  Engine::GetWindow().setView(scene1view3);

  Engine::GetWindow().draw(hpBarS1);
  
  Engine::GetWindow().setView(scene1view);

  Scene::Render();
  
}
