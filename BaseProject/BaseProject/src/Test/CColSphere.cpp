#include "CColSphere.h"
#include "Primitive.h"

// コンストラクタ
CColSphere::CColSphere(float rad, const CColor& defaultColor,
	bool isKinematic, float weight)
	: CColBase(defaultColor)
	, mRadius(rad)
{
	mpCollider = new CColliderSphere
	(
		this, ELayer::eTest,
		mRadius, isKinematic, weight
	);
	mpCollider->SetCollisionLayers({ ELayer::eTest });
}

// デストラクタ
CColSphere::~CColSphere()
{
	SAFE_DELETE(mpCollider);
}

// 衝突処理
void CColSphere::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CColBase::Collision(self, other, hit);

	//if (mIsSelect)
	{
		//CColliderSphere* cs = dynamic_cast<CColliderSphere*>(other);
		//if (cs != nullptr)
		{
			Position(Position() + hit.adjust * hit.weight);
		}
	}
}

// 描画
void CColSphere::Render()
{
	Primitive::DrawWireSphere
	(
		Matrix(),
		mRadius,
		GetColor()
	);
}
