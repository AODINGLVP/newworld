// NewWorld.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define kuan 1920
#define gao 1080
#define WINDOW_GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define WINDOW_GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#include <Windows.h>
#include <iostream>
#include <string>
#include "Core.h"
#include "MathTool.h"
#include <fstream>
#include <sstream>
#include "PSOManager.h"
using namespace MathTool;
using namespace std;
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
using namespace std;
struct PRIM_VERTEX
{
	Vec3 position;
	Colour colour;
};


class Mesh {
public:
	ID3D12Resource* vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vbView;

	//Define layout
	D3D12_INPUT_ELEMENT_DESC inputLayout[2];
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;
	void init(Core* core, void* vertices, int vertexSizeInBytes, int numVertices) {
		//Specify vertex buffer will be in GPU memory heap
		D3D12_HEAP_PROPERTIES heapprops = {};
		heapprops.Type = D3D12_HEAP_TYPE_DEFAULT;
		heapprops.CreationNodeMask = 1;
		heapprops.VisibleNodeMask = 1;

		//Create vertex buffer on heap
		D3D12_RESOURCE_DESC vbDesc = {};
		vbDesc.Width = numVertices * vertexSizeInBytes;
		vbDesc.Height = 1;
		vbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vbDesc.DepthOrArraySize = 1;
		vbDesc.MipLevels = 1;
		vbDesc.SampleDesc.Count = 1;
		vbDesc.SampleDesc.Quality = 0;
		vbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		//Allocate memory
		core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &vbDesc,
			D3D12_RESOURCE_STATE_COMMON, NULL, IID_PPV_ARGS(&vertexBuffer));
		//Allocate memory
		core->uploadResource(vertexBuffer, vertices, numVertices * vertexSizeInBytes,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		//Fill in view in helper function
		vbView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vbView.StrideInBytes = vertexSizeInBytes;
		vbView.SizeInBytes = numVertices * vertexSizeInBytes;
		//Fill in Layout
		inputLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		inputLayout[1] = { "COLOUR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		inputLayoutDesc.NumElements = 2;
		inputLayoutDesc.pInputElementDescs = inputLayout;
	}
	void draw(Core* core)
	{
		core->getCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		core->getCommandList()->IASetVertexBuffers(0, 1, &vbView);
		core->getCommandList()->DrawInstanced(3, 1, 0, 0);
	}
};
class ScreenSpaceTriangle {
public:
	PRIM_VERTEX vertices[3];
	Mesh mesh;
	void init(Core* core) {
		
		vertices[0].position = Vec3(0, 1.0f, 0);
		vertices[0].colour = Colour(0, 1.0f, 0);
		vertices[1].position = Vec3(-1.0f, -1.0f, 0);
		vertices[1].colour = Colour(1.0f, 0, 0);
		vertices[2].position = Vec3(1.0f, -1.0f, 0);
		vertices[2].colour = Colour(0, 0, 1.0f);
		mesh.init(core, &vertices[0], sizeof(PRIM_VERTEX), 3);
	}
};
class Shader {
public:
	ID3DBlob* vertexShader;
	ID3DBlob* pixelShader;
	PSOManager psos;
	ScreenSpaceTriangle triangle;
	int debug = 0;
	
	string ReadShader(string filename) {
		std::ifstream file(filename);
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}
	void Compile(Core* core) {
		debug = 1;
		OutputDebugStringA(to_string(debug).c_str());
		//Compile vertex shader
		ID3DBlob* status;
		string vertexShadersStr=ReadShader("ShaderVertices.hlsl");
		HRESULT hr = D3DCompile(vertexShadersStr.c_str(), strlen(vertexShadersStr.c_str()), NULL,
			NULL, NULL, "VS", "vs_5_0", 0, 0, &vertexShader, &status);
		string pixelShaderStr = ReadShader("ShaderPixel.hlsl");
		hr = D3DCompile(pixelShaderStr.c_str(), strlen(pixelShaderStr.c_str()), NULL, NULL,
			NULL, "PS", "ps_5_0", 0, 0, &pixelShader, &status);
		if (FAILED(hr)) {
			if (status) {
				// Print the error to the Visual Studio Output window
				OutputDebugStringA((char*)status->GetBufferPointer());
				status->Release();
			}
			return;
		}
		debug = 2;
	
		psos.createPSO(core, "Triangle", vertexShader, pixelShader, triangle.mesh.inputLayoutDesc);
		debug = 3;
		
	}
	void init(Core* core) {
		triangle.init(core);
		Compile(core);
	}
	void draw(Core* core) {
		core->beginRenderPass();
		psos.bind(core, "Triangle");
		triangle.mesh.draw(core);
	}

};
struct alignas(16) ConstantBuffer1
{
	float time;
};
class ConstantBuffer {
	ID3D12Resource* constantBuffer;
	unsigned char* buffer;
	unsigned int cbSizeInBytes;
	void init(Core* core, unsigned int sizeInBytes, int frames) {
		cbSizeInBytes = (sizeInBytes + 255) & ~255;
		HRESULT hr;
		D3D12_HEAP_PROPERTIES heapprops = {};
		heapprops.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapprops.CreationNodeMask = 1;
		heapprops.VisibleNodeMask = 1;
		D3D12_RESOURCE_DESC cbDesc = {};
		cbDesc.Width = cbSizeInBytes * frames;
		cbDesc.Height = 1;
		cbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbDesc.DepthOrArraySize = 1;
		cbDesc.MipLevels = 1;
		cbDesc.SampleDesc.Count = 1;
		cbDesc.SampleDesc.Quality = 0;
		cbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		hr = core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &cbDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, NULL, __uuidof(ID3D12Resource), (void**)&constantBuffer);
		hr = constantBuffer->Map(0, NULL, (void**)&buffer);
	}
	//Update via a memcpy
	void update(void* data, unsigned int sizeInBytes, int frame)
	{
		memcpy(buffer + (frame * cbSizeInBytes), data, sizeInBytes);
	}
	//Will need GPU address of contents

	D3D12_GPU_VIRTUAL_ADDRESS getGPUAddress(int frame)
	{
		return (constantBuffer->GetGPUVirtualAddress() + (frame * cbSizeInBytes));
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
	Core core;
	Window win;
	Shader scv;
	ConstantBuffer1 constBufferCPU;
	constBufferCPU.time = 0;
	GamesEngineeringBase::Timer timer;
	
	
	win.create(kuan, gao, "My Window");
	
	core.init(window->hwnd,kuan,gao);
	scv.init(&core);

	while (1) {
		core.beginFrame();
		win.processMessages();
		if (win.keys[VK_ESCAPE] == 1)
		{
			break;
		}
		scv.draw(&core);
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
