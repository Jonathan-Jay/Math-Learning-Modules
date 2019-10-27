#include "HelloWorld.h"

HelloWorld::HelloWorld(std::string name)
	: Scene(name)
{
}

void HelloWorld::InitScene(float windowWidth, float windowHeight)
{
	m_sceneReg = new entt::registry;

	ECS::AttachRegister(m_sceneReg);

	float aspectRatio = windowWidth / windowHeight;

	{
		auto entity = ECS::CreateEntity();
		EntityIdentifier::MainCamera(entity);

		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		vec4 temp = ECS::GetComponent<Camera>(entity).GetOrthoSize();
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);
		
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<HorizontalScroll>(entity).SetOffset(15.f);

		unsigned int bitHolder = EntityIdentifier::CameraBit() | EntityIdentifier::HoriScrollCameraBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Horizontal Scrolling Cam");
		ECS::SetIsMainCamera(entity, true);
	}
	
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<HealthBar>(entity);
		ECS::GetComponent<HealthBar>(entity).SetHealth(0.7f);

		unsigned int bitHolder = EntityIdentifier::HealthBarBit();
		ECS::SetUpIdentifier(entity, bitHolder, "HealthBar Entity");
	}

	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string filename = "HelloWorld.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 100, 50);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 75.f, 100.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Hello World Sign");
	}

	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string filename = "bun1.gif";
		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 59, 54);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 1.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "bun1");
	}

	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string filename = "STOP.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 200, 200);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, -10.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "STOP");
	}

	{
		auto entity = ECS::CreateEntity();
		EntityIdentifier::MainPlayer(entity);

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

		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 61, 56, true, &animController);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, -60.f, 10.f));
		ECS::GetComponent<Transform>(entity).SetRotationAngleZ(Degrees(1.6f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "bunsheetBottom");
		ECS::SetIsMainPlayer(entity, true);
	}
	
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		std::string filename = "bunsheet.png";
		auto &animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(filename);

		animController.AddAnimation(Animation());

		animController.SetActiveAnim(0);

		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(365.f, 55.f), vec2(306.f, 1.f));
		anim.AddFrame(vec2(426.f, 55.f), vec2(367.f, 1.f));
		anim.AddFrame(vec2(487.f, 55.f), vec2(428.f, 1.f));
		anim.AddFrame(vec2(548.f, 55.f), vec2(489.f, 1.f));
		anim.AddFrame(vec2(60.f, 55.f), vec2(1.f, 1.f));
		anim.AddFrame(vec2(121.f, 55.f), vec2(62.f, 1.f));
		anim.AddFrame(vec2(182.f, 55.f), vec2(123.f, 1.f));
		anim.AddFrame(vec2(243.f, 55.f), vec2(184.f, 1.f));
		anim.AddFrame(vec2(304.f, 55.f), vec2(245.f, 1.f));

		anim.SetRepeating(true);

		anim.SetSecPerFrame(0.09f);

		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 61, 56, true, &animController);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-60.f, 0.f, 10.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "bunsheet");
	}

	{
		auto bunanim = File::LoadJSON("bunjson.json");

		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		std::string filename = "bunsheet.png";
		auto &animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(filename);

		animController.AddAnimation(bunanim["bunWalk"]);

		animController.SetActiveAnim(0);

		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 61, 56, true, &animController);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(60.f, 0.f, 10.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "bunanimation");
	}

	ECS::GetComponent<HorizontalScroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityIdentifier::MainPlayer()));
}

void Spawn::SpawnBun(int x) {
	
	
	{
		auto bunanim = File::LoadJSON("bunjson.json");
		
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		std::string filename = "bunsheet.png";
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(filename);

		animController.AddAnimation(Animation());
		animController.AddAnimation(bunanim["bunWalk"]);

		animController.SetActiveAnim(x);

		auto& anim = animController.GetAnimation(0);
		anim.AddFrame(vec2(365.f, 55.f), vec2(306.f, 1.f));
		anim.AddFrame(vec2(426.f, 55.f), vec2(367.f, 1.f));
		anim.AddFrame(vec2(487.f, 55.f), vec2(428.f, 1.f));
		anim.AddFrame(vec2(548.f, 55.f), vec2(489.f, 1.f));
		anim.AddFrame(vec2(60.f, 55.f), vec2(1.f, 1.f));
		anim.AddFrame(vec2(121.f, 55.f), vec2(62.f, 1.f));
		anim.AddFrame(vec2(183.f, 55.f), vec2(123.f, 1.f));
		anim.AddFrame(vec2(243.f, 55.f), vec2(185.f, 1.f));
		anim.AddFrame(vec2(304.f, 55.f), vec2(245.f, 1.f));

		anim.SetRepeating(true);

		anim.SetSecPerFrame(0.09f);

		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 61, 56, true, &animController);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 100.f));
	
		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit() | EntityIdentifier::AnimationBit();
		ECS::SetUpIdentifier(entity, bitHolder, "bunClick");
		//Look at camera set up
	}

}