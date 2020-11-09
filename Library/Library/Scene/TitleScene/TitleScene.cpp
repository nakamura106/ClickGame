#include"TitleScene.h"
#include"../../../Definition/Definition.h"
#include"../../Input/Input.h"
#include"../../Sound/Sound.h"
#include"../../FBX/FBX.h"

TitleScene::TitleScene()
{
}

void TitleScene::InitScene()
{
	//THE_VIDEO->Load("Res/Movie/【PS3】-El-Shaddai-エルシャダイ-　_ルシフェルver_-高画質版.avi");
	//THE_VIDEO->Play(5000);
	
	THE_SOUND->RegisterSound();
	THE_FBX->LoadFbxMesh("Box", "Res/Fbx/WorldBoxA02.fbx");
	THE_GRAPHICS->LoadTexture("Res/Tex/Title_3D_A.png", &tex);

	


	D3DXMatrixIdentity(&world);
	D3DXMatrixScaling(&scale, 250, 350, 250);
	D3DXMatrixTranslation(&move, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&world, &scale, &move);

	THE_SCENE->SetSceneStep(SceneStep::Main);

}

void TitleScene::UpdateScene()
{
	if(THE_INPUT->GetKeyDown(KeyCode::D_KEY))
	{
		THE_SCENE->SetSceneStep(SceneStep::End);
	}
}

void TitleScene::EndScene()
{
	THE_SCENE->SetSceneStep(SceneStep::Init);
	THE_SCENE->SetSceneId(SceneId::Game);
}

void TitleScene::Draw()
{
	if (THE_INPUT->GetKey(KeyCode::A_KEY) || THE_INPUT->IsButtonPush(KeyCode::A_BUTTON) || THE_INPUT->OnMousePush(KeyCode::LEFT))
	{
		THE_GRAPHICS->DrawTexture(&tex, D3DXVECTOR2(0, 0));
		THE_GRAPHICS->DrawFont(0, 0, "ABCDE", Graphics::FontSize::SMALL, Graphics::BLACK);
	}

	THE_FBX->DrawFbx("Box", world);

}
