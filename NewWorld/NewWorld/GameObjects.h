#pragma once
#include "Objects.h"

class GameObjects
{
};
class Hero {
public:
	AnimatedModel heromodel;
	AnimationInstance heromodelinstace;
	float movespeed = 10.f;
	Vec3 position;
	Vec3 forward;
	Vec3 right;
	Vec3 to;
	float cooldown = 0.2f;
	float timecount = 0.f;
	void init(Core* core, Shaders* shaders, PSOManager* psos, Vec3 position) {
		heromodel.load(core, "../Resources/UZI/Uzi.gem", shaders, psos, Animatemodels::UZI, position);
		heromodelinstace.init(&heromodel.animation, 0);
	}

};

class Enemies {
public:
	string Animatestatus;
	AnimatedModel enemymodel;
	AnimationInstance enemymodelinstace;
	float health = 100.f;
	Vec3 forward;
	Vec3 position;
	float cooldown = 5.f;
	float timecount = 0.f;
	float movespeed = 0.5f;
	void init(Core* core, Shaders* shaders, PSOManager* psos, Vec3 position) {
		Animatestatus = "run";
		enemymodel.load(core, "../Resources/Trex/TRex.gem", shaders, psos, Animatemodels::TRex, position);
		enemymodelinstace.init(&enemymodel.animation, 0);
	}

};
