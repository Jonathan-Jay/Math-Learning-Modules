#pragma once

#include "ECS.h"
#include "Timer.h"
#include "PhysicsBody.h"
#include "Shader.h"

struct Box
{
public:
	vec3 m_bottomLeft;
	vec3 m_bottomRight;
	vec3 m_topLeft;
	vec3 m_topRight;
	vec3 m_center;
};

struct Circle
{
public:
	vec3 m_center;
	float m_radius;
};

namespace PhysicsSystem
{
	void Update(entt::registry* reg);

	void Run(entt::registry* reg);

	bool BoxBoxCollision(std::pair<PhysicsBody&, Box> group1, std::pair<PhysicsBody&, Box> group2);

	static Shader physicsDrawShader;

	void Init();

	void Draw(entt::registry* reg);
}
