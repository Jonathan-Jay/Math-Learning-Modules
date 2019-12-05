#ifndef __VECTOR_H__
#define __VECTOR_H__
#include <math.h>
#include <vector>

class vec2
{
public:
	//Empty constructor
	vec2();
	//Creates vec2 with x and y
	vec2(float _x, float _y);

	//Defaults to zero
	float x, y = 0.f;
	//Used for indexing using [] operator
	float* hold[2] = { &x, &y };

	void Subtract(vec2 v1);
	void MultScalar(float s);
	void DivScalar(float s);

	float Dot(vec2 v2);

	float GetMagnitude();
	float GetMagnitudeSquared();
	vec2 Normalize();
	vec2 Project(vec2 b);
	float GetAngle(vec2 b);

	vec2 Rotate(float angle);

	//Operator overload for indexing using []
	float operator[](int i);
	vec2 operator-();
	vec2 operator+(vec2 v1);
	vec2 operator-(vec2 v1);

	vec2 operator*(float f);
	vec2 operator/(float s);
};

class vec3
{
public:
	//Empty constructor
	vec3();
	//Creates vec3 with x, y, and z
	vec3(float _x, float _y, float _z);
	
	//Defaults to zero
	float x, y, z = 0.f;
	//Used for indexing using [] operator
	float* hold[3] = { &x, &y, &z };

	void Subtract(vec3 v1);
	void MultScalar(float s);
	void DivScalar(float s);

	float Dot(vec3 v2);

	float GetMagnitude();
	float GetMagnitudeSquared();
	vec3 Normalize();
	vec3 Project(vec3 b);

	//Operator overload for indexing using []
	float operator[](int i);
	//Negation operator for creating a negative version of the vector
	vec3 operator-();
	vec3 operator+(vec3 v2);
	vec3 operator-(vec3 v2);

	vec3 operator*(float f);
	vec3 operator/(float f);
};

class vec4
{
public:
	//empty constructor
	vec4();
	//Creates vec4 with x,y,z and w
	vec4(float _x, float _y, float _z, float _w);

	//Defaults to zero
	float x, y, z, w = 0.f;
	//Used for indexing using [] operator
	float* hold[4] = { &x, &y, &z, &w };

	void Subtract(vec4 v1);
	void MultScalar(float s);
	void DivScalar(float s);

	float Dot(vec4 v2);

	float GetMagnitude();
	float GetMagnitudeSquared();
	vec4 Normalize();
	vec4 Project(vec4 b);

	//Operator overload for indexing using []
	float operator[](int i);
	vec4 operator-();
	vec4 operator+(vec4 v2);
	vec4 operator-(vec4 v2);

	vec4 operator*(float s);
	vec4 operator/(float s);
};

class vecN
{
public:
	
	//creates a Vec5 (default size)
	vecN();

	//creates a vecN of size "size"
	vecN(int size);

	std::vector<float> m_components;

	int GetSize();

	float Dot(vecN v2);
	float GetMagnitude();
	float GetMagnitudeSquared();
	vecN Normalize();

	//Operator overload for indexing using []
	float operator[](int i);
	vecN operator-();
	vecN operator+(vecN v2);
	vecN operator-(vecN v2);

	vecN operator*(float s);
	vecN operator/(float s);
};

#endif // !__VECTOR_H__
