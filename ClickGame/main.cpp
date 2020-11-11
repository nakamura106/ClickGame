#include <fbxsdk.h>
#include <Windows.h>
#include <thread>
#include"Manager/LibraryManager.h"
#include"Manager/SoundManager.h"
#include"Manager/FbxManager.h"
#include"Manager/SceneManager.h"
#include"Definition/Definition.h"
#include"Library/Engine/Engine.h"
#include"Library/Input/Input.h"
#include"Library/Graphics/Graphics.h"
#include"Library/Video/Video.h"
#include"Library/Engine/Camera.h"

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmpLine,
	INT nCmdShow)
{
	//LibraryManager::Instance()->Init();
	// エンジンの初期化
	if (THE_ENGINE->InitEngine(1920, 1080, "Library") == false)
	{
		return 0;
	}

	
	while (true)
	{
		bool message_ret = false;
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else if (msg.message == WM_CLOSE)
			{
				break;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
		
			THE_SCENE->Update();

			THE_SCENE->Draw();

			
		}
	}
	THE_FBX->ReleaseFbxMesh("Box");
	// エンジン終了
	THE_ENGINE->EndEngine();
}