#pragma once
#include "CCollider.h"

/// <summary>
/// ボックスコライダー
/// </summary>
class CColliderBox : public CCollider
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">コライダーの持ち主</param>
	/// <param name="layer">衝突判定用レイヤー</param>
	/// <param name="halfSize">各軸の半分のサイズ</param>
	/// <param name="isKinematic">trueならば、衝突時に押し戻しの影響を受けない</param>
	/// <param name="weight">コライダーの重量</param>
	CColliderBox(CObjectBase* owner, ELayer layer, const CVector& halfSize,
		bool isKinematic = false, float weight = 1.0f);

	/// <summary>
	/// ボックスコライダーの設定
	/// </summary>
	/// <param name="owner">コライダーの持ち主</param>
	/// <param name="layer">衝突判定用レイヤー</param>
	/// <param name="halfSize">各軸の半分のサイズ</param>
	void Set(CObjectBase* owner, ELayer layer, const CVector& halfSize);

	/// <summary>
	/// ボックスの値を取得
	/// </summary>
	void Get(CVector* outPos, CVector* outAxisX, CVector* outAxisY, CVector* outAxisZ, CVector* outHalfSize) const;

#if _DEBUG
	// コライダー描画
	void Render() override;
#endif

protected:
	// コライダーの情報を更新
	void UpdateCol(bool isInit = false) override;

private:
	CVector mHalfSize;		// 各軸の半分のサイズ

	CVector mWorldPos;		// ワールド空間での座標
	CVector mWorldAxis[3];	// ワールド空間での各軸
	CVector mWorldHalfSize;	// ワールド空間での半分のサイズ
};