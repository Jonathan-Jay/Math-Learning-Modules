#include "EntityManager.h"

Selectable EntityManager::m_selectable = Selectable("Entity Manager");
int CurrentAnim = 0;
bool Animation0 = false;
bool Animation1 = false;
bool Swaping = false;

bool randomized = false;
bool randomized2 = false;
bool rotating = false;
bool rotating2 = false;
entt::registry* EntityManager::m_register = nullptr;

void EntityManager::InitManager(entt::registry* temp)
{
	m_register = temp;
}

void EntityManager::CreateTab()
{
	bool temp(false);

	if (ImGui::BeginTabItem(m_selectable.GetName().c_str(), m_selectable.GetSelected())) {

		temp = true;

		ImGui::EndTabItem();
	}

	if (temp) {
		CreateManager();
	}
}

void EntityManager::CreateManager()
{
	if (m_selectable.GetSelected()) {
		int mainplayer = EntityIdentifier::MainPlayer(), helloworld = EntityIdentifier::HelloWorld(), maincamera = EntityIdentifier::MainCamera();
		vec3(MainPlayerPos) = m_register->get<Transform>(mainplayer).GetPosition();
		vec3(HelloWorldPos) = m_register->get<Transform>(helloworld).GetPosition();
		Degrees CameraRot = Transform::ToDegrees(-m_register->get<Camera>(maincamera).GetRotationAngleZ());
		vec3(CameraPos) = m_register->get<Camera>(maincamera).GetPosition();

		if (ImGui::Button("Reset All", ImVec2(200.f, 20.f))) {
			MainPlayerPos = vec3(0.f, 0.f, 10.f);
			HelloWorldPos = vec3(0.f, 75.f, 100.f);
			CameraRot = 0.f;
			randomized = false;
			randomized2 = false;
			rotating = false;
			rotating2 = false;
			m_register->get<Camera>(maincamera).SetPosition(MainPlayerPos);
		}

		if (ImGui::TreeNode("Main Player")) {
			if (ImGui::TreeNode("Animation")) {
				CurrentAnim = CurrentAnim = m_register->get<AnimationController>(mainplayer).GetActiveAnim();

				if (ImGui::Button("Switch Animation", ImVec2(200.f, 20.f))) {
					if (CurrentAnim == 0)	CurrentAnim = 1;
					else	CurrentAnim = 0;
				}

				if (CurrentAnim == 0) {
					Animation0 = true;
					Animation1 = false;
				}
				else {
					Animation0 = false;
					Animation1 = true;
				}

				if (ImGui::Checkbox("Facing Left", &Animation0)) {
					Animation1 = false;
					CurrentAnim = 0;
				}
				if (ImGui::Checkbox("Facing Right", &Animation1)) {
					Animation0 = false;
					CurrentAnim = 1;
				}

				ImGui::Checkbox("Continuous Swap", &Swaping);
				if (Swaping) {
					if (CurrentAnim == 0)	CurrentAnim = 1;
					else	CurrentAnim = 0;
				}

				m_register->get<AnimationController>(mainplayer).SetActiveAnim(CurrentAnim);
				ImGui::TreePop();
			}

			float Range[3] = { MainPlayerPos.x, MainPlayerPos.y + 200, MainPlayerPos.z * 2.5 };
			if (ImGui::DragFloat3("Position (x, y, z)", &Range[0], 1.f, -250.f, 250.f)) {
				MainPlayerPos = vec3(Range[0], Range[1] - 200, Range[2] / 2.5);
			}

			if (ImGui::Button("Reset Position", ImVec2(200.f, 20.f))) {
				MainPlayerPos = vec3(0.f, 0.f, 10.f);
			}

			ImGui::Checkbox("Randomize Position", &randomized);
			if (randomized) {
				MainPlayerPos = vec3(((rand() % 500) - 250), ((rand() % 500) - 250), 0.f);
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Hello World Sign")) {
			float Range2[2] = { HelloWorldPos.x, HelloWorldPos.y };
			if (ImGui::DragFloat2("Position (x, y)", &Range2[0], 1.f, -250.f, 250.f)) {
				HelloWorldPos = vec3(Range2[0], Range2[1], HelloWorldPos.z);
			}

			if (ImGui::Button("Reset Position ", ImVec2(200.f, 20.f))) {
				HelloWorldPos = vec3(0.f, 75.f, 100.f);
			}

			ImGui::Checkbox("Randomize Position ", &randomized2);
			if (randomized2) {
				HelloWorldPos = vec3(((rand() % 500) - 250), ((rand() % 500) - 250), HelloWorldPos.z);
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Camera")) {
			ImGui::DragFloat("Camera Angle", &CameraRot);

			if (ImGui::Button("Reset Angle", ImVec2(200.f, 20.f))) {
				CameraRot = 0.f;
			}

			ImGui::Checkbox("Rotating Clockwise", &rotating);
			if (rotating) {
				CameraRot++;
			}

			ImGui::Checkbox("Rotating Counter Clockwise", &rotating2);
			if (rotating2) {
				CameraRot--;
			}

			float Range3[2] = { CameraPos.x, CameraPos.y };
			if (ImGui::DragFloat2("Position (x, y)", &Range3[0], 1.f, -250.f, 250.f)) {
				CameraPos = vec3(Range3[0], Range3[1], CameraPos.z);
			}

			if (ImGui::Button("Focus", ImVec2(200.f, 20.f))) {
				m_register->get<Camera>(maincamera).SetPosition(MainPlayerPos);
			}

			if (CameraRot < 0) {
				CameraRot = 359;
			}
			else if (CameraRot >= 360) {
				CameraRot = 0;
			}
			
			ImGui::TreePop();
		}

		m_register->get<Transform>(mainplayer).SetPosition(MainPlayerPos);
		m_register->get<Transform>(helloworld).SetPosition(HelloWorldPos);
		m_register->get<Camera>(maincamera).SetRotationAngleZ(Transform::ToRadians(-CameraRot));
		m_register->get<Camera>(maincamera).SetPosition(CameraPos);
	}
}