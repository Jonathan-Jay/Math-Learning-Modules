#include "Game.h"
#include <random>

float Width(59);
float Height(54);
float Jump(50);
bool controller = false;
vec2 Acceleration = vec2(0.f, -500.f);
vec2(movement) = vec2(0.f, 0.f);

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
	glCullFace(GL_NONE);

	m_name = "Assignment";
	m_clearColor = vec4(1.f, 0.5f, 0.5f, 0.f);

	BackEnd::InitBackEnd(m_name);

	m_window = BackEnd::GetWindow();

	m_scenes.push_back(new Assignment("Scene Name"));

	m_activeScene = m_scenes[0];

	m_activeScene->InitScene(float(BackEnd::GetWindowWidth()), float(BackEnd::GetWindowHeight()));
	//*m_activeScene = File::LoadJSON("Scene Name.json");			//if I want to load a JSON


	m_register = m_activeScene->GetScene();

	EntityManager::InitManager(m_register);
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
	int mainplayer = EntityIdentifier::MainPlayer();
	controller = false;

	//Just calls all the other input functions 
	GamepadInput(mainplayer);
	KeyboardHold(mainplayer);
	KeyboardDown(mainplayer);
	KeyboardUp(mainplayer);
	if (!controller) MovementKey(mainplayer);

	//Resets the key flags
	//Must be done once per frame for input to work
	Input::ResetKeys();
	Game::Movement(mainplayer);
}

void Game::GamepadInput(int mainplayer)
{
	XInputController* tempCon;
	for (int i = 0; i < 3; i++)
	{
		if (XInputManager::ControllerConnected(i))
		{
			tempCon = XInputManager::GetController(i);
			tempCon->SetStickDeadZone(0.1f);

			GamepadStroke(tempCon, mainplayer);
			GamepadUp(tempCon, mainplayer);
			GamepadDown(tempCon, mainplayer);
			GamepadStick(tempCon, mainplayer);
			GamepadTrigger(tempCon, mainplayer);
		}
	}
}

void Game::GamepadStroke(XInputController* con, int mainplayer)
{

}

void Game::GamepadUp(XInputController* con, int mainplayer)
{

}

void Game::GamepadDown(XInputController* con, int mainplayer)
{
	if (con->IsButtonPressed(Buttons::A))
	{
		if (m_register->get<AnimationController>(mainplayer).GetActiveAnim() == 0)
			m_register->get<AnimationController>(mainplayer).SetActiveAnim(1);
		else m_register->get<AnimationController>(mainplayer).SetActiveAnim(0);
	}
}

void Game::GamepadStick(XInputController* con, int mainplayer)
{
	Stick sticks[2];
	con->GetSticks(sticks);

	if (sticks[0].x > 0.1f)		{ Acceleration.x += sticks[0].x * 40.f; controller = true; }
	if (sticks[0].x < -0.1f)	{ Acceleration.x += sticks[0].x * 40.f; controller = true; }
	if (sticks[0].y > 0.1f)		{ Acceleration.y += sticks[0].y * 50.f; controller = true; }
	if (sticks[0].y < -0.1f)	{ Acceleration.y += sticks[0].y * 75.f; controller = true; }
}

void Game::GamepadTrigger(XInputController* con, int mainplayer)
{
	Triggers triggers;
	con->GetTriggers(triggers);

	if (triggers.RT > 0.8f)
	{
		m_register->get<AnimationController>(mainplayer).SetActiveAnim(1);
	}

	if (triggers.LT > 0.8f)
	{
		m_register->get<AnimationController>(mainplayer).SetActiveAnim(0);
	}
}

void Game::KeyboardHold(int mainplayer)
{
	//Keyboard button held
	if (Input::GetKey(Key::Space) && Width > 0.01 && Height > 0.01) {
		Width *= 0.99;
		Height *= 0.99;
	}
	else if (Height < 54 || Width < 59) {
		Width *= 1.1;
		Height *= 1.1;
	}
	if (Height > 54 || Width > 59) {
		Height = 54;
		Width = 59;
	}
	m_register->get<Sprite>(mainplayer).SetHeight(Height);
	m_register->get<Sprite>(mainplayer).SetWidth(Width);

	if (Input::GetKey(Key::Space) && Jump < 75) {
		Jump++;
	}

}

void Game::KeyboardDown(int mainplayer)
{
	//Keyboard button down
}

void Game::KeyboardUp(int mainplayer)
{
	if (Input::GetKeyUp(Key::Home))
	{
		if (!UI::m_isInit)
		{
			UI::InitImGUI();
		}
		m_guiActive = !m_guiActive;
	}

	if (Input::GetKeyUp(Key::Space)) {
		if (m_register->get<Transform>(mainplayer).GetPositionY() <= -49.99) {
			Acceleration.y = Jump * 1000;
		}
		Jump = 25;
	}
}

void Game::MovementKey(int mainplayer)
{
	if (Input::GetKey(Key::W) || Input::GetKey(Key::UpArrow))		Acceleration.y += 50.f;
	if (Input::GetKey(Key::A) || Input::GetKey(Key::LeftArrow))		Acceleration.x -= 40.f;
	if (Input::GetKey(Key::S) || Input::GetKey(Key::DownArrow))		Acceleration.y -= 75.f;
	if (Input::GetKey(Key::D) || Input::GetKey(Key::RightArrow))	Acceleration.x += 40.f;
}

void Game::Movement(int mainplayer)
{
	vec3(CurrentPosition) = m_register->get<Transform>(mainplayer).GetPosition();
	auto& CurrentAnim = m_register->get<AnimationController>(mainplayer).GetAnimation(m_register->get<AnimationController>(mainplayer).GetActiveAnim());

	if (movement.GetMagnitude() != 0) {
		CurrentAnim.SetRepeating(false);
		if (CurrentAnim.GetAnimationDone() == true) {
			CurrentAnim.SetSecPerFrame(90.f / (movement.GetMagnitude() + 1000.f));
		}

		if (movement.x > 0.01f) Acceleration.x -= 10.f;
		else if (movement.x < -0.01f) Acceleration.x += 10.f;

		if (movement.x > 0)			m_register->get<AnimationController>(mainplayer).SetActiveAnim(1);
		else if (movement.x < 0)	m_register->get<AnimationController>(mainplayer).SetActiveAnim(0);
	}
	else {
		if (CurrentAnim.GetSecPerFrame() != 0.09f) {
			CurrentAnim.SetSecPerFrame(0.09f);
		}
		CurrentAnim.SetRepeating(true);
	}

	if (CurrentPosition.y <= -49.99) movement.y = 500.f;

	movement = movement + Acceleration * Timer::deltaTime;

	if (movement.x > 250.f) movement.x = 250.f;
	else if (movement.x < -250.f) movement.x = -250.f;
	if (movement.y > 10000.f) movement.y = 10000.f;
	else if (movement.y < -250.f) movement.y = -250.f;

	CurrentPosition = CurrentPosition + (vec3(movement.x, movement.y, 0.f) * Timer::deltaTime) + vec3(Acceleration.x, Acceleration.y, 0.f) * (Timer::deltaTime * Timer::deltaTime) / 2;
	
	if (CurrentPosition.x > 250)	CurrentPosition.x = 250;
	if (CurrentPosition.x < -250)	CurrentPosition.x = -250;
	if (CurrentPosition.y > 450)	CurrentPosition.y = 450;
	if (CurrentPosition.y < -50)	CurrentPosition.y = -50;
	
	m_register->get<Transform>(mainplayer).SetPosition(CurrentPosition);
	Acceleration = vec2(0.f, -500.f);
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