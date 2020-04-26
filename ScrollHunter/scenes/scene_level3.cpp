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
#include <fstream>
#include <string>
using namespace std;
using namespace sf;

const int static screenWidth = 1920;
const int static screenHeight = 1080;

//hud and hud background
static Texture HUD2;
static Texture HUDbg2;
static Sprite HUDs2;
static Sprite HUDbgs2;

static Texture tex;

shared_ptr<Entity> pedestal;
static Texture ped;

shared_ptr<Entity> scroll;
static Texture scrollT;

//health bar
static Sprite hpBarS;
static Texture hpBarT;
//mana bar
static Sprite essBarS;
static Texture essBarT;

View scene3view;
View scene3view2;
View scene3view3;

Sprite bckSprites3[4];
Texture bckTextures3[4];

Texture templeTile;

static Font font;
static Text scoreT;
static Text plName;
static string playerName;

static shared_ptr<Entity> player;
static shared_ptr<Entity> boss;

static ofstream scoring;
static ofstream score;
static ifstream chkScore;
static ifstream nameF;
static string line;

static float timer = 0.1f;

//Boss Patterns
static Vector2f bossPosOrigin = Vector2f(2460,510);
static Vector2f bossPos1 = Vector2f(1800, 510);
static Vector2f bossPos2 = Vector2f(1800, 270);
static Vector2f bossPos3 = Vector2f(1800, 748);
static Vector2f bossPos4 = Vector2f(2460, 270);
static Vector2f bossPos5 = Vector2f(2460, 748);
static int bossPattern = 1;
static bool bossReturn = false;
static bool startBoss = false;

static bool wallsLoaded = false;

void Level3Scene::Load() {
  cout << "Scene 3 Load" << endl;
  ls::loadLevelFile("res/level_3.txt", 40.0f);
  templeTile.loadFromFile("res/templeTile.png");

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


  //Background
// ***************************************************************************
  for (int i = 0; i < 4; i++)
  {
	  if (!bckTextures3[i].loadFromFile("res/temple/temple" + to_string(i + 1) + ".png"))
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

  //Score
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
  //***************************************************

  //Make Boss
  boss = Enemies::makeBoss(this);

  auto invis = makeEntity();
  invis->addTag("enemy");

  //Pedestal
  pedestal = makeEntity();
  pedestal->setPosition(Vector2f(3550,550));
  auto s = pedestal->addComponent<ShapeComponent>();
  s->setShape<sf::RectangleShape>(Vector2f(840.f, 660.f));
  ped.loadFromFile("res/pedestal.png");
  s->getShape().setTexture(&ped);
  s->getShape().setOrigin(420.f, 380.f);

  //Scroll
  scroll = makeEntity();
  scroll->setPosition(Vector2f(3933, 1050));
  auto s2 = scroll->addComponent<ShapeComponent>();
  s2->setShape<sf::RectangleShape>(Vector2f(50.f, 50.f));
  scrollT.loadFromFile("res/scroll.png");
  s2->getShape().setTexture(&scrollT);
  s2->getShape().setOrigin(420.f, 380.f);
  scroll->addTag("scroll"); 


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



void Level3Scene::Update(const double& dt) 
{
	//Start boss
	if (player->getPosition().x > 1000)
	{
		startBoss = true;
	}


	//Boss Patterns
	//**********************************************************************************
	{
		if (startBoss == true)
		{
			if (bossPattern == 1 && bossReturn == false)
			{
				timer -= dt;
				if (timer <= 0)
				{
					boss->Move(bossPos1);
					if (length(bossPos1 - boss->getPosition()) < 10.0)
					{
						bossReturn = true;
						timer = 5.0f;
					}
				}
			}

			if (bossPattern == 2 && bossReturn == false)
			{
				timer -= dt;
				if (timer <= 0)
				{
					boss->Move(bossPos2);
					if (length(bossPos2 - boss->getPosition()) < 10.0)
					{
						bossReturn = true;
						timer = 5.0f;
					}
				}
			}

			if (bossPattern == 3 && bossReturn == false)
			{
				timer -= dt;
				if (timer <= 0)
				{
					boss->Move(bossPos3);
					if (length(bossPos3 - boss->getPosition()) < 10.0)
					{
						bossReturn = true;
						timer = 5.0f;
					}
				}
			}

			if (bossPattern == 4 && bossReturn == false)
			{
				timer -= dt;
				if (timer <= 0)
				{
					boss->Move(bossPos4);
					if (length(bossPos4 - boss->getPosition()) < 10.0)
					{
						bossReturn = true;
						timer = 5.0f;
					}
				}
			}

			if (bossPattern == 5 && bossReturn == false)
			{
				timer -= dt;
				if (timer <= 0)
				{
					boss->Move(bossPos5);
					if (length(bossPos5 - boss->getPosition()) < 10.0)
					{
						bossReturn = true;
						timer = 5.0f;
					}
				}
			}

			if (bossReturn == true)
			{
				timer -= dt;
				if (timer <= 0)
				{
					boss->Move(bossPosOrigin);
					if (length(bossPosOrigin - boss->getPosition()) < 10.0)
					{
						bossPattern = rand() % 5 + 1;
						bossReturn = false;
						timer = 5.0f;
						if (bossPattern > 5)
						{
							bossPattern = 1;
						}
					}
				}
			}
		}		
	}
	//**********************************************************************************

	if (boss->isAlive() == false)
	{
		pedestal->setPosition(Vector2f(3500, 550));
		scroll->setPosition(Vector2f(3883, 1050));

		for (auto w : player->scene->ents.find("wall"))
		{
			w->setForDelete();
			templeTile.loadFromFile("res/templeTile.png");
		}

		ls::loadLevelFile("res/level_3_2.txt", 40.0f);

		if (wallsLoaded == false)
		{
			auto walls = ls::findTiles(ls::WALL);
			for (auto w : walls) {
				auto pos = ls::getTilePosition(w);
				pos += Vector2f(20.0f, 20.0f); // offset to centre
				auto e = makeEntity();
				e->setPosition(pos);
				e->addComponent<PhysicsComponent>(false, Vector2f(40.0f, 40.0f));
				e->addTag("wall2");
				auto s = e->addComponent<SpriteSheetComponent>(Vector2f(40.f, 40.f));
				s->setSpritesheet(templeTile);
			}
			wallsLoaded = true;
		}

		auto plPosition = player->scene->ents.find("player")[0]->getPosition();

		if (player->getPosition().x >= 2950)
		{
			player->scene->ents.find("player")[0]->GetCompatibleComponent<PlayerPhysicsComponent>()[0]->teleport(plPosition);
		}

		if (length(Vector2f(3490, 700) - player->getPosition()) < 25)
		{			
			scroll->setForDelete();
		}
	}
	

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
	if (player->getPosition().x < 2950)
	{
		if (Keyboard::isKeyPressed(Keyboard::Right) && position.x > 0)
		{
			bckSprites3[0].move(Vector2f(-350 * dt, 0));
			bckSprites3[1].move(Vector2f(-300 * dt, 0));
			bckSprites3[2].move(Vector2f(-250 * dt, 0));
			bckSprites3[3].move(Vector2f(-200 * dt, 0));
		}
		if (Keyboard::isKeyPressed(Keyboard::Left) && position.x > 0)
		{
			bckSprites3[0].move(Vector2f(350 * dt, 0));
			bckSprites3[1].move(Vector2f(300 * dt, 0));
			bckSprites3[2].move(Vector2f(250 * dt, 0));
			bckSprites3[3].move(Vector2f(200 * dt, 0));
		}
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

	  scene3view.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene3view2.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene3view3.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene3view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
	  Engine::ChangeScene((Scene*)&finalScene);
  } 
  else if (!player->isAlive()) 
  {
	  bckSprites3[0].setPosition(Vector2f(0, 0));
	  bckSprites3[1].setPosition(Vector2f(0, 0));
	  bckSprites3[2].setPosition(Vector2f(0, 0));
	  bckSprites3[3].setPosition(Vector2f(0, 0));

	  //Save final score to Scores.txt if you die
	  //*********************************************************************
	  score.open("Scores.txt", std::ios_base::app);
	  if (score.is_open())
	  {
		  score << playerName << ", " << player->scene->ents.find("player")[0]->getScore() << "\n";
		  score.close();
	  }
	  else cout << "Unable to open Scores file";
	  //*********************************************************************


	  Engine::ChangeScene((Scene*)&gameOver);
  } 

  if (sf::Keyboard::isKeyPressed(Keyboard::B)) {
	  Engine::ChangeScene(&menu);
  }

  if (sf::Keyboard::isKeyPressed(Keyboard::V)) {
	  Level3Scene::UnLoad();
	  Engine::ChangeScene(&level1);
  }

  Scene::Update(dt);
  
}

void Level3Scene::Render() {
	Engine::GetWindow().setView(scene3view2);

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
