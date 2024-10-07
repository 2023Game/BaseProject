#include "CColQuad.h"
#include "Primitive.h"

// コンストラクタ
CColQuad::CColQuad(const CVector2& size, const CColor& defaultColor,
	bool isKinematic, float weight)
	: CColBase(defaultColor)
	, mSize(size)
{
	CVector2 half = size * 0.5f;
	mVertices[0] = CVector(-half.X(), 0.0f, -half.Y());
	mVertices[1] = CVector(-half.X(), 0.0f,  half.Y());
	mVertices[2] = CVector( half.X(), 0.0f,  half.Y());
	mVertices[3] = CVector( half.X(), 0.0f, -half.Y());

	mpCollider0 = new CColliderTriangle
	(
		this, ELayer::eTest,
		mVertices[0], mVertices[1], mVertices[2],
		isKinematic, weight
	);
	mpCollider1 = new CColliderTriangle
	(
		this, ELayer::eTest,
		mVertices[2], mVertices[3], mVertices[0],
		isKinematic, weight
	);
	mpCollider0->SetCollisionLayers({ ELayer::eTest });
	mpCollider1->SetCollisionLayers({ ELayer::eTest });
}

// デストラクタ
CColQuad::~CColQuad()
{
	SAFE_DELETE(mpCollider0);
	SAFE_DELETE(mpCollider1);
}

// 描画
void CColQuad::Render()
{
	Primitive::DrawQuad
	(
		Matrix(),
		mSize,
		GetColor()
	);
}
