#include "Game.h"
#include <random>

float Width(59);
float Height(54);
float Launch(25);
bool controller = false;
bool Bounce = false;
bool SpeedCap = true;
bool Deccel = true;
vec2 Force = vec2(0.f, 0.f);
vec2 movement = vec2(0.f, 0.f);
vec2 objectMov = vec2(0.f, 0.f);
float weight = 1.f;

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
	int object = EntityIdentifier::Object();
	int tracker = EntityIdentifier::Tracker();
	controller = false;

	//Just calls all the other input functions 
	GamepadInput(mainplayer);
	KeyboardHold(mainplayer);
	KeyboardDown(mainplayer);
	KeyboardUp(mainplayer);
	if (!controller) MovementKey(mainplayer, tracker);

	//Resets the key flags
	//Must be done once per frame for input to work
	Input::ResetKeys();
	Game::Movement(mainplayer, object, tracker);
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

	if (sticks[0].x > 0.1f)		{ Force.x += sticks[0].x * 200.f; controller = true; }
	if (sticks[0].x < -0.1f)	{ Force.x += sticks[0].x * 200.f; controller = true; }
	if (sticks[0].y > 0.1f)		{ Force.y += sticks[0].y * 200.f; controller = true; }
	if (sticks[0].y < -0.1f)	{ Force.y += sticks[0].y * 200.f; controller = true; }
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
		weight *= 0.997;
	}
	else if (Height < 54 || Width < 59) {
		Width *= 1.1;
		Height *= 1.1;
		if (weight < 1)	weight *= 1.1;
	}
	if (Height > 54 || Width > 59) {
		Height = 54;
		Width = 59;
		weight = 1;
	}
	m_register->get<Sprite>(mainplayer).SetHeight(Height);
	m_register->get<Sprite>(mainplayer).SetWidth(Width);

	if (Input::GetKey(Key::Space) && Launch < 100) {
		Launch++;
	}

}

void Game::KeyboardDown(int mainplayer)
{
	//Keyboard button down
	if (Input::GetKeyDown(Key::One)) {
		if (Bounce) {
			Bounce = false;
			printf("Bounce off\n");
		}
		else {
			Bounce = true;
			printf("Bounce on\n");
		}
	}
	
	if (Input::GetKeyDown(Key::Two)) {
		if (SpeedCap) {
			SpeedCap = false;
			printf("Speed Cap off\n");
		}
		else {
			SpeedCap = true;
			printf("Speed Cap on\n");
		}
	}

	if (Input::GetKeyDown(Key::Three)) {
		if (Deccel) {
			Deccel = false;
			printf("Decceleration off\n");
		}
		else {
			Deccel = true;
			printf("Decceleration on\n");
		}
	}

	if (Input::GetKeyDown(Key::Four)) {
		Bounce = false;
		SpeedCap = true;
		Deccel = true;
		printf("Settings Reset:\nBounce = off\nSpeed Cap = on\nDecceleration = on\n");
	}
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
		if (movement.GetMagnitude() > 0) {
			movement = movement + movement.Normalize() * Launch * 10;
		}
		Launch = 25;
	}
}

void Game::MovementKey(int mainplayer, int tracker)
{
	if (Input::GetKey(Key::W) || Input::GetKey(Key::UpArrow))		Force.y += 200.f;
	if (Input::GetKey(Key::A) || Input::GetKey(Key::LeftArrow))		Force.x -= 200.f;
	if (Input::GetKey(Key::S) || Input::GetKey(Key::DownArrow))		Force.y -= 200.f;
	if (Input::GetKey(Key::D) || Input::GetKey(Key::RightArrow))	Force.x += 200.f;

	if (Input::GetKey(Key::I))	objectMov.y += 0.25f;
	if (Input::GetKey(Key::J))	objectMov.x -= 0.25f;
	if (Input::GetKey(Key::K))	objectMov.y -= 0.25f;
	if (Input::GetKey(Key::L))	objectMov.x += 0.25f;
}

void Game::Movement(int mainplayer, int object, int tracker)
{
	vec3(CurrentPosition) = m_register->get<Transform>(mainplayer).GetPosition();
	vec3(ObjectPos) = m_register->get<Transform>(object).GetPosition();	
	vec3(TrackerPos) = m_register->get<Transform>(tracker).GetPosition();
	auto& CurrentAnim = m_register->get<AnimationController>(mainplayer).GetAnimation(m_register->get<AnimationController>(mainplayer).GetActiveAnim());

	if (movement.GetMagnitude() != 0) {
		CurrentAnim.SetRepeating(false);
		if (CurrentAnim.GetAnimationDone() == true) {
			CurrentAnim.SetSecPerFrame(27.f / (movement.GetMagnitude() + 300.f));
		}

 		if (Deccel)	movement = movement * 0.995;

 		if (movement.x > 0.1f) {
			m_register->get<AnimationController>(mainplayer).SetActiveAnim(1);
			m_register->get<Transform>(mainplayer).SetRotationAngleZ(PI / 2 - movement.GetAngle(vec2(0.f, 1.f)));
		}
		else if (movement.x < 0.1f) {
			m_register->get<AnimationController>(mainplayer).SetActiveAnim(0);
			m_register->get<Transform>(mainplayer).SetRotationAngleZ(-(PI / 2 - movement.GetAngle(vec2(0.f, 1.f))));
		}
		if (movement.GetMagnitude() < 1.f) movement = vec2(0.f, 0.f);
	}
	else {
		if (CurrentAnim.GetSecPerFrame() != 0.09f) {
			CurrentAnim.SetSecPerFrame(0.09f);
		}
		CurrentAnim.SetRepeating(true);
	}

	vec2(Acceleration) = Force / weight;
	movement = movement + Acceleration * Timer::deltaTime;

	if (CurrentPosition.y <= -250) movement.y = -movement.y;
	if (CurrentPosition.y >= 250) movement.y = -movement.y;
	if (CurrentPosition.x <= -250) movement.x = -movement.x;
	if (CurrentPosition.x >= 250) movement.x = -movement.x;

	if (SpeedCap) {
		if (movement.x > 500.f) movement.x = 500.f;
		else if (movement.x < -500.f) movement.x = -500.f;
		if (movement.y > 500.f) movement.y = 500.f;
		else if (movement.y < -500.f) movement.y = -500.f;
	}

	//getting some vectors
	vec2(Pos1) = vec2(CurrentPosition.x, CurrentPosition.y);
	vec2(Pos2) = vec2(ObjectPos.x, ObjectPos.y);
	vec2(Pos3) = vec2(TrackerPos.x, TrackerPos.y);
	vec2(tracking) = Pos3 - Pos2;
	vec2(result) = Pos1 - Pos2;

	//Tracker Movement
	if (TrackerPos.x <= -100.f && TrackerPos.y >= -100.f)		TrackerPos.y -= 0.115;
	else if (TrackerPos.x <= 100.f && TrackerPos.y <= -100.f)	TrackerPos.x += 0.115;
	else if (TrackerPos.x >= 100.f && TrackerPos.y <= 100.f)	TrackerPos.y += 0.115;
	else if (TrackerPos.x >= -100.f && TrackerPos.y >= 100.f)	TrackerPos.x -= 0.115;
	else TrackerPos.y += 0.115;

	//Object Movement
	if (tracking.GetMagnitude() > 1.f) {
		tracking = tracking.Normalize() / 10;
		ObjectPos = ObjectPos + vec3(tracking.x, tracking.y, 0.f) + vec3(objectMov.x, objectMov.y, 0.f);
		if (tracking.x > 0)	m_register->get<Transform>(object).SetRotationAngleZ(PI - tracking.GetAngle(vec2(0.f, 1.f)));
		else				m_register->get<Transform>(object).SetRotationAngleZ(PI + tracking.GetAngle(vec2(0.f, 1.f)));
	}

	//Object Collision (Circular)
	if (result.GetMagnitude() < m_register->get<Sprite>(object).GetWidth() / 2 + 20.f) {
		result = result.Normalize();
		if (Bounce)	movement = result * movement.GetMagnitude();
		else	CurrentPosition = CurrentPosition + vec3(result.x, result.y, 0.f);
		ObjectPos = ObjectPos + vec3(-result.x / 2.f, -result.y / 2.f, 0.f);
	}
	else	CurrentPosition = CurrentPosition + (vec3(movement.x, movement.y, 0.f) * Timer::deltaTime)
		+ vec3(Acceleration.x, Acceleration.y, 0.f) * (Timer::deltaTime * Timer::deltaTime) / 2;

	if (CurrentPosition.x > 250)	CurrentPosition.x = 250;
	if (CurrentPosition.x < -250)	CurrentPosition.x = -250;
	if (CurrentPosition.y > 250)	CurrentPosition.y = 250;
	if (CurrentPosition.y < -250)	CurrentPosition.y = -250;

	if (ObjectPos.x > 200)	ObjectPos.x = 200;
	if (ObjectPos.x < -200)	ObjectPos.x = -200;
	if (ObjectPos.y > 200)	ObjectPos.y = 200;
	if (ObjectPos.y < -200)	ObjectPos.y = -200;

	m_register->get<Transform>(mainplayer).SetPosition(CurrentPosition);
	m_register->get<Transform>(object).SetPosition(ObjectPos);
	m_register->get<Transform>(tracker).SetPosition(TrackerPos);
	Force = vec2(0.f, -0.f);
	objectMov = vec2(0.f, -0.f);
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
		vec2(click) = vec2(evnt.x / windowWidth * windowHeight / 3.5 - 100, -evnt.y / 3.5 + 100)
			+ vec2(m_register->get<Camera>(EntityIdentifier::MainCamera()).GetPositionX(),
			m_register->get<Camera>(EntityIdentifier::MainCamera()).GetPositionY());

		for (int x(0); x < 4; x++) {
			vec3(buttons) = m_register->get<Transform>(EntityIdentifier::Button(x)).GetPosition();
			vec2(Pos) = click.Rotate(m_register->get<Camera>(EntityIdentifier::MainCamera()).GetRotationAngleZ()) - vec2(buttons.x, buttons.y);
			switch (x) {
			case 0:
				if (Pos.GetMagnitude() <= m_register->get<Sprite>(EntityIdentifier::Object()).GetWidth() / 2)	std::cout << "Will stop you\n";
				break;
			case 1:
				if (Pos.GetMagnitude() <= 20.f)	std::cout << "That's you\n";
				break;
			}
		}

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