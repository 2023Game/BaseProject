#pragma once
#include "CCollider.h"

/// <summary>
/// 球コライダー
/// </summary>
class CColliderSphere : public CCollider
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">コライダーの持ち主</param>
	/// <param name="layer">衝突判定用レイヤー</param>
	/// <param name="radius">球の半径</param>
	/// <param name="isKinematic">trueならば、衝突時に押し戻しの影響を受けない</param>
	/// <param name="weight">コライダーの重量</param>
	CColliderSphere(CObjectBase* owner, ELayer layer, float radius,
		bool isKinematic = false, float weight = 1.0f);

	/// <summary>
	/// 球コライダーの設定
	/// </summary>
	/// <param name="owner">コライダーの持ち主</param>
	/// <param name="layer">衝突判定用レイヤー</param>
	/// <param name="radius">球の半径</param>
	void Set(CObjectBase* owner, ELayer layer, float radius);

	/// <summary>
	/// 球の座標と半径を取得
	/// </summary>
	/// <param name="pos">球の座標</param>
	/// <param name="rad">球の半径</param>
	void Get(CVector* pos, float* rad) const;

	// コライダー描画
	void Render() override;

protected:
	// コライダーの情報を更新
	void UpdateCol() override;

private:
	float mRadius;	// 球の半径
	CVector mWPos;	// ワールド座標
	float mWRadius;	// ワールド半径
};
