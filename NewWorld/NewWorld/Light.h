#pragma once
#include"MathTool.h"
using namespace MathTool;
class Light
{
public:
	Vec3 Strength;
	Vec3 Direction;
	Light() {
		Strength = Vec3(1.0f, 1.0f, 1.0f);
		Direction = Vec3(0, -1, 0);
	}

};

