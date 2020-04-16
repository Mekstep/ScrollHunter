#include "cmp_bullet_phsics.h"
#include "system_physics.h"
#include <LevelSystem.h>

using namespace std;
using namespace sf;

using namespace Physics;

void BulletPhysicsComponent::update(double dt) 
{
  //_parent->setPosition(invert_height(bv2_to_sv2(_body->GetPosition())));
    _parent->setPosition(_parent->getPosition() + Vector2f(-500*dt,0));
  _parent->setRotation((180 / b2_pi) * _body->GetAngle());
  

}

BulletPhysicsComponent::BulletPhysicsComponent(Entity* p, bool dyn, const Vector2f& size) : Component(p), _dynamic(dyn) 
{
  b2BodyDef BodyDef;
  // Is Dynamic(moving), or static(Stationary)
  BodyDef.type = _dynamic ? b2_dynamicBody : b2_staticBody;
  BodyDef.position = sv2_to_bv2(invert_height(p->getPosition()));

  // Create the body
  _body = Physics::GetWorld()->CreateBody(&BodyDef);
  _body->SetActive(true);
  {
    // Create the fixture shape
    b2PolygonShape Shape;
    // SetAsBox box takes HALF-Widths!
    Shape.SetAsBox(sv2_to_bv2(size).x * 0.5f, sv2_to_bv2(size).y * 0.5f);
    b2FixtureDef FixtureDef;
    // Fixture properties
    // FixtureDef.density = _dynamic ? 10.f : 0.f;
    FixtureDef.friction = _dynamic ? 0.1f : 0.8f;
    FixtureDef.restitution = .2;
    FixtureDef.shape = &Shape;
    // Add to body
    _fixture = _body->CreateFixture(&FixtureDef);
    //_fixture->SetRestitution(.9)
    FixtureDef.restitution = .2;
  }

  // An ideal Pod/capusle shape should be used for hte player,
  // this isn't built into B2d, but we can combine two shapes to do so.
  // This would allwo the player to go up steps
  /*
    BodyDef.bullet = true;
    b2PolygonShape shape1;
    shape1.SetAsBox(sv2_to_bv2(size).x * 0.5f, sv2_to_bv2(size).y * 0.5f);
    {
      b2PolygonShape poly ;
      poly.SetAsBox(0.45f, 1.4f);
      b2FixtureDef FixtureDefPoly;

      FixtureDefPoly.shape = &poly;
      _body->CreateFixture(&FixtureDefPoly);

    }
    {
      b2CircleShape circle;
      circle.m_radius = 0.45f;
      circle.m_p.Set(0, -1.4f);
      b2FixtureDef FixtureDefCircle;
      FixtureDefCircle.shape = &circle;
      _body->CreateFixture(&FixtureDefCircle);
    }
  */
}


const sf::Vector2f BulletPhysicsComponent::getVelocity() const 
{    
  return bv2_to_sv2(_body->GetLinearVelocity(), true);
}
void BulletPhysicsComponent::setVelocity(const sf::Vector2f& v) 
{
  _body->SetLinearVelocity(sv2_to_bv2(v, true));
}

b2Fixture* const BulletPhysicsComponent::getFixture() const 
{ return _fixture; }

BulletPhysicsComponent::~BulletPhysicsComponent() 
{
  auto a = Physics::GetWorld();
  _body->SetActive(false);
  Physics::GetWorld()->DestroyBody(_body);
  // delete _body;
  _body = nullptr;
}

void BulletPhysicsComponent::render() {}

bool BulletPhysicsComponent::isTouching(const BulletPhysicsComponent& pc) const 
{
  b2Contact* bc; 
  return isTouching(pc, bc);
}

bool BulletPhysicsComponent::isTouching(const BulletPhysicsComponent& pc, b2Contact const* bc) const 
{
  const auto _otherFixture = pc.getFixture();
  const auto& w = *Physics::GetWorld();
  const auto contactList = w.GetContactList();
  const auto clc = w.GetContactCount();
  for (int32 i = 0; i < clc; i++) {
      
    const auto& contact = (contactList[i]);
    if (contact.IsTouching() && ((contact.GetFixtureA() == _fixture &&
                                  contact.GetFixtureA() == _otherFixture) ||
                                 (contact.GetFixtureA() == _otherFixture &&
                                  contact.GetFixtureA() == _fixture))) {
      bc = &contact;
      
      return true;
    }
  }
  
  return false;
}

std::vector<const b2Contact const*> BulletPhysicsComponent::getTouching() const {
  std::vector<const b2Contact const*> ret;
  
  b2ContactEdge* edge = _body->GetContactList();
  while (edge != NULL) {
    const b2Contact* contact = edge->contact;
    if (contact->IsTouching()) {
      ret.push_back(contact);
      
    }
    edge = edge->next;
    
  }
  return ret;
}
