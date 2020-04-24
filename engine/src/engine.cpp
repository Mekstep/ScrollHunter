#include "engine.h"
#include "maths.h"
#include "ecm.h"
#include "system_physics.h"
#include "system_renderer.h"
#include "system_resources.h"
#include <SFML/Graphics.hpp>
#include <future>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <conio.h>

using namespace sf;
using namespace std;
Scene* Engine::_activeScene = nullptr;
std::string Engine::_gameName;

static bool loading = false;
static float loadingspinner = 0.f;
static float loadingTime;
static RenderWindow* _window;

string playerName;
Text playerText;

static Font font;
static Text plName;

static ofstream nameFile;
static string line;

//views
static View sceneview;

void Loading_update(float dt, const Scene* const scn) {
  //  cout << "Eng: Loading Screen\n";
  if (scn->isLoaded()) {
    cout << "Eng: Exiting Loading Screen\n";
    loading = false;
  } else {
    loadingspinner += 220.0f * dt;
    loadingTime += dt;
  }
}
void Loading_render() {
  // cout << "Eng: Loading Screen Render\n";
  sceneview.reset(sf::FloatRect(0, 0, 1920, 1080));
  sceneview.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
  static CircleShape octagon(80, 8);
  octagon.setOrigin(80, 80);
  octagon.setRotation(loadingspinner);
  octagon.setPosition(Vcast<float>(Engine::getWindowSize()) * .5f);
  octagon.setFillColor(Color(255,255,255,min(255.f,40.f*loadingTime)));
  static Text t("Loading", *Resources::get<sf::Font>("RobotoMono-Regular.ttf"));
  t.setFillColor(Color(255,255,255,min(255.f,40.f*loadingTime)));
  t.setPosition(Vcast<float>(Engine::getWindowSize()) * Vector2f(0.4f,0.3f));
  _window->setView(sceneview);
  Renderer::queue(&t);
  Renderer::queue(&octagon);
}

float frametimes[256] = {};
uint8_t ftc = 0;

void Engine::Update() 
{

  static sf::Clock clock;
  float dt = clock.restart().asSeconds();
  {
    frametimes[++ftc] = dt;
    static string avg = _gameName + " FPS:";
    if (ftc % 60 == 0) {
      double davg = 0;
      for (const auto t : frametimes) {
        davg += t;
      }
      davg = 1.0 / (davg / 255.0);
      _window->setTitle(avg + toStrDecPt(2, davg));
    }
  }
  
  if (loading) {
    Loading_update(dt, _activeScene);
  } else if (_activeScene != nullptr) {
    Physics::update(dt);
    _activeScene->Update(dt);
  }
}

void Engine::Render(RenderWindow& window) {
    
  if (loading) {
    Loading_render();
  } else if (_activeScene != nullptr) {     
    _activeScene->Render();
  }  
  Renderer::render();
  
}

void Engine::Start(unsigned int width, unsigned int height,
                   const std::string& gameName, Scene* scn) {
  RenderWindow window(VideoMode(width, height), gameName, Style::Fullscreen);
  _gameName = gameName;
  _window = &window;
  Renderer::initialise(window);
  Physics::initialise();
  ChangeScene(scn);

  if (!font.loadFromFile("res/fonts/Gameplay.ttf"))
  {
      cout << "Couldn't load font!" << endl;
  }

  plName.setFont(font);
  plName.setString("Type your name and hit enter");
  plName.setCharacterSize(20);
  plName.setPosition(1920 / 2 - 190, 1080 / 2);
  
  //Set up Initial window to ask for player name
  //****************************************************************
  sf::RenderWindow window2(sf::VideoMode(1920, 1080), "Text reader!", Style::Fullscreen);

  while (window2.isOpen()) {
      Event event;
      while (window2.pollEvent(event)) {
          switch (event.type)
          {
          case sf::Event::Closed:
              window2.close();

              break;

          case sf::Event::TextEntered:

              //Add userinput to playername string
              playerName += event.text.unicode;
              playerText.setString(playerName);
              playerText.setColor(Color::White);
              playerText.setPosition(0, 250);
              cout << playerName << endl;

              //Save player name to file and pass to main menu
              //*******************************************
              if (Keyboard::isKeyPressed(Keyboard::Enter)) 
              {
                  nameFile.open("PlayerName.txt");
                  if (nameFile.is_open())
                  {
                      nameFile << playerName;
                      nameFile.close();
                  }
                  else cout << "Unable to open file";
                  

                  window2.close();
              }
              //*******************************************

              break;
          }


      }
      if (Keyboard::isKeyPressed(Keyboard::Escape)) {
          window2.close();
      }

      window2.clear();     
      Update();      
      Render(window2);      
      window2.draw(plName);
      window2.draw(playerText);
      window2.display();
      
  }
  //****************************************************************

  sf::Vector2f position(0,0);

  while (window.isOpen()) {
    Event event;
    while (window.pollEvent(event)) {
        switch (event.type)
        {
        case sf::Event::Closed:
            if (remove("PlayerName.txt") != 0)
                perror("Error deleting file");
            else
                puts("File successfully deleted");
            window.close();

            break;
        }


    }
    if (Keyboard::isKeyPressed(Keyboard::Escape)) 
    {
        if (remove("PlayerName.txt") != 0)
            perror("Error deleting file");
        else
            puts("File successfully deleted");
      window.close();
    }

    window.clear();
    Update();
    Render(window);
    window.display();
  }
  if (_activeScene != nullptr) {
    _activeScene->UnLoad();
    _activeScene = nullptr;
  }
  window.close();
  Physics::shutdown();
  // Render::shutdown();
}

std::shared_ptr<Entity> Scene::makeEntity() {
  auto e = make_shared<Entity>(this);
  ents.list.push_back(e);
  return std::move(e);
}

void Engine::setVsync(bool b) { _window->setVerticalSyncEnabled(b); }

void Engine::ChangeScene(Scene* s) {
  cout << "Eng: changing scene: " << s << endl;
  auto old = _activeScene;
  _activeScene = s;

  if (old != nullptr) {
    old->UnLoad(); // todo: Unload Async
  }

  if (!s->isLoaded()) {
    cout << "Eng: Entering Loading Screen\n";
    loadingTime =0;
    _activeScene->LoadAsync();
	//_activeScene->Load();
    loading = true;
  }
}

void Scene::Update(const double& dt) { ents.update(dt); }

void Scene::Render() { ents.render(); }

bool Scene::isLoaded() const {
  {
    std::lock_guard<std::mutex> lck(_loaded_mtx);
    // Are we already loading asynchronously?
    if (_loaded_future.valid() // yes
        &&                     // Has it finished?
        _loaded_future.wait_for(chrono::seconds(0)) ==
            future_status::ready) {
      // Yes
      _loaded_future.get();
      _loaded = true;
    }
    return _loaded;
  }
}
void Scene::setLoaded(bool b) {
  {
    std::lock_guard<std::mutex> lck(_loaded_mtx);
    _loaded = b;
  }
}

void Scene::UnLoad() {
  ents.list.clear();
  setLoaded(false);
}

void Scene::LoadAsync() { _loaded_future = std::async(&Scene::Load, this); }

sf::Vector2u Engine::getWindowSize() { return _window->getSize(); }

sf::RenderWindow& Engine::GetWindow() { return *_window; }

namespace timing {
// Return time since Epoc
long long now() {
  return std::chrono::high_resolution_clock::now()
      .time_since_epoch()
      .count();
}
// Return time since last() was last called.
long long last() {
  auto n = now();
  static auto then = now();
  auto dt = n - then;
  then = n;
  return dt;
}
} // namespace timing

Scene::~Scene() { UnLoad(); }