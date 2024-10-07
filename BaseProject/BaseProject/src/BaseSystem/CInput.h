#pragma once
#include <Windows.h>
#include "GLFW/glfw3.h"
#include <map>
#include "CVector.h"
class CDebugInput;

// 入力判定クラス
class CInput
{
	friend CDebugInput;
public:
	/// <summary>
	/// ウィンドウを設定
	/// </summary>
	/// <param name="pwindow"></param>
	static void Window(GLFWwindow* pwindow);

	// マウス座標を取得
	static CVector2 GetMousePos();

	// マウス座標の移動量を取得
	static CVector2 GetDeltaMousePos();

	// マウスホイールの回転量を加算
	static void AddMouseWheel(int wheel);
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
	/// マウスカーソルの表示設定
	/// </summary>
	/// <param name="isShow">trueならば、表示する</param>
	static void ShowCursor(bool isShow);

	/// <summary>
	/// キーの入力状態を更新
	/// （呼び出しは1フレームに1回まで）
	/// </summary>
	static void Update();

private:
	// コンストラクタ
	// （インスタンスを生成できないようにprivate）
	CInput();

	static GLFWwindow* spWindow;	// ウィンドウのポインタ

	// キーの入力状態ビット
	enum InputBit
	{
		eInputBit_On,	//押しているかどうか
		eInputBit_Push,	//押した瞬間かどうか
		eInputBit_Pull,	//離した瞬間かどうか
		Num,
	};
	// キーの入力状態を管理するリスト
	static std::map<int, int> msInputBits;
	// 現在のマウス座標
	static CVector2 msMousePos;
	// 前回のマウス座標
	static CVector2 msLastMousePos;
	// マウスホイールの回転量
	static int msMouseWheel;
	// 前回のマウスホイールの回転量
	static int msLastMouseWheel;
	// マウスホイールの回転量の差分
	static int msDeltaMouseWheel;

#if _DEBUG
	// 通常入力が無効化される前のマウス座標
	static CVector2 msDebugLastMousePos;
#endif
};