// NewWorld.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include"Window.h"
#include "LoadControl.h"

#include <iostream>
#include "Light.h"










extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow) {
	PSOManager psos;
	vector<StaticModle> staticmodles;
	vector<AnimatedModel>animateModels;
	vector<AnimationInstance>animationinstances;
	vector<Enemies> enemies;
	vector<StaticModle> test;
	LoadControl loadcontrol;
	vector<Objectload> loadgamestatic;
	vector<Objectload> loadgameanim;
	Window win;
	Core core;
	core.init(window->hwnd, kuan, gao);
	Light light;
	Shader shader;


	Shaders shaders;
	shaders.load(&core, "shader1", "ShaderVertices.hlsl", "ShaderTexture.hlsl");
	shaders.load(&core, "shaderAnim", "ShaderVerticesAnim.hlsl", "ShaderPixel.hlsl");
	shaders.load(&core, "shaderTexture", "ShaderVerticesAnim.hlsl", "ShaderTexture.hlsl");
	shaders.load(&core, "shaderSkyBox", "ShaderskyboxVector.hlsl", "ShaderTexture.hlsl");
	shaders.load(&core, "shaderlight", "ShaderTextureLight.hlsl", "ShaderTextureLight.hlsl");
	Cube cube;
	cube.init(&core,&psos, &shaders.shaders["shader1"],Vec3(10,0,10));
	Sphere sphere;
	sphere.init(&core, &psos, &shaders.shaders["shader1"], Vec3(30, 0, 40));


	
	vector<string> texturenames;
	TextureManager textures;


	

	texturenames.push_back("../Resources/NightSkyHDRI009_12K_TONEMAPPED.jpg");
	textures.load(&core, texturenames, "SkyBox");
	texturenames.clear();
	texturenames.push_back("../Resources/Grass/Grass01_2K_BaseColor.png");
	textures.load(&core, texturenames, "Grass");
	

;


	
	  loadcontrol.LoadData(&loadgamestatic,&loadgameanim);
	Hero hero;
	hero.init(&core, &shaders, &psos, Vec3(0, 0, 0),&textures,"hero");
	hero.heromodel.collision.hero(hero.position);
	
	for (int i = 0; i < loadgamestatic.size(); i++) {
		StaticModle scvv;
		staticmodles.push_back(scvv);
		staticmodles[i].load(&core, loadgamestatic[i].location, &shaders, &psos, Staticmodels::Tree, loadgamestatic[i].position, &textures, loadgamestatic[i].textureName,loadgamestatic[i].iscollider);
	}
	for (int i = 0; i < loadgameanim.size(); i++) {
		Enemies scvv;
		enemies.push_back(scvv);
		enemies[i].init(&core, &shaders, &psos, loadgameanim[i].position, &textures, loadgameanim[i].textureName);
	}




	StaticModleLight lighttest;
	lighttest.load(&core, "../Resources/OtherTree/pine1.gem", &shaders, &psos, Staticmodels::Tree, Vec3(10, 0, 10), &textures, "pine", 0);
	
	
	
	
	
	
	
	Matrix world;
	world = world.scale(0.01f, 0.01f, 0.01f);
	Matrix prespection;
	prespection = prespection.Perspective(M_PI / 4, kuan / gao, 0.1f, 100.0f);
	Matrix lookat;
	Matrix vp;
	GamesEngineeringBase::Timer timer;
	win.create(kuan, gao, "My Window");
	float dt=0;
	float rexdt;
	float cameramovespeed = 5.f;
	Vec4 to = Vec4(0, 0, 0, 0);
	Vec4 up = Vec4(0, 1, 0, 0);
	Vec4 from = Vec4(0,0,0, 0);
	Vec4 forward = Vec4(1, 0, 0, 0);
	Vec4 right;
	to = forward + from;
	right = forward.Cross(Vec4(0, 1, 0, 0));
	float yaw = 0.0f;    // ÈÆYÖáÐý×ª
	float pitch = 0.0f;
	static int lastMouseX = window->mousex;
	static int lastMouseY = window->mousey;

	int deltaX = window->mousex - lastMouseX;
	int deltaY = window->mousey - lastMouseY;
	float mouseSensitivity = 0.01f;
	while (1) {
		rexdt = timer.dt();
		dt += rexdt;
		
		deltaX = window->mousex - lastMouseX;
		deltaY = window->mousey - lastMouseY;
		yaw -= deltaX * mouseSensitivity;
		pitch -= deltaY * mouseSensitivity;

		float limit = 1.5f; 
		if (pitch > limit) pitch = limit;
		if (pitch < -limit) pitch = -limit;

		forward.x = cosf(pitch) * cosf(yaw);
		forward.y = sinf(pitch);
		forward.z = cosf(pitch) * sinf(yaw);
		forward.w = 0;
		lastMouseX = window->mousex;
		lastMouseY = window->mousey;
		
		if (win.keys[VK_ESCAPE] == 1)
		{
			break;
		}
		
		if (win.keys['A']) {
			from = from + Vec4(right.x, 0, right.z, 0) * cameramovespeed * rexdt;
			hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
			
			for (int i = 0; i < enemies.size(); i++) {
				if (hero.heromodel.collision.AABBtest(enemies[i].enemymodel.collision.realminpoint, enemies[i].enemymodel.collision.realmaxpoint)) {
					from = from - Vec4(right.x, 0, right.z, 0) * cameramovespeed * rexdt;
					hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
				}
			}
			for (int i = 0; i < staticmodles.size(); i++) {
				if (hero.heromodel.collision.AABBtest(staticmodles[i].collision.realminpoint, staticmodles[i].collision.realmaxpoint)) {
					from = from - Vec4(right.x, 0, right.z, 0) * cameramovespeed * rexdt;
					hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
				}
			}
		}
		if (win.keys['D']) {
			from = from - Vec4(right.x, 0, right.z, 0) * cameramovespeed * rexdt;
			hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
			for (int i = 0; i < enemies.size(); i++) {
				if (hero.heromodel.collision.AABBtest(enemies[i].enemymodel.collision.realminpoint, enemies[i].enemymodel.collision.realmaxpoint)) {
					from = from + Vec4(right.x, 0, right.z, 0) * cameramovespeed * rexdt;
					hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
				}
			}
			for (int i = 0; i < staticmodles.size(); i++) {
				if (hero.heromodel.collision.AABBtest(staticmodles[i].collision.realminpoint, staticmodles[i].collision.realmaxpoint)) {
					from = from + Vec4(right.x, 0, right.z, 0) * cameramovespeed * rexdt;
					hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
				}
			}
		}
		if (win.keys['W']) {
			from = from+Vec4(forward.x,0, forward.z,0) * cameramovespeed * rexdt;
			hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
			for (int i = 0; i < enemies.size(); i++) {
				if (hero.heromodel.collision.AABBtest(enemies[i].enemymodel.collision.realminpoint, enemies[i].enemymodel.collision.realmaxpoint)) {
					from = from - Vec4(forward.x, 0, forward.z, 0) * cameramovespeed * rexdt;
					hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
					
				}
			}
			for (int i = 0; i < staticmodles.size(); i++) {
				if (hero.heromodel.collision.AABBtest(staticmodles[i].collision.realminpoint, staticmodles[i].collision.realmaxpoint)) {
					from = from - Vec4(forward.x, 0, forward.z, 0) * cameramovespeed * rexdt;
					hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));

				}
			}

		}
		if (win.keys['S']) {
			from = from - Vec4(forward.x, 0, forward.z, 0) * cameramovespeed * rexdt;
			hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
			for (int i = 0; i < enemies.size(); i++) {
				if (hero.heromodel.collision.AABBtest(enemies[i].enemymodel.collision.realminpoint, enemies[i].enemymodel.collision.realmaxpoint)) {
					from = from + Vec4(forward.x, 0, forward.z, 0) * cameramovespeed * rexdt;
					hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
				}
			}
			for (int i = 0; i < staticmodles.size(); i++) {
				if (hero.heromodel.collision.AABBtest(staticmodles[i].collision.realminpoint, staticmodles[i].collision.realmaxpoint)) {
					from = from + Vec4(forward.x, 0, forward.z, 0) * cameramovespeed * rexdt;
					hero.heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
				}
			}
		}
		hero.heromodel.position = Vec3(from.x,from.y,from.z);

		
		to = forward + from;
		right = forward.Cross(Vec4(0, 1, 0, 0));
		lookat=lookat.LookatMatrix(from, to, up);

		vp = prespection.mul(lookat);
	
		core.beginFrame();
		win.processMessages();
		

		core.beginRenderPass();



		for (int i = 0; i < staticmodles.size(); i++) {
			
			//staticmodles[i].draw(&core, &staticmodles[i].realshow, &vp, &shaders.shaders["shader1"], &psos, textures.find(staticmodles[i].texturename));
		}

	
		
		//othertree.draw(&core, &othertree.realshow, &vp, &shaders.shaders["shader1"], &psos, textures.find(othertree.texturename));
		//othertree2.draw(&core, &othertree2.realshow, &vp, &shaders.shaders["shader1"], &psos, textures.find(othertree2.texturename));
		//othertree3.draw(&core, &othertree3.realshow, &vp, &shaders.shaders["shader1"], &psos, textures.find(othertree3.texturename));
		
		for (int i = 0; i < enemies.size(); i++) {
			
			enemies[i].enemymodelinstace.updatewithControl(enemies[i].Animatestatus, rexdt);
			
			
			Vec3 scv;
			enemies[i].enemymodel.position = enemies[i].enemymodel.position + enemies[i].enemymodel.forward * enemies[i].movespeed * rexdt;//move
			if (enemies[i].enemymodel.collision.AABBtest(hero.heromodel.collision.realminpoint, hero.heromodel.collision.realmaxpoint)) {
				enemies[i].enemymodel.position = enemies[i].enemymodel.position - enemies[i].enemymodel.forward * enemies[i].movespeed * rexdt;
				enemies[i].Animatestatus = "attack";
			}
			else {
				enemies[i].Animatestatus = "run";
			}
			scv =  hero.heromodel.position- enemies[i].enemymodel.position;//calculate the new forward
			scv = scv.normalize();
			enemies[i].enemymodel.forward = scv;

			Matrix R = Matrix::ForwardtoOnlyTRex(scv);
			enemies[i].enemymodel.collision.update(enemies[i].enemymodel.position, R);
			enemies[i].enemymodel.draw(&core, &enemies[i].enemymodel.realshow, &vp, &shaders.shaders["shaderTexture"], &psos, &enemies[i].enemymodelinstace, R,textures.find(enemies[i].enemymodel.texturename));
				
			
		}
		
		hero.heromodelinstace.update("08 fire", rexdt);
		if (hero.heromodelinstace.animationFinished()) {
			hero.heromodelinstace.resetAnimationTime();
		}
		
		Matrix R;
		R = Matrix::ForwardtoTOnlyHero(forward.TransToVec3RemoveW());
		
		hero.heromodel.draw(&core, &hero.heromodel.realshow, &vp, &shaders.shaders["shaderTexture"], &psos, &hero.heromodelinstace,R, textures.find(hero.heromodel.texturename));
		
		cube.draw(&core, &cube.realshow, &vp, &shaders.shaders["shader1"], &psos, textures.find("Grass"));
		Matrix C;
		C=C.translation(Vec3(from.x,0,from.z));
		Vec3 from3 = Vec3(from.x, from.y, from.z);
		lighttest.draw(&core, &lighttest.realshow, &vp,&from3,&light.Strength,&light.Direction, &shaders.shaders["shaderlight"], &psos, textures.find("pine"));
		
		
		
		sphere.draw(&core, &C, &vp, &shaders.shaders["shader1"], &psos, textures.find("SkyBox"));
		
		
		core.finishFrame();
	}
	core.flushGraphicsQueue();

}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
