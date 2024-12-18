#include "CWeapon.h"

// コンストラクタ
CWeapon::CWeapon(ETag tag)
	: CObjectBase(tag, ETaskPriority::eWeapon, 0, ETaskPauseType::eGame)
	, mpOwner(nullptr)
	, mpAttachMtx(nullptr)
	, mpCollider(nullptr)
{
	// 最初はコライダーオフ
	SetEnableCol(false);
}

// デストラクタ
CWeapon::~CWeapon()
{
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
		mpOwner = nullptr;
	}

	// コライダーを削除
	SAFE_DELETE(mpCollider);
}

// 武器の持ち主を設定
void CWeapon::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// 武器の持ち主を取得
CObjectBase* CWeapon::GetOwner() const
{
	return mpOwner;
}

// 武器をアタッチする行列を設定
void CWeapon::SetAttachMtx(const CMatrix* mtx)
{
	mpAttachMtx = mtx;
}

// 行列を取得
CMatrix CWeapon::Matrix() const
{
	CMatrix m = CTransform::Matrix();
	// くっつける行列が設定されていれば
	if (mpAttachMtx != nullptr)
	{
		// その行列にくっつける
		m = m * mAttachMtx;
	}
	// 持ち主が設定されていれば
	else if (mpOwner != nullptr)
	{
		// 持ち主の行列に附属
		m = m * mpOwner->Matrix();
	}
	return m;
}

// 衝突処理
void CWeapon::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (mpOwner != nullptr)
	{
		mpOwner->Collision(self, other, hit);
	}
}

// コライダーを取得
CCollider* CWeapon::Collider() const
{
	return mpCollider;
}

// 行列を更新
void CWeapon::UpdateMtx()
{
	// くっつける行列が設定されていれば、
	if (mpAttachMtx != nullptr)
	{
		// くっつける行列をメンバ変数にコピー
		mAttachMtx = *mpAttachMtx;
	}
}
