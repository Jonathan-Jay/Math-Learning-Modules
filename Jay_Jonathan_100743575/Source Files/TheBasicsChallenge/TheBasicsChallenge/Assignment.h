#pragma once

#include "Scene.h"

class Assignment : public Scene
{
public:
	Assignment(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;
};