#include "Assignment.h"

Scene1::Scene1(std::string name)
	: Scene(name)
{
	m_gravity = b2Vec2(float32(0.f), float32(-20.f));
	m_physicsWorld->SetGravity(m_gravity);
}

void Scene1::InitScene(float windowWidth, float windowHeight)
{
	m_sceneReg = new entt::registry;

	ECS::AttachRegister(m_sceneReg);

	float aspectRatio = windowWidth / windowHeight;

	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<Scroll>(entity);

		ECS::GetComponent<Scroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<Scroll>(entity).SetOffsetx(150.f);
		ECS::GetComponent<Scroll>(entity).SetOffsety(100.f);

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

		std::string filename = "background.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 1280, 800);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, -10.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Background");
	}

	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string filename = "Char Sprites.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 20, 100);
		ECS::GetComponent<Sprite>(entity).SetUVs(vec2(0.f, 199.f), vec2(39.f, 0.f));

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-400.f, -220.f, 0.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Slingshot Back");
	}

	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string filename = "Char Sprites.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 22, 63);
		ECS::GetComponent<Sprite>(entity).SetUVs(vec2(942.f, 125.f), vec2(984.f, 0.f));

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-415.f, -200.f, 50.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Slingshot Front");
	}

#pragma region Box borders
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(-275.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, 1280, 50, vec2(0.f, -25.f), false);

		unsigned int bitHolder = EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Floor");
	}

	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(0.f), float32(400.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, 1280, 50, vec2(0.f, 25.f), false);

		unsigned int bitHolder = EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Roof");
	}

	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(640.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, 50, 800, vec2(25.f, 0.f), false);

		unsigned int bitHolder = EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Wall right");
	}

	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(-640.f), float32(0.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, 50, 800, vec2(-25.f, 0.f), false);

		unsigned int bitHolder = EntityIdentifier::PhysicsBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Wall left");
	}
#pragma endregion
	

	for (int x(1); x <= 4; x++)
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		std::string filename = "Char Sprites.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 25, 25);
		ECS::GetComponent<Sprite>(entity).SetUVs(vec2(916.f, 968.f), vec2(964.f, 915.f));

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-(405 + x * 30.f)), float32(-190.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, 11.f, vec2(0.f, 0.f), true);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-(405.f + x * 30.f), -190.f, 10.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "Bird " + std::to_string(x));
		EntityStorage::StoreEntity(entity, x);
	}

#pragma region enemies
	for (int x(0); x < 4; x++)
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		std::string filename = "Char Sprites.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(filename, 25, 25);
		ECS::GetComponent<Sprite>(entity).SetUVs(vec2(551.f, 923.f), vec2(598.f, 877.f));

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		if (x == 3) {
			EntityStorage::StoreEntity(entity, 0);
			tempDef.position.Set(float32(x * 30.f), float32(-190.f));
		}
		else	tempDef.position.Set(float32(x * 30.f), float32(-190.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(tempBody, 11.f, vec2(0.f, 0.f), true);

		ECS::GetComponent<Transform>(entity).SetPosition(vec3((x * 30.f), -190.f, 10.f));

		unsigned int bitHolder = EntityIdentifier::SpriteBit() | EntityIdentifier::TransformBit();
		ECS::SetUpIdentifier(entity, bitHolder, "pig " + std::to_string(x));
	}
#pragma endregion

	ECS::GetComponent<Scroll>(EntityIdentifier::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(EntityStorage::GetEntity(0)));
}

void Scene1::Update(entt::registry* reg)
{
	//Polls events and then checks them
	BackEnd::PollEvents(reg, &m_close, &m_motion, &m_click, &m_wheel);

	if (playing) {

		vec2(temp) = MousePosition + vec2(405, 180);
		if (clicked) {
			if (temp.GetMagnitude() > 25.f) {
				temp = temp.Normalize() * 25.f - vec2(405.f, 180.f);
			}
			else {
				temp = temp.Normalize() * temp.GetMagnitude() - vec2(405.f, 180.f);
			}
			reg->get<PhysicsBody>(EntityStorage::GetEntity(current)).GetBody()->SetTransform(b2Vec2(temp.x, temp.y), atan2(temp.y + 180, temp.x + 405) + PI);
		}
		else {
			if (launched) {
				std::cout << "pew\n";
				if (temp.GetMagnitude() > 25.f) {
					temp = temp.Normalize() * 25.f;
				}
				else {
					temp = temp.Normalize() * temp.GetMagnitude();
				}

				reg->get<PhysicsBody>(EntityStorage::GetEntity(current)).ApplyForce(-vec3(temp.x * 100000.f, temp.y * 500000.f, 0.f));
				launched = false;
				waiting = false;
			}
			else if (waiting) {
				if (current > 0)
					reg->get<PhysicsBody>(EntityStorage::GetEntity(current)).GetBody()->SetTransform(b2Vec2(-405.f, -180.f), 0.f);
			}
		}

		if (m_motion)
			MouseMotion(BackEnd::GetMotionEvent(), reg);

		if (m_click)
			MouseClick(BackEnd::GetClickEvent(), reg);

		if (m_wheel)
			MouseWheel(BackEnd::GetWheelEvent(), reg);
	}
	else {
		
	}
}

void Scene1::MouseMotion(SDL_MouseMotionEvent evnt, entt::registry* reg)
{
	float windowWidth = BackEnd::GetWindowWidth();
	float windowHeight = BackEnd::GetWindowHeight();
	auto& maincamera = reg->get<Camera>(EntityIdentifier::MainCamera());
	if (current > 0) {
		auto& mainplayer = reg->get<PhysicsBody>(EntityStorage::GetEntity(current));
		float temp = maincamera.GetOrthoSize().y;
		MousePosition = vec2(
			evnt.x / windowHeight * 2.f * temp - temp * windowWidth / windowHeight,
			(1 - evnt.y / windowHeight * 2.f) * temp
		)
			+ vec2(maincamera.GetPositionX(),
				maincamera.GetPositionY());
	}
	//Resets the enabled flag
	m_motion = false;
}

void Scene1::MouseClick(SDL_MouseButtonEvent evnt, entt::registry* reg)
{
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		if (!clicked) {
			launched = true;
			if (current < 4)	current++;
			else playing = false;
			reg->get<Scroll>(EntityIdentifier::MainCamera()).SetFocus(&reg->get<Transform>(EntityStorage::GetEntity(current)));
			reg->get<Camera>(EntityIdentifier::MainCamera()).SetPosition(vec3(-405, -180.f, 0.f));
		}

		clicked = !clicked;
	}
	
	//Resets the enabled flag
	m_click = false;
}

void Scene1::MouseWheel(SDL_MouseWheelEvent evnt, entt::registry* reg)
{
	//Resets the enabled flag
	m_wheel = false;
}