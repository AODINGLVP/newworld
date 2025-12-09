// NewWorld.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define WINDOW_GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define WINDOW_GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

#include "Objects.h"
#include <Windows.h>
#include <iostream>
#include <string>




#include <Vector>





using namespace MathTool;
using namespace std;
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
using namespace std;

struct alignas(16) ConstantBuffer3 {
	Matrix w;
	Matrix VP;
};
struct alignas(16) ConstantBuffer1
{
	float time;
};

struct alignas(16) ConstantBuffer2
{
	float time;
	float padding[3];
	Vec4 lights[4];

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
	void init(Core* core, Shaders* shaders, PSOManager* psos,Vec3 position) {
		Animatestatus = "run";
		enemymodel.load(core, "../Resources/Trex/TRex.gem", shaders, psos, Animatemodels::TRex, position);
		enemymodelinstace.init(&enemymodel.animation, 0);
	}
	
};

class LoadControl
{
public:
	string type;
	string name;
	Vec3 position;
	Vec3 rotation;


	map<string, Animatemodels> Aniname;
	map<string, Staticmodels> Staticname;
	LoadControl() {

	}





	void Loadgame(vector<Enemies>* enemies, vector<StaticModle>* staticmodles, Core* core, PSOManager* psos, Shaders* shaders) {
		ifstream file("../Resources/loadfile1.csv");

		string line;

		getline(file, line);

		while (getline(file, line)) {
			std::stringstream theline(line);
			std::string word;

			getline(theline, word, ';');
			type = word;
			getline(theline, word, ';');
			name = word;
			getline(theline, word, ';');
			position.x = stof(word);
			getline(theline, word, ';');
			position.y = stof(word);
			getline(theline, word, ';');
			position.z = stof(word);
			getline(theline, word, ';');
			rotation.x = stof(word);
			getline(theline, word, ';');
			rotation.y = stof(word);
			getline(theline, word, ';');
			rotation.z = stof(word);


			if (type == "static") {
				if (name == "tree") {
					StaticModle tree;
					staticmodles->push_back(tree);
					staticmodles->back().load(core, "../Resources/acacia_003.gem", shaders, psos, Staticmodels::Tree, position);

				}

			}
			else if (type == "anim") {

				if (name == "trex") {
					Enemies enemy;
					enemy.init(core, shaders, psos, position);
					enemies->push_back(enemy);
				}
			}

		}


		file.close();
	}

};






class Window;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
Window* window;
class Window
{
public:
	bool keys[256];
	int mousex;
	int mousey;
	bool mouseButtons[3];
	std::wstring wname;
	HWND hwnd;
	HINSTANCE hinstance;
	string name;
	int __width = 800;
	int __height = 600;
	DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

	Window() {
		__width = kuan;
		__height = gao;

		WNDCLASSEX wc = {};
		hinstance = GetModuleHandle(NULL);
		name = "window_name";
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hinstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName = NULL;
		wname = std::wstring(name.begin(), name.end());
		wc.lpszClassName = wname.c_str();
		wc.cbSize = sizeof(WNDCLASSEX);
		RegisterClassEx(&wc);


		hwnd = CreateWindowEx(WS_EX_APPWINDOW, wname.c_str(), wname.c_str(), style,
			0, 0, __width, __height, NULL, NULL, hinstance, this);
		window = this;
	}

	void updateMouse(int x, int y)
	{
		mousex = x;
		mousey = y;
	}
	void processMessages() {
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void create(int w, int h, string n) {
		__width = w;
		__height = h;
		name = n;
		SetWindowTextA(hwnd, name.c_str());
		SetWindowPos(hwnd, 0, 0, 0, __width, __height, SWP_NOMOVE | SWP_NOZORDER);
	}
};
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		exit(0);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		exit(0);
		return 0;


	case WM_KEYDOWN:
	{
		window->keys[(unsigned int)wParam] = true;
		return 0;
	}
	case WM_KEYUP:
	{
		window->keys[(unsigned int)wParam] = false;
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
		window->mouseButtons[0] = true;
		return 0;
	}
	case WM_LBUTTONUP:
	{
		window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
		window->mouseButtons[0] = false;
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
		return 0;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
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
	Window win;
	Core core;
	core.init(window->hwnd, kuan, gao);
	
	Shader shader;


	Shaders shaders;
	shaders.load(&core, "shader1", "ShaderVertices.hlsl", "ShaderPixel.hlsl");
	shaders.load(&core, "shaderAnim", "ShaderVerticesAnim.hlsl", "ShaderPixel.hlsl");
	shaders.load(&core, "shaderTexture", "ShaderVerticesAnim.hlsl", "ShaderTexture.hlsl");
	
	Cube cube;
	cube.init(&core,&psos, &shaders.shaders["shader1"]);


	//loadcontrol.Loadgame(&enemies, &test, &core, &psos, &shaders);

	TextureManager textures;
	textures.load(&core, "../Resources/Trex/Textures/T-rex_Base_Color_alb.png","Trexalb");
	textures.load(&core, "../Resources/Trex/Textures/T-rex_Base_Color_rmax.png","Trexrmax");
	textures.load(&core, "../Resources/UZI/Textures/Uzi_Albedo_alb.png", "UZIalb");

	
	StaticModle tree;
	tree.load(&core, "../Resources/acacia_003.gem", &shaders, &psos, Staticmodels::Tree,Vec3(0,0,0));
	staticmodles.push_back(tree);
	StaticModle tree1;
	tree1.load(&core, "../Resources/acacia_003.gem", &shaders, &psos, Staticmodels::Tree, Vec3(0, 0, 5));
	staticmodles.push_back(tree1);
	StaticModle tree2;
	tree2.load(&core, "../Resources/acacia_003.gem", &shaders, &psos, Staticmodels::Tree, Vec3(0, 0, 10));
	staticmodles.push_back(tree2);
	StaticModle tree3;
	tree3.load(&core, "../Resources/acacia_003.gem", &shaders, &psos, Staticmodels::Tree, Vec3(0, 0, 15));
	staticmodles.push_back(tree3);
	
	/*
	AnimatedModel animatedModel;
	animatedModel.load(&core, "../Resources/TRex.gem", &shaders, &psos,Animatemodels::TRex, Vec3(0, 0, 0));
	animateModels.push_back(animatedModel);
	AnimationInstance animatedInstance;
	animatedInstance.init(&animatedModel.animation, 0);
	animationinstances.push_back(animatedInstance);
	*/
	//AnimatedModel UZI;
	//UZI.load(&core, "../Resources/UZI/Uzi.gem", &shaders, &psos, Animatemodels::UZI, Vec3(0, 0, 10));
	//AnimationInstance UZIInstance;
	//UZIInstance.init(&UZI.animation, 0);
	//animateModels.push_back(UZI);
	//animationinstances.push_back(UZIInstance);


	Hero hero;
	hero.init(&core, &shaders, &psos, Vec3(0, 0, 0));
	hero.heromodel.collision.hero(hero.position);
	
	Enemies enemy;
	enemy.init(&core, &shaders, &psos, Vec3(0, 0, 10));
	enemies.push_back(enemy);
	Enemies enemy1;
	enemy1.init(&core, &shaders, &psos, Vec3(0, 0, 20));
	enemies.push_back(enemy1);
	Enemies enemy2;
	enemy2.init(&core, &shaders, &psos, Vec3(0, 0, 30));
	enemies.push_back(enemy2);
	
	//Collider hero;
	//hero.hero(Vec3(16, 0, 4));








	Matrix world;
	world = world.scale(0.01f, 0.01f, 0.01f);
	Matrix prespection;
	prespection = prespection.Perspective(M_PI / 4, kuan / gao, 0.1f, 100.0f);
	Matrix lookat;
	ConstantBuffer3 constBufferCPU3;
	Matrix vp;
	constBufferCPU3.VP= prespection.Perspective(M_PI / 4, kuan / gao, 0.1f, 100.0f);
	
	GamesEngineeringBase::Timer timer;
	
	win.create(kuan, gao, "My Window");
	float dt=0;
	float rexdt;
	float cameramovespeed = 5.f;
	Vec4 to = Vec4(0, 0, 0, 0);
	Vec4 up = Vec4(0, 1, 0, 0);
	Vec4 from = Vec4(16,0,4, 0);
	Vec4 forward = Vec4(0, 0,1, 0);
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




		//OutputDebugStringA(to_string(hero.heromodel.position.x).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA(to_string(hero.heromodel.collision.realmaxpoint.y).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA("x::");
		//OutputDebugStringA(to_string(to.x).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA("z::");
		//OutputDebugStringA(to_string(to.z).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA(to_string(to.y).c_str());
		//OutputDebugStringA("\n");











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





		//OutputDebugStringA(to_string(from.x).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA(to_string(from.z).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA("x::");
		//OutputDebugStringA(to_string(to.x).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA("z::");
		//OutputDebugStringA(to_string(to.z).c_str());
		//OutputDebugStringA("\n");
		//OutputDebugStringA(to_string(to.y).c_str());
		//OutputDebugStringA("\n");

		//Vec4 from = Vec4(11 * cos(dt), 5, 11 * sin(dt), 0);
		//constBufferCPU1.time += dt;
		to = forward + from;
		right = forward.Cross(Vec4(0, 1, 0, 0));
		lookat=lookat.LookatMatrix(from, to, up);
		vp = prespection.mul(lookat);
		//constBufferCPU3.w = constBufferCPU3.w.lookAtMatrix(from.TransToVec3(), to.TransToVec3(), up.TransToVec3());
		core.beginFrame();
		win.processMessages();
		

		core.beginRenderPass();



		for (int i = 0; i < staticmodles.size(); i++) {
			
			staticmodles[i].draw(&core, &staticmodles[i].realshow, &vp, &shaders.shaders["shader1"], &psos);
		}

	
		//tree.position = Vec3(10, 0, 0);
		
		//tree.draw(&core, &tree.realshow, &vp, &shaders.shaders["shader1"], &psos);
		//cube.draw(&core, &constBufferCPU3.w, &constBufferCPU3.VP, &shaders.shaders["shader1"], &psos);
		

		

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
			enemies[i].enemymodel.draw(&core, &enemies[i].enemymodel.realshow, &vp, &shaders.shaders["shaderTexture"], &psos, &enemies[i].enemymodelinstace, R,textures.find("Trexalb"));
				
			
		}
		
		hero.heromodelinstace.update("08 fire", rexdt);
		if (hero.heromodelinstace.animationFinished()) {
			hero.heromodelinstace.resetAnimationTime();
		}
		
		Matrix R;
		R = Matrix::ForwardtoTOnlyHero(forward.TransToVec3RemoveW());
		
		hero.heromodel.draw(&core, &hero.heromodel.realshow, &vp, &shaders.shaders["shaderTexture"], &psos, &hero.heromodelinstace,R, textures.find("UZIalb"));
	/*	animatedInstance.update("run", rexdt);
		if (animatedInstance.animationFinished() == true)
		{
			animatedInstance.resetAnimationTime();
		}
		animatedModel.draw(&core, &animatedModel.realshow, &vp,&shaders.shaders["shaderAnim"],&psos,&animatedInstance);

	*/
		/*UZI.position = Vec3(0, 0, 20);
		
		UZIInstance.update("08 fire",rexdt);
		if (UZIInstance.animationFinished() == true)
		{
			UZIInstance.resetAnimationTime();
		}
		UZI.draw(&core, &UZI.realshow, &vp, &shaders.shaders["shaderAnim"], &psos, &UZIInstance);

		*/

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
