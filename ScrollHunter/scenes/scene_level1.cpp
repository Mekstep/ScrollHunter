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
#include <fstream>
#include <string>
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

//hud and hud background
static Texture HUD2;
static Texture HUDbg2;
static Sprite HUDs2;
static Sprite HUDbgs2;

//health bar
static Sprite hpBarS;
static Texture hpBarT;
//mana bar
static Sprite essBarS;
static Texture essBarT;

//sound
static SoundBuffer buffer;
static Sound level;

View scene1view;
View scene1view1;
View scene1view3;

static Font font;
static Text scoreT;
static Text plName;
static string playerName;


static ofstream scoring;
static ofstream score;
static ifstream nameF;
static string line;


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

  //reset positions
  bckSprites1[0].setPosition(Vector2f(0, 0));
  bckSprites1[1].setPosition(Vector2f(0, 0));
  bckSprites1[2].setPosition(Vector2f(0, 0));
  bckSprites1[3].setPosition(Vector2f(0, 0));
  bckSprites1[4].setPosition(Vector2f(0, 0));
  bckSprites1[5].setPosition(Vector2f(0, 0));


  //Background
  // ***************************************************************************
  for (int i = 0; i < 6; i++)
  {
      if (!bckTextures1[i].loadFromFile("res/scene1/sky" + to_string(i + 1) + ".png"))
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

   //Score & Name
  //***************************************************
  if (!font.loadFromFile("res/fonts/Gameplay.ttf"))
  {
      cout << "Couldn't load font!" << endl;
  }

  player->scene->ents.find("player")[0]->setScore(0);

  scoreT.setFont(font);
  scoreT.setString("Score: " + to_string(player->scene->ents.find("player")[0]->getScore()));
  scoreT.setCharacterSize(50);
  scoreT.setFillColor(Color::White);
  scoreT.setOutlineColor(Color::Black);
  scoreT.setOutlineThickness(5);
  scoreT.setPosition(820, 1010);

  //Set name from name file
  nameF.open("PlayerName.txt");
  if (nameF.is_open())
  {
      while (getline(nameF, line))
      {
          plName.setString(line);
          playerName = line;
      }
      nameF.close();
  }
  else cout << "Unable to open PlayerName file";

  plName.setFont(font);  
  plName.setCharacterSize(50);
  plName.setFillColor(Color::White);
  plName.setOutlineColor(Color::Black);
  plName.setOutlineThickness(5);
  plName.setPosition(1320, 1010);
  //***************************************************

  //HP Bar & Essence & HUD
  //***********************************************
  {
      //hp
      hpBarT.loadFromFile("res/hp.png");
      hpBarS.setTexture(hpBarT);
      hpBarS.setScale(player->getHealth() / 10, 1);
	  hpBarS.setPosition(Vector2f(163.f, 995.f));
      //essence
      essBarT.loadFromFile("res/es.png");
      essBarS.setTexture(essBarT);
      essBarS.setScale(player->getEssence() / 10, 1);
	  essBarS.setPosition(Vector2f(164.f, 1041.f));

      HUD2.loadFromFile("res/HUD2.png");
      HUDs2.setTexture(HUD2);
      HUDbg2.loadFromFile("res/HUDbg2.png");
      HUDbgs2.setTexture(HUDbg2);
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
		  e->setVisible(false);
		  //auto s = e->addComponent<SpriteSheetComponent>(Vector2f(40.f, 40.f));
		  //s->setSpritesheet(templeTile1);
	  }
    // *********************************
  }

  auto invis = makeEntity();
  invis->addTag("enemy");

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
    //hp bar scaling
    hpBarS.setScale(player->getHealth() / 10, 1);
    //essence bar scaling
    essBarS.setScale(player->getEssence() / 10, 1);
    //Update Score
    scoreT.setString("Score: " + to_string(player->scene->ents.find("player")[0]->getScore()));

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
    }
    if (Keyboard::isKeyPressed(Keyboard::Left) && position.x > 0)
    {
        bckSprites1[0].move(Vector2f(350 * dt, 0));
        bckSprites1[1].move(Vector2f(300 * dt, 0));
        bckSprites1[2].move(Vector2f(250 * dt, 0));
        bckSprites1[3].move(Vector2f(200 * dt, 0));
        bckSprites1[4].move(Vector2f(150 * dt, 0));
        bckSprites1[5].move(Vector2f(100 * dt, 0));
    }
    //***********************************************************

  const auto pp = player->getPosition();

  if (ls::getTileAt(pp) == ls::END) 
  {
      //Save current score at end of level to file so it can be carried over to next scene
      //****************************************************************
      scoring.open("keepScore.txt");
      if (scoring.is_open())
      {
          scoring << player->scene->ents.find("player")[0]->getScore();
          scoring.close();
      }
      else cout << "Unable to open keepScore file";
      //****************************************************************

	  scene1view.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene1view1.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene1view3.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene1view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
	  Engine::ChangeScene((Scene*)&level2);
  } 
  else if (!player->isAlive()) 
  {
	  bckSprites1[0].setPosition(Vector2f(0, 0));
	  bckSprites1[1].setPosition(Vector2f(0, 0));
	  bckSprites1[2].setPosition(Vector2f(0, 0));
	  bckSprites1[3].setPosition(Vector2f(0, 0));
	  bckSprites1[4].setPosition(Vector2f(0, 0));
	  bckSprites1[5].setPosition(Vector2f(0, 0));

      //Save final score to Scores.txt if you die
      //*********************************************************************
      score.open("Scores.txt", std::ios_base::app);
      if (score.is_open())
      {
          score << playerName << ", " <<player->scene->ents.find("player")[0]->getScore() << "\n";
          score.close();
      }
      else cout << "Unable to open Scores file";
      //*********************************************************************
	  level.stop();
	  Engine::ChangeScene((Scene*)&gameOver);
  }

  if (sf::Keyboard::isKeyPressed(Keyboard::B)) {
	  Engine::ChangeScene(&menu);
  }

  if (sf::Keyboard::isKeyPressed(Keyboard::V)) {
	  Level1Scene::UnLoad();
	  Engine::ChangeScene(&level1);
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

  //draw hud
  Engine::GetWindow().draw(HUDbgs2);
  Engine::GetWindow().draw(hpBarS);
  Engine::GetWindow().draw(essBarS);
  Engine::GetWindow().draw(HUDs2);
  Engine::GetWindow().draw(scoreT); 
  Engine::GetWindow().draw(plName);

  Engine::GetWindow().setView(scene1view);

  Scene::Render();
  
}
