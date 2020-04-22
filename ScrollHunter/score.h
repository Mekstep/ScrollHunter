#pragma once

#include "engine.h"

class Score 
{
protected:

    String _playerName;
    int _score;

public:

    String getName();

    void setName(String _playerName);

    int getScore();
    
    void setScore(int _score);

    void Load();

    void Update(const double& dt);

    void Render();
};