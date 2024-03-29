#pragma once

#include <GL/glew.h>
#include <Box2D/Box2D.h>
#include "JSON.h"
#include "Vector.h"
#include "VertexManager.h"
#include "EntityIdentifier.h"
#include "Transform.h"

enum class BodyType
{
	BOX,
	CIRCLE,

	NUM_TYPES
};

class CollisionIDs
{
public:
	static unsigned int Player();
	static unsigned int Environment();
	static unsigned int Enemy();
private:
	static unsigned int m_playerID;
	static unsigned int m_environmentID;
	static unsigned int m_enemyID;
};

class PhysicsBody
{
public:
	PhysicsBody() { }

	PhysicsBody(b2Body* body, float radius, vec2 centerOffset, bool isDynamic);
	PhysicsBody(b2Body* body, float width, float height, vec2 centerOffset, bool isDynamic);

	void ApplyForce(vec3 force);
	void Update(Transform* trans);

	//getters
	vec3 GetForce() const;
	vec3 GetAcceleration() const;
	float GetMaxVelo() const;
	vec3 GetVelocity() const;
	float GetFriction() const;

	static vec3 GetGravityAcceleration();
	bool GetGravity() const;

	float GetMass() const;

	BodyType GetBodyType() const;
	vec2 GetCenterOffset() const;
	vec2 GetBottomLeft() const;
	vec2 GetBottomRight() const;
	vec2 GetTopLeft() const;
	vec2 GetTopRight() const;
	float GetWidth() const;
	float GetHeight() const;
	float GetRadius() const;

	unsigned int GetBodyID() const;
	unsigned int GetCollideID() const;

	bool GetDynamic() const;

	b2Body* GetBody() const;
	b2Vec2 GetPosition() const;

	//setters
	void SetForce(vec3 force);
	void SetAcceleration(vec3 accel);
	void SetMaxVelo(float velo);
	void SetVelocity(vec3 velo);
	
	void SetFriction(float fric);
	static void SetGravityAcceleration(vec3 grav);
	void SetGravity(bool grav);
	
	void SetMass(float mass);
	
	void SetBodyType(BodyType type);
	void SetCenterOffset(vec2 cent);
	void SetBottomLeft(vec2 BL);
	void SetBottomRight(vec2 BR);
	void SetTopLeft(vec2 TL);
	void SetTopRight(vec2 TR);
	void SetWidth(float width);
	void SetHeight(float height);
	void SetRadius(float radius);

	void SetBodyID(unsigned int bodyID);
	void SetCollideID(unsigned int collideID);

	void SetDynamic(bool isDynamic);

	void SetBody(b2Body* body);
	void SetPosition(b2Vec2 bodyPos);

	//drawing
	static bool GetDraw();
	static void SetDraw(bool drawBodies);

	void InitBody();
	void DrawBody();
private:
	vec3 m_appliedForce = vec3(0.f, 0.f, 0.f);
	vec3 m_frictionForce = vec3(0.f, 0.f, 0.f);
	vec3 m_netForce = vec3(0.f, 0.f, 0.f);
	vec3 m_acceleration = vec3(0.f, 0.f, 0.f);
	float m_maxVelo = 20.f;
	vec3 m_velocity = vec3(0.f, 0.f, 0.f);
	float m_friction = 0.35f;

	static vec3 m_gravityAcceleration;
	bool m_applyGravity = false;

	float m_mass = 1.f;

	BodyType m_bodyType = BodyType::CIRCLE;
	vec2 m_centerOffset = vec2();
	vec2 m_bottomLeft = vec2();
	vec2 m_bottomRight = vec2();
	vec2 m_topLeft = vec2();
	vec2 m_topRight = vec2();

	float m_width = 0.f;
	float m_height = 0.f;

	float m_radius = 0.f;

	unsigned int m_bodyID;
	unsigned int m_collideID;

	bool m_dynamic = false;

	//others
	static bool m_drawbodies;
	GLuint m_vao = GL_NONE;
	GLuint m_vboPos = GL_NONE;

	b2Body* m_body = nullptr;
	b2Vec2 m_position = b2Vec2(0.f, 0.f);
};

inline void to_json(nlohmann::json& j, const PhysicsBody& phys)
{
	j["BodyPosition"] = { phys.GetPosition().x, phys.GetPosition().y };
	j["BodyType"] = phys.GetBodyType();
	
	j["CenterOffset"] = { phys.GetCenterOffset().x, phys.GetCenterOffset().x };
	j["BottomLeft"] = { phys.GetBottomLeft().x, phys.GetBottomLeft().y };
	j["BottomRight"] = { phys.GetBottomRight().x, phys.GetBottomRight().y };
	j["TopLeft"] = { phys.GetTopLeft().x, phys.GetTopLeft().y };
	j["TopRight"] = { phys.GetTopRight().x, phys.GetTopRight().y };
	
	j["Width"] = phys.GetWidth();
	j["Height"] = phys.GetHeight();
	j["Radius"] = phys.GetRadius();

	j["MaxVelo"] = phys.GetMaxVelo();

	j["Friction"] = phys.GetFriction();

	j["Gravity"] = phys.GetGravity();

	j["BodyID"] = phys.GetBodyID();
	j["CollideID"] = phys.GetCollideID();

	j["isDynamic"] = phys.GetDynamic();
}

inline void from_json(const nlohmann::json& j, PhysicsBody& phys)
{
	phys.SetPosition(b2Vec2(j["BodyPosition"][0], j["BodyPosition"][1]));
	phys.SetBodyType(j["BodyType"]);

	phys.SetCenterOffset(vec2(j["CenterOffset"][0], j["CenterOffset"][1]));
	phys.SetBottomLeft(vec2(j["BottomLeft"][0], j["BottomLeft"][1]));
	phys.SetBottomRight(vec2(j["BottomRight"][0], j["BottomRight"][1]));
	phys.SetTopLeft(vec2(j["TopLeft"][0], j["TopLeft"][1]));
	phys.SetTopRight(vec2(j["TopRight"][0], j["TopRight"][1]));

	phys.SetWidth(j["Width"]);
	phys.SetHeight(j["Height"]);
	phys.SetRadius(j["Radius"]);

	phys.SetMaxVelo(j["MaxVelo"]);

	phys.SetFriction(j["Friction"]);

	phys.SetGravity(j["Gravity"]);

	phys.SetBodyID(j["BodyID"]);
	phys.SetCollideID(j["CollideID"]);

	phys.SetDynamic(j["isDynamic"]);
}