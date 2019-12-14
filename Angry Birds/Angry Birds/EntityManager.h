#pragma once
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

class EntityStorage
{
public:
	static void StoreEntity(int entity, int number);
	static int GetEntity(int number);
private:
	static std::vector<int> storage;
};