#include "EntityManager.h"

Selectable EntityManager::m_selectable = Selectable("Camera Manager");
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
		int maincamera = EntityIdentifier::MainCamera();

		vec3(CameraPos) = m_register->get<Camera>(maincamera).GetPosition();

		Degrees CameraRot = Transform::ToDegrees(-m_register->get<Camera>(maincamera).GetRotationAngleZ());

		if (ImGui::TreeNode("Camera")) {
			ImGui::DragFloat("Camera Angle Z", &CameraRot);

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

			if (CameraRot < 0) {
				CameraRot = 359;
			}
			else if (CameraRot >= 360) {
				CameraRot = 0;
			}
			
			ImGui::TreePop();
		}

		m_register->get<Camera>(maincamera).SetRotationAngleZ(Transform::ToRadians(-CameraRot));
		m_register->get<Camera>(maincamera).SetPosition(CameraPos);
	}
}

std::vector<int> EntityStorage::storage = {};

void EntityStorage::StoreEntity(int entity, int number)
{
	if (storage.size() < number + 1) {
		storage.resize(number + 1);
	}

	storage[number] = entity;
}

int EntityStorage::GetEntity(int number)
{
	return storage[number];
}