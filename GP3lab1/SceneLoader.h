#pragma once
#include "pch.h"
#include "Entity.h"
#include "Resources.h"

class Camera;
//class Application;

class SceneLoader
{
private:

public:
	SceneLoader();
	std::vector<Entity*> LoadScene(int scene);
};



