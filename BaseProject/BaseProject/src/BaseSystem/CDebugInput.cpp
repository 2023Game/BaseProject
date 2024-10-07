#include "CDebugInput.h"

#if _DEBUG

// デバッグ入力モードがオンになっているかどうか
bool CDebugInput::msIsOn = false;

// コンストラクタ
CDebugInput::CDebugInput()
{
}

// マウス座標を取得
CVector2 CDebugInput::GetMousePos()
{
	return CInput::msMousePos;
}

// マウス座標の移動量を取得
CVector2 CDebugInput::GetDeltaMousePos()
{
	return CInput::msMousePos - CInput::msLastMousePos;
}

// マウスホイールの回転量の差分を取得
int CDebugInput::GetDeltaMouseWheel()
{
	return CInput::msDeltaMouseWheel;
}

// キーが入力されているか
bool CDebugInput::Key(int key)
{
	return (CInput::msInputBits[key] & (1 << CInput::eInputBit_On)) != 0;
}

// キーを押した瞬間かどうか
bool CDebugInput::PushKey(int key)
{
	return (CInput::msInputBits[key] & (1 << CInput::eInputBit_Push)) != 0;
}

// キーを離した瞬間かどうか
bool CDebugInput::PullKey(int key)
{
	return (CInput::msInputBits[key] & (1 << CInput::eInputBit_Pull)) != 0;
}

// デバッグ入力モードがオンになっているかどうか
bool CDebugInput::IsOn()
{
	return msIsOn;
}

// デバッグ入力モードをオン
void CDebugInput::On()
{
	msIsOn = true;
}

// デバッグ入力モードをオフ
void CDebugInput::Off()
{
	msIsOn = false;
}

#endif
