#include "Assignment.h"

Assignment::Assignment(std::string name)
	: Scene(name)
{
	m_gravity = b2Vec2(float32(0.f), float32(-20.f));
	m_physicsWorld->SetGravity(m_gravity);
}

void Assignment::InitScene(float windowWidth, float windowHeight)
{
	m_sceneReg = new entt::registry;

	ECS::AttachRegister(m_sceneReg);

	float aspectRatio = windowWidth / windowHeight;

	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<Scroll>(entity);

		ECS::GetComponent<Scroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<Scroll>(entity).SetOffsetx(15.f);
		ECS::GetComponent<Scroll>(entity).SetOffsety(15.f);

		vec4 temp = ECS::GetComponent<Camera>(entity).GetOrthoSize();
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		unsigned int bitHolder = EntityIdentifier::CameraBit() | EntityIdentifier::ScrollCameraBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Scrolling Camera");
		ECS::SetIsMainCamera(entity, true);
	}

	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string filename = "Background.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 1200, 670);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, -10.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Background");
	}
	
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string filename = "FrontFence.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 1200, 670);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 20.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Front Fence");
	}

	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<PhysicsBody>(entity);

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(-285.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, 1000, 50, vec2(0.f, -25.f), false);

		unsigned int bitHolder = EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Floor");
	}

	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<PhysicsBody>(entity);

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(285.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, 1100, 50, vec2(0.f, 25.f), false);

		unsigned int bitHolder = EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Roof");
	}

	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<PhysicsBody>(entity);

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(550.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, 50, 670, vec2(25.f, 0.f), false);

		unsigned int bitHolder = EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Wall right");
	}

	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<PhysicsBody>(entity);

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-550.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, 50, 670, vec2(-25.f, 0.f), false);

		unsigned int bitHolder = EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Wall left");
	}

	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		std::string filename = "STOP.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 210, 30);

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(110.f), float32(-250.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, 210, 30, vec2(0.f, 0.f), true);

		unsigned int bitHolder = EntityIdentifier::TransformBit() | EntityIdentifier::SpriteBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Pyramid base");
		EntityStorage::StoreEntity(entity, 0);
	}

	{
		auto bunanim = File::LoadJSON("bunAnimation.json");

		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		std::string filename = "bunsheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(filename);

		animController.AddAnimation(bunanim["bunLeft"]);
		animController.AddAnimation(bunanim["bunRight"]);

		animController.SetActiveAnim(1);

		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 59, 54, true, &animController);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-50.f), float32(-200.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetHeight() / 2.f), vec2(0.f, 0.f), true);

		tempPhsBody.SetMaxVelo(1000.f);

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit()
			| EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "bun");
		ECS::SetIsMainPlayer(entity, true);
	}

	for (int x(1); x <= 21; x++)
	{
		auto Fairy = File::LoadJSON("Fairy.json");

		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		std::string filename = "fairy.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(filename);

		animController.AddAnimation(Fairy["fairy1left"]);
		animController.AddAnimation(Fairy["fairy1right"]);
		animController.AddAnimation(Fairy["fairy2left"]);
		animController.AddAnimation(Fairy["fairy2right"]);
		animController.AddAnimation(Fairy["fairy3left"]);
		animController.AddAnimation(Fairy["fairy3right"]);

		animController.SetActiveAnim(rand() % 6);

		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 28, 27, true, &animController);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 5.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		if (x < 7) {
			tempDef.position.Set(float32(20.f + 30.f * x), float32(-200.f));
		}
		else if (x < 12) {
			tempDef.position.Set(float32(200.f - 30.f * (x - 7)), float32(-170.f));
		}
		else if (x < 16) {
			tempDef.position.Set(float32(110.f + 30.f * (x - 12)), float32(-140.f));
		}
		else if (x < 19) {
			tempDef.position.Set(float32(200.f - 30.f * (x - 16)), float32(-110.f));
		}
		else if (x < 21) {
			tempDef.position.Set(float32(170.f + 30.f * (x - 19)), float32(-80.f));
		}
		else tempDef.position.Set(float32(200.f), float32(-50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, 14.f, vec2(0.f, 0.f), true);
		tempPhsBody.SetMass(0.f);

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit()
			| EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "fairy" + std::to_string(x));
		EntityStorage::StoreEntity(entity, x);
	}

	ECS::GetComponent<Scroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
}