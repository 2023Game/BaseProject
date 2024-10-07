#pragma once
#if _DEBUG
#include "CInput.h"

// 通常の入力判定を無効にする
// デバッグモード時の入力判定を行うクラス
class CDebugInput
{
public:
	// マウス座標を取得
	static CVector2 GetMousePos();

	// マウス座標の移動量を取得
	static CVector2 GetDeltaMousePos();

	// マウスホイールの回転量の差分を取得
	static int GetDeltaMouseWheel();

	/// <summary>
	/// キーが入力されているか
	/// </summary>
	/// <param name="key">判定するキー</param>
	/// <returns>trueならば、押されている</returns>
	static bool Key(int key);

	/// <summary>
	/// キーを押した瞬間かどうか
	/// 前回のフレームは押していない状態で
	/// 今回のフレームで押した状態になった時の
	/// 1フレームのみtrueを返す
	/// </summary>
	/// <param name="key">判定するキー</param>
	/// <returns>trueならば、押した瞬間である</returns>
	static bool PushKey(int key);

	/// <summary>
	/// キーを離した瞬間かどうか
	/// 前回のフレームは押している状態で
	/// 今回のフレームで離した状態になった時の
	/// 1フレームのみtrueを返す
	/// </summary>
	/// <param name="key">判定するキー</param>
	/// <returns>trueならば、離した瞬間である</returns>
	static bool PullKey(int key);

	/// <summary>
	/// デバッグ入力モードがオンになっているかどうか
	/// </summary>
	/// <returns></returns>
	static bool IsOn();
	/// <summary>
	/// デバッグ入力モードをオン
	/// </summary>
	static void On();
	/// <summary>
	/// デバッグ入力モードをオフ
	/// </summary>
	static void Off();

private:
	CDebugInput();

	// デバッグ入力モードかどうか
	static bool msIsOn;
};
#endif