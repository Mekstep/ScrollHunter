#include "scene_level2.h"
#include "../components/cmp_enemy_ai.h"
#include "../components/cmp_enemy_turret.h"
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_player_physics.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

sf::Texture skele;
sf::Texture skeleArcher;
sf::Texture skeletChief;
sf::Sprite skeleton;

void Level2Scene::Load() {
  cout << "Scene 2 Load" << endl;
  ls::loadLevelFile("res/level_2.txt", 40.0f);

  //auto ho = Engine::getWindowSize().y - (ls::getHeight() * 40.f);
  //ls::setOffset(Vector2f(0, ho));

  // Create player
  {
    // *********************************
	player = makeEntity();
	player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	auto s = player->addComponent<ShapeComponent>();
	s->setShape<sf::RectangleShape>(Vector2f(20.f, 30.f));
	s->getShape().setFillColor(Color::Magenta);
	s->getShape().setOrigin(10.f, 15.f);
    // *********************************
    player->addTag("player");
    player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));
  }

  // Create Skeleton
  {
    auto skeleton = makeEntity();
    skeleton->setPosition(ls::getTilePosition(ls::findTiles('k')[0]) +
        Vector2f(20, 0));
    // *********************************
    // Add HurtComponent
	skeleton->addComponent<HurtComponent>();

    // Add ShapeComponent, Red 16.f Circle
    auto s = skeleton->addComponent<ShapeComponent>();
    s->setShape<sf::RectangleShape>(Vector2f(30.f, 50.f));
    skele.loadFromFile("res/skeleton.png");
    s->getShape().setTexture(&skele);
    s->getShape().setOrigin(10.f, 25.f);

    auto turret = makeEntity();
    turret->setPosition(skeleton->getPosition());
    //auto t = turret->addComponent<ShapeComponent>();
    //t->setShape<sf::CircleShape>(15.0f, 3);
    //t->getShape().setFillColor(Color::Red);
    //t->getShape().setOrigin(16.f, 16.f);

    // Add EnemyAIComponent
	skeleton->addComponent<EnemyAIComponent>();
    //turret->addComponent<EnemyTurretComponent>();
    //turret->addComponent<EnemyAIComponent>();
    // *********************************
  }

  // Create Skeleton Chief
  {
      auto skeleChief = makeEntity();
      skeleChief->setPosition(ls::getTilePosition(ls::findTiles('c')[0]) +
          Vector2f(20, 0));
      // *********************************
      // Add HurtComponent
      skeleChief->addComponent<HurtComponent>();
      // Add ShapeComponent, Red 16.f Circle
      auto s = skeleChief->addComponent<ShapeComponent>();
      s->setShape<sf::RectangleShape>(Vector2f(30.f, 50.f));


      skeletChief.loadFromFile("res/skeletonchief.png");
      s->getShape().setTexture(&skeletChief);
      s->getShape().setOrigin(10.f, 25.f);

      auto turret = makeEntity();
      turret->setPosition(skeleChief->getPosition());
      //auto t = turret->addComponent<ShapeComponent>();
      //t->setShape<sf::CircleShape>(15.0f, 3);
      //t->getShape().setFillColor(Color::Red);
      //t->getShape().setOrigin(16.f, 16.f);

      // Add EnemyAIComponent
      skeleChief->addComponent<EnemyAIComponent>();
      //turret->addComponent<EnemyTurretComponent>();
      //turret->addComponent<EnemyAIComponent>();
      // *********************************
  }

  // Create Skeleton Chief
  {
      auto skeleChief = makeEntity();
      skeleChief->setPosition(ls::getTilePosition(ls::findTiles('a')[0]) +
          Vector2f(20, 0));
      // *********************************
      // Add HurtComponent
      skeleChief->addComponent<HurtComponent>();
      // Add ShapeComponent, Red 16.f Circle
      auto s = skeleChief->addComponent<ShapeComponent>();
      s->setShape<sf::RectangleShape>(Vector2f(30.f, 50.f));


      skeleArcher.loadFromFile("res/skeletonarcher.png");
      s->getShape().setTexture(&skeleArcher);
      s->getShape().setOrigin(10.f, 25.f);

      auto turret = makeEntity();
      turret->setPosition(skeleChief->getPosition());
      //auto t = turret->addComponent<ShapeComponent>();
      //t->setShape<sf::CircleShape>(15.0f, 3);
      //t->getShape().setFillColor(Color::Red);
      //t->getShape().setOrigin(16.f, 16.f);

      // Add EnemyAIComponent
      skeleChief->addComponent<EnemyAIComponent>();
      //turret->addComponent<EnemyTurretComponent>();
      //turret->addComponent<EnemyAIComponent>();
      // *********************************
  }

  // Create Turret
  {
    auto turret = makeEntity();
    turret->setPosition(ls::getTilePosition(ls::findTiles('t')[0]) +
                        Vector2f(20, 0));
    auto s = turret->addComponent<ShapeComponent>();
    s->setShape<sf::RectangleShape>(Vector2f(30.f, 50.f));
    s->getShape().setFillColor(Color::Red);
    s->getShape().setOrigin(16.f, 16.f);
    turret->addComponent<EnemyTurretComponent>();
  }

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
	  }
    // *********************************
  }

  cout << " Scene 2 Load Done" << endl;
  setLoaded(true);
}

void Level2Scene::UnLoad() {
  cout << "Scene 2 UnLoad" << endl;
  player.reset();
  ls::unload();
  Scene::UnLoad();
}

void Level2Scene::Update(const double& dt) {
  Scene::Update(dt);
  const auto pp = player->getPosition();
  if (ls::getTileAt(pp) == ls::END) {
    Engine::ChangeScene((Scene*)&level3);
  } else if (!player->isAlive()) {
    Engine::ChangeScene((Scene*)&level2);
  }
}

void Level2Scene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
