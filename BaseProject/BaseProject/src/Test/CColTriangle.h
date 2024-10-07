#pragma once
#include "CColBase.h"
#include "CColliderTriangle.h"

class CColTriangle : public CColBase
{
public:
	// コンストラクタ
	CColTriangle(const CVector& v0, const CVector& v1, const CVector& v2, const CColor& defaultColor,
		bool isKinematic = false, float weight = 1.0f);
	// デストラクタ
	~CColTriangle();

	// 描画
	void Render() override;

private:
	// 三角形コライダー
	CColliderTriangle* mpCollider;
	CVector mVertices[3];	// 三角形を構成する3頂点
};