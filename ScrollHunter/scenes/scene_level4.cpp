#include "scene_level4.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_player_physics.h"
#include "../game.h"
#include "../components/cmp_bullet.h"
#include "../Player.h"
#include "../EnemyFactory.h"
#include "../components/cmp_spritesheet.h"
#include <LevelSystem.h>
#include <iostream>
#include <fstream>
#include <string>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

//ScreenBounds
const int static screenWidth = 1920;
const int static screenHeight = 1080;

//hud and hud background
static Texture HUD2;
static Texture HUDbg2;
static Sprite HUDs2;
static Sprite HUDbgs2;

static Texture tex;

//sound
static SoundBuffer buffer;
static Sound level;

//health bar
static Sprite hpBarS;
static Texture hpBarT;
//mana bar
static Sprite essBarS;
static Texture essBarT;

//Views
static View scene3view;
static View scene3view2;
static View scene3view3;

//Backgrounds
static Sprite bckSprites3[4];
static Texture bckTextures3[4];
static Sprite bckSprites1[6];
static Texture bckTextures1[6];

//Player name
static Font font;
static Text scoreT;
static Text plName;
static string playerName;

//Player reference
static shared_ptr<Entity> player;

//Files for holding/checking scores and playername
static ofstream scoring;
static ofstream score;
static ifstream chkScore;
static ifstream nameF;
static string line;

static float timer = 0.1f;
static bool shake = true;
static Vector2f tempPos;


void Level4Scene::Load() {
  cout << "Scene 4 Load" << endl;
  ls::loadLevelFile("res/level_4.txt", 40.0f);

  //Set Viewports for scrolling screen
  scene3view.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene3view2.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene3view3.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
  scene3view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));

  //reset positions
  bckSprites3[0].setPosition(Vector2f(0, 0));
  bckSprites3[1].setPosition(Vector2f(0, 0));
  bckSprites3[2].setPosition(Vector2f(0, 0));
  bckSprites3[3].setPosition(Vector2f(0, 0));

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

  for (int i = 0; i < 4; i++)
  {
	  if (!bckTextures3[i].loadFromFile("res/finalScene/temple" + to_string(i + 1) + ".png"))
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

  //Score & Name
  //***************************************************
  if (!font.loadFromFile("res/fonts/Gameplay.ttf"))
  {
	  cout << "Couldn't load font!" << endl;
  }

  //Get score from last level
  chkScore.open("keepScore.txt");
  if (chkScore.is_open())
  {
	  while (getline(chkScore, line))
	  {
		  cout << line << '\n';
	  }
	  chkScore.close();
  }
  else cout << "Unable to open keepScore file";

  player->scene->ents.find("player")[0]->setScore(stoi(line));

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
  plName.setPosition(1520, 1010);

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

	  auto exit = ls::findTiles(ls::END);
	  for (auto e : exit)
	  {
		  auto pos = ls::getTilePosition(e);
		  pos += Vector2f(20.0f, 20.0f); // offset to centre
		  auto et = makeEntity();
		  et->setPosition(pos);
		  auto s = et->addComponent<SpriteSheetComponent>(Vector2f(40.f, 40.f));
		  tex.loadFromFile("res/arrowTile.png");
		  s->setSpritesheet(tex);
	  }
	  // *********************************
  }



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
	  if (!buffer.loadFromFile("res/music/level4.ogg"))
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

	  auto invis = makeEntity();
	  invis->addTag("enemy");

  cout << " Scene 3 Load Done" << endl;
  setLoaded(true);
}

void Level4Scene::UnLoad() {
  cout << "Scene 3 UnLoad" << endl;
  player.reset();
  ls::unload();
  Scene::UnLoad();
}



void Level4Scene::Update(const double& dt) {

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

	scene3view.reset(FloatRect(position.x, 0, screenWidth, screenHeight));
	//*****************************************************


	//Background Speeds
	//***********************************************************

	/*
	if (shake == true)
	{
		bckSprites3[0].move(Vector2f(-700 * dt, 0));
		bckSprites3[1].move(Vector2f(-600 * dt, 0));
		bckSprites3[2].move(Vector2f(-500 * dt, 0));
		bckSprites3[3].move(Vector2f(-400 * dt, 0));
	}
	else
	{
		bckSprites3[0].move(Vector2f(700 * dt, 0));
		bckSprites3[1].move(Vector2f(600 * dt, 0));
		bckSprites3[2].move(Vector2f(500 * dt, 0));
		bckSprites3[3].move(Vector2f(400 * dt, 0));
	}

	if (bckSprites3[0].getPosition().x < tempPos.x - 5)
	{
		tempPos = bckSprites3[0].getPosition();
		shake = false;
	}

	if (bckSprites3[0].getPosition().x > tempPos.x + 5)
	{
		tempPos = bckSprites3[0].getPosition();
		shake = true;
	}

	*/

	if (Keyboard::isKeyPressed(Keyboard::Right) && position.x > 0)
	{
		bckSprites3[0].move(Vector2f(-350 * dt, 0));
		bckSprites3[1].move(Vector2f(-300 * dt, 0));
		bckSprites3[2].move(Vector2f(-250 * dt, 0));
		bckSprites3[3].move(Vector2f(-200 * dt, 0));

		bckSprites1[0].move(Vector2f(-350 * dt, 0));
		bckSprites1[1].move(Vector2f(-300 * dt, 0));
		bckSprites1[2].move(Vector2f(-250 * dt, 0));
		bckSprites1[3].move(Vector2f(-200 * dt, 0));
		bckSprites1[4].move(Vector2f(-150 * dt, 0));
		bckSprites1[5].move(Vector2f(-100 * dt, 0));
	}
	if (Keyboard::isKeyPressed(Keyboard::Left) && position.x > 0)
	{
		bckSprites3[0].move(Vector2f(350 * dt, 0));
		bckSprites3[1].move(Vector2f(300 * dt, 0));
		bckSprites3[2].move(Vector2f(250 * dt, 0));
		bckSprites3[3].move(Vector2f(200 * dt, 0));

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
	  level.stop();
	  //Save players score once they complete game
	  //*********************************************************************
	  score.open("Scores.txt", std::ios_base::app);
	  if (score.is_open())
	  {
		  score << player->scene->ents.find("player")[0]->getScore() << " : " << playerName << "\n";
		  score.close();
	  }
	  else cout << "Unable to open Scores file";
	  //*********************************************************************

	  scene3view.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene3view2.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene3view3.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene3view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
	  Engine::ChangeScene((Scene*)&victory);
  } 
  else if (!player->isAlive()) 
  {
	  level.stop();
	  bckSprites3[0].setPosition(Vector2f(0, 0));
	  bckSprites3[1].setPosition(Vector2f(0, 0));
	  bckSprites3[2].setPosition(Vector2f(0, 0));
	  bckSprites3[3].setPosition(Vector2f(0, 0));

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
		  score << player->scene->ents.find("player")[0]->getScore() << " : " << playerName << "\n";
		  score.close();
	  }
	  else cout << "Unable to open Scores file";
	  //*********************************************************************


	  Engine::ChangeScene((Scene*)&gameOver);
  } 

  if (sf::Keyboard::isKeyPressed(Keyboard::B)) {
	  //Remove the players current score
	  if (remove("keepScore.txt") != 0)
		  perror("Error deleting file");
	  else
		  puts("keepScore file successfully deleted");
	  level.stop();
	  Engine::ChangeScene(&menu);
  }

  if (sf::Keyboard::isKeyPressed(Keyboard::V)) {
	  //Remove the players current score
	  if (remove("keepScore.txt") != 0)
		  perror("Error deleting file");
	  else
		  puts("keepScore file successfully deleted");
	  level.stop();
	  Level4Scene::UnLoad();
	  Engine::ChangeScene(&level1);
  }

  Scene::Update(dt);
  
}

void Level4Scene::Render() {
	Engine::GetWindow().setView(scene3view2);

	//Draw Background Sprites
	for (int i = 5; i > -1; i--)
	{
		Engine::GetWindow().draw(bckSprites1[i]);
	}
	for (int i = 3; i > -1; i--)
	{
		Engine::GetWindow().draw(bckSprites3[i]);
	}

	Engine::GetWindow().setView(scene3view);

	ls::render(Engine::GetWindow());

	Engine::GetWindow().setView(scene3view3);

	//draw hud
	Engine::GetWindow().draw(HUDbgs2);
	Engine::GetWindow().draw(hpBarS);
	Engine::GetWindow().draw(essBarS);
	Engine::GetWindow().draw(HUDs2);
	Engine::GetWindow().draw(scoreT);
	Engine::GetWindow().draw(plName);

	Engine::GetWindow().setView(scene3view);

	Scene::Render();
}
