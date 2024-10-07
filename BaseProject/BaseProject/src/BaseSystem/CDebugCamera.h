#pragma once
#include "CCamera.h"
#if _DEBUG

// デバッグカメラ
class CDebugCamera : public CCamera
{
public:
	// デバッグカメラを取得
	static CDebugCamera* DebugCamera();

	// デバッグカメラがオンになっているかどうか
	static bool IsOn();
	/// <summary>
	/// デバッグカメラをオン
	/// </summary>
	/// <param name="disableInput">通常の入力処理を無効にするかどうか</param>
	static void On(bool disableInput = true);
	// デバッグカメラをオフ
	static void Off();

	// 更新
	void Update() override;

private:
	// コンストラクタ
	CDebugCamera();
	// デストラクタ
	~CDebugCamera();

	// デバッグカメラのインスタンス
	static CDebugCamera* spDebugCamera;
	// デバッグカメラがオンになっているかどうか
	static bool msIsOn;
	// カメラの移動速度
	CVector mMoveSpeed;
	// カメラの移動速度の倍率
	float mMoveSpeedRatio;
	// カメラの回転角度
	CVector mRotateAngle;
};

#endif
