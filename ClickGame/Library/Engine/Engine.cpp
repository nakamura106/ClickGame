#include"Engine.h"
#include"../Graphics/Graphics.h"
#include"../Input/Input.h"
#include"../../Definition/Definition.h"
#include"../../Manager/LibraryManager.h"

bool Engine::InitEngine(int width_, int height_, const char* title_name_)
{
	if (MakeWindow(width_, height_, title_name_) == false)
	{
		return false;
	}
	if (THE_GRAPHICS->InitGraphics() == false)
	{
		return false;
	}
	if (THE_INPUT->InitInput() == false)
	{
		return false;
	}

	return true;
}

void Engine::EndEngine()
{
	THE_GRAPHICS->ReleaseGraphics();
}

LRESULT CALLBACK WindowProc(HWND window_handle_, UINT message_id_, WPARAM wparam_, LPARAM lparam_)
{
	switch (message_id_)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wparam_ == VK_ESCAPE)
		{
			PostMessage(window_handle_, WM_CLOSE, NULL, NULL);
		}
	default:
		return DefWindowProc(window_handle_, message_id_, wparam_, lparam_);
		break;
	}
	return 0;
}

bool Engine::MakeWindow(int width_, int height_, const char* title_)
{
	WNDCLASSEX window_class = {
		sizeof(WNDCLASSEXA),			// 構造体のサイズ
		CS_HREDRAW | CS_VREDRAW,		// クラスのスタイル
		WindowProc,						// ウィンドウプロシージャ
		0,								// 補助メモリ
		0,								// 補助メモリ
		GetModuleHandle(nullptr),		// このプログラムのインスタンスハンドル
		LoadIcon(NULL, IDI_APPLICATION),// アイコン画像
		LoadCursor(NULL, IDC_ARROW),	// カーソル画像
		NULL,							// 背景ブラシ(背景色)
		NULL,							// メニュー名
		WINDOW_CLASS_NAME,				// クラス名									
		NULL							// 小さいアイコン
	};

	// 構造体の登録
	if (RegisterClassEx(&window_class) == 0)
	{
		return 0;
	}

	// ウィンドウ作成
	hWnd_ = CreateWindowA(
		WINDOW_CLASS_NAME,
		title_,
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) | WS_VISIBLE,
		CW_USEDEFAULT,
		0,
		width_,
		height_,
		NULL,
		NULL,
		GetModuleHandle(nullptr),
		NULL);

	if (hWnd_ == NULL)
	{
		return 0;
	}

	RECT window_rect;
	RECT client_rect;

	GetWindowRect(hWnd_, &window_rect);
	GetClientRect(hWnd_, &client_rect);

	int frame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
	int frame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);

	SetWindowPos(hWnd_, NULL, CW_USEDEFAULT, CW_USEDEFAULT, frame_size_x + width_, frame_size_y + height_, SWP_NOMOVE);

	ShowWindow(hWnd_, SW_SHOW);
	UpdateWindow(hWnd_);

	return hWnd_;
}