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
#include "../EnemyFactory.h"
#include "../score.h"
#include <LevelSystem.h>
#include <iostream>
#include <SFML/Audio.hpp>
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> monst;

//screen dimensions
const int screenWidth = 1920;
const int screenHeight = 1080;

//hud and hud background
static Texture HUD;
static Texture HUDbg;
static Sprite HUDs;
static Sprite HUDbgs;

Sprite skeleton;

//parallax background
Sprite bckSprites2[6];
Texture bckTextures2[6];

//health bar
static Sprite hpBarS;
static Texture hpBarT;
//mana bar
static Sprite essBarS;
static Texture essBarT;

//views
View scene2view;
View scene2view2;
View scene2view3;

SoundBuffer buffer;
Sound level;

Font font;
Text scoreT;

static Score score;

void Level2Scene::Load() 
{
  cout << "Scene 2 Load" << endl;
  ls::loadLevelFile("res/level_2.txt", 40.0f);

  //Set Viewports for scrolling screen
  scene2view.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene2view2.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene2view3.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene2view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));

  //Score
  //***************************************************
  if (!font.loadFromFile("res/fonts/Gameplay.ttf"))
  {
      cout << "Couldn't load font!" << endl;
  }

  scoreT.setFont(font);
  scoreT.setString(to_string(score.getScore()));
  scoreT.setCharacterSize(20);
  scoreT.setFillColor(Color::Red);
  scoreT.setPosition(900,1000);
  //***************************************************

  //Background
  // ***************************************************************************
  for (int i = 0; i < 6; i++)
  {
      if (!bckTextures2[i].loadFromFile("res/scene2/sky" + to_string(i + 1) + ".png"))
      {
          cout << "Couldn't load Background" + to_string(i + 1) + "!" << endl;
      }
  
      bckSprites2[i].setTexture(bckTextures2[i]);
  }
  // ***************************************************************************

  // Create player
  //**************************************************************************************
  {
	player = Player::makePlayer(this, (ls::getTilePosition(ls::findTiles(ls::START)[0])));
  }
  //**************************************************************************************

  //HP Bar & Essence & HUD
  //***********************************************
  {
	  //hp
      hpBarT.loadFromFile("res/hp.png");
      hpBarS.setTexture(hpBarT);
      hpBarS.setScale(player->getHealth() / 10, 1);

	  //essence
	  essBarT.loadFromFile("res/es.png");
	  essBarS.setTexture(essBarT);
	  essBarS.setScale(player->getEssence() / 20, 1);
	  essBarS.setPosition(Vector2f(250.f, 75.f));

	  HUD.loadFromFile("res/HUD.png");
	  HUDs.setTexture(HUD);
	  HUDbg.loadFromFile("res/HUDbg.png");
	  HUDbgs.setTexture(HUDbg);
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
	  }
    // *********************************
  }

  //Simulate long loading times
  //std::this_thread::sleep_for(std::chrono::milliseconds(2000));

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
	//hp bar scaling
    hpBarS.setScale(player->getHealth() / 10, 1);

	//essence bar scaling
	essBarS.setScale(player->getEssence() / 20, 1);

    //Score Update
    scoreT.setString(to_string(score.getScore()));

    cout << score.getScore() << endl;


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
        bckSprites2[0].move(Vector2f(-350 * dt,0));
        bckSprites2[1].move(Vector2f(-300 * dt, 0));
        bckSprites2[2].move(Vector2f(-250 * dt, 0));
        bckSprites2[3].move(Vector2f(-200 * dt, 0));
        bckSprites2[4].move(Vector2f(-150 * dt, 0));
        bckSprites2[5].move(Vector2f(-100 * dt, 0));
    }
    if (Keyboard::isKeyPressed(Keyboard::Left) && position.x > 0)
    {
        bckSprites2[0].move(Vector2f(350 * dt, 0));
        bckSprites2[1].move(Vector2f(300 * dt, 0));
        bckSprites2[2].move(Vector2f(250 * dt, 0));
        bckSprites2[3].move(Vector2f(200 * dt, 0));
        bckSprites2[4].move(Vector2f(150 * dt, 0));
        bckSprites2[5].move(Vector2f(100 * dt, 0));
    }
    //***********************************************************

  const auto pp = player->getPosition();

  if (ls::getTileAt(pp) == ls::END) 
  {
	  scene2view.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene2view2.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene2view3.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene2view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
	  Engine::ChangeScene((Scene*)&level3);
  } 
  else if (!player->isAlive()) 
  {
    level.stop();
    Engine::ChangeScene((Scene*)&gameOver);
  }

  Scene::Update(dt);
}

void Level2Scene::Render() 
{
  Engine::GetWindow().setView(scene2view2);
    
  for (int i = 5; i > -1; i--)
  {
      Engine::GetWindow().draw(bckSprites2[i]);
  }
  
  Engine::GetWindow().setView(scene2view);

  ls::render(Engine::GetWindow());

  Engine::GetWindow().setView(scene2view3);

  //draw hud
  Engine::GetWindow().draw(HUDbgs);
  Engine::GetWindow().draw(hpBarS);
  Engine::GetWindow().draw(essBarS);
  Engine::GetWindow().draw(HUDs);
  Engine::GetWindow().draw(scoreT);
  
  Engine::GetWindow().setView(scene2view);

  Scene::Render();
  
}
