#pragma once
#include "Objects.h"
#include <random>;
//#include"Window.h"
class GameObjects
{
};
class Hero;
class Enemies {
public:
	float distance = 10000;
	bool isdying = false;
	float attatcktime=0;
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
		//respawn at random position around hero
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
				isdying = true;
				dieanim = 0;

			}
			else {
				if (enemymodelinstace.animationFinished() == true || dieanim > 2.8999f) {
					health = 100.f;
					Animatestatus = "run";

					die(from);
					isdying = false;
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
			if (enemymodelinstace.animationFinished()) {
				enemymodelinstace.resetAnimationTime();
				attatcktime = 0;
			}

		}
	}

};

class Hero {
public:
	static Hero instance() {
		static Hero instance;
		return instance;
	}
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
	void anim(Vec3 from, vector<Enemies*> enemies, bool mouse[3], float rexdt, Vec3 forawrd,bool changeR,vector<Fire*> fires) {
		if (changeR&&!ischange) {
			ischange = true;
			heromodelinstace.update("17 reload", rexdt);
			//start reload
		}
		if (bullet <= 0 && !ischange) {
			ischange = true;
			heromodelinstace.update("17 reload", rexdt);
			//auto reload
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
				//zoom
				if (mouse[0] && bullet > 0) {
					if (shot) {
						// zoom shot
						raytest.init(from+forward*3, forawrd);
						///add three Make sure the bullet is fired in front of the camera.
						for (int i = 0; i < enemies.size(); i++) {
							float t;
							if (enemies[i]->enemymodel.collision.rayAABB(raytest, t)) {
								//hit the target or not
								enemies[i]->health -= 5;
								for(int i=0;i< fires.size();i++){
									if(!fires[i]->active){
										
											fires[i]->work(rexdt, from + (forawrd * (t + 3)));
										//particle effect
										
										
										break;
									}
								}
								if (enemies[i]->health <= 0&&!enemies[i]->isdying) {
									score += 5;
									enemies[i]->isdying = true;
								}
								break;
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
				//normal shot
				if (shot) {
					raytest.init(from + forward * 3, forawrd);
					for (int i = 0; i < enemies.size(); i++) {
						float t;
						if (enemies[i]->enemymodel.collision.rayAABB(raytest, t)) {
							enemies[i]->health -= 5;
							for (int i = 0; i < fires.size(); i++) {
								if (!fires[i]->active) {
									
										fires[i]->work(rexdt, from + (forawrd * (t+3)));
									
									
									break;
								}
							}
							if (enemies[i]->health <= 0 && !enemies[i]->isdying) {
								score += 5;
								enemies[i]->isdying = true;
							}
							break;
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
				//idle
			}
		}
	}
};

