#include "scene_menu.h"
#include "../components/cmp_text.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

SoundBuffer buff;
SoundBuffer buff2;
Sound music;
Sound gameOver;

void MenuScene::Load() {
  cout << "Menu Load \n";
  {
    auto txt = makeEntity();
    auto t = txt->addComponent<TextComponent>(
        "Scroll Hunter\nPress Space to Start");
  }
  if (!buff.loadFromFile("res/menu.ogg"))
  {
      cout << "Couldn't load menu music!" << endl;
  }
  if (!buff2.loadFromFile("res/gameOver.ogg"))
  {
      cout << "Couldn't load game Over music!" << endl;
  }
  gameOver.setBuffer(buff2);
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

void GameOver::Load() {

	{
        gameOver.play();
		auto txt = makeEntity();
		auto t = txt->addComponent<TextComponent>("GAME OVER!\nPress Space to retry!");
	}

	setLoaded(true);
}

void GameOver::Update(const double& dt) {
	// cout << "Menu Update "<<dt<<"\n";

	if (sf::Keyboard::isKeyPressed(Keyboard::Space)) {
        gameOver.stop();
		Engine::ChangeScene(&level2);
	}

	Scene::Update(dt);
}
