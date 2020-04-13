#pragma once
#include "ecm.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class SpriteSheetComponent : public Component {
protected:
	float animationTimer;
	float frameTime;
	int frameCount;
	sf::Vector2f set_size;
	sf::Sprite sprite;
	sf::Texture spritesheet;
	sf::IntRect currentFrame;
	sf::Vector2u currentTexture;
	bool end;

public:
	explicit SpriteSheetComponent(Entity* p, sf::Vector2f size);
	const sf::Vector2f& getSize() const;
	const sf::Sprite& getSprite() const;
	int getFrameCount() const;
	float getFrameTime() const;
	bool finished() const;
	void update(double dt) override;
	void render() override;
	void setSpritesheet(const sf::Texture& setSpritesheet);
	void setFrameTime(float time);
	void setFrameCount(int count);
	void setSize(const sf::Vector2f& size);
	SpriteSheetComponent() = delete;
};