#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_spritesheet.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <SFML/Audio.hpp>
#include <LevelSystem.h>


using namespace std;
using namespace sf;

SoundBuffer buff;
SoundBuffer buff2;
Sound music;
Sound gameOverMusic;

Texture mainMenu;
Sprite menuGFX;

static Texture tex;

Texture gameOverImg;
Sprite gameOverGFX;

Texture helpMenuImg;
Sprite helpMenuGFX;

//screen dimensions
const int static screenWidth = 1920;
const int static screenHeight = 1080;

//views
static View sceneview;



void MenuScene::Load() {
  cout << "Menu Load \n";
  {
	  //load main menu screen
	  mainMenu.loadFromFile("res/mainMenu.png");
	  menuGFX.setTexture(mainMenu);
  }
  if (!buff.loadFromFile("res/menu.ogg"))
  {
      cout << "Couldn't load menu music!" << endl;
  }
  if (!buff2.loadFromFile("res/gameOver.ogg"))
  {
      cout << "Couldn't load game Over music!" << endl;
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
  music.setBuffer(buff);
  music.play();
  music.setLoop(true);
  setLoaded(true);
}

void MenuScene::Update(const double& dt) {
  // cout << "Menu Update "<<dt<<"\n";

  if (sf::Keyboard::isKeyPressed(Keyboard::Num1)) {
      music.stop();
	  Engine::ChangeScene(&level1);
  }

  if (sf::Keyboard::isKeyPressed(Keyboard::Num2)) {
	  music.stop();
	  Engine::ChangeScene(&help);
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

	setLoaded(true);
}

void GameOver::Update(const double& dt) {
	// cout << "Menu Update "<<dt<<"\n";

	if (sf::Keyboard::isKeyPressed(Keyboard::Num1)) {
        gameOverMusic.stop();
		Engine::ChangeScene(&level1);
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


