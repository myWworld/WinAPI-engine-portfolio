#pragma once
#include "math.h"


namespace ME::math
{
#define PI 3.141592

	struct Vector2
	{

		static Vector2 One;
		static Vector2 Zero;
		static Vector2 Left;
		static Vector2 Right;
		static Vector2 Up;
		static Vector2 Down;

		float x;
		float y;

		Vector2()
			:x(0.0f)
			, y(0.0f)
		{

		}

		Vector2(float _x, float _y)
			: x(_x)
			, y(_y)
		{

		}

		static float ConvetDegree(float radian)
		{
			return radian * (180 / PI);
		}


		Vector2 operator-(Vector2 other)
		{
			Vector2 rst;
			rst.x = x - other.x;
			rst.y = y - other.y;

			return rst;
		}


		Vector2 operator-()
		{
			return Vector2(-x, -y);

		}

		Vector2 operator+(Vector2 other)
		{
			Vector2 rst;
			rst.x = x + other.x;
			rst.y = y + other.y;

			return rst;
		}

		void operator+=(Vector2 pos)
		{

			x = x + pos.x;
			y = y + pos.y;

		}

		void operator-=(Vector2 pos)
		{

			x = x - pos.x;
			y = y - pos.y;

		}

		void operator*=(Vector2 pos)
		{

			x = x * pos.x;
			y = y * pos.y;

		}

		void operator*=(float pos)
		{

			x = x * pos;
			y = y * pos;

		}


		Vector2 operator/(float value)
		{
			Vector2 rst;
			rst.x = x / value;
			rst.y = y / value;

			return rst;

		}

		Vector2 operator*(float value)
		{
			Vector2 rst;
			rst.x = x * value;
			rst.y = y * value;

			return rst;

		}

		Vector2 operator*(Vector2 pos)
		{
			Vector2 rst;
			rst.x = x * pos.x;
			rst.y = y * pos.y;

			return rst;

		}


		bool operator==(Vector2 pos)
		{


			if (x == pos.x && y == pos.y)
				return true;

			return false;

		}

		void clear()
		{
			x = 0.0f;
			y = 0.0f;
		}

		float length()
		{
			return sqrtf(x * x + y * y);
		}

		Vector2 normalize()
		{
			float len = length();

			if (len == 0)
			{
				x = 0;
				y = 0;
				return *this;
			}

			x = x / len;
			y = y / len;

			return *this;
		}

		Vector2 Rotate(float degree)
		{
			float radian = (degree / 180.0f) * PI;

			this->normalize();
			float x = cosf(radian) * x - sinf(radian) * y;
			float y = sinf(radian) * x + cosf(radian) * y;

			return Vector2(x, y);
		}

		static float Dot(Vector2 v1, Vector2 v2)
		{
			return v1.x * v2.x + v1.y * v2.y;
		}

		static float Cross(Vector2 v1, Vector2 v2)
		{
			return v1.x * v2.y - v1.y * v2.x;
		}



	};




}