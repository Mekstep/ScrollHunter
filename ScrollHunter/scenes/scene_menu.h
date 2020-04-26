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

class VictoryScene : public Scene {
public:
	VictoryScene() = default;
	~VictoryScene() override = default;

	void Load() override;

	void Update(const double& dt) override;

	void Render() override;
};

class HelpScene : public Scene {
public:
	HelpScene() = default;
	~HelpScene() override = default;

	void Load() override;

	void Update(const double& dt) override;

	void Render() override;
};