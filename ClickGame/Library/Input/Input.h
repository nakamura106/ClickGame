#ifndef INPUT_H_
#define INPUT_H_

#include <Windows.h>
#include <dinput.h>
#include<d3dx9.h>
struct KeyCode
{
	// @brife キーの種類
	enum KEY_INFO
	{
		ESC_KEY,
		SPACE_KEY,
		UP_KEY,
		DOWN_KEY,
		RIGHT_KEY,
		LEFT_KEY,
		A_KEY,
		W_KEY,
		S_KEY,
		D_KEY,
		F_KEY,
		E_KEY,
		Q_KEY,
		T_KEY,
		Y_KEY,
		RETURN_KEY,
		L_CONTROL,
		L_SHIFT,
		MAX_KEY_INFO,
	};

	enum ButtonKind
	{
		A_BUTTON,
		B_BUTTON,
		X_BUTTON,
		Y_BUTTON,
		LEFT_B_BUTTON,
		RIGHT_B_BUTTON,
		LEFT_T_BUTTON,
		RIGHT_T_BUTTON,
		L_UP_STICK,
		L_DOWN_STICK,
		L_LEFT_STICK,
		L_RIGHT_STICK,
		R_UP_STICK,
		R_DOWN_STICK,
		R_LEFT_STICK,
		R_RIGHT_STICK,
		UP_BUTTON,
		DOWN_BUTTON,
		LEFT_BUTTON,
		RIGHT_BUTTON,
		BUTTON_KIND_MAX,
	};
	enum MouseButton
	{
		LEFT,		//!< 左
		RIGHT,		//!< 右
		CENTER,		//!< 真ん中
	};

};

struct KeyState
{
	
	enum ButtonState
	{
		BUTTON_STATE_NONE,
		BUTTON_STATE_DOWN,
		BUTTON_STATE_PUSH,
		BUTTON_STATE_UP,
		BUTTON_STATE_MAX,
	};

	enum InputState
	{
		NOT_PUSH,		//押されていない
		PUSH_DOWN,		//押された瞬間
		PUSH,			//押されている
		PUSH_UP,		//離した瞬間
	};
};

class Input {
public:
	/** @brief マウスボタンの種類 */
	

	struct DeviceEnumParameter
	{
		LPDIRECTINPUTDEVICE8* gamepad_device_ptr;
		int find_count;
	};
	
public:
/**
	* @brief Input機能の初期化関数@n
	* デバイスの入力取得に必要な初期化を行います
	* InitEngineで実行されるので開発側が実行する必要はありません
	* @return 初期化結果、成功の場合はtrue
	* @param[in] hInstance インスタンスハンドル
	* @param[in] hWindow ウィンドウハンドル
	*/
	bool InitInput();

	/**
	* @brief Input機能の終了関数@n
	* Input機能を終了させます@n
	* EndEngineで実行されるので開発側が実行する必要はありません
	*/
	void EndInput();


	
	

	/**
	* @brief キーが押されている状態の判定関数@n
	* 指定したキーが押されているかを判定します
	* @return 判定結果、押されているならtrue
	* @param[in] key 判定したいキー
	*/
	bool GetKey(KeyCode::KEY_INFO key_);

	/**
	* @brief 入力情報の更新@n
	* デバイスの入力情報の更新を行う@n
	* 毎フレームに１度必ず実行する必要がある
	*/
	void UpdateInput();

	

	/**
	* @brief キーが押された瞬間の判定関数@n
	* 指定したキーが押された瞬間化を判定します
	* @return 判定結果、押された瞬間ならtrue
	* @param[in] key 判定したいキー
	*/
	bool GetKeyDown(KeyCode::KEY_INFO key_);

	/**
	* @brief キーが離された瞬間の判定関数@n
	* 指定したキーが離された瞬間化を判定します
	* @return 判定結果、離されたならtrue
	* @param[in] key 判定したいキー
	*/
	bool GetKeyUp(KeyCode::KEY_INFO key_);

	/**
	* @brief クリックした瞬間判定関数@n
	* 指定したマウスのボタンがクリックされた瞬間かどうかを判定する
	* @return 判定結果(押された瞬間ならtrue)
	* @param[in] button_type 判定したいボタンの種類
	*/
	bool OnMouseDown(KeyCode::MouseButton button_type_);

	/**
	* @brief クリック中判定関数@n
	* 指定したマウスのボタンがクリック中かどうかを判定する
	* @return 判定結果(クリック中瞬間ならtrue)
	* @param[in] button_type 判定したいボタンの種類
	*/
	bool OnMousePush(KeyCode::MouseButton button_type_);

	/**
	* @brief クリック終了判定関数@n
	* 指定したマウスのボタンがクリックを終了した瞬間かどうかを判定する
	* @return 判定結果(クリックをやめた瞬間ならtrue)
	* @param[in] button_type 判定したいボタンの種類
	*/
	bool OnMouseUp(KeyCode::MouseButton button_type_);

	/** @brief マウスの座標取得関数 */
	D3DXVECTOR2 GetMousePos();

	bool IsButtonPush(KeyCode::ButtonKind button_);
	bool IsButtonUp(KeyCode::ButtonKind button_);
	bool IsButtonDown(KeyCode::ButtonKind button_);

	private:
		BOOL StartGamePadControl();
		BOOL SetUpGamePadProperty(LPDIRECTINPUTDEVICE8 device_);
		static BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi_, LPVOID pvRef_);		
		/**
	* @brief キーボードの入力情報の更新@n
	* デバイスの入力情報の更新を行います@n
	* 毎フレームに1度必ず実行する必要があります
	*/
		void KeyStateUpdate();
		void UpdateMouse();
		void UpdateGamePad();
		bool CreateInputInterface();
		bool CreateMouseDevice();
		bool CreateGamePadDevice();
		bool CreateKeyboardDevice();

	private:
		LPDIRECTINPUT8 input_interface_;

		LPDIRECTINPUTDEVICE8 key_device_;
		LPDIRECTINPUTDEVICE8 mouse_device_;		//!< DIRECTINPUTDEVICE8のポインタ
		LPDIRECTINPUTDEVICE8 gamepad_device_;

		DIMOUSESTATE prev_mouse_state_;			//!< マウスの１フレーム前の入力情報
		DIMOUSESTATE curr_mouse_state_;		//!< マウスの現在の入力情報

		D3DXVECTOR2 mouse_pos_;
		D3DXVECTOR2 mouse_move_vec_;


		KeyState::ButtonState ButtonStates[KeyCode::ButtonKind::BUTTON_KIND_MAX];

		KeyState::InputState InputState[KeyCode::KEY_INFO::MAX_KEY_INFO];

		int KeyInfo[KeyCode::KEY_INFO::MAX_KEY_INFO] = {
			DIK_ESCAPE,
			DIK_SPACE,
			DIK_UP,
			DIK_DOWN,
			DIK_RIGHT,
			DIK_LEFT,
			DIK_A,
			DIK_W,
			DIK_S,
			DIK_D,
			DIK_F,
			DIK_E,
			DIK_Q,
			DIK_T,
			DIK_Y,
			DIK_RETURN,
			DIK_LCONTROL,
			DIK_LSHIFT,
		};
};



#endif