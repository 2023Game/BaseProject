#include "CCactusNeedle.h"
#include "CColliderSphere.h"
#include "CCharaBase.h"

// コンストラクタ
CCactusNeedle::CCactusNeedle(float speed, float dist)
	: CObjectBase(ETag::eEnemy, ETaskPriority::eWeapon, 0, ETaskPauseType::eGame)
	, mpModel(nullptr)
	, mpCollider(nullptr)
	, mMoveSpeed(speed)
	, mMoveDist(dist)
	, mCurrDist(0.0f)
{
	// 針を黄色にする
	mColor = CColor::yellow;

	// モデルデータ取得
	mpModel = CResourceManager::Get<CModel>("CactusNeedle");

	// コライダーを作成
	mpCollider = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		3.0f
	);
	// プレイヤーとフィールドと衝突するように設定
	mpCollider->SetCollisionTags({ ETag::ePlayer/*, ETag::eField*/ });
	mpCollider->SetCollisionLayers({ ELayer::ePlayer, ELayer::eField });
}

// デストラクタ
CCactusNeedle::~CCactusNeedle()
{
	// コライダー削除
	SAFE_DELETE(mpCollider);
}

// 衝突処理
void CCactusNeedle::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// プレイヤーに衝突した
	if (other->Layer() == ELayer::ePlayer)
	{
		// プレイヤーにダメージを与える
		CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
		if (chara != nullptr)
		{
			chara->TakeDamage(1, this);
		}
	}

	// 何かにぶつかったら、自身を削除
	Kill();
}

// 更新
void CCactusNeedle::Update()
{
	// このフレームで移動距離を求める
	float moveDist = mMoveSpeed * Times::DeltaTime();

	// 移動距離分、移動させる
	CVector pos = Position();
	pos += VectorZ() * moveDist;
	Position(pos);

	// 移動した距離を加算
	mCurrDist += moveDist;
	// 移動出来る距離を超えたら、自身を削除
	if (mCurrDist >= mMoveDist)
	{
		Kill();
	}
}

// 描画
void CCactusNeedle::Render()
{
	mpModel->SetColor(mColor);
	mpModel->Render(Matrix());
}
