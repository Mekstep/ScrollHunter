#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../components/cmp_sprite.h"
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

Texture gameOverImg;
Sprite gameOverGFX;


void MenuScene::Load() {
  cout << "Menu Load \n";
  {
	  //load main menu screen
	  mainMenu.loadFromFile("res/mainMenu.png");
	  menuGFX.setTexture(mainMenu);
		
	  //auto txt = makeEntity();
      //auto t = txt->addComponent<TextComponent>("Scroll Hunter\nPress Space to Start");
  }
  if (!buff.loadFromFile("res/menu.ogg"))
  {
      cout << "Couldn't load menu music!" << endl;
  }
  if (!buff2.loadFromFile("res/gameOver.ogg"))
  {
      cout << "Couldn't load game Over music!" << endl;
  }


  gameOverMusic.setBuffer(buff2);
  music.setBuffer(buff);
  music.play();
  music.setLoop(true);
  setLoaded(true);
}

void MenuScene::Update(const double& dt) {
  // cout << "Menu Update "<<dt<<"\n";

  if (sf::Keyboard::isKeyPressed(Keyboard::Space)) {
      music.stop();
    Engine::ChangeScene(&level2);
  }

  Scene::Update(dt);
}

void MenuScene::Render()
{
	Engine::GetWindow().draw(menuGFX);


	Scene::Render();

}

void GameOver::Load() {

	{
		//load game over screen
		gameOverImg.loadFromFile("res/gameOverScreen.png");
		gameOverGFX.setTexture(gameOverImg);

        gameOverMusic.play();
		//auto txt = makeEntity();
		//auto t = txt->addComponent<TextComponent>("GAME OVER!\nPress Space to retry!");
	}

	setLoaded(true);
}

void GameOver::Update(const double& dt) {
	// cout << "Menu Update "<<dt<<"\n";

	if (sf::Keyboard::isKeyPressed(Keyboard::Space)) {
        gameOverMusic.stop();
		Engine::ChangeScene(&level2);
	}

	Scene::Update(dt);
}

void GameOver::Render()
{
	Engine::GetWindow().draw(gameOverGFX);

	Scene::Render();

}


