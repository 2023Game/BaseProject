#pragma once
#include "CCollider.h"

/// <summary>
/// カプセルコライダー
/// </summary>
class CColliderCapsule : public CCollider
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">コライダーの持ち主</param>
	/// <param name="layer">衝突判定用レイヤー</param>
	/// <param name="v0">カプセルを構成する線分の始点</param>
	/// <param name="v1">カプセルを構成する線分の終点</param>
	/// <param name="radius">カプセルの半径</param>
	/// <param name="isKinematic">trueならば、衝突時に押し戻しの影響を受けない</param>
	/// <param name="weight">コライダーの重量</param>
	CColliderCapsule(CObjectBase* owner, ELayer layer,
		const CVector& v0, const CVector& v1, float radius,
		bool isKinematic = false, float weight = 1.0f);

	/// <summary>
	/// カプセルコライダーの設定
	/// </summary>
	/// <param name="owner">コライダーの持ち主</param>
	/// <param name="layer">衝突判定用レイヤー</param>
	/// <param name="v0">カプセルを構成する線分の始点</param>
	/// <param name="v1">カプセルを構成する線分の終点</param>
	/// <param name="radius">カプセルの半径</param>
	void Set(CObjectBase* owner, ELayer layer,
		const CVector& v0, const CVector& v1, float radius);

	/// <summary>
	/// カプセルを構成する線分の視点と終点を取得
	/// </summary>
	/// <param name="v0">始点格納用</param>
	/// <param name="v1">終点格納用</param>
	void Get(CVector* v0, CVector* v1) const;

	// カプセルの半径を取得
	float Radius() const;

	// コライダー描画
	void Render() override;

protected:
	// コライダーの情報を更新
	void UpdateCol() override;

private:
	CVector mV[2];	// カプセルを構成する線分の頂点配列
	CVector mWV[2];	// カプセルを構成する線分の頂点のワールド座標
	float mRadius;	// カプセルの半径
};