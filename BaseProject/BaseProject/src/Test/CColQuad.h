#pragma once
#include "CColBase.h"
#include "CColliderTriangle.h"

// 板ポリゴン表示用のテストクラス
class CColQuad : public CColBase
{
public:
	// コンストラクタ
	CColQuad(const CVector2& size, const CColor& defaultColor,
		bool isKinematic = false, float weight = 1.0f);
	// デストラクタ
	~CColQuad();

	// 描画
	void Render() override;

private:
	// 三角形コライダー0
	CColliderTriangle* mpCollider0;
	// 三角形コライダー1
	CColliderTriangle* mpCollider1;
	// 板ポリゴンのサイズ
	CVector2 mSize;
	// 板ポリゴンの4頂点
	CVector mVertices[4];
};
