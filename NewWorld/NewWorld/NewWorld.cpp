// NewWorld.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <random>

#include"Window.h"
#include "LoadControl.h"

#include <iostream>
#include "Light.h"





vector<int> calculatenumber(int number) {
	vector<int> scv;
	if (number == 0) {
		scv.push_back(0);
		return scv;
	}
	int sss;
	while (number / 10 != 0) {
		scv.push_back(number % 10);

		number = number / 10;
	}
	scv.push_back(number % 10);
	return scv;
}




extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow) {
	PSOManager psos;
	vector<StaticModleLight> staticmodles;
	vector<AnimatedModel>animateModels;
	vector<AnimationInstance>animationinstances;
	vector<Enemies*> enemies;
	vector<StaticModle> test;
	vector<Cube*> cubes;
	vector<Font*> healthnumber;
	vector<Font*> scorenumber;
	vector<Font*> bulletnumber;
	LoadControl loadcontrol;
	vector<Objectload> loadgamestatic;
	vector<Objectload> loadgameanim;
	vector<Objectload> loadinsatnceinformatiojn;
	vector<vector<Vec3>> loadinstanceposition;
	vector<Fire*>fires;
	Window win;
	Core core;
	core.init(window->hwnd, kuan, gao);
	Light light;
	Shader shader;


	Shaders shaders;
	shaders.load(&core, "shader1", "shaders/ShaderVertices.hlsl", "shaders/ShaderTexture.hlsl");
	shaders.load(&core, "shaderAnim", "shaders/ShaderVerticesAnim.hlsl", "shaders/ShaderPixel.hlsl");
	shaders.load(&core, "shaderTexture", "shaders/ShaderVerticesAnim.hlsl", "shaders/ShaderTexture.hlsl");
	
	shaders.load(&core, "shaderlight", "shaders/ShaderTextureLight.hlsl", "shaders/ShaderTextureLight.hlsl");
	shaders.load(&core, "shaderAnimlight", "shaders/ShaderTextureLightAnim.hlsl", "shaders/ShaderTextureLightAnim.hlsl");
	shaders.load(&core, "shaderinstance", "shaders/ShaderTextureLightInstace.hlsl", "shaders/ShaderTextureLightInstace.hlsl");
	shaders.load(&core, "shaderinstancegrass", "shaders/ShaderTextureLightInstacegrass.hlsl", "shaders/ShaderTextureLightInstacegrass.hlsl");
	shaders.load(&core, "shaderfont", "shaders/ShaderFont.hlsl", "shaders/ShaderFont.hlsl");
	shaders.load(&core, "shaderfire", "shaders/Shaderfire.hlsl", "shaders/Shaderfire.hlsl");

	Fire fire;
	fire.init(&core, &psos, &shaders.shaders["shaderfire"], Vec3(0, 5, 0),"fire");
	fire.active = true;

	for (int i = 0; i < 40; i++) {
				Fire* fire1 = new Fire();
		fire1->init(&core, &psos, &shaders.shaders["shaderfire"], Vec3(0, 0, 0),"fire");
		fires.push_back(fire1);
	}

	//Cube cube;
	//cube.init(&core,&psos, &shaders.shaders["shader1"],Vec3(10,0,10),"plane");
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j <30; j++) {
			Cube* cube1 = new Cube();
			cube1->init(&core, &psos, &shaders.shaders["shaderlight"], Vec3(-150+i*15, 0, -150 + j * 15), "plane");
			cubes.push_back(cube1);
		}
	}
	Sphere sphere;
	sphere.init(&core, &psos, &shaders.shaders["shader1"], Vec3(30, 0, 40));


	
	vector<string> texturenames;
	TextureManager textures;
	vector<string> texturenamesNH;
	
	

	texturenames.push_back("../Resources/citrus_orchard_road_puresky_8k.png");
	textures.load(&core, texturenames, "SkyBox");
	texturenames.clear();
	texturenames.push_back("../Resources/Grass/Grass01_2K_BaseColor.png");
	texturenamesNH.push_back("../Resources/Grass/Grass01_2K_Normal.png");
	textures.load(&core, texturenames, texturenamesNH, "Grass");
	texturenames.clear();
	//texturenames.push_back("../Resources/Grass/Grass01_2K_BaseColor.png");
	//textures.load(&core, texturenames, "font");
	//texturenames.clear();
	for (int i = 0; i < 10; i++) {
		
		texturenames.push_back("../Resources/character/"+string(to_string(i))+string(".png"));
	}
	texturenames.push_back("../Resources/character/bullet1.png");//10
	texturenames.push_back("../Resources/character/health.png");//11
	texturenames.push_back("../Resources/character/score.png");//12
	texturenames.push_back("../Resources/character/Snipe1.png");//13
	texturenames.push_back("../Resources/character/end.png");//14
	textures.load(&core, texturenames, "font");
	texturenames.clear();

	for (int i = 0; i < 10; i++) {
		Font* font=new Font();
		font->init(&core, &psos, &shaders.shaders["shaderfont"], 72, Vec3(200+100 * i, 980, 980), Vec3(300+100*i, 1080, 1080));
		healthnumber.push_back(font);

	}
	for (int i = 0; i < 10; i++) {
		Font* font = new Font();
		font->init(&core, &psos, &shaders.shaders["shaderfont"], 72, Vec3(200 + 100 * i, 880, 880), Vec3(300 + 100 * i, 980, 980));
		scorenumber.push_back(font);

	}
	for (int i = 0; i < 30; i++) {
		Font* font = new Font();
		font->init(&core, &psos, &shaders.shaders["shaderfont"], 72, Vec3(0 + 25 * i, 0, 0), Vec3(25 + 25 * i, 100, 100));
		bulletnumber.push_back(font);
	}
	
	Font* healthUI = new Font();
	healthUI->init(&core, &psos, &shaders.shaders["shaderfont"], 72, Vec3(0, 980, 980), Vec3(200 , 1080, 1080));
	Font* sorceUI = new Font();
	sorceUI->init(&core, &psos, &shaders.shaders["shaderfont"], 72, Vec3(0, 880, 880), Vec3(200, 980, 980));
	Font* end = new Font();
	end->init(&core, &psos, &shaders.shaders["shaderfont"], 72, Vec3(0, 0, 0), Vec3(1920, 1080, 1080));
	
	Hero* hero=new Hero();
	hero->init(&core, &shaders, &psos, Vec3(0, 0, 0),&textures,"hero","gun");
	hero->heromodel.collision.hero(hero->position);


	loadcontrol.LoadData(&loadgamestatic, &loadgameanim);
	/*for (int i = 0; i < loadgamestatic.size(); i++) {
		StaticModleLight scvv;
		staticmodles.push_back(scvv);
		staticmodles[i].load(&core, loadgamestatic[i].location, &shaders, &psos, Staticmodels::Tree, loadgamestatic[i].position, &textures, loadgamestatic[i].textureName,loadgamestatic[i].iscollider,loadgamestatic[i].meshname);
	}*/
	for (int i = 0; i < loadgameanim.size(); i++) {
		Enemies* scvv=new Enemies();
		scvv->init(&core, &shaders, &psos, loadgameanim[i].position, &textures, loadgameanim[i].textureName, loadgameanim[i].meshname);
		enemies.push_back(scvv);
		//enemies[i]->init(&core, &shaders, &psos, loadgameanim[i].position, &textures, loadgameanim[i].textureName,loadgameanim[i].meshname);
	}

	loadcontrol.LoadinstaceData(&loadinsatnceinformatiojn);
	loadcontrol.LoadinstacepositionData(&loadinstanceposition);

	vector<vector<INSTANCE>> scv111;
	for (int j = 0; j < loadinsatnceinformatiojn.size(); j++) {
		vector<INSTANCE> wf3ee;
		if (loadinsatnceinformatiojn[j].type == "normal") {
			for (int i = 0; i < loadinstanceposition[j].size(); i++) {


				INSTANCE inst;

				Matrix scale = scale.scale(0.01f, 0.01f, 0.01f);
				Matrix trans = trans.translation(loadinstanceposition[j][i].x, loadinstanceposition[j][i].y, loadinstanceposition[j][i].z);
				inst.w = trans * scale;
				wf3ee.push_back(inst);
			}
			scv111.push_back(wf3ee);
			
		}
		else if (loadinsatnceinformatiojn[j].type == "grass") {

			for (int i = 0; i < loadinstanceposition[j].size(); i++) {


				INSTANCE inst;

				Matrix scale = scale.scale(1.f,1.f,1.f);
				Matrix trans = trans.translation(loadinstanceposition[j][i].x, loadinstanceposition[j][i].y, loadinstanceposition[j][i].z);
				inst.w = trans * scale;
				wf3ee.push_back(inst);
			}
			scv111.push_back(wf3ee);

		
		}
		
	}
	StaticModleLightInstance instancetest;
	StaticModleLightInstanceGrass grasstest;

	for (int i = 0; i < loadinsatnceinformatiojn.size(); i++) {
		if (loadinsatnceinformatiojn[i].type == "normal") {
			
			instancetest.load(&core, loadinsatnceinformatiojn[i].location, &shaders, &psos, Staticmodels::Tree, loadinsatnceinformatiojn[i].position, &textures, loadinsatnceinformatiojn[0].textureName, 0, scv111[i]);
		}
		else if (loadinsatnceinformatiojn[i].type == "grass") {

			
			grasstest.load(&core, loadinsatnceinformatiojn[i].location, &shaders, &psos, Staticmodels::Tree, loadinsatnceinformatiojn[i].position, &textures, loadinsatnceinformatiojn[i].textureName, 0, scv111[i]);
		}
	}
	
	


	



	
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
	Vec4 from = Vec4(0,1,0, 0);
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
		Vec4 groundright = Vec4(right.x, 0, right.z, 0).normalize();
		Vec4 groundforawrd = Vec4(forward.x, 0, forward.z, 0).normalize();
		Vec4 delta(0, 0, 0, 0);
		if (win.keys['A']) delta += Vec4(groundright.x, 0, groundright.z, 0);
		if (win.keys['D']) delta -= Vec4(groundright.x, 0, groundright.z, 0);
		if (win.keys['W']) delta += Vec4(groundforawrd.x, 0, groundforawrd.z, 0);
		if (win.keys['S']) delta -= Vec4(groundforawrd.x, 0, groundforawrd.z, 0);

		/*
		if (win.keys['N']) {
			for (int i = 0; i < enemies.size(); i++) {
				enemies[i]->die(from.TransToVec3RemoveW());
			}
		}*/
		//delta=delta.normalize();
		delta *= cameramovespeed * rexdt;
		from = from + delta;
		hero->heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
		for (int i = 0; i < enemies.size(); i++) {
			if (hero->heromodel.collision.AABBtest(enemies[i]->enemymodel.collision.realminpoint, enemies[i]->enemymodel.collision.realmaxpoint)) {
				from = from - delta ;
				hero->heromodel.collision.updatehero(Vec3(from.x, from.y, from.z));
				break;
			}
		}
		
		hero->heromodel.position = Vec3(from.x,from.y,from.z)+forward.TransToVec3RemoveW()*0;
		to = forward + from;
		right = forward.Cross(Vec4(0, 1, 0, 0));
		lookat=lookat.LookatMatrix(from, to, up);
		vp = prespection.mul(lookat);



		core.beginFrame();
		win.processMessages();
		core.beginRenderPass();


		
		if (enemies.size()<3+dt/10&&enemies.size()<8) {
			Enemies* scvv=new Enemies();
			scvv->init(&core, &shaders, &psos, Vec3(11111, 111111, 11111), &textures, "enemy", "enemy");
			scvv->die(from.TransToVec3RemoveW());
			enemies.push_back(scvv);
			
	}

		for (int i = 0; i < enemies.size(); i++) {
			enemies[i]->distance = (enemies[i]->enemymodel.position - hero->heromodel.position).length();
			
			
			
			Vec3 scv;
			enemies[i]->enemymodel.position = enemies[i]->enemymodel.position + enemies[i]->enemymodel.forward * enemies[i]->movespeed * rexdt;//move
			if (enemies[i]->health < 0) {
				enemies[i]->attatcktime = 0;
				enemies[i]->anim(rexdt, "death", from.TransToVec3RemoveW());
				enemies[i]->enemymodel.position = enemies[i]->enemymodel.position - enemies[i]->enemymodel.forward * enemies[i]->movespeed * rexdt;
			}
			else if (enemies[i]->enemymodel.collision.AABBtest(hero->heromodel.collision.realminpoint, hero->heromodel.collision.realmaxpoint)) {
				enemies[i]->enemymodel.position = enemies[i]->enemymodel.position - enemies[i]->enemymodel.forward * enemies[i]->movespeed * rexdt;
				enemies[i]->attatcktime += rexdt;
				if (enemies[i]->attatcktime > 2.8f) {
					enemies[i]->attatcktime = 0;
					hero->health -= 5;
				}
				enemies[i]->anim(rexdt, "attack", from.TransToVec3RemoveW());
			}
			else {
				enemies[i]->attatcktime = 0;
				enemies[i]->anim(rexdt, "run", from.TransToVec3RemoveW());
			}
			scv =  hero->heromodel.position- enemies[i]->enemymodel.position;//calculate the new forward
			scv = scv.normalize();
			//enemies[i]->enemymodel.forward = scv;
			Vec3 from3 = Vec3(from.x, from.y, from.z);
			Matrix R = Matrix::ForwardtoOnlyTRex(scv, &enemies[i]->enemymodel.forward,dt);
			enemies[i]->enemymodel.collision.update(enemies[i]->enemymodel.position, R);
			//if (enemies[i]->enemymodel.collision.AABBtest(hero->heromodel.collision.realminpoint, hero->heromodel.collision.realmaxpoint)) {
			//	R = enemies[i]->enemymodel.roationnow;
				//enemies[i]->enemymodel.collision.update(enemies[i]->enemymodel.position, R);
			//}

			enemies[i]->enemymodel.draw(&core, &enemies[i]->enemymodel.realshow, &vp,&from3,&light.Strength,&light.Direction,&shaders.shaders["shaderTexture"], &psos, &enemies[i]->enemymodelinstace, R,textures.find(enemies[i]->enemymodel.texturename), textures.findNH(enemies[i]->enemymodel.texturename));
			
			
		}	
		for (int i = 0; i < enemies.size(); i++) {
			for (int j = 0; j < enemies.size()-1; j++) {
				if(enemies[j]->distance>enemies[j+1]->distance){
					Enemies* temp;
					temp = enemies[j];
					enemies[j] = enemies[j + 1];
					enemies[j + 1] = temp;
				}
			}
		}
		for (int i = 0; i < cubes.size(); i++) {
			cubes[i]->draw(&core, &cubes[i]->realshow, &vp, &shaders.shaders["shaderlight"], &psos, textures.find("Grass"),textures.findNH("Grass"),&light.Strength,&light.Direction);
		}
		Vec3 from3 = Vec3(from.x, from.y, from.z);

		instancetest.draw(&core, &instancetest.realshow, &vp, &from3, &light.Strength, &light.Direction, &shaders.shaders["shaderinstance"], &psos, textures.find(instancetest.texturename), textures.findNH(instancetest.texturename), &dt);

		grasstest.draw(&core, &grasstest.realshow, &vp, &from3, &light.Strength, &light.Direction, &shaders.shaders["shaderinstancegrass"], &psos, textures.find(grasstest.texturename), textures.findNH(grasstest.texturename), &dt);

	
		//cube.draw(&core, &cube.realshow, &vp, &shaders.shaders["shader1"], &psos, textures.find("Grass"));
	


		Matrix R;
		R = Matrix::ForwardtoTOnlyHero(forward.TransToVec3RemoveW());
		Vec3 from33 = Vec3(from.x, from.y, from.z);

		
		hero->anim(from33, enemies, win.mouseButtons, rexdt, forward.TransToVec3RemoveW(), win.keys['R'],fires);
		hero->heromodel.draw(&core, &hero->heromodel.realshow, &vp, &from33, &light.Strength, &light.Direction, &shaders.shaders["shaderTexture"], &psos, &hero->heromodelinstace, R, textures.find(hero->heromodel.texturename), textures.findNH(hero->heromodel.texturename));
		//R = R.LookRotation(forward.TransToVec3RemoveW(),Vec3(0,1,0));
		for (int i = 0; i < fires.size(); i++) {
			if (fires[i]->active) {

				fires[i]->timecount += rexdt;
				Vec3 scv;
				scv = hero->heromodel.position - fires[i]->position;//calculate the new forward
				scv = scv.normalize();
				R = R.ForwardtoOnlyfire(forward.TransToVec3RemoveW());
				fires[i]->draw(&core, &fires[i]->realshow, &vp, &shaders.shaders["shaderfire"], &psos, textures.find("font"), &dt, R);
				if (fires[i]->timecount >=0.95f) {
					fires[i]->end();
				}
				
			}
		}
		
		








		//fire.draw(&core, &fire.realshow, &vp, &shaders.shaders["shaderfire"], &psos, textures.find("font"), &dt);
		//fire.position.x -= 0.5f*rexdt;







		Matrix C;
		C = C.translation(Vec3(from.x, 0, from.z));
		sphere.draw(&core, &C, &vp, &shaders.shaders["shader1"], &psos, textures.find("SkyBox"));
		vector<int>UIcount=calculatenumber(hero->health);
		if (hero->health <= 0) {
			UIcount.clear();
		}
		for (int i = 0; i < UIcount.size(); i++) {
			healthnumber[i]->draw(&core, &R, &vp, &shaders.shaders["shaderfont"], &psos, textures.find("font"), UIcount[UIcount.size()-1-i]);
		

		}
		UIcount = calculatenumber(hero->score);
		for (int i = 0; i < UIcount.size(); i++) {
			scorenumber[i]->draw(&core, &R, &vp, &shaders.shaders["shaderfont"], &psos, textures.find("font"), UIcount[UIcount.size() - 1 - i]);

		}
		
		for (int i = 0; i < hero->bullet; i++) {
			bulletnumber[i]->draw(&core, &R, &vp, &shaders.shaders["shaderfont"], &psos, textures.find("font"), 13);

		}
		healthUI->draw(&core, &R, &vp, &shaders.shaders["shaderfont"], &psos, textures.find("font"),11);
		sorceUI->draw(&core, &R, &vp, &shaders.shaders["shaderfont"], &psos, textures.find("font"), 12);
		if (hero->health <= 0) {
			end->draw(&core, &R, &vp, &shaders.shaders["shaderfont"], &psos, textures.find("font"), 14);
		}
		
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
