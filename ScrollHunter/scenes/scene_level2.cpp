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
#include <SFML/Audio.hpp>
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> monst;

const int screenWidth = 1920;
const int screenHeight = 1080;

Sprite skeleton;

Sprite bckSprites[7];
Texture bckTextures[7];

Sprite hpBarS;
Texture hpBarT;


View scene2view;
View scene2view2;
View scene2view3;

SoundBuffer buffer;
Sound level;

void Level2Scene::Load() 
{
  cout << "Scene 2 Load" << endl;
  ls::loadLevelFile("res/level_2.txt", 40.0f);

  //Set Viewports for scrolling screen
  scene2view.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene2view2.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene2view3.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene2view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));


  //Background
  // ***************************************************************************
  for (int i = 0; i < 7; i++)
  {
      if (!bckTextures[i].loadFromFile("res/layer" + to_string(i + 1) + ".png"))
      {
          cout << "Couldn't load Background" + to_string(i + 1) + "!" << endl;
      }
  
      bckSprites[i].setTexture(bckTextures[i]);
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
      hpBarT.loadFromFile("res/hp.png");
      hpBarS.setTexture(hpBarT);
      hpBarS.setScale(player->getHealth() / 10, 1);
  }
  //***********************************************

  //Level Music
  //************************************************
  if (!buffer.loadFromFile("res/level.ogg"))
  {
      cout << "Couldn't load level music!" << endl;
  }
  level.setBuffer(buffer);
  level.play();
  level.setLoop(true);
  //************************************************

  // Create Skeleton
  // *****************************************************************
  {
	  auto skeleton = Enemies::makeSkeleton(this, ls::getTilePosition(ls::findTiles('k')[0]) + Vector2f(20, 0)); 
  }
  // *****************************************************************
  

  // Create Skeleton Chief
  // *********************************************************************
  {
	  auto skeletonChief = Enemies::makeSkeletonChief(this, ls::getTilePosition(ls::findTiles('c')[0]) + Vector2f(20, 0));
  }
  // *********************************************************************


  // Create Skeleton Archer
  // ***********************************************************************
  {
	  auto skeletonArcher = Enemies::makeSkeletonArcher(this, ls::getTilePosition(ls::findTiles('a')[0]) + Vector2f(20, 0));
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
	  }
    // *********************************
  }

  //Simulate long loading times
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  cout << " Scene 2 Load Done" << endl;
  setLoaded(true);
}

void Level2Scene::UnLoad() 
{
  cout << "Scene 2 UnLoad" << endl;
  player.reset();
  ls::unload();
  Scene::UnLoad();
}

void Level2Scene::Update(const double& dt) 
{

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

    scene2view.reset(FloatRect(position.x, 0, screenWidth, screenHeight));
    //*****************************************************


    //Background Speeds
    //***********************************************************
    if(Keyboard::isKeyPressed(Keyboard::Right) && position.x > 0)
    {
        bckSprites[0].move(Vector2f(-350 * dt,0));
        bckSprites[1].move(Vector2f(-300 * dt, 0));
        bckSprites[2].move(Vector2f(-250 * dt, 0));
        bckSprites[3].move(Vector2f(-200 * dt, 0));
        bckSprites[4].move(Vector2f(-150 * dt, 0));
        bckSprites[5].move(Vector2f(-100 * dt, 0));
        bckSprites[6].move(Vector2f(-50 * dt, 0));
    }
    if (Keyboard::isKeyPressed(Keyboard::Left) && position.x > 0)
    {
        bckSprites[0].move(Vector2f(350 * dt, 0));
        bckSprites[1].move(Vector2f(300 * dt, 0));
        bckSprites[2].move(Vector2f(250 * dt, 0));
        bckSprites[3].move(Vector2f(200 * dt, 0));
        bckSprites[4].move(Vector2f(150 * dt, 0));
        bckSprites[5].move(Vector2f(100 * dt, 0));
        bckSprites[6].move(Vector2f(50 * dt, 0));
    }
    //***********************************************************
    

  Scene::Update(dt);  

  const auto pp = player->getPosition();

  if (ls::getTileAt(pp) == ls::END) 
  {
	  Engine::ChangeScene((Scene*)&level3);
  } 
  else if (!player->isAlive()) 
  {
    level.stop();
    Engine::ChangeScene((Scene*)&gameOver);
  }
}

void Level2Scene::Render() 
{
  Engine::GetWindow().setView(scene2view2);
    
  for (int i = 6; i > -1; i--)
  {
      Engine::GetWindow().draw(bckSprites[i]);
  }
  
  Engine::GetWindow().setView(scene2view);

  ls::render(Engine::GetWindow());

  Engine::GetWindow().setView(scene2view3);

  Engine::GetWindow().draw(hpBarS);
  
  Engine::GetWindow().setView(scene2view);

  Scene::Render();
  
}
