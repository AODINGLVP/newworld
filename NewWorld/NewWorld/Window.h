#define WINDOW_GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define WINDOW_GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#include <Windows.h>
#include <iostream>
#include <string>
using namespace std;
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
		__width = 1920;
		__height = 1080;

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
