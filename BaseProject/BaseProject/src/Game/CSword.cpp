#include "CSword.h"
#include "CColliderCapsule.h"

#define SWORD_LENGTH 100.0f
#define SWORD_RADIUS 1.0f

// コンストラクタ
CSword::CSword(CObjectBase* owner, ETag tag,
	Tags hitTags, Layers hitLayers)
	: CWeapon(tag)
	, mpModel(nullptr)
{
	mpOwner = owner;

	// モデルデータ取得
	mpModel = CResourceManager::Get<CModel>("Sword");

	// 剣のコライダーを作成
	mpCollider = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(SWORD_LENGTH - SWORD_RADIUS, 0.0f, 0.0f),
		SWORD_RADIUS
	);
	// タグとレイヤーの衝突設定
	mpCollider->SetCollisionTags(hitTags);
	mpCollider->SetCollisionLayers(hitLayers);
}

// デストラクタ
CSword::~CSword()
{
}

// 描画
void CSword::Render()
{
	mpModel->Render(Matrix());
}
