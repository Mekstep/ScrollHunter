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

//health bar
static Sprite hpBarS;
static Texture hpBarT;
//mana bar
static Sprite essBarS;
static Texture essBarT;

View scene3view;
View scene3view2;
View scene3view3;

Sprite bckSprites3[6];
Texture bckTextures3[6];

Texture templeTile;

static Font font;
static Text scoreT;
static Text plName;

static shared_ptr<Entity> player;

static ofstream scoring;
static ofstream score;
static ifstream chkScore;
static ifstream nameF;
static string line;

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
  else cout << "Unable to open file";

  player->scene->ents.find("player")[0]->setScore(stoi(line));

  scoreT.setFont(font);
  scoreT.setString("Score: " + to_string(player->scene->ents.find("player")[0]->getScore()));
  scoreT.setCharacterSize(50);
  scoreT.setFillColor(Color::Red);
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
	  }
	  nameF.close();
  }
  else cout << "Unable to open file";

  plName.setFont(font);
  plName.setCharacterSize(50);
  plName.setFillColor(Color::Red);
  plName.setOutlineColor(Color::Black);
  plName.setOutlineThickness(5);
  plName.setPosition(1520, 1010);
  //***************************************************

  //Make Boss
  auto boss = Enemies::makeBoss(this);


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

		  HUD2.loadFromFile("res/HUD.png");
		  HUDs2.setTexture(HUD2);
		  HUDbg2.loadFromFile("res/HUDbg.png");
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



void Level3Scene::Update(const double& dt) {

	//hp bar scaling
	hpBarS.setScale(player->getHealth() / 10, 1);
	//essence bar scaling
	essBarS.setScale(player->getEssence() / 20, 1);
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
	  //Save current score at end of level to file so it can be carried over to next scene
	  //****************************************************************
	  scoring.open("keepScore.txt");
	  if (scoring.is_open())
	  {
		  scoring << player->scene->ents.find("player")[0]->getScore();
		  scoring.close();
	  }
	  else cout << "Unable to open file";
	  //****************************************************************

	  scene3view.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene3view2.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene3view3.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	  scene3view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
	  Engine::ChangeScene((Scene*)&level1);
  } 
  else if (!player->isAlive()) 
  {
	  //Save final score to Scores.txt if you die
	  //*********************************************************************
	  score.open("Scores.txt", std::ios_base::app);
	  if (score.is_open())
	  {
		  score << player->scene->ents.find("player")[0]->getScore() << "\n";
		  score.close();
	  }
	  else cout << "Unable to open file";
	  //*********************************************************************

	  //Remove the keep score file if you die
	  if (remove("keepScore.txt") != 0)
		  perror("Error deleting file");
	  else
		  puts("File successfully deleted");

	  //Remove player name file if you die
	  if (remove("PlayerName.txt") != 0)
		  perror("Error deleting file");
	  else
		  puts("File successfully deleted");

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
	Engine::GetWindow().draw(HUDbgs2);
	Engine::GetWindow().draw(hpBarS);
	Engine::GetWindow().draw(essBarS);
	Engine::GetWindow().draw(HUDs2);
	Engine::GetWindow().draw(scoreT);
	Engine::GetWindow().draw(plName);

	Engine::GetWindow().setView(scene3view);

	Scene::Render();
}
