#include "CColTriangle.h"
#include "Primitive.h"

// コンストラクタ
CColTriangle::CColTriangle(const CVector& v0, const CVector& v1, const CVector& v2, const CColor& defaultColor,
	bool isKinematic, float weight)
	: CColBase(defaultColor)
{
	mVertices[0] = v0;
	mVertices[1] = v1;
	mVertices[2] = v2;

	mpCollider = new CColliderTriangle
	(
		this, ELayer::eTest,
		mVertices[0], mVertices[1], mVertices[2],
		isKinematic, weight
	);
	mpCollider->SetCollisionLayers({ ELayer::eTest });
}

// デストラクタ
CColTriangle::~CColTriangle()
{
	SAFE_DELETE(mpCollider);
}

// 描画
void CColTriangle::Render()
{
	Primitive::DrawTriangle
	(
		mVertices[0],
		mVertices[1],
		mVertices[2],
		GetColor()
	);
}
