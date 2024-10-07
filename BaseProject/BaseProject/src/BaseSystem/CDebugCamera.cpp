#include "CDebugCamera.h"
#if _DEBUG
#include "CDebugInput.h"
#include "glut.h"
#include "Maths.h"

// 加速度
#define MOVE_ADD_SPEED 0.0625f
// 最大速度
#define MOVE_MAX_SPEED 10.0f
// 移動速度の減衰率
#define MOVE_ATTEN_RATIO 0.95f
// 移動速度の倍率の最小値
#define MOVE_SPEED_RATIO_MIN 0.1f
// 移動速度の倍率の最大値
#define MOVE_SPEED_RATIO_MAX 3.0f
// 移動速度の倍率の変換量
#define MOVE_SPEED_RATIO_VALUE 0.05f
// 回転速度
#define ROTATE_SPEED 0.125f
// 平行移動速度
#define TRANSLATE_SPEED 0.125f

// デバッグカメラのインスタンス
CDebugCamera* CDebugCamera::spDebugCamera = nullptr;
// デバッグカメラがオンになっているかどうか
bool CDebugCamera::msIsOn = false;

// コンストラクタ
CDebugCamera::CDebugCamera()
	: CCamera(CVector(0.0f, 0.0f, 10.0f), CVector(0.0f, 0.0f, 0.0f), false)
	, mMoveSpeed(0.0f, 0.0f, 0.0f)
	, mMoveSpeedRatio(1.0f)
	, mRotateAngle(0.0f, 0.0f, 0.0f)
{
}

// デストラクタ
CDebugCamera::~CDebugCamera()
{
	spDebugCamera = nullptr;
}

// デバッグカメラを取得
CDebugCamera* CDebugCamera::DebugCamera()
{
	if (spDebugCamera == nullptr)
	{
		spDebugCamera = new CDebugCamera();
	}
	return spDebugCamera;
}

// デバッグカメラがオンになっているかどうか
bool CDebugCamera::IsOn()
{
	return msIsOn;
}

// デバッグカメラをオン
void CDebugCamera::On(bool disableInput)
{
	// 通常の入力処理を無効にする場合は、
	// デバッグ入力モードをオンにする
	if (disableInput)
	{
		CDebugInput::On();
	}

	// デバッグカメラの有効フラグを立てる
	msIsOn = true;

	// デバッグカメラをオンにした時に
	// デバッグカメラの状態をメインカメラに合わせる
	CCamera* mainCamera = CCamera::MainCamera();
	if (mainCamera != nullptr)
	{
		spDebugCamera->CopyCamera(mainCamera);
		spDebugCamera->mMoveSpeed = CVector::zero;
		spDebugCamera->mMoveSpeedRatio = 1.0f;
		spDebugCamera->mRotateAngle = spDebugCamera->EulerAngles();
		spDebugCamera->mRotateAngle.Z(0.0f);
	}

	spDebugCamera->Reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
}

// デバッグカメラをオフ
void CDebugCamera::Off()
{
	CDebugInput::Off();
	msIsOn = false;

	CCamera* mainCamera = CCamera::MainCamera();
	if (mainCamera != nullptr)
	{
		mainCamera->Reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
	}
}

// 更新
void CDebugCamera::Update()
{
	// [SHIFT] + [0]でデバッグカメラのオンオフ切り替え
	if (CDebugInput::Key(VK_SHIFT))
	{
		if (CDebugInput::PushKey('0'))
		{
			if (IsOn()) Off();
			else On();
		}
	}

	// デバッグカメラが有効でなければ、以降処理しない
	if (!msIsOn) return;

	// マウスホイールの回転で移動速度の倍率を変更
	mMoveSpeedRatio += MOVE_SPEED_RATIO_VALUE * CDebugInput::GetDeltaMouseWheel();
	mMoveSpeedRatio = Math::Clamp(mMoveSpeedRatio, MOVE_SPEED_RATIO_MIN, MOVE_SPEED_RATIO_MAX);

	// キーの入力ベクトルを取得
	CVector input;
	if (CDebugInput::Key('W'))		input.Z(-1.0f);
	else if (CDebugInput::Key('S'))	input.Z(1.0f);
	if (CDebugInput::Key('A'))		input.X(-1.0f);
	else if (CDebugInput::Key('D'))	input.X(1.0f);

	// 入力ベクトルの長さで入力されているか判定
	if (input.LengthSqr() > 0.0f)
	{
		// 移動速度を加速
		float addSpeed = MOVE_ADD_SPEED * mMoveSpeedRatio;
		CVector move = CQuaternion(mRotateAngle) * input;
		mMoveSpeed += move.Normalized() * addSpeed;
		// 移動速度を最大値でクランプ
		float maxSpeed = MOVE_MAX_SPEED * mMoveSpeedRatio;
		if (mMoveSpeed.Length() > maxSpeed)
		{
			mMoveSpeed = mMoveSpeed.Normalized() * maxSpeed;
		}
	}

	// 移動速度を減衰
	mMoveSpeed *= MOVE_ATTEN_RATIO;
	// カメラの位置を更新
	Position(Position() + mMoveSpeed);

	// 左ドラッグでカメラを平行移動
	if (CDebugInput::Key(VK_LBUTTON))
	{
		CVector2 delta = CDebugInput::GetDeltaMousePos();
		Position(Position() + VectorX() * delta.X() * TRANSLATE_SPEED);
		Position(Position() + VectorY() * delta.Y() * TRANSLATE_SPEED);
	}
	// 右ドラッグでカメラを回転
	else if (CDebugInput::Key(VK_RBUTTON))
	{
		CVector2 delta = CDebugInput::GetDeltaMousePos();
		float x = Math::Repeat(mRotateAngle.X() + delta.Y() * ROTATE_SPEED, 360.0f);
		float y = Math::Repeat(mRotateAngle.Y() + delta.X() * ROTATE_SPEED, 360.0f);
		mRotateAngle.X(x);
		mRotateAngle.Y(y);
	}
	Rotation(CQuaternion(mRotateAngle));

	mEye = Position();
	mAt = mEye + -VectorZ().Normalized();
	mUp = VectorY();

	// デバッグ表示
	CVector pos = Position();
	CDebugPrint::Print("■■■■■■ デバッグカメラ：オン ■■■■■■\n");
	CDebugPrint::Print("[W][A][S][D]：カメラ移動\n");
	CDebugPrint::Print("左ドラッグ：カメラ平行移動\n");
	CDebugPrint::Print("右ドラッグ：カメラ回転\n");
	CDebugPrint::Print("ホイール：カメラ移動速度割合 x %.2f\n", mMoveSpeedRatio);
	CDebugPrint::Print("カメラ座標：%f, %f, %f\n", pos.X(), pos.Y(), pos.Z());
	CDebugPrint::Print("■■■■■■■■■■■■■■■■■■■■■■■\n");

	// カメラのベース更新
	CCamera::Update();
}

#endif
