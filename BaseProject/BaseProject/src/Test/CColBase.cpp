#include "CColBase.h"
#include "CDebugInput.h"
#include "CCamera.h"

// コンストラクタ
CColBase::CColBase(const CColor& defaultColor)
	: CObjectBase(ETag::eNone, ETaskPriority::eDefault)
	, mDefaultColor(defaultColor)
	, mIsCollision(false)
	, mIsSelect(false)
{
}

// デストラクタ
CColBase::~CColBase()
{
}

// 選択状態を設定
void CColBase::SetSelect(bool isSelect)
{
	mIsSelect = isSelect;
}

// 衝突処理
void CColBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	mIsCollision = true;
}

// 更新
void CColBase::Update()
{
#if _DEBUG
	mIsCollision = false;

	// 選択状態でなければ、以降処理しない
	if (!mIsSelect) return;

	if (CDebugInput::Key(VK_CONTROL))
	{
		// 入力ベクトルを取得
		CVector input;
		if (CDebugInput::Key(VK_SHIFT))
		{
			if (CDebugInput::Key(VK_LEFT))		input.Z(1.0f);
			else if (CDebugInput::Key(VK_RIGHT))input.Z(-1.0f);
		}
		else
		{
			if (CDebugInput::Key(VK_UP))		input.X(1.0f);
			else if (CDebugInput::Key(VK_DOWN))	input.X(-1.0f);
			if (CDebugInput::Key(VK_LEFT))		input.Y(-1.0f);
			else if (CDebugInput::Key(VK_RIGHT))input.Y(1.0f);
		}

		// 入力ベクトルの長さで入力されているか判定
		if (input.LengthSqr() > 0.0f)
		{
			static const float rotateSpeed = 5.0f;
			Rotate(input * rotateSpeed);
		}
	}
	else
	{
		// 入力ベクトルを取得
		CVector input;
		if (CDebugInput::Key(VK_SHIFT))
		{
			if (CDebugInput::Key(VK_UP))		input.Y(1.0f);
			else if (CDebugInput::Key(VK_DOWN))
			{
				input.Y(-1.0f);
			}
		}
		else
		{
			if (CDebugInput::Key(VK_UP))		input.Z(-1.0f);
			else if (CDebugInput::Key(VK_DOWN))	input.Z(1.0f);
			if (CDebugInput::Key(VK_LEFT))		input.X(-1.0f);
			else if (CDebugInput::Key(VK_RIGHT))input.X(1.0f);
		}

		// 入力ベクトルの長さで入力されているか判定
		if (input.LengthSqr() > 0.0f)
		{
			CVector move = input;
			if (input.Y() == 0.0f)
			{
				// カメラの向きに合わせた移動ベクトルに変換
				CQuaternion camRot = CCamera::CurrentCamera()->Rotation();
				move = camRot * input;
				move.Y(0.0f);
			}
			move.Normalize();

			// コライダーを移動
			static const float moveSpeed = 0.25f;
			Position(Position() + move * moveSpeed);
		}
	}
#endif
}

// 現在の色を取得
CColor CColBase::GetColor() const
{
	CColor color = mDefaultColor;
	if (mIsSelect)
	{
		if (mIsCollision) color = CColor(1.0f, 0.0f, 1.0f, 1.0f);
		else color = CColor(1.0f, 0.75f, 0.0f, 1.0f);
	}
	else
	{
		if (mIsCollision) color = CColor::red;
	}
	return color;
}
