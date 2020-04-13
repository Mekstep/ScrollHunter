#include "scene_level2.h"
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
#include <LevelSystem.h>
#include <iostream>
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;


Texture skele;
Texture skeletArcher;
Texture skeletChief;
Sprite skeleton;

Texture bckgT;
Sprite bckgS;

View view;

GameOver gameOver;

//ability cooldowns start at max value so you can cast right away
float qCooldown = 50.f;
float qCooldownMax = 50.f;
float wCooldown = 300.f;
float wCooldownMax = 300.f;
float eCooldown = 300.f;
float eCooldownMax = 300.f;

void Level2Scene::Load() {
  cout << "Scene 2 Load" << endl;
  ls::loadLevelFile("res/level_2.txt", 40.0f);

  //Set Viewport for scrolling screen
  view.reset(sf::FloatRect(0, 0, 1920, 1080));
  view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));

  //Background
  // *********************************
  if (!bckgT.loadFromFile("res/back.jpg"))
  {
      cout << "Couldn't load Background!" << endl;
  }
  bckgS.setTexture(bckgT);
  bckgS.setScale(1, 1);
  // *********************************

  // Create player
  {
  // *********************************
	player = Player::makePlayer(this, (ls::getTilePosition(ls::findTiles(ls::START)[0])));

	player->addTag("player");
	player->addComponent<PlayerMovementComponent>();
	player->setHealth(100);

	/*

	player = makeEntity();
	player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));

	auto s = player->addComponent<ShapeComponent>();
	s->setShape<sf::RectangleShape>(Vector2f(38.f, 64.f));
	mage.loadFromFile("res/mage.png");
	s->getShape().setTexture(&mage);
	s->getShape().setOrigin(10.f, 15.f);



	*/





  }
  // *********************************


  // Create Skeleton
  // *********************************
  {
    auto skeleton = makeEntity();
    skeleton->setHealth(100);
    skeleton->setPosition(ls::getTilePosition(ls::findTiles('k')[0]) +
        Vector2f(20, 0));
    
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
  }
  // *********************************


  // Create Skeleton Chief
  // *********************************
  {
      auto skeleChief = makeEntity();
      skeleChief->setHealth(100);
      skeleChief->setPosition(ls::getTilePosition(ls::findTiles('c')[0]) +
          Vector2f(20, 0));

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
  }
  // *********************************


  // Create Skeleton Archer
  // *********************************
  {
      auto skeleArcher = makeEntity();
      skeleArcher->setHealth(100);
      skeleArcher->setPosition(ls::getTilePosition(ls::findTiles('a')[0]) +
          Vector2f(20, 0));

      // Add ShapeComponent, Red 16.f Circle
      auto s = skeleArcher->addComponent<ShapeComponent>();
      s->setShape<sf::RectangleShape>(Vector2f(30.f, 50.f));

      skeletArcher.loadFromFile("res/skeletonarcher.png");
      s->getShape().setTexture(&skeletArcher);
      s->getShape().setOrigin(10.f, 25.f);

      auto turret = makeEntity();
      turret->setPosition(skeleArcher->getPosition());
      //auto t = turret->addComponent<ShapeComponent>();
      //t->setShape<sf::CircleShape>(15.0f, 3);
      //t->getShape().setFillColor(Color::Red);
      //t->getShape().setOrigin(16.f, 16.f);

      // Add EnemyAIComponent
      skeleArcher->addComponent<EnemyAIComponent>();
      //turret->addComponent<EnemyTurretComponent>();
      //turret->addComponent<EnemyAIComponent>();
  }
  // *********************************

  // Create Turret
  //*************************
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
  //*************************

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

    view.reset(FloatRect(position.x, position.y, 1920, 1080));

    //*****************************************************

  Scene::Update(dt);

  

  const auto pp = player->getPosition();

  if (ls::getTileAt(pp) == ls::END) {
    Engine::ChangeScene((Scene*)&level3);
  } else if (!player->isAlive()) {
    Engine::ChangeScene((Scene*)&gameOver);
  }

  


  //Q ability cooldown
  if (qCooldown < qCooldownMax)
  {
	  qCooldown += 0.05f;
  }

  //W ability cooldown
  if (wCooldown < wCooldownMax)
  {
	  wCooldown += 0.05f;
  }

  //E ability cooldown
  if (eCooldown < eCooldownMax)
  {
	  eCooldown += 0.05f;
  }


  //Q Ability
  if (Keyboard::isKeyPressed(Keyboard::Q) && qCooldown >= qCooldownMax)
  {
	  auto bullet = makeEntity();
	  bullet->setPosition(player->getPosition());
	  //bullet->addComponent<HurtComponent>();
	  bullet->addComponent<BulletComponent>();
	  auto s = bullet->addComponent<ShapeComponent>();

	  s->setShape<sf::CircleShape>(8.f);
	  s->getShape().setFillColor(Color::Red);
	  s->getShape().setOrigin(8.f, 8.f);
	  auto p = bullet->addComponent<PhysicsComponent>(true, Vector2f(8.f, 8.f));
	  p->setRestitution(.4f);
	  p->setFriction(.005f);
	  p->impulse(sf::rotate(Vector2f(60.f, 20.f), -player->getRotation()));
	  qCooldown = 0.f;
  }

  //W Ability
  if (Keyboard::isKeyPressed(Keyboard::W) && wCooldown >= wCooldownMax)
  {
	  auto bullet = makeEntity();
	  bullet->setPosition(player->getPosition());
	  //bullet->addComponent<HurtComponent>();
	  bullet->addComponent<BulletComponent>();
	  auto s = bullet->addComponent<ShapeComponent>();

	  s->setShape<sf::RectangleShape>(Vector2f(30.0f, 30.0f));
	  s->getShape().setFillColor(Color::Blue);
	  s->getShape().setOrigin(8.f, 8.f);
	  auto p = bullet->addComponent<PhysicsComponent>(true, Vector2f(30.f, 30.f));
	  p->setRestitution(.4f);
	  p->setFriction(.005f);
	  p->impulse(sf::rotate(Vector2f(20.f, 20.f), -player->getRotation()));

	  auto bullet2 = makeEntity();
	  bullet2->setPosition(player->getPosition());
	  //bullet->addComponent<HurtComponent>();
	  bullet2->addComponent<BulletComponent>();
	  auto s2 = bullet->addComponent<ShapeComponent>();

	  s2->setShape<sf::RectangleShape>(Vector2f(30.0f, 30.0f));
	  s2->getShape().setFillColor(Color::Blue);
	  s2->getShape().setOrigin(8.f, 8.f);
	  auto p2 = bullet->addComponent<PhysicsComponent>(true, Vector2f(30.f, 30.f));
	  p2->setRestitution(.4f);
	  p2->setFriction(.005f);
	  p2->impulse(sf::rotate(Vector2f(20.f, 40.f), player->getRotation()));

	  auto bullet3 = makeEntity();
	  bullet3->setPosition(player->getPosition());
	  //bullet->addComponent<HurtComponent>();
	  bullet3->addComponent<BulletComponent>();
	  auto s3 = bullet->addComponent<ShapeComponent>();

	  s3->setShape<sf::RectangleShape>(Vector2f(30.0f, 30.0f));
	  s3->getShape().setFillColor(Color::Blue);
	  s3->getShape().setOrigin(8.f, 8.f);
	  auto p3 = bullet->addComponent<PhysicsComponent>(true, Vector2f(30.f, 30.f));
	  p3->setRestitution(.4f);
	  p3->setFriction(.005f);
	  p3->impulse(sf::rotate(Vector2f(20.f, 60.f), player->getRotation()));
	  wCooldown = 0.f;
  }


  //E ability
  if (Keyboard::isKeyPressed(Keyboard::E) && eCooldown >= eCooldownMax)
  {
	  auto bullet = makeEntity();
	  bullet->setPosition(player->getPosition());
	  //bullet->addComponent<HurtComponent>();
	  bullet->addComponent<BulletComponent>();
	  auto s = bullet->addComponent<ShapeComponent>();

	  s->setShape<sf::CircleShape>(40.f);
	  s->getShape().setFillColor(Color::Green);
	  s->getShape().setOrigin(8.f, 8.f);
	  auto p = bullet->addComponent<PhysicsComponent>(true, Vector2f(40.f, 40.f));
	  p->setRestitution(.4f);
	  p->setFriction(.005f);
	  p->impulse(sf::rotate(Vector2f(60.f, 20.f), -player->getRotation()));
	  eCooldown = 0.f;
  }

  if (Keyboard::isKeyPressed(Keyboard::G))
  {
	  Engine::ChangeScene((Scene*)&gameOver);
  }

}

void Level2Scene::Render() {
  Engine::GetWindow().draw(bckgS);
  ls::render(Engine::GetWindow());
  Engine::GetWindow().setView(view);
  
  Scene::Render();
}
