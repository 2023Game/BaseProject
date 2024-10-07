#include "CColCapsule.h"
#include "Primitive.h"

// コンストラクタ
CColCapsule::CColCapsule(
	const CVector& start, const CVector& end,
	float radius, const CColor& defaultColor,
	bool isKinematic, float weight)
	: CColBase(defaultColor)
	, mStartPos(start)
	, mEndPos(end)
	, mRadius(radius)
{
	mpCollider = new CColliderCapsule
	(
		this, ELayer::eTest,
		mStartPos, mEndPos, mRadius,
		isKinematic, weight
	);
	mpCollider->SetCollisionLayers({ ELayer::eTest });
}

// デストラクタ
CColCapsule::~CColCapsule()
{
	SAFE_DELETE(mpCollider);
}

// 衝突処理
void CColCapsule::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CColBase::Collision(self, other, hit);

	Position(Position() + hit.adjust * hit.weight);
}

// 描画
void CColCapsule::Render()
{
	CMatrix m = Matrix();
	CVector s = mStartPos * m;
	CVector e = mEndPos * m;
	Primitive::DrawWireCapsule
	(
		s, e, mRadius,
		GetColor()
	);
}
