#include "CGameCamera.h"
#include "CInput.h"

// カメラの回転速度
#define ROTATE_SPEED 0.1f

// コンストラクタ
CGameCamera::CGameCamera(const CVector& eye, const CVector& center, bool isMainCamera)
	: CCamera(eye, center, isMainCamera)
{
}

// デストラクタ
CGameCamera::~CGameCamera()
{
}

// 更新
void CGameCamera::Update()
{
	// マウスの横方向の移動量に合わせて、回転値（クォータニオン）を求める
	CVector2 delta = CInput::GetDeltaMousePos();
	CQuaternion rot = CQuaternion(0.0f, delta.X() * ROTATE_SPEED, 0.0f);

	// 追従時のオフセット位置を回転
	mFollowOffsetPos = rot * mFollowOffsetPos;

	// カメラの向きを回転
	CVector vec = -mEyeVec.Normalized();
	CVector forward = rot * vec;
	Rotation(CQuaternion::LookRotation(forward));

	// カメラのベースの更新処理
	CCamera::Update();
}
