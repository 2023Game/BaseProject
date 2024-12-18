#pragma once
#include "CObjectBase.h"

// 武器のベースクラス
class CWeapon : public CObjectBase
{
public:
	// コンストラクタ
	CWeapon(ETag tag);
	// デストラクタ
	virtual ~CWeapon();

	// 武器の持ち主を設定
	void SetOwner(CObjectBase* owner);
	// 武器の持ち主を取得
	CObjectBase* GetOwner() const;

	// 武器をアタッチする行列を設定
	void SetAttachMtx(const CMatrix* mtx);

	// 行列を取得
	CMatrix Matrix() const override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// コライダーを取得
	CCollider* Collider() const;

	// 行列を更新
	void UpdateMtx();

protected:
	CObjectBase* mpOwner;		// 武器の持ち主
	CCollider* mpCollider;		// 武器のコライダー
	const CMatrix* mpAttachMtx;	// くっつける行列のポインター
	CMatrix mAttachMtx;			// くっつける行列の本体
};
