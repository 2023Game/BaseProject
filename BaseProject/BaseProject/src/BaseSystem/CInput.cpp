#include "CInput.h"
#include "CDebugInput.h"
#include <stdio.h>

GLFWwindow* CInput::spWindow = nullptr;	// ウィンドウのポインタ
std::map<int, int> CInput::msInputBits;	// キーの入力状態を管理するリスト
CVector2 CInput::msMousePos = CVector2(0.0f, 0.0f);	// 現在のマウス座標
CVector2 CInput::msLastMousePos = CVector2(0.0f, 0.0f);	// 前回のマウス座標
int CInput::msMouseWheel = 0;		// マウスホイールの回転量
int CInput::msLastMouseWheel = 0;	// 前回のマウスホイールの回転量
int CInput::msDeltaMouseWheel = 0;	// マウスホイールの回転量の差分

#if _DEBUG
// 通常入力が無効化される前のマウス座標
CVector2 CInput::msDebugLastMousePos = CVector2(0.0f, 0.0f);
#endif

// ウィンドウを設定
void CInput::Window(GLFWwindow* pwindow)
{
	spWindow = pwindow;
}

// マウス座標を取得
CVector2 CInput::GetMousePos()
{
#if _DEBUG
	if (CDebugInput::IsOn()) return msDebugLastMousePos;
#endif
	return msMousePos;
}

// マウス座標の移動量を取得
CVector2 CInput::GetDeltaMousePos()
{
#if _DEBUG
	if (CDebugInput::IsOn()) return CVector2(0.0f, 0.0f);
#endif
	return msMousePos - msLastMousePos;
}

// マウスホイールの回転量を加算
void CInput::AddMouseWheel(int wheel)
{
	msMouseWheel += wheel;
}

// マウスホイールの回転量を取得
int CInput::GetDeltaMouseWheel()
{
#if _DEBUG
	if (CDebugInput::IsOn()) return 0;
#endif
	return msDeltaMouseWheel;
}

// コンストラクタ
CInput::CInput()
{
	printf("入力インスタンスが生まれました\n");
}

// キーが入力されているか
bool CInput::Key(int key)
{
#if _DEBUG
	if (CDebugInput::IsOn()) return false;
#endif
	return (msInputBits[key] & (1 << eInputBit_On)) != 0;
}

// キーを押した瞬間かどうか
bool CInput::PushKey(int key)
{
#if _DEBUG
	if (CDebugInput::IsOn()) return false;
#endif
	return (msInputBits[key] & (1 << eInputBit_Push)) != 0;
}

// キーを離した瞬間かどうか
bool CInput::PullKey(int key)
{
#if _DEBUG
	if (CDebugInput::IsOn()) return false;
#endif
	return (msInputBits[key] & (1 << eInputBit_Pull)) != 0;
}

// マウスカーソルの表示設定
void CInput::ShowCursor(bool isShow)
{
	glfwSetInputMode
	(
		spWindow,
		GLFW_CURSOR,
		isShow ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED
	);
}

// キーの入力状態を更新
void CInput::Update()
{
	auto itBegin = msInputBits.begin();
	auto itEnd = msInputBits.end();
	for (auto it = itBegin; it != itEnd; ++it)
	{
		int key = it->first;
		int bit = it->second;

		// 現在押している状態か取得
		bool isOn = (GetAsyncKeyState(key) & 0x8000) != 0;
		// 前回押している状態か取得
		bool isLastOn = bit & (1 << eInputBit_On);

		// ビットフラグ初期化
		bit = 0;

		// 現在押している状態であれば、
		// eInputBit_Onのビットを立てる
		if (isOn) bit |= (1 << eInputBit_On);
		// 前回は離していて、現在は押している状態であれば、
		// eInputBit_Pushのビットを立てる
		if (!isLastOn && isOn) bit |= (1 << eInputBit_Push);
		// 前回は押していて、現在は離している状態であれば、
		// eInputBit_Pullのビットを立てる
		if (isLastOn && !isOn) bit |= (1 << eInputBit_Pull);

		// リスト内のビットフラグを上書き
		it->second = bit;
	}

	// マウス座標更新
	msLastMousePos = msMousePos;
	double xpos, ypos;
	glfwGetCursorPos(spWindow, &xpos, &ypos);
	//	glfwGet
	msMousePos = CVector2((float)xpos, (float)ypos);
#if _DEBUG
	if (!CDebugInput::IsOn())
	{
		msDebugLastMousePos = msMousePos;
	}
#endif

	// マウスホイールの回転量を更新
	msDeltaMouseWheel = msMouseWheel - msLastMouseWheel;
	msLastMouseWheel = msMouseWheel;
}
