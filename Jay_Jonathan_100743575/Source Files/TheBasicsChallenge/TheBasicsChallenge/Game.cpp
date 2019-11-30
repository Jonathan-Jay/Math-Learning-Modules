#include "Game.h"
#include <random>

std::vector<b2RopeJoint*> joint;
bool destroyJoint = true;

Game::~Game()
{
	//If window isn't equal to nullptr
	if (m_window != nullptr)
	{
		//Delete window
		delete m_window;
		//set window to nullptr
		m_window = nullptr;
	}

	//Goes through the scenes and deletes them
	for (unsigned i = 0; i < m_scenes.size(); i++)
	{
		if (m_scenes[i] != nullptr)
		{
			delete m_scenes[i];
			m_scenes[i] = nullptr;
		}
	}
}

void Game::InitGame()
{
	m_name = "Assignment";
	m_clearColor = vec4(1.f, 0.5f, 0.5f, 0.f);

	BackEnd::InitBackEnd(m_name);

	m_window = BackEnd::GetWindow();

	m_scenes.push_back(new Assignment("Scene Name"));

	m_activeScene = m_scenes[0];

	m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));
	//*m_activeScene = File::LoadJSON("Scene Name.json");			//if I want to load a JSON

	m_register = m_activeScene->GetScene();

	PhysicsSystem::Init();

	joint.resize(21);
	
	EntityManager::InitManager(m_register);
	for (int x(1); x < 21; x++) {
		b2RopeJointDef jointDef;
		b2Body* body1 = m_register->get<PhysicsBody>(EntityStorage::GetEntity(x)).GetBody();
		b2Body* body2 = m_register->get<PhysicsBody>(EntityStorage::GetEntity(x + 1)).GetBody();
		jointDef.bodyA = body1;
		jointDef.bodyB = body2;
		jointDef.collideConnected = true;
		jointDef.maxLength = 0.f;

		joint[x - 1] = (b2RopeJoint*)m_activeScene->GetPhysicsWorld().CreateJoint(&jointDef);
	}
}

bool Game::Run()
{
	//While window is still open
	while (m_window->isOpen())
	{
		//Clear window with clearColor
		m_window->Clear(m_clearColor);
		//Updates the game
		Update();
		//Draws the game
		BackEnd::Draw(m_register);

		//Draws ImGUI
		if (m_guiActive)
			GUI();
		
		//Flips the windows
		m_window->Flip();
		
		//Polls events and then checks them
		BackEnd::PollEvents(m_register, &m_close, &m_motion, &m_click, &m_wheel);
		CheckEvents();

		//does the window have keyboard focus?
		if (Input::m_windowFocus)
		{
			//Accept all input
			AcceptInput();
		}
	}

	return true;
}

void Game::Update()
{
	//Update timer
	Timer::Update();
	//Update the backend
	BackEnd::Update(m_register);

	PhysicsSystem::Update(m_register, m_activeScene->GetPhysicsWorld());
}

void Game::GUI()
{
	UI::Start(BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());

	ImGui::Text("Place your different tabs below.");

	if (ImGui::BeginTabBar(""))
	{
		BackEnd::GUI(m_register, m_activeScene);

		ImGui::EndTabBar();
	}

	UI::End();
}

void Game::CheckEvents()
{
	if (m_close)
		m_window->Close();

	if (m_motion)
		MouseMotion(BackEnd::GetMotionEvent());

	if (m_click)
		MouseClick(BackEnd::GetClickEvent());

	if (m_wheel)
		MouseWheel(BackEnd::GetWheelEvent());
}

void Game::AcceptInput()
{
	XInputManager::Update();

	//Just calls all the other input functions 
	GamepadInput();
	KeyboardHold();
	KeyboardDown();
	KeyboardUp();

	//Resets the key flags
	//Must be done once per frame for input to work
	Input::ResetKeys();
}

void Game::GamepadInput()
{
	XInputController* tempCon;
	for (int i = 0; i < 3; i++)
	{
		if (XInputManager::ControllerConnected(i))
		{
			tempCon = XInputManager::GetController(i);
			tempCon->SetStickDeadZone(0.1f);

			GamepadStroke(tempCon);
			GamepadUp(tempCon);
			GamepadDown(tempCon);
			GamepadStick(tempCon);
			GamepadTrigger(tempCon);
		}
	}
}

void Game::GamepadStroke(XInputController* con)
{

}

void Game::GamepadUp(XInputController* con)
{

}

void Game::GamepadDown(XInputController* con)
{

}

void Game::GamepadStick(XInputController* con)
{
	Stick sticks[2];
	con->GetSticks(sticks);
}

void Game::GamepadTrigger(XInputController* con)
{
	Triggers triggers;
	con->GetTriggers(triggers);
}

void Game::KeyboardHold()
{
	//Keyboard button held
	if (Input::GetKey(Key::D)) {
		m_register->get<PhysicsBody>(EntityIdentifier::MainPlayer()).ApplyForce(vec3(10000.f, 0.f, 0.f));
	}
	if (Input::GetKey(Key::A)) {
		m_register->get<PhysicsBody>(EntityIdentifier::MainPlayer()).ApplyForce(vec3(-10000.f, 0.f, 0.f));
	}
	if (Input::GetKey(Key::S)) {
		m_register->get<PhysicsBody>(EntityIdentifier::MainPlayer()).ApplyForce(vec3(0.f, -100000.f, 0.f));
	}
	if (Input::GetKey(Key::W)) {
		m_register->get<PhysicsBody>(EntityIdentifier::MainPlayer()).ApplyForce(vec3(0.f, 100000.f, 0.f));
	}

	if (Input::GetKey(Key::Z)) {
		m_register->get<Camera>(EntityIdentifier::MainCamera()).Zoom(0.5f);
	}
	if (Input::GetKey(Key::X)) {
		m_register->get<Camera>(EntityIdentifier::MainCamera()).Zoom(-0.5f);
	}

	if (Input::GetKey(Key::Q)) {
		for (int x(1); x <= 21; x++) {
			m_register->get<AnimationController>(EntityStorage::GetEntity(x)).SetActiveAnim(rand() % 6);
		}
	}
}

void Game::KeyboardDown()
{
	//Keyboard button down
	if (Input::GetKeyDown(Key::P)) {
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}

	if (Input::GetKeyDown(Key::F)) {
		if (joint[20] == NULL) {
			b2RopeJointDef jointDef;
			b2Body* body1 = m_register->get<PhysicsBody>(EntityStorage::GetEntity(1)).GetBody();
			b2Body* body2 = m_register->get<PhysicsBody>(EntityIdentifier::MainPlayer()).GetBody();
			jointDef.bodyA = body1;
			jointDef.bodyB = body2;
			jointDef.collideConnected = true;
			jointDef.maxLength = 40.f;

			joint[20] = (b2RopeJoint*)m_activeScene->GetPhysicsWorld().CreateJoint(&jointDef);
		}
		else {
			m_activeScene->GetPhysicsWorld().DestroyJoint(joint[20]);
			joint[20] = NULL;
		}
	}

	if (Input::GetKeyDown(Key::Space)) {
		if (destroyJoint) {
			for (int x(0); x < 20; x++) {
				m_activeScene->GetPhysicsWorld().DestroyJoint(joint[x]);
				m_register->get<PhysicsBody>(EntityStorage::GetEntity(x + 1)).ApplyForce(
					vec3((rand() % 3 - 1 ) * 1000000.f * (rand() % 11), 10000000.f, 0.f));
				joint[x] = NULL;
			}
			m_register->get<PhysicsBody>(EntityStorage::GetEntity(21)).ApplyForce(
				vec3((rand() % 3 - 1) * 1000000.f * (rand() % 11), 10000000.f, 0.f));
			m_register->get<PhysicsBody>(EntityStorage::GetEntity(0)).ApplyForce(
				vec3((rand() % 3 - 1) * 1000000.f * (rand() % 11), 10000000.f, 0.f));
			destroyJoint = false;
		}
		else {
			for (int x(1); x < 21; x++) {
				b2RopeJointDef jointDef;
				jointDef.bodyA = m_register->get<PhysicsBody>(EntityStorage::GetEntity(x)).GetBody();
				jointDef.bodyB = m_register->get<PhysicsBody>(EntityStorage::GetEntity(x + 1)).GetBody();
				jointDef.collideConnected = true;
				jointDef.maxLength = 0.f;

				joint[x - 1] = (b2RopeJoint*)m_activeScene->GetPhysicsWorld().CreateJoint(&jointDef);
			}
			destroyJoint = true;
		}
	}

	if (Input::GetKeyDown(Key::R)) {
		for (int x(1); x < 21; x++) {
			if (x < 7) {
				m_register->get<PhysicsBody>(EntityStorage::GetEntity(x)).GetBody()->SetTransform(b2Vec2(
					float32(20.f + 30.f * x), float32(-200.f)), float32(0.f));
			}
			else if (x < 12) {
				m_register->get<PhysicsBody>(EntityStorage::GetEntity(x)).GetBody()->SetTransform(b2Vec2(
					float32(200.f - 30.f * (x - 7)), float32(-170.f)), float32(0.f));
			}
			else if (x < 16) {
				m_register->get<PhysicsBody>(EntityStorage::GetEntity(x)).GetBody()->SetTransform(b2Vec2(
					float32(110.f + 30.f * (x - 12)), float32(-140.f)), float32(0.f));
			}
			else if (x < 19) {
				m_register->get<PhysicsBody>(EntityStorage::GetEntity(x)).GetBody()->SetTransform(b2Vec2(
					float32(200.f - 30.f * (x - 16)), float32(-110.f)), float32(0.f));
			}
			else if (x < 21) {
				m_register->get<PhysicsBody>(EntityStorage::GetEntity(x)).GetBody()->SetTransform(b2Vec2(
					float32(170.f + 30.f * (x - 19)), float32(-80.f)), float32(0.f));
			}
		}
		m_register->get<PhysicsBody>(EntityStorage::GetEntity(21)).GetBody()->SetTransform(b2Vec2(float32(200.f), float32(-50.f)), float32(0.f));
		m_register->get<PhysicsBody>(EntityStorage::GetEntity(0)).GetBody()->SetTransform(b2Vec2(float32(110.f), float32(-250.f)), float32(0.f));
	}
}

void Game::KeyboardUp()
{
	if (Input::GetKeyUp(Key::Home))
	{
		if (!UI::m_isInit)
		{
			UI::InitImGUI();
		}
		m_guiActive = !m_guiActive;
	}

}

void Game::MouseMotion(SDL_MouseMotionEvent evnt)
{
	if (m_guiActive)
	{
		ImGui::GetIO().MousePos = ImVec2(float(evnt.x), float(evnt.y));

		if (!ImGui::GetIO().WantCaptureMouse)
		{

		}
	}

	//Resets the enabled flag
	m_motion = false;
}

void Game::MouseClick(SDL_MouseButtonEvent evnt)
{
	float windowWidth = BackEnd::GetWindowWidth();
	float windowHeight = BackEnd::GetWindowHeight();
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		int maincamera = EntityIdentifier::MainCamera();
		vec2(click) = vec2(
			evnt.x / windowHeight * 200.f - 100.f * windowWidth / windowHeight,
			-evnt.y / windowHeight * 200.f + 100.f
			)
			+ vec2(m_register->get<Camera>(maincamera).GetPositionX(),
			m_register->get<Camera>(maincamera).GetPositionY());
		auto & mainplayer = m_register->get<PhysicsBody>(EntityIdentifier::MainPlayer());

		vec2(change) = vec2(click.x - mainplayer.GetPosition().x, click.y - mainplayer.GetPosition().y).Normalize();

		mainplayer.ApplyForce(vec3(change.x, change.y, 0.f) * 10000000.f);
	}

	if (m_guiActive)
	{
		ImGui::GetIO().MousePos = ImVec2(float(evnt.x), float(evnt.y));
		ImGui::GetIO().MouseDown[0] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT));
		ImGui::GetIO().MouseDown[1] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT));
		ImGui::GetIO().MouseDown[2] = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE));
	}

	//Resets the enabled flag
	m_click = false;
}

void Game::MouseWheel(SDL_MouseWheelEvent evnt)
{
	if (m_guiActive)
	{
		ImGui::GetIO().MouseWheel = float(evnt.y);
	}
	//Resets the enabled flag
	m_wheel = false;
}