#include"SceneManager.h"
#include"../Definition/Definition.h"
#include"../Library/Graphics/Graphics.h"
#include"../Library/Engine/Camera.h"
#include"../Library/Input/Input.h"
#include"../Library/Scene/BaseScene.h"
#include"../Library/Scene/TitleScene/TitleScene.h"
#include"../Library/Scene/HelpScene/HelpScene.h"
#include"../Library/Scene/GameMainScene/GameMainScene.h"
#include"../Library/Scene/GameEndScene/GameEndScene.h"

SceneManager* SceneManager::p_instance = 0;

SceneManager* SceneManager::Instance()
{
	if (p_instance == 0)
	{
		p_instance = new SceneManager;
	}
	return p_instance;
}

void SceneManager::Init()
{
}

void SceneManager::Update()
{
	THE_INPUT->UpdateInput();
	THE_CAMERA->Update();
	switch (Scene_Info.CurrentSceneStep)
	{
	case SceneStep::Init:
		InitScene(Scene_Info.CurrentSceneID);
		break;
	case SceneStep::Main:
		MainScene(Scene_Info.CurrentSceneID);
		break;
	case SceneStep::End:
		EndScene(Scene_Info.CurrentSceneID);
		break;
	default:
		break;
	}
}

void SceneManager::Draw()
{
	if (Scene_Info.CurrentSceneStep == SceneStep::Main)
	{
		THE_GRAPHICS->DrawStart();

		THE_GRAPHICS->SetRenderMode(Graphics::ERenderMode::NORMAL,true);

		THE_GRAPHICS->GetD3DDevice()->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		
		switch (Scene_Info.CurrentSceneID)
		{
		case SceneId::Title:
			DrawScene(Scene_Instance.title);
			break;
		case SceneId::Help:
			DrawScene(Scene_Instance.help);
			break;
		case SceneId::Game:
			DrawScene(Scene_Instance.main);
			break;
		case SceneId::End:
			DrawScene(Scene_Instance.end);
			break;
		default:
			break;
		}
		

		THE_GRAPHICS->DrawEnd();
	}
	
}

SceneManager::SceneManager()
{
	Scene_Instance.title = new TitleScene;
	Scene_Instance.help = new HelpScene;
	Scene_Instance.main = new GameMainScene;
	Scene_Instance.end = new GameEndScene;

	Scene_Info.CurrentSceneID = SceneId::Title;
	Scene_Info.CurrentSceneStep = SceneStep::Init;
}

SceneManager::~SceneManager()
{
	if (Scene_Instance.title != nullptr)
	{
		delete Scene_Instance.title;
		Scene_Instance.title = nullptr;
	}
	if (Scene_Instance.help != nullptr)
	{
		delete Scene_Instance.help;
		Scene_Instance.help = nullptr;
	}
	if (Scene_Instance.main != nullptr)
	{
		delete Scene_Instance.main;
		Scene_Instance.main = nullptr;
	}
	if (Scene_Instance.end != nullptr)
	{
		delete Scene_Instance.end;
		Scene_Instance.end = nullptr;
	}
}

void SceneManager::InitScene(SceneId id_)
{
	switch (id_)
	{
	case SceneId::Title:
		Scene_Instance.title->InitScene();
		break;
	case SceneId::Help:
		Scene_Instance.help->InitScene();
		break;
	case SceneId::Game:
		Scene_Instance.main->InitScene();
		break;
	case SceneId::End:
		Scene_Instance.end->InitScene();
		break;
	default:
		break;
	}
}

void SceneManager::MainScene(SceneId id_)
{
	switch (id_)
	{
	case SceneId::Title:
		Scene_Instance.title->UpdateScene();
		break;
	case SceneId::Help:
		Scene_Instance.help->UpdateScene();
		break;
	case SceneId::Game:
		Scene_Instance.main->UpdateScene();
		break;
	case SceneId::End:
		Scene_Instance.end->UpdateScene();
		break;
	default:
		break;
	}
}

void SceneManager::EndScene(SceneId id_)
{
	switch (id_)
	{
	case SceneId::Title:
		Scene_Instance.title->EndScene();
		break;
	case SceneId::Help:
		Scene_Instance.help->EndScene();
		break;
	case SceneId::Game:
		Scene_Instance.main->EndScene();
		break;
	case SceneId::End:
		Scene_Instance.end->EndScene();
		break;
	default:
		break;
	}
}

void SceneManager::DrawScene(BaseScene* scene_)
{
	scene_->Draw();
}




