#include "Scroll.h"

Scroll::Scroll()
{
}

void Scroll::Update()
{
	Radians(angle) = m_cam->GetRotationAngleZ();
	vec3(temp) = m_focus->GetPosition();
	vec3(current) = m_cam->GetPosition();
	current = vec3(current.x * cos(angle) - current.y * sin(angle), current.y * cos(angle) + current.x * sin(angle), current.z);
	vec3(LP) = m_cam->m_localPosition;
	LP = vec3(LP.x * cos(angle) - LP.y * sin(angle), LP.y * cos(angle) + LP.x * sin(angle), LP.z);
	bool change = false;
	
	//above focus y
	if (temp.y > LP.y + m_offsety) {
		current.y += (temp.y - (LP.y + m_offsety));
		change = true;
	}

	//below focus y
	if (temp.y < LP.y - m_offsety) {
		current.y += (temp.y - (LP.y - m_offsety));
		change = true;
	}

	//above focus x
	if (temp.x > LP.x + m_offsetx) {
		current.x += (temp.x - (LP.x + m_offsetx));
		change = true;
	}

	//below focus x
	if (temp.x < LP.x - m_offsetx) {
		current.x += (temp.x - (LP.x - m_offsetx));
		change = true;
	}

	if (change) {
		current = vec3(current.x * cos(-angle) - current.y * sin(-angle), current.y * cos(-angle) + current.x * sin(-angle), current.z);
		m_cam->SetPosition(current);
	}
}

Camera* Scroll::GetCam() const
{
	return m_cam;
}

Transform* Scroll::GetFocus() const
{
	return m_focus;
}

float Scroll::GetOffsetx() const
{
	return m_offsetx;
}

float Scroll::GetOffsety() const
{
	return m_offsety;
}

void Scroll::SetCam(Camera* cam)
{
	m_cam = cam;
}

void Scroll::SetFocus(Transform* focus)
{
	m_focus = focus;
}

void Scroll::SetOffsetx(float offset)
{
	m_offsetx = offset;
}

void Scroll::SetOffsety(float offset)
{
	m_offsety = offset;
}