#pragma once
#include "CCamera.h"

// ゲーム中のカメラ
class CGameCamera : public CCamera
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="eye">カメラの視点</param>
	/// <param name="center">カメラの注視点</param>
	/// <param name="isMainCamera">メインカメラかどうか</param>
	CGameCamera(const CVector& eye, const CVector& center, bool isMainCamera = true);
	// デストラクタ
	~CGameCamera();

	// 更新
	void Update() override;
};