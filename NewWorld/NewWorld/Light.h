#pragma once
#include"MathTool.h"
using namespace MathTool;
class Light
{
public:
	Vec3 Strength;
	Vec3 Direction;
	Light() {
		Strength = Vec3(3,3,3);
		Direction = Vec3(0.5f, -1.0f, 0.3f).normalize();
	}

};

