#pragma once
#include "Scene.h"
#include "EntityManager.h"
#include <string>

class Scene1 : public Scene
{
public:
	Scene1(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update(entt::registry* reg) override;

	void MouseMotion(SDL_MouseMotionEvent evnt, entt::registry* reg);
	void MouseClick(SDL_MouseButtonEvent evnt, entt::registry* reg);
	void MouseWheel(SDL_MouseWheelEvent evnt, entt::registry* reg);

private:
	bool m_close = false;
	bool m_motion = false;
	bool m_click = false;
	bool m_wheel = false;

	bool clicked = false;
	bool launched = false;
	bool waiting = true;
	bool playing = true;
	int current = 0;
	vec2 MousePosition = vec2(0.f, 0.f);
};