#pragma once
#include "CColBase.h"
#include "CColliderCapsule.h"

// カプセルコライダー表示用のテストクラス
class CColCapsule : public CColBase
{
public:
	// コンストラクタ
	CColCapsule(const CVector& start, const CVector& end, float radius,
		const CColor& defaultColor, bool isKinematic = false, float weight = 1.0f);
	// デストラクタ
	~CColCapsule();

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
	// カプセルコライダー
	CColliderCapsule* mpCollider;
	CVector mStartPos;	// カプセルを構成する線分の始点
	CVector mEndPos;	// カプセルを構成する線分の終点
	float mRadius;		// カプセルの半径
};
