#pragma once
#include "Scene.h"
#include "EntityManager.h"
#include <string>

class Assignment : public Scene
{
public:
	Assignment(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;
};