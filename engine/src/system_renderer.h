#pragma once

#include <SFML/Graphics.hpp>
//#include "cmp_sprite.h"

namespace Renderer {
void initialise(sf::RenderWindow&);

void shutdown();

void update(const double&);

void queue(const sf::Drawable* s);

void queueSpritesheet(const sf::Sprite& sprite);

void render();
}; // namespace Renderer
