#include "CColliderCapsule.h"
#include <glut.h>
#include "Maths.h"
#include "CColor.h"
#include "Primitive.h"

// コンストラクタ
CColliderCapsule::CColliderCapsule(CObjectBase* owner, ELayer layer,
	const CVector& v0, const CVector& v1, float radius,
	bool isKinematic, float weight)
	: CCollider(owner, layer, EColliderType::eCapsule, isKinematic, weight)
	, mRadius(radius)
{
	// カプセルを構成する線分の頂点を設定
	mV[0] = v0;
	mV[1] = v1;
}

// カプセルコライダーの設定
void CColliderCapsule::Set(CObjectBase* owner, ELayer layer,
	const CVector& v0, const CVector& v1, float radius)
{
	CCollider::Set(owner, layer);

	// カプセルを構成する線分の頂点を設定
	mV[0] = v0;
	mV[1] = v1;

	// 半径を設定
	mRadius = radius;
}

// カプセルを構成する線分の視点と終点を取得
void CColliderCapsule::Get(CVector* v0, CVector* v1) const
{
	*v0 = mWV[0];
	*v1 = mWV[1];
}

// カプセルの半径を取得
float CColliderCapsule::Radius() const
{
	return mRadius;
}

// コライダー描画
void CColliderCapsule::Render()
{
	// DIFFUSE赤色設定
	CColor col = CColor::red;
	if (!IsEnable()) col = CColor::gray;

	CMatrix m = Matrix();
	CVector s = mV[0] * m;
	CVector e = mV[1] * m;
	Primitive::DrawWireCapsule
	(
		s, e, mRadius, col
	);
}

// コライダーの情報を更新
void CColliderCapsule::UpdateCol()
{
	// 頂点のワールド座標を算出
	CMatrix m = Matrix();
	mWV[0] = mV[0] * m;
	mWV[1] = mV[1] * m;

	// バウンディングボックスを更新
	mBounds = CBounds::GetCapsuleBounds(mWV[0], mWV[1], mRadius);
}
