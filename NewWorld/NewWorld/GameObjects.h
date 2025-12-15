#pragma once
#include "Objects.h"
#include <random>;
//#include"Window.h"
class GameObjects
{
};
class Enemies {
public:

	string Animatestatus;
	AnimatedModel enemymodel;
	AnimationInstance enemymodelinstace;
	float health = 100.f;
	Vec3 forward;
	Vec3 position;
	float dieanim = 0;
	float cooldown = 5.f;
	float timecount = 0.f;
	float movespeed = 4.f;
	void init(Core* core, Shaders* shaders, PSOManager* psos, Vec3 position, TextureManager* textures, string selftexturename, string meshname) {
		Animatestatus = "run";
		enemymodel.load(core, "../Resources/Trex/TRex.gem", shaders, psos, Animatemodels::TRex, position, textures, selftexturename, meshname);
		enemymodelinstace.init(&enemymodel.animation, 0);
	}
	void die(Vec3 heroposition) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dist(100.f, 150.f);
		float diatance = dist(gen);
		std::uniform_real_distribution<float> distAngle(0.0f, 2.0f * M_PI);
		float angle = distAngle(gen);
		float x = cosf(angle);
		float y = sinf(angle);
		enemymodel.position = Vec3(heroposition.x + diatance * x, 0, heroposition.z + diatance * y);
		health = 100.f;

	}
	void anim(float rexdt, string status, Vec3 from) {
		if (status == "death") {

			if (status != Animatestatus) {
				Animatestatus = status;
				dieanim = 0;

			}
			else {
				if (enemymodelinstace.animationFinished() == true || dieanim > 2.8999f) {
					health = 100.f;
					Animatestatus = "run";
					die(from);
					dieanim = 0;
				}
				else {

					enemymodelinstace.update("death", rexdt);
					dieanim += rexdt;
				}


			}


		}
		else {
			Animatestatus = status;
			enemymodelinstace.updatewithControl(Animatestatus, rexdt);
		}

		//enemymodelinstace.updatewithControl(Animatestatus, rexdt);
	}

};

class Hero {
public:

	bool ischange = false;;
	int score = 0;
	int health=100;
	int bullet = 30;
	bool shot=true;
	string statusanim;
	Ray raytest;
	AnimatedModel heromodel;
	AnimationInstance heromodelinstace;
	float movespeed = 10.f;
	Vec3 position;
	Vec3 forward;
	Vec3 right;
	Vec3 to;
	float cooldown = 0.2f;
	float timecount = 0.f;
	void init(Core* core, Shaders* shaders, PSOManager* psos, Vec3 position, TextureManager* textures, string selftexturename,string meshname) {
		raytest.init(Vec3(1, 1, 1), Vec3(1, 1, 1));
		heromodel.load(core, "../Resources/UZI/Uzi.gem", shaders, psos, Animatemodels::UZI, position,textures,selftexturename, meshname);
		heromodelinstace.init(&heromodel.animation, 0);
	}
	void anim(Vec3 from, vector<Enemies*> enemies, bool mouse[3], float rexdt, Vec3 forawrd,bool changeR) {
		if (changeR&&!ischange) {
			ischange = true;
			heromodelinstace.update("17 reload", rexdt);
		}
		if (bullet <= 0 && !ischange) {
			ischange = true;
			heromodelinstace.update("17 reload", rexdt);
		}
		if (ischange) {
			heromodelinstace.update("17 reload", rexdt);
			if (heromodelinstace.animationFinished()) {
				heromodelinstace.resetAnimationTime();
				ischange = false;
				bullet = 30;
				shot = true;
			}
		}
		else {
			if (!shot) {
				timecount += rexdt;
				if (timecount > cooldown) {
					shot = true;
					timecount = 0.f;
				}
			}



			if (mouse[1]) {

				if (mouse[0] && bullet > 0) {
					if (shot) {
						raytest.init(from, forawrd);
						for (int i = 0; i < enemies.size(); i++) {
							float t;
							if (enemies[i]->enemymodel.collision.rayAABB(raytest, t)) {
								enemies[i]->health -= 5;
							}

						}
						bullet--;
						shot = false;
					}
					heromodelinstace.update("13 zoom fire", rexdt);

					if (heromodelinstace.animationFinished()) {
						heromodelinstace.resetAnimationTime();
						shot = true;
					}
				}
				else {
					heromodelinstace.updatewithControl("11 zoom idle", rexdt);

				}

			}
			else if (mouse[0] && bullet > 0) {
				if (shot) {
					raytest.init(from, forawrd);
					for (int i = 0; i < enemies.size(); i++) {
						float t;
						if (enemies[i]->enemymodel.collision.rayAABB(raytest, t)) {
							enemies[i]->health -= 5;
						}

					}
					bullet--;
					shot = false;
				}
				heromodelinstace.update("08 fire", rexdt);
				if (heromodelinstace.animationFinished()) {
					heromodelinstace.resetAnimationTime();
					shot = true;
				}
			}
			else {
				heromodelinstace.updatewithControl("04 idle", rexdt);
			}
		}
	}
};

