#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_spritesheet.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <SFML/Audio.hpp>
#include <LevelSystem.h>
#include <list>


using namespace std;
using namespace sf;

//sounds
SoundBuffer buff;
SoundBuffer buff2;
SoundBuffer buff3;
Sound music;
Sound gameOverMusic;
Sound victoryMusic;

Texture mainMenu;
Sprite menuGFX;

static Texture tex;

Texture gameOverImg;
Sprite gameOverGFX;

Texture helpMenuImg;
Sprite helpMenuGFX;

Texture victoryImg;
Sprite victoryGFX;

//screen dimensions
const int static screenWidth = 1920;
const int static screenHeight = 1080;

//views
static View sceneview;

static ifstream scores;
static string line;
static vector<string> topScores;
static vector<Text> names;
static vector<string> points;
static list<string> scorelist;

static Font font;

static Vector2f scoreTop;

void MenuScene::Load() {
  cout << "Menu Load \n";
  {
	  //load main menu screen
	  mainMenu.loadFromFile("res/mainMenu.png");
	  menuGFX.setTexture(mainMenu);
  }
  if (!buff.loadFromFile("res/music/menu.ogg"))
  {
      cout << "Couldn't load menu music!" << endl;
  }


  {
	  auto button1 = makeEntity();
	  auto anim = button1->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
	  tex.loadFromFile("res/keys/key1.png");
	  anim->setSpritesheet(tex);
	  anim->setFrameCount(2);
	  anim->setFrameTime(0.6f);
	  button1->setPosition(Vector2f(500.f, 360.f));
  }

  {
	  auto button2 = makeEntity();
	  auto anim = button2->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
	  tex.loadFromFile("res/keys/key2.png");
	  anim->setSpritesheet(tex);
	  anim->setFrameCount(2);
	  anim->setFrameTime(0.6f);
	  button2->setPosition(Vector2f(500.f, 580.f));
  }

  {
	  auto button3 = makeEntity();
	  auto anim = button3->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
	  tex.loadFromFile("res/keys/escape.png");
	  anim->setSpritesheet(tex);
	  anim->setFrameCount(2);
	  anim->setFrameTime(0.6f);
	  button3->setPosition(Vector2f(500.f, 800.f));
  }



  music.setBuffer(buff);
  music.play();
  music.setLoop(true);
  setLoaded(true);
}

void MenuScene::Update(const double& dt) {
  // cout << "Menu Update "<<dt<<"\n";

  if (sf::Keyboard::isKeyPressed(Keyboard::Num1)) {
      music.stop();
	  Engine::ChangeScene(&victory);
  }

  if (sf::Keyboard::isKeyPressed(Keyboard::Num2)) {
	  music.stop();
	  Engine::ChangeScene(&help);
  }

  if (sf::Keyboard::isKeyPressed(Keyboard::V)) {
	  music.stop();
	  Engine::ChangeScene(&victory);
  }

  Scene::Update(dt);
}

void MenuScene::Render()
{
	Engine::GetWindow().draw(menuGFX);


	Scene::Render();

}

void GameOver::Load() {

	sceneview.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	sceneview.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));

	if (!buff2.loadFromFile("res/music/gameOver.ogg"))
	{
		cout << "Couldn't load game Over music!" << endl;
	}

	{
		//load game over screen
		gameOverImg.loadFromFile("res/gameOverScreen.png");
		gameOverGFX.setTexture(gameOverImg);

        gameOverMusic.play();
	}

	{
		auto button1 = makeEntity();
		auto anim = button1->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
		tex.loadFromFile("res/keys/key1.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(2);
		anim->setFrameTime(0.6f);
		button1->setPosition(Vector2f(500.f, 360.f));
	}

	{
		auto button2 = makeEntity();
		auto anim = button2->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
		tex.loadFromFile("res/keys/key2.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(2);
		anim->setFrameTime(0.6f);
		button2->setPosition(Vector2f(500.f, 580.f));
	}

	{
		auto button3 = makeEntity();
		auto anim = button3->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
		tex.loadFromFile("res/keys/escape.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(2);
		anim->setFrameTime(0.6f);
		button3->setPosition(Vector2f(500.f, 800.f));
	}

	gameOverMusic.setBuffer(buff2);
	gameOverMusic.play();
	gameOverMusic.setLoop(true);

	setLoaded(true);
}

void GameOver::Update(const double& dt) {
	// cout << "Menu Update "<<dt<<"\n";

	if (sf::Keyboard::isKeyPressed(Keyboard::Num1)) {
        gameOverMusic.stop();
		Engine::ChangeScene(&victory);
	}

	if (sf::Keyboard::isKeyPressed(Keyboard::Num2)) {
		gameOverMusic.stop();

		if (remove("keepScore.txt") != 0)
			perror("Error deleting file");
		else
			puts("keepScore file successfully deleted");

		Engine::ChangeScene(&menu);
	}

	Scene::Update(dt);
}

void GameOver::Render()
{
	Engine::GetWindow().draw(gameOverGFX);

	Engine::GetWindow().setView(sceneview);

	Scene::Render();

}

void VictoryScene::Load() {

	sceneview.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	sceneview.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));

	if (!buff3.loadFromFile("res/music/victory.ogg"))
	{
		cout << "Couldn't load victory music!" << endl;
	}

	{
		//load game over screen
		victoryImg.loadFromFile("res/victoryScreen.png");
		victoryGFX.setTexture(victoryImg);

		victoryMusic.play();
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
		tex.loadFromFile("res/keys/keyB.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(2);
		anim->setFrameTime(0.6f);
		button->setPosition(Vector2f(80.f, 50.f));
	}

	if (!font.loadFromFile("res/fonts/Gameplay.ttf"))
	{
		cout << "Couldn't load font!" << endl;
	}	

	scores.open("Scores.txt");
	if (scores.is_open())
	{
		int i = 0;
		while (getline(scores, line))
		{
			cout << line << '\n';
			scorelist.push_back(line);
		}
		scores.close();
	}
	else cout << "Unable to open Scores file";

	int yPos = 200;

	scorelist.sort();
	string scoreT = "! High Scores !";
	scorelist.push_back(scoreT);
	scorelist.reverse();

	int place = 0;

	for (auto n : scorelist)
	{
		

		Text temp;

		temp.setFont(font);
		temp.setString(n);
		temp.setCharacterSize(50);
		if (place == 0)
		{
			temp.setFillColor(Color::Red);
		}
		else
		{
			temp.setFillColor(Color::White);
		}
		temp.setOutlineColor(Color::Black);
		temp.setOutlineThickness(5);
		temp.setPosition(960 - temp.getGlobalBounds().width / 2, yPos);

		names.push_back(temp);

		yPos += 75;
		place++;
	}

	scoreTop = names[0].getPosition() + Vector2f(0,-10);
	
	victoryMusic.setBuffer(buff3);
	victoryMusic.play();
	victoryMusic.setLoop(true);

	setLoaded(true);
}

void VictoryScene::Update(const double& dt) {	

	if (sf::Keyboard::isKeyPressed(Keyboard::B)) {
		victoryMusic.stop();

		//Remove the players current score
		if (remove("keepScore.txt") != 0)
			perror("Error deleting file");
		else
			puts("keepScore file successfully deleted");

		Engine::ChangeScene(&menu);
	}

	//Scroll scores up
	if (Keyboard::isKeyPressed(Keyboard::Up) && !Keyboard::isKeyPressed(Keyboard::Down) && names[0].getPosition().y < scoreTop.y)
	{
		for (int i = 0; i < names.size(); i ++)
		{
			names[i].setPosition(names[i].getPosition() + Vector2f(0, 500 * dt));
		}
	}

	//scroll scores down
	if (Keyboard::isKeyPressed(Keyboard::Down) && !Keyboard::isKeyPressed(Keyboard::Up) && names.back().getPosition().y > scoreTop.y + names[0].getGlobalBounds().height)
	{
		for (int i = 0; i < names.size(); i++)
		{
			names[i].setPosition(names[i].getPosition() + Vector2f(0, -500 * dt));
		}
	}

	Scene::Update(dt);
}

void VictoryScene::Render()
{
	Engine::GetWindow().draw(victoryGFX);

	for (auto r : names)
	{
		if (r.getPosition().y > scoreTop.y)
		{
			Engine::GetWindow().draw(r);
		}
	}

	Engine::GetWindow().setView(sceneview);

	Scene::Render();

}

void HelpScene::Load() {

	sceneview.reset(sf::FloatRect(0, 0, screenWidth, screenHeight));
	sceneview.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));

	{
		//load help screen
		helpMenuImg.loadFromFile("res/helpMenu.png");
		helpMenuGFX.setTexture(helpMenuImg);
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
		tex.loadFromFile("res/keys/keyLeft.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(2);
		anim->setFrameTime(0.6f);
		button->setPosition(Vector2f(90.f, 380.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
		tex.loadFromFile("res/keys/keyRight.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(2);
		anim->setFrameTime(0.6f);
		button->setPosition(Vector2f(370.f, 380.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
		tex.loadFromFile("res/keys/keyUp.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(2);
		anim->setFrameTime(0.6f);
		button->setPosition(Vector2f(230.f, 270.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
		tex.loadFromFile("res/keys/keyDown.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(2);
		anim->setFrameTime(0.6f);
		button->setPosition(Vector2f(230.f, 380.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
		tex.loadFromFile("res/keys/escape.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(2);
		anim->setFrameTime(0.6f);
		button->setPosition(Vector2f(700.f, 320.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
		tex.loadFromFile("res/keys/keyB.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(2);
		anim->setFrameTime(0.6f);
		button->setPosition(Vector2f(1050.f, 320.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
		tex.loadFromFile("res/keys/keyV.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(2);
		anim->setFrameTime(0.6f);
		button->setPosition(Vector2f(1600.f, 320.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
		tex.loadFromFile("res/keys/keyB.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(2);
		anim->setFrameTime(0.6f);
		button->setPosition(Vector2f(80.f, 50.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
		tex.loadFromFile("res/keys/keyQ.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(2);
		anim->setFrameTime(0.6f);
		button->setPosition(Vector2f(90.f, 600.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(55.f, 55.f));
		tex.loadFromFile("res/attackOrbSheet.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(8);
		anim->setFrameTime(0.05f);
		button->setPosition(Vector2f(230.f, 600.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
		tex.loadFromFile("res/keys/keyW.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(2);
		anim->setFrameTime(0.6f);
		button->setPosition(Vector2f(690.f, 600.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(50.f, 50.f));
		tex.loadFromFile("res/beamSheet.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(8);
		anim->setFrameTime(0.05f);
		button->setPosition(Vector2f(830.f, 600.f));
	}
	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(50.f, 50.f));
		tex.loadFromFile("res/beamSheet.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(8);
		anim->setFrameTime(0.05f);
		button->setPosition(Vector2f(870.f, 600.f));
	}
	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(50.f, 50.f));
		tex.loadFromFile("res/beamSheet.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(8);
		anim->setFrameTime(0.05f);
		button->setPosition(Vector2f(910.f, 600.f));
	}
	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(50.f, 50.f));
		tex.loadFromFile("res/beamSheet.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(8);
		anim->setFrameTime(0.05f);
		button->setPosition(Vector2f(950.f, 600.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
		tex.loadFromFile("res/keys/keyE.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(2);
		anim->setFrameTime(0.6f);
		button->setPosition(Vector2f(1300.f, 600.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(100.f, 100.f));
		tex.loadFromFile("res/spell3Sheet.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(91);
		anim->setFrameTime(0.03f);
		button->setPosition(Vector2f(1450.f, 600.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
		tex.loadFromFile("res/keys/key1.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(2);
		anim->setFrameTime(0.6f);
		button->setPosition(Vector2f(90.f, 900.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(100.f, 100.f));
		tex.loadFromFile("res/familiarSheet.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(61);
		anim->setFrameTime(0.05f);
		button->setPosition(Vector2f(230.f, 900.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
		tex.loadFromFile("res/keys/key2.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(2);
		anim->setFrameTime(0.6f);
		button->setPosition(Vector2f(690.f, 900.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(150.f, 150.f));
		tex.loadFromFile("res/shieldSheet.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(8);
		anim->setFrameTime(0.09f);
		button->setPosition(Vector2f(880.f, 900.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(128.f, 128.f));
		tex.loadFromFile("res/keys/key3.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(2);
		anim->setFrameTime(0.6f);
		button->setPosition(Vector2f(1300.f, 900.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(55.f, 55.f));
		tex.loadFromFile("res/attackOrbSheet.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(8);
		anim->setFrameTime(0.05f);
		button->setPosition(Vector2f(1440.f, 850.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(55.f, 55.f));
		tex.loadFromFile("res/attackOrbSheet.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(8);
		anim->setFrameTime(0.05f);
		button->setPosition(Vector2f(1440.f, 900.f));
	}

	{
		auto button = makeEntity();
		auto anim = button->addComponent<SpriteSheetComponent>(Vector2f(55.f, 55.f));
		tex.loadFromFile("res/attackOrbSheet.png");
		anim->setSpritesheet(tex);
		anim->setFrameCount(8);
		anim->setFrameTime(0.05f);
		button->setPosition(Vector2f(1440.f, 950.f));
	}



	setLoaded(true);
}

void HelpScene::Update(const double& dt) {

	if (sf::Keyboard::isKeyPressed(Keyboard::B)) {
		Engine::ChangeScene(&menu);
	}

	Scene::Update(dt);
}

void HelpScene::Render()
{
	Engine::GetWindow().draw(helpMenuGFX);

	Engine::GetWindow().setView(sceneview);

	Scene::Render();

}


