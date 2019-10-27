#include "Scroll.h"

Scroll::Scroll()
{
}

void Scroll::Update()
{
	Radians(angle) = -m_cam->GetRotationAngleZ();
	vec3(temp) = m_focus->GetPosition();
	vec3(current) = m_cam->GetPosition();
	vec3(LP) = current;
	LP.x = LP.x * cos(angle) + LP.y * sin(angle);
	LP.y = LP.y * cos(angle) + LP.x * sin(angle);
	float setx = m_offsetx;
	float sety = m_offsety;
	printf("t: %f, %f\n", temp.x, temp.y);
	
	bool change = false;
	
	//above focus y
	if (temp.y > LP.y + sety) {
		current.y += (temp.y - (LP.y + sety)) * cos(angle);
		current.x += (temp.y - (LP.y + sety)) * sin(angle);
		change = true;
	}

	//below focus y
	if (temp.y < LP.y - sety) {
		current.y += (temp.y - (LP.y - sety)) * cos(angle);
		current.x += (temp.y - (LP.y - sety)) * sin(angle);
		change = true;
	}

	//above focus x
	if (temp.x > LP.x + setx) {
		current.x += (temp.x - (LP.x + setx)) * cos(angle);
		current.y += (temp.x - (LP.x + setx)) * sin(angle);
		change = true;
	}

	//below focus x
	if (temp.x < LP.x - setx) {
		current.x += (temp.x - (LP.x - setx)) * cos(angle);
		current.y += (temp.x - (LP.x - setx)) * sin(angle);
		change = true;
	}

	printf("c: %f, %f\n", current.x, current.y);

	if (change) m_cam->SetPosition(current);
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