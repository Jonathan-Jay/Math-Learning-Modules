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
		vec3(temp) = m_register->get<Transform>(mainplayer).GetPosition();
		vec3(temp2) = m_register->get<Transform>(helloworld).GetPosition();
		Degrees temp3 = Transform::ToDegrees(-m_register->get<Camera>(maincamera).GetRotationAngleZ());

		if (ImGui::Button("Reset All", ImVec2(200.f, 20.f))) {
			temp = vec3(0.f, 0.f, 10.f);
			temp2 = vec3(0.f, 75.f, 100.f);
			temp3 = 0.f;
			m_register->get<Camera>(maincamera).SetPosition(temp);
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

			float Range[3] = { temp.x, temp.y, temp.z * 2.5 };
			if (ImGui::DragFloat3("Position (x, y, z)", &Range[0], 1.f, -250.f, 250.f)) {
				temp = vec3(Range[0], Range[1], Range[2] / 2.5);
			}

			if (ImGui::Button("Reset Position", ImVec2(200.f, 20.f))) {
				temp = vec3(0.f, 0.f, 10.f);
			}

			ImGui::Checkbox("Randomize Position", &randomized);
			if (randomized) {
				temp = vec3(((rand() % 500) - 250), ((rand() % 500) - 250), 0.f);
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Hello World Sign")) {
			float Range2[2] = { temp2.x, temp2.y };
			if (ImGui::DragFloat2("Position (x, y)", &Range2[0], 1.f, -250.f, 250.f)) {
				temp2 = vec3(Range2[0], Range2[1], temp2.z);
			}

			if (ImGui::Button("Reset Position ", ImVec2(200.f, 20.f))) {
				temp2 = vec3(0.f, 75.f, 100.f);
			}

			ImGui::Checkbox("Randomize Position ", &randomized2);
			if (randomized2) {
				temp2 = vec3(((rand() % 500) - 250), ((rand() % 500) - 250), temp2.z);
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Camera")) {
			ImGui::DragFloat("Camera Angle", &temp3);

			if (ImGui::Button("Reset Angle", ImVec2(200.f, 20.f))) {
				temp3 = 0.f;
			}

			ImGui::Checkbox("Rotating Clockwise", &rotating);
			if (rotating) {
				temp3++;
			}

			ImGui::Checkbox("Rotating Counter Clockwise", &rotating2);
			if (rotating2) {
				temp3--;
			}

			if (ImGui::Button("Focus", ImVec2(200.f, 20.f))) {
				m_register->get<Camera>(maincamera).SetPosition(temp);
			}

			if (temp3 < 0) {
				temp3 = 359;
			}
			else if (temp3 >= 360) {
				temp3 = 0;
			}
			
			ImGui::TreePop();
		}

		m_register->get<Transform>(mainplayer).SetPosition(temp);
		m_register->get<Transform>(helloworld).SetPosition(temp2);
		m_register->get<Camera>(maincamera).SetRotationAngleZ(Transform::ToRadians(-temp3));
	}
}