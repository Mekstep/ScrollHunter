#include "system_renderer.h"
#include <queue>

using namespace std;
using namespace sf;

static queue<const Drawable*> sprites;
static queue<Sprite> spritesheets;
static RenderWindow* rw;

void Renderer::initialise(sf::RenderWindow& r) { rw = &r; }

void Renderer::shutdown() {
  while (!sprites.empty())
    sprites.pop();
  while (!spritesheets.empty())
	  spritesheets.pop();
}

void Renderer::update(const double&) {}

void Renderer::render() {
  if (rw == nullptr) {
    throw("No render window set! ");
  }
  while (!sprites.empty()) {
    rw->draw(*sprites.front());
    sprites.pop();
  }
  while (!spritesheets.empty()) {
	  rw->draw(spritesheets.front());
	  spritesheets.pop();
  }
}

void Renderer::queue(const sf::Drawable* s) { sprites.push(s); }

void Renderer::queueSpritesheet(const Sprite& sprite) { spritesheets.push(sprite); }