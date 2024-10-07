#pragma once
#include "CColBase.h"
#include "CColliderLine.h"

// 線分コライダー表示用のテストクラス
class CColLine : public CColBase
{
public:
	// コンストラクタ
	CColLine(const CVector& start, const CVector& end, const CColor& defaultColor,
		bool isKinematic = false, float weight = 1.0f);
	// デストラクタ
	~CColLine();

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
	// 線分コライダー
	CColliderLine* mpCollider;
	CVector mStartPos;	// 線分の始点
	CVector mEndPos;	// 線分の終点
};
