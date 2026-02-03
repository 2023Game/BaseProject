#include "CColliderBox.h"
#include <glut.h>
#include "CColor.h"
#include "Primitive.h"

// コンストラクタ
CColliderBox::CColliderBox(CObjectBase* owner, ELayer layer, const CVector& halfSize,
	bool isKinematic, float weight)
	: CCollider(owner, layer, EColliderType::eBox, isKinematic, weight)
	, mHalfSize(halfSize)
	, mWorldPos(0.0f, 0.0f, 0.0f)
	, mWorldHalfSize(1.0f, 1.0f, 1.0f)
{
}

// ボックスコライダーの設定
void CColliderBox::Set(CObjectBase* owner, ELayer layer, const CVector& halfSize)
{
	CCollider::Set(owner, layer);
	mHalfSize = halfSize;
}

// ボックスの値を取得
void CColliderBox::Get(CVector* outPos, CVector* outAxisX, CVector* outAxisY, CVector* outAxisZ, CVector* outHalfSize) const
{
	*outPos = mWorldPos;
	*outAxisX = mWorldAxis[0];
	*outAxisY = mWorldAxis[1];
	*outAxisZ = mWorldAxis[2];
	*outHalfSize = mWorldHalfSize;
}

#if _DEBUG
// コライダー描画
void CColliderBox::Render()
{
	// DIFFUSE赤色設定
	CColor col = CColor::red;
	if (!IsEnable() ||
		(Owner() != nullptr && !Owner()->IsEnableCol()))
	{
		col = CColor::gray;
	}
	col.A(0.2f);

	// ボックスを描画
	CMatrix m = Matrix();

	CMatrix sm;
	sm.Scale(mHalfSize * 2.0f);

	Primitive::DrawWireBox(sm * m, col);
}
#endif

// コライダーの情報を更新
void CColliderBox::UpdateCol(bool isInit)
{
	// 前回の更新と同じ行列であれば、処理しない
	CMatrix m = Matrix();
	if (!isInit && m == mLastMtx) return;
	mLastMtx = m;

	mWorldPos = m.Position();

	mWorldAxis[0] = m.VectorX().Normalized();
	mWorldAxis[1] = m.VectorY().Normalized();
	mWorldAxis[2] = m.VectorZ().Normalized();

	CVector scale = m.Scale();
	mWorldHalfSize.X(mHalfSize.X() * scale.X());
	mWorldHalfSize.Y(mHalfSize.Y() * scale.Y());
	mWorldHalfSize.Z(mHalfSize.Z() * scale.Z());

	// バウンディングボックスを更新
	mBounds = CBounds::GetBoxBounds(mWorldPos, mWorldAxis, mWorldHalfSize);
}
