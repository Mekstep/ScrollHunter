#include "score.h"
#include <system_renderer.h>
using namespace std;
using namespace sf;


int Score::getScore() { return _score; }

void Score::setScore(int _score) { Score::_score = _score; }

String Score::getName() { return _playerName; }

void Score::setName(String _playerName) { Score::_playerName = _playerName; }

void Score::Load()
{



}

void Score::Update(const double& dt)
{

}

void Score::Render(RenderWindow& window)
{

}

