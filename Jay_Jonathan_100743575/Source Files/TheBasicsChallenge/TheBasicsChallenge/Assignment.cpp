#include "Assignment.h"

Assignment::Assignment(std::string name)
	: Scene(name)
{
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
		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 670, 670);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, -10.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Background");
	}
	
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string filename = "FrontFence.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 670, 670);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 20.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Front Fence");
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

		animController.SetActiveAnim(0);

		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 59, 54, true, &animController);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		tempPhsBody = PhysicsBody(float(tempSpr.GetWidth() / 2.f),
						vec2(0.f, 0.f), CollisionIDs::Player(),
						(CollisionIDs::Enemy() | CollisionIDs::Environment()), true);

		tempPhsBody.SetFriction(10.f);
		tempPhsBody.SetMaxVelo(250.f);
		tempPhsBody.SetGravity(false);

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit()
			| EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "bun");
		ECS::SetIsMainPlayer(entity, true);
		ECS::SetIsButton(entity, true, 1);
	}

	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		std::string filename = "STOP.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 20, 20);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 100.f, 10.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		tempPhsBody = PhysicsBody(float(tempSpr.GetWidth() / 2.f),
			vec2(0.f, 0.f), CollisionIDs::Enemy(), CollisionIDs::Player(), true);

		tempPhsBody.SetFriction(0.15f);
		tempPhsBody.SetMaxVelo(250.f);
		tempPhsBody.SetGravity(false);

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit()
			| EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Object");
		ECS::SetIsObject(entity, true);
		ECS::SetIsButton(entity, true, 0);
	}
	
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Transform>(entity);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.f));

		unsigned int bitHolder = EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Tracker");
		ECS::SetIsTracker(entity, true);
	}

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

		animController.SetActiveAnim(0);

		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 28, 27, true, &animController);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, -50.f, 5.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		tempPhsBody = PhysicsBody(float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), CollisionIDs::Enemy(),
			(CollisionIDs::Player() | CollisionIDs::Environment()), true);

		tempPhsBody.SetFriction(0.15f);
		tempPhsBody.SetMaxVelo(250.f);
		tempPhsBody.SetGravity(false);

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit()
			| EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "fairy1");
	}
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

		animController.SetActiveAnim(4);

		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 28, 27, true, &animController);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(50.f, 0.f, 5.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		tempPhsBody = PhysicsBody(float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), CollisionIDs::Enemy(),
			(CollisionIDs::Player() | CollisionIDs::Environment()), true);

		tempPhsBody.SetFriction(0.15f);
		tempPhsBody.SetMaxVelo(250.f);
		tempPhsBody.SetGravity(false);

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit()
			| EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "fairy2");
	}
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

		animController.SetActiveAnim(5);

		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 28, 27, true, &animController);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-50.f, 0.f, 5.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		tempPhsBody = PhysicsBody(float(tempSpr.GetWidth()), float(tempSpr.GetHeight()),
			vec2(0.f, 0.f), CollisionIDs::Enemy(),
			(CollisionIDs::Player() | CollisionIDs::Environment()), true);

		tempPhsBody.SetFriction(0.15f);
		tempPhsBody.SetMaxVelo(250.f);
		tempPhsBody.SetGravity(false);

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit()
			| EntityIdentifier::AnimationBit() | EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "fairy3");
	}

	{
		auto controller = File::LoadJSON("Controller.json");

		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		std::string filename = "controller.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(filename);

		animController.AddAnimation(controller["controller"]);

		animController.SetActiveAnim(0);

		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 20, 14, true, &animController);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 25.f, 5.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "controller");
		ECS::SetIsButton(entity, true, 2);
	}

	ECS::GetComponent<Scroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
}
