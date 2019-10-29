#include "Vector.h"

vec2::vec2()
{
}

vec2::vec2(float _x, float _y)
{
	x = _x;
	y = _y;
}

float vec2::GetMagnitude()
{
	return float(sqrt((x * x) + (y * y)));
}

float vec2::GetMagnitudeSquared()
{
	float magnitude = GetMagnitude();
	
	return magnitude * magnitude;
}

vec2 vec2::Normalize()
{
	vec2 normalized = *this / GetMagnitude();
	
	return normalized;
}

vec2 vec2::Project(vec2 b)
{
	vec2 a = *this;
	
	//Equation
	//     A dot B
	//B * ---------
	//     ||B||^2
	
	
	float numerator = a.Dot(b);
	float denominator = b.GetMagnitudeSquared();

	return (b * (numerator / denominator));
}

void vec2::Subtract(vec2 v1)
{
	this->x -= v1.x;
	this->y -= v1.y;
}

void vec2::MultScalar(float s)
{
	this->x *= s;
	this->y *= s;
}

void vec2::DivScalar(float s)
{
	this->x /= s;
	this->y /= s;
}

float vec2::Dot(vec2 v2)
{
	return float(x * v2.x + y * v2.y);
}

float vec2::operator[](int i)
{
	return *hold[i];
}

vec2 vec2::operator-()
{
	return vec2(-x, -y);
}

vec2 vec2::operator+(vec2 v1)
{
	return vec2(this->x + v1.x, this->y + v1.y);
}

vec2 vec2::operator-(vec2 v1)
{
	return vec2(this->x - v1.x, this->y - v1.y);
}

vec2 vec2::operator*(float f)
{
	return vec2(this->x * f, this->y * f);
}

vec2 vec2::operator/(float s)
{
	return vec2(this->x / s, this->y / s);
}

vec3::vec3()
{
}

vec3::vec3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

float vec3::GetMagnitude()
{
	return float(sqrt((x * x) + (y * y) + (z * z)));
}

float vec3::operator[](int i)
{
	return *hold[i];
}

vec3 vec3::operator-()
{
	return vec3( -x, -y, -z);
}

vec3 vec3::operator+(vec3 v2)
{
	return vec3(this->x + v2.x, this->y + v2.y, this->z + v2.z);
}

vec3 vec3::operator-(vec3 v2)
{
	return vec3(this->x - v2.x, this->y - v2.y, this->z - v2.z);
}

vec3 vec3::operator*(float f)
{
	return vec3(this->x * f, this->y * f, this->z * f);
}

vec3 vec3::operator/(float f)
{
	return vec3(this->x / f, this->y / f, this->z / f);
}

vec4::vec4()
{
}

vec4::vec4(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

float vec4::GetMagnitude()
{
	return float(sqrt((x * x) + (y * y) + (z * z) + (w * w)));
}

float vec4::operator[](int i)
{
	return *hold[i];
}
