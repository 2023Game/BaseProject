#include "CColBox.h"
#include "Primitive.h"

// コンストラクタ
CColBox::CColBox(const CVector& halfSize,
	const CColor& defaultColor,
	bool isKinematic, float weight)
	: CColBase(defaultColor)
	, mHalfSize(halfSize)
{
	mpCollider = new CColliderBox
	(
		this, ELayer::eTest,
		mHalfSize,
		isKinematic, weight
	);
	mpCollider->SetCollisionLayers({ ELayer::eTest });
}

// デストラクタ
CColBox::~CColBox()
{
	SAFE_DELETE(mpCollider);
}

// 衝突処理
void CColBox::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
void CColBox::Render()
{
	CMatrix m = mpCollider->Matrix();

	CMatrix sm;
	sm.Scale(mHalfSize * 2.0f);

	Primitive::DrawWireBox(sm * m, GetColor());

	CMatrix ctm;
	ctm.Translate(Position());
	Primitive::DrawSphere(ctm, 0.2f, GetColor());
}
