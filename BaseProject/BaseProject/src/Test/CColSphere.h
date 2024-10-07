#pragma once
#include "CColBase.h"
#include "CColliderSphere.h"

// 球コライダー表示用のテストクラス
class CColSphere : public CColBase
{
public:
	// コンストラクタ
	CColSphere(float rad, const CColor& defaultColor,
		bool isKinematic = false, float weight = 1.0f);
	// デストラクタ
	~CColSphere();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 描画
	void Render() override;

private:
	// 球コライダー
	CColliderSphere* mpCollider;
	float mRadius;			// 球の半径
};
