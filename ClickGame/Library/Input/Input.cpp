#define DIRECTINPUT_VERSION 0x0800
#include<d3dx9math.h>
#include<math.h>
#include "Input.h"
#include"../Engine/Engine.h"
#include"../../Manager/LibraryManager.h"
#include"../../Definition/Definition.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define MOUSE_ON_VALUE (0x80)		// オン状態の値

// @brief 入力状態の種類
enum INPUT_STATE
{
	NOT_PUSH,		//押されていない
	PUSH_DOWN,		//押された瞬間
	PUSH,			//押されている
	PUSH_UP,		//離した瞬間
};

//キーの状態を保持するグローバル変数


BOOL SetUpCooperativeLevel(LPDIRECTINPUTDEVICE8 device_)
{
	//協調モードの設定
	if (FAILED(device_->SetCooperativeLevel(
		FindWindow(WINDOW_CLASS_NAME, nullptr),
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)
	))
	{
		return false;
	}
	return true;
}

//ライブラリ側のキー情報配列


BOOL Input::StartGamePadControl()
{
	if (gamepad_device_ == nullptr)
	{
		return false;
	}

	if (FAILED(gamepad_device_->Acquire()))
	{
		return false;
	}

	return true;
}

BOOL Input::SetUpGamePadProperty(LPDIRECTINPUTDEVICE8 device_)
{
	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	// ここからDIPROPのヘッダーにステータス設定
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;
	if (FAILED(device_->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return false;
	}

	// X軸の値の範囲設定
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	if (FAILED(device_->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	// Y軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Y;
	if (FAILED(device_->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	return true;
}

BOOL CALLBACK Input::DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi_, LPVOID pvRef_)
{
	DeviceEnumParameter* parameter = (DeviceEnumParameter*)pvRef_;
	LPDIRECTINPUTDEVICE8 device = nullptr;

	if (parameter->find_count >= 1)
	{
		return DIENUM_STOP;
	}

	HRESULT hr = THE_INPUT->input_interface_->CreateDevice(
		lpddi_->guidInstance,
		parameter->gamepad_device_ptr,
		NULL);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	device = *parameter->gamepad_device_ptr;
	hr = device->SetDataFormat(&c_dfDIJoystick);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	if (THE_INPUT->SetUpGamePadProperty(device) == false)
	{
		return DIENUM_STOP;
	}

	if (SetUpCooperativeLevel(device) == false)
	{
		return DIENUM_STOP;
	}

	//if (FAILED(device_->Poll()))
	//{
	//	return DIENUM_STOP;
	//}

	parameter->find_count++;

	return DIENUM_CONTINUE;
}

bool Input::InitInput()
{
	if (CreateInputInterface() == false)
	{
		return false;
	}

	if (CreateMouseDevice() == false)
	{
		EndInput();
		return false;
	}

	if (CreateKeyboardDevice() == false) {
		EndInput();
		return false;
	}

	if (CreateGamePadDevice() == false)
	{
		EndInput();
		return false;
	}

	for (int i = 0; i < KeyCode::ButtonKind::BUTTON_KIND_MAX; i++)
	{
		ButtonStates[i] = KeyState::ButtonState::BUTTON_STATE_NONE;
	}

	ZeroMemory(&curr_mouse_state_, sizeof(DIMOUSESTATE));
	ZeroMemory(&prev_mouse_state_, sizeof(DIMOUSESTATE));

	return true;
}

void Input::EndInput()
{
	if (mouse_device_ != nullptr)
	{
		mouse_device_->Release();
		mouse_device_ = nullptr;
	}

	if (input_interface_ != nullptr)
	{
		input_interface_->Release();
		input_interface_ = nullptr;
	}

	if (gamepad_device_ != nullptr)
	{
		gamepad_device_->Release();
		gamepad_device_ = nullptr;
	}

	if (key_device_ != nullptr)
	{
		key_device_->Release();
		key_device_ = nullptr;
	}
}

bool Input::CreateInputInterface()
{
	// IDirectInput8インターフェイスの取得
	HRESULT hr = DirectInput8Create(GetModuleHandle(nullptr),
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)& input_interface_,
		NULL);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

bool Input::CreateGamePadDevice()
{
	Input::DeviceEnumParameter parameter;

	parameter.find_count = 0;
	parameter.gamepad_device_ptr = &gamepad_device_;

	// 指定したデバイスを列挙する（今回であればゲームパッド）
	if (FAILED(input_interface_->EnumDevices(
		DI8DEVTYPE_GAMEPAD,
		DeviceFindCallBack,		// 第2引数：コールバック関数
		&parameter,				// 第3引数：コールバック関数に渡すことができるデータ
		DIEDFL_ATTACHEDONLY		// 第4引数：現状でささっているデバイスのみを使用する設定
	)))
	{
		return false;
	}

	/*if (parameter.find_count == 0)
	{
		return false;
	}*/

	int count = 0;
	while (StartGamePadControl() == false)
	{
		Sleep(1000);
		count++;
		if (count >= 5)
		{
			break;
		}
	}

	return true;
}

bool Input::CreateMouseDevice()
{
	// マウス用にデバイスオブジェクトを作成
	if (FAILED(input_interface_->CreateDevice(GUID_SysMouse, &mouse_device_, NULL)))
	{
		// デバイスの作成に失敗
		return false;
	}

	// データフォーマットを設定
	if (FAILED(mouse_device_->SetDataFormat(&c_dfDIMouse)))
	{
		// データフォーマットに失敗
		return false;
	}

	// モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(mouse_device_->SetCooperativeLevel(
		FindWindowA(WINDOW_CLASS_NAME, nullptr),
		DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
	{
		// モードの設定に失敗
		return false;
	}

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	if (FAILED(mouse_device_->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		// デバイスの設定に失敗
		return false;
	}

	// 入力制御開始
	mouse_device_->Acquire();

	return true;
}

bool Input::CreateKeyboardDevice()
{
	// マウス用にデバイスオブジェクトを作成
	if (FAILED(input_interface_->CreateDevice(GUID_SysKeyboard, &key_device_, NULL)))
	{
		// デバイスの作成に失敗
		return false;
	}

	// データフォーマットを設定
	if (FAILED(key_device_->SetDataFormat(&c_dfDIKeyboard)))
	{
		// データフォーマットに失敗
		return false;
	}

	// モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(key_device_->SetCooperativeLevel(
		FindWindowA(WINDOW_CLASS_NAME, nullptr),
		DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
	{
		// モードの設定に失敗
		return false;
	}

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	if (FAILED(key_device_->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		// デバイスの設定に失敗
		return false;
	}

	// 入力制御開始
	key_device_->Acquire();

	return true;
}

void Input::KeyStateUpdate()
{
	BYTE Key[256];
	HRESULT hr = key_device_->GetDeviceState(256, Key);

	if (FAILED(hr))
	{
		return;
	}

	for (int i = 0; i < KeyCode::MAX_KEY_INFO; i++)
	{
		if (Key[KeyInfo[i]] & 0x80)
		{
			if (InputState[i] == KeyState::InputState::NOT_PUSH || InputState[i] == KeyState::InputState::PUSH_UP)
			{
				InputState[i] = KeyState::InputState::PUSH_DOWN;
			}
			else
			{
				InputState[i] = KeyState::InputState::PUSH;
			}
		}
		else
		{
			if (InputState[i] == KeyState::InputState::PUSH || InputState[i] == KeyState::InputState::PUSH_DOWN)
			{
				InputState[i] = KeyState::InputState::PUSH_UP;
			}
			else
			{
				InputState[i] = KeyState::InputState::NOT_PUSH;
			}
		}
	}
}

bool Input::GetKey(KeyCode::KEY_INFO key_)
{
	return (InputState[key_] == KeyState::InputState::PUSH);
}

bool Input::GetKeyDown(KeyCode::KEY_INFO key_)
{
	return (InputState[key_] == KeyState::InputState::PUSH_DOWN);
}

bool Input::GetKeyUp(KeyCode::KEY_INFO key_)
{
	return (InputState[key_] == KeyState::InputState::PUSH_UP);
}

void Input::UpdateInput()
{
	KeyStateUpdate();
	UpdateMouse();
	UpdateGamePad();

	prev_mouse_state_ = curr_mouse_state_;
	// マウスの状態を取得します
	HRESULT	hr = mouse_device_->GetDeviceState(sizeof(DIMOUSESTATE), &curr_mouse_state_);
	if (FAILED(hr))
	{
		mouse_device_->Acquire();
		hr = mouse_device_->GetDeviceState(sizeof(DIMOUSESTATE), &curr_mouse_state_);
	}
}

bool Input::OnMouseDown(KeyCode::MouseButton button_type_)
{
	if (!(prev_mouse_state_.rgbButtons[button_type_] & MOUSE_ON_VALUE) &&
		curr_mouse_state_.rgbButtons[button_type_] & MOUSE_ON_VALUE)
	{
		return true;
	}

	return false;
}

bool Input::OnMousePush(KeyCode::MouseButton button_type_)
{
	if (prev_mouse_state_.rgbButtons[button_type_] & MOUSE_ON_VALUE &&
		curr_mouse_state_.rgbButtons[button_type_] & MOUSE_ON_VALUE)
	{
		return true;
	}

	return false;
}

bool Input::OnMouseUp(KeyCode::MouseButton button_type_)
{
	if (prev_mouse_state_.rgbButtons[button_type_] & MOUSE_ON_VALUE &&
		!(curr_mouse_state_.rgbButtons[button_type_] & MOUSE_ON_VALUE))
	{
		return true;
	}

	return false;
}

void Input::UpdateMouse()
{
	D3DXVECTOR2 prev = mouse_pos_;
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(FindWindowA(TEXT("XFileDraw"), nullptr), &p);

	mouse_pos_.x = (float)p.x;
	mouse_pos_.y = (float)p.y;

	mouse_move_vec_.x = mouse_pos_.x - prev.x;
	mouse_move_vec_.y = mouse_pos_.y - prev.y;
}

D3DXVECTOR2 Input::GetMousePos()
{
	return mouse_pos_;
}

void Input::UpdateGamePad()
{

	if (gamepad_device_ == nullptr)
	{
		return;
	}

	DIJOYSTATE pad_data;
	HRESULT hr = gamepad_device_->GetDeviceState(sizeof(DIJOYSTATE), &pad_data);
	if (FAILED(hr))
	{
		if (FAILED(gamepad_device_->Acquire()))
		{
			for (int i = 0; i < KeyCode::ButtonKind::BUTTON_KIND_MAX; i++)
			{
				ButtonStates[i] = KeyState::ButtonState::BUTTON_STATE_NONE;
			}
		}
	}

	bool is_push[KeyCode::ButtonKind::BUTTON_KIND_MAX] = { false };

	//左アナログスティック※500のとこでデッドゾーン調整
	if (pad_data.lX < -500)
	{
		is_push[KeyCode::ButtonKind::L_LEFT_STICK] = true;
	}
	else if (pad_data.lX > 500)
	{
		is_push[KeyCode::ButtonKind::L_RIGHT_STICK] = true;
	}

	if (pad_data.lY < -500)
	{
		is_push[KeyCode::ButtonKind::L_UP_STICK] = true;
	}
	else if (pad_data.lY > 500)
	{
		is_push[KeyCode::ButtonKind::L_DOWN_STICK] = true;
	}
	if (pad_data.lZ > 50000)
	{
		is_push[KeyCode::ButtonKind::LEFT_T_BUTTON] = true;
	}
	else if (pad_data.lZ < 500)
	{
		is_push[KeyCode::ButtonKind::RIGHT_T_BUTTON] = true;
	}

	if (pad_data.lRx < 10000)
	{
		is_push[KeyCode::ButtonKind::R_LEFT_STICK] = true;
	}
	else if (pad_data.lRx > 40000)
	{
		is_push[KeyCode::ButtonKind::R_RIGHT_STICK] = true;
	}

	if (pad_data.lRy < 10000)
	{
		is_push[KeyCode::ButtonKind::R_UP_STICK] = true;
	}
	else if (pad_data.lRy > 40000)
	{
		is_push[KeyCode::ButtonKind::R_DOWN_STICK] = true;
	}
	// 十字キー
	if (pad_data.rgdwPOV[0] != 0xFFFFFFFF)
	{
		float rad = D3DXToRadian((pad_data.rgdwPOV[0] / 100.0f));
		// 本来はxがcos、yがsinだけど、rgdwPOVは0が上から始まるので、
		// cosとsinを逆にした方が都合がいい
		float x = sinf(rad);
		float y = cosf(rad);

		if (x < -0.01f)
		{
			is_push[KeyCode::ButtonKind::LEFT_BUTTON] = true;
		}
		else if (x > 0.01f)
		{
			is_push[KeyCode::ButtonKind::RIGHT_BUTTON] = true;
		}

		if (y > 0.01f)
		{
			is_push[KeyCode::ButtonKind::UP_BUTTON] = true;
		}
		else if (y < -0.01f)
		{
			is_push[KeyCode::ButtonKind::DOWN_BUTTON] = true;
		}
	}

	for (int i = 0; i < 6; ++i)
	{
		if (pad_data.rgbButtons[i] == 0x80)
		{
			is_push[i] = true;
		}
		else {
			is_push[i] = false;
		}
	}


	for (int i = 0; i < KeyCode::ButtonKind::BUTTON_KIND_MAX; i++)
	{
		if (is_push[i] == true)
		{
			if (ButtonStates[i] == KeyState::ButtonState::BUTTON_STATE_NONE)
			{
				ButtonStates[i] = KeyState::ButtonState::BUTTON_STATE_DOWN;
			}
			else
			{
				ButtonStates[i] = KeyState::ButtonState::BUTTON_STATE_PUSH;
			}
		}
		else
		{
			if (ButtonStates[i] == KeyState::ButtonState::BUTTON_STATE_PUSH)
			{
				ButtonStates[i] = KeyState::ButtonState::BUTTON_STATE_UP;
			}
			else
			{
				ButtonStates[i] = KeyState::ButtonState::BUTTON_STATE_NONE;
			}
		}
	}
}

bool Input::IsButtonPush(KeyCode::ButtonKind button_)
{
	if (ButtonStates[button_] == KeyState::ButtonState::BUTTON_STATE_PUSH)
	{
		return true;
	}

	return false;
}

bool Input::IsButtonUp(KeyCode::ButtonKind button_)
{
	if (ButtonStates[button_] == KeyState::ButtonState::BUTTON_STATE_UP)
	{
		return true;
	}

	return false;
}

bool Input::IsButtonDown(KeyCode::ButtonKind button_)
{
	if (ButtonStates[button_] == KeyState::ButtonState::BUTTON_STATE_DOWN)
	{
		return true;
	}

	return false;
}