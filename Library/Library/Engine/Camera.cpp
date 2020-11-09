#include"Camera.h"
#include"../Input/Input.h"
#include"../Graphics/Graphics.h"
#include"../../Definition/Definition.h"
#include"../../Manager/LibraryManager.h"

void Camera::Update()
{
	//ビュー座標変換用の行列算出 start
	D3DXVECTOR3 camera_pos(camera_data_.camera_pos.x, camera_data_.camera_pos.y, camera_data_.camera_pos.z);	// カメラ位置
	D3DXVECTOR3 eye_pos(camera_data_.eye_pos.x, camera_data_.eye_pos.y, camera_data_.eye_pos.z);				// 注視点
	D3DXVECTOR3 up_vector(camera_data_.camera_up.x, camera_data_.camera_up.y, camera_data_.camera_up.z);		// カメラの向き;
	D3DXMatrixLookAtLH(&camera_data_.mat_view,
		&camera_pos,				// カメラ座標
		&eye_pos,					// 注視点座標
		&up_vector);				// カメラの上の向きのベクトル

	THE_GRAPHICS->GetD3DDevice()->SetTransform(D3DTS_VIEW, &camera_data_.mat_view);
	//ビュー座標変換用の行列算出 end

	D3DXMATRIX matProj;
	D3DXMatrixIdentity(&matProj);

	//射影座標変換用の行列算出 start
	D3DVIEWPORT9 vp;
	THE_GRAPHICS->GetD3DDevice()->GetViewport(&vp);
	float aspect = (float)vp.Width / (float)vp.Height;

	// 視錐台の作成
	D3DXMatrixPerspectiveFovLH(
		&matProj,
		D3DXToRadian(60),	// 画角
		aspect,				// アスペクト比
		1.1f,				// near
		1000.0f);			// far
	THE_GRAPHICS->GetD3DDevice()->SetTransform(D3DTS_PROJECTION, &matProj);
	//射影座標変換用の行列算出 endMove();
	
	MouseRotate();
	StickRotate();

	camera_data_.forward = camera_data_.eye_pos - camera_data_.camera_pos;
	D3DXVec3Normalize(&camera_data_.forward, &camera_data_.forward);
}

void Camera::Move()
{
	if (THE_INPUT->GetKey(KeyCode::A_KEY))
	{
		camera_data_.camera_pos.x--;
	}
	if (THE_INPUT->GetKey(KeyCode::S_KEY))
	{
		camera_data_.camera_pos.z++;
	}
	if (THE_INPUT->GetKey(KeyCode::D_KEY))
	{
		camera_data_.camera_pos.x++;
	}
	if (THE_INPUT->GetKey(KeyCode::W_KEY))
	{
		camera_data_.camera_pos.z--;
	}
}



void Camera::MouseRotate()
{
	SetCursorPos(960, 540);
	camera_data_.yaw += (THE_INPUT->GetMousePos().x - 960) / 1920 * 50;//ここでカメラ感度変更可能
	camera_data_.pitch -= (THE_INPUT->GetMousePos().y - 540) / 1080 * 20;
	if (camera_data_.pitch > 90.0f) { camera_data_.pitch = 180.0f - camera_data_.pitch; }
	if (camera_data_.pitch < -90.0f) { camera_data_.pitch = -180.0f - camera_data_.pitch; }

	
	camera_data_.eye_pos.x = camera_data_.camera_pos.x + sinf(D3DXToRadian(camera_data_.yaw)) * cosf(D3DXToRadian(camera_data_.pitch));
	camera_data_.eye_pos.y = camera_data_.camera_pos.y + sinf(D3DXToRadian(camera_data_.pitch));
	camera_data_.eye_pos.z = camera_data_.camera_pos.z + cosf(D3DXToRadian(camera_data_.yaw)) * cosf(D3DXToRadian(camera_data_.pitch));
}

void Camera::StickRotate()
{
	if (THE_INPUT->IsButtonPush(KeyCode::R_LEFT_STICK))
	{
		camera_data_.yaw -= 2.0f;
	}
	if (THE_INPUT->IsButtonPush(KeyCode::R_RIGHT_STICK))
	{
		camera_data_.yaw += 2.0f;
	}
	if (THE_INPUT->IsButtonPush(KeyCode::R_UP_STICK))
	{
		camera_data_.pitch += 2.0f;
	}
	if (THE_INPUT->IsButtonPush(KeyCode::R_DOWN_STICK))
	{
		camera_data_.pitch -= 2.0f;
	}
	camera_data_.eye_pos.x = camera_data_.camera_pos.x + sinf(D3DXToRadian(camera_data_.yaw)) * cosf(D3DXToRadian(camera_data_.pitch));
	camera_data_.eye_pos.y = camera_data_.camera_pos.y + sinf(D3DXToRadian(camera_data_.pitch));
	camera_data_.eye_pos.z = camera_data_.camera_pos.z + cosf(D3DXToRadian(camera_data_.yaw)) * cosf(D3DXToRadian(camera_data_.pitch));
}