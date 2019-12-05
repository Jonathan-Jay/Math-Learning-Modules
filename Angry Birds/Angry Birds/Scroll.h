#pragma once
#include "Camera.h"

class Scroll
{
public:
	Scroll();

	void Update();

	Camera* GetCam() const;
	Transform* GetFocus() const;
	float GetOffsetx() const;
	float GetOffsety() const;

	void SetCam(Camera* cam);
	void SetFocus(Transform* focus);
	void SetOffsetx(float offsetx);
	void SetOffsety(float offsety);

private:
	Camera* m_cam = nullptr;
	Transform* m_focus = nullptr;

	float m_offsetx = 5.f;
	float m_offsety = 5.f;

};

inline void from_json(const nlohmann::json& j, Scroll& Scroll) {
	Scroll.SetOffsetx(j["Offsetx"]);
	Scroll.SetOffsety(j["Offsety"]);
}

inline void to_json(nlohmann::json& j, const Scroll& Scroll) {
	j["Offsetx"] = Scroll.GetOffsetx();
	j["Offsety"] = Scroll.GetOffsety();
}