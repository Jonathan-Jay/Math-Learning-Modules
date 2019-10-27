#include "Assignment.h"
#include "Game.h"

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
		EntityIdentifier::MainCamera(entity);

		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<Scroll>(entity);

		ECS::GetComponent<Scroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<Scroll>(entity).SetOffsetx(30.f);
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

		std::string filename = "STOP.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 750, 750);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(40.f, 0.f, -99.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "STOP");
	}

	{
		auto entity = ECS::CreateEntity();
		EntityIdentifier::HelloWorld(entity);

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string filename = "HelloWorld.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 100, 50);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 75.f, 100.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Hello World Sign");
		ECS::SetIsHelloWorld(entity, true);
	}

	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		std::string filename = "bunsheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(filename);

		animController.AddAnimation(Animation());

		animController.SetActiveAnim(0);

		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(60.f, 55.f), vec2(1.f, 1.f));
		anim.AddFrame(vec2(121.f, 55.f), vec2(62.f, 1.f));
		anim.AddFrame(vec2(182.f, 55.f), vec2(123.f, 1.f));
		anim.AddFrame(vec2(243.f, 55.f), vec2(184.f, 1.f));
		anim.AddFrame(vec2(304.f, 55.f), vec2(245.f, 1.f));
		anim.AddFrame(vec2(365.f, 55.f), vec2(306.f, 1.f));
		anim.AddFrame(vec2(426.f, 55.f), vec2(367.f, 1.f));
		anim.AddFrame(vec2(487.f, 55.f), vec2(428.f, 1.f));
		anim.AddFrame(vec2(548.f, 55.f), vec2(489.f, 1.f));

		anim.SetRepeating(true);

		anim.SetSecPerFrame(0.09f);

		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 59, 54, true, &animController);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-60.f, 0.f, 0.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "bunAnimLeft");
	}

	{
		auto bunanim = File::LoadJSON("bunAnimation.json");

		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		std::string filename = "bunsheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(filename);

		animController.AddAnimation(bunanim["bunLeft"]);
		
		animController.SetActiveAnim(0);

		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 59, 54, true, &animController);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(60.f, 0.f, 0.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "bunAnimRight");
	}

	{
		auto bunanim = File::LoadJSON("bunAnimation.json");

		auto entity = ECS::CreateEntity();
		EntityIdentifier::MainPlayer(entity);

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		std::string filename = "bunsheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(filename);

		animController.AddAnimation(bunanim["bunLeft"]);
		animController.AddAnimation(bunanim["bunRight"]);

		animController.SetActiveAnim(0);

		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 59, 54, true, &animController);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 10.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "bun");
		ECS::SetIsMainPlayer(entity, true);
	}

	ECS::GetComponent<Scroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
}
