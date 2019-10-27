#pragma once

#include "Scene.h"
#include "BackEnd.h"
#include "JSON.h"

class Assignment : public Scene
{
public:
	Assignment(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;
};