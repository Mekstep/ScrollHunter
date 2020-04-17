#pragma once

#include "engine.h"

class MenuScene : public Scene {
public:
  MenuScene() = default;
  ~MenuScene() override = default;

  void Load() override;

  void Update(const double& dt) override;

  void Render() override;
};

class GameOver : public Scene {
public:
	GameOver() = default;
	~GameOver() override = default;

	void Load() override;

	void Update(const double& dt) override;

	void Render() override;
};