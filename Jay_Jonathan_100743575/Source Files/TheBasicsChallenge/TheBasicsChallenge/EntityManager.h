#pragma once
#include "JSON.h"
#include "Game.h"

class EntityManager
{
public:
	static void InitManager(entt::registry* temp);
	static void CreateTab();
	static void CreateManager();

private:
	static Selectable m_selectable;
	static entt::registry* m_register;
};