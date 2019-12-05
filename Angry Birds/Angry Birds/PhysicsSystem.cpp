#include "PhysicsSystem.h"

void PhysicsSystem::Update(entt::registry* reg, b2World& world)
{
	auto view = reg->view<PhysicsBody, Transform>();

	for (auto entity : view) {
		auto& physBod = view.get<PhysicsBody>(entity);
		auto& trans = view.get<Transform>(entity);

		physBod.Update(&trans);
	}

	Run(world);
}

void PhysicsSystem::Run(b2World& world)
{
	float32 timeStep = 1.f / 60.f;

	int32 velocityIterations = 8;
	int32 positionIterations = 3;

	world.Step(timeStep, velocityIterations, positionIterations);
}

bool PhysicsSystem::BoxBoxCollision(std::pair<PhysicsBody&, Box> group1, std::pair<PhysicsBody&, Box> group2)
{
	if (group1.first.GetCollideID() & group2.first.GetBodyID()) {
		bool axisXCollide = group1.second.m_bottomRight.x >= group2.second.m_bottomLeft.x &&
			group2.second.m_bottomRight.x >= group1.second.m_bottomLeft.x;

		bool axisYCollide = group1.second.m_topLeft.y >= group2.second.m_bottomLeft.y &&
			group2.second.m_topLeft.y >= group1.second.m_bottomLeft.y;

		return axisXCollide && axisYCollide;
	}
	else	return false;
}

void PhysicsSystem::Init()
{
	physicsDrawShader.Load("./assets/shader/PhysicsDraw.vert", "./assets/shader/PhysicsDraw.frag");
}

void PhysicsSystem::Draw(entt::registry* reg)
{
	auto view = reg->view<PhysicsBody, Transform>();
	auto& cam = reg->get<Camera>(EntityIdentifier::MainCamera());

	if (PhysicsBody::GetDraw()) {
		for (auto entity : view) {
			auto& physBody = view.get<PhysicsBody>(entity);
			auto& trans = view.get<Transform>(entity);

			Transform temp = trans;
			temp.SetScale(vec3(physBody.GetWidth(), physBody.GetHeight(), 1.f));
			temp.SetPosition(temp.GetPosition() + vec3(physBody.GetCenterOffset().x, physBody.GetCenterOffset().y, 0.f));
			temp.SetPositionZ(100.f);

			temp.Update();

			std::string filename = "Masks/";

			if (physBody.GetBodyType() == BodyType::BOX) {
				filename += "SquareMask.png";
			}
			else if (physBody.GetBodyType() == BodyType::CIRCLE) {
				filename += "CircleMask.png";
			}

			Texture* mask = TextureManager::FindTexture(filename);

			physicsDrawShader.Bind();

			physicsDrawShader.SendUniform("uView", cam.GetView());
			physicsDrawShader.SendUniform("uProj", cam.GetProjection());
			physicsDrawShader.SendUniform("uModel", temp.GetLocalToWorldMatrix());
			physicsDrawShader.SendUniform("uColor", vec4(1.f, 0.f, 0.f, 0.3f));

			mask->Bind(0);

			physBody.DrawBody();

			mask->Unbind(0);

			physicsDrawShader.Unbind();
		}
	}
}
