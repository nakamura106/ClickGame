#ifndef CAMERA_H_
#define CAMERA_H_

#include <d3dx9.h>

class Camera
{
private:
	/* カメラの移動速度 Walk */
	const float walk_speed_;

	/* カメラの移動速度 Sprint */
	const float sprint_speed_;

	struct CAMERA_DATA {

		/* カメラの位置 */
		D3DXVECTOR3 camera_pos;

		/* 注視点 */
		D3DXVECTOR3 eye_pos;

		/* カメラの上向きのベクトル */
		D3DXVECTOR3 camera_up;

		D3DXVECTOR3 forward;

		/* カメラのビュー情報を保存する変数 */
		D3DXMATRIX mat_view;

		/* 角度 */
		float rad;

		/* 回転速度 */
		float rotate_speed;

		/* X軸回転 */
		float yaw;

		/* Y軸回転 */
		float pitch;

		/* Z軸回転 */
		float roll;

		/* スピード */
		float speed;

		/* 移動速度 */
		//D3DXVECTOR3 m_Velocity;

	}camera_data_;

public:
	Camera(D3DXVECTOR3 pos_) :
		walk_speed_(0.5f),
		sprint_speed_(6.0f)
	{
		camera_data_.camera_pos.x = pos_.x;
		camera_data_.camera_pos.y = pos_.y;//+ 0.5;	// 今だけ(FirstPerson用)
		camera_data_.camera_pos.z = pos_.z;

		camera_data_.eye_pos.x = 0.0f;
		camera_data_.eye_pos.y = 0.0f;
		camera_data_.eye_pos.z = 0.0f;

		camera_data_.camera_up.x = 0.0f;
		camera_data_.camera_up.y = 1.0f;
		camera_data_.camera_up.z = 0.0f;

		camera_data_.rotate_speed = 0.05f;

		camera_data_.yaw = 0.0f;
		camera_data_.pitch = 0.0f;
		camera_data_.roll = 0.0f;

		camera_data_.speed = walk_speed_;

		D3DXMatrixIdentity(&camera_data_.mat_view);
	}

	// カメラ座標の更新
	void Update();

	// カメラの移動
	void Move();

	void MouseRotate();

	void StickRotate();

	CAMERA_DATA* GetCameraData() { return &camera_data_; }


};

#endif