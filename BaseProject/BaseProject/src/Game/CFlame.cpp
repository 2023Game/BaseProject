#include "CFlame.h"
#include "Easing.h"

// アニメーションの1コマ表示時間
#define ANIM_TIME 0.0625f
// 炎のエフェクトのアニメーションデータ
TexAnimData CFlame::msAnimData = TexAnimData(8, 8, false, 64, ANIM_TIME);

// コンストラクタ
CFlame::CFlame(ETag tag)
	: CBillBoardImage("Effect/flame.png", tag, ETaskPauseType::eGame)
	, mMoveSpeed(CVector::zero)
	, mElapsedTime(0.0f)
	, mIsDeath(false)
	, mFlameScale(FLAME_SCALE)
	, mFlameScaleAnimTime(FLAME_SCALE_ANIM_TIME)
{
	SetAnimData(&msAnimData);

	mpCollider = new CColliderSphere
	(
		this,
		ELayer::eAttackCol,
		1.0f
	);
	mpCollider->SetCollisionTags({ ETag::eField, ETag::eRideableObject });
	mpCollider->SetCollisionLayers({ ELayer::eGround });
}

// デストラクタ
CFlame::~CFlame()
{
	SAFE_DELETE(mpCollider);
}

// 各パラメータを設定
void CFlame::Setup(const CVector& pos, const CVector& dir, float speed)
{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
}

// 削除フラグが立っているかどうか
bool CFlame::IsDeath() const
{
	return mIsDeath;
}

// カラーを設定
void CFlame::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// ブレンドタイプを設定
void CFlame::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// 炎のスケールの最大値を設定
void CFlame::SetFlameScale(float flameScale)
{
	mFlameScale = flameScale;
}

// 炎のスケール値が最大値になるまでの時間を設定
void CFlame::SetFlameScaleAnimTime(float flameScaleAnimTime)
{
	mFlameScaleAnimTime = flameScaleAnimTime;
}

// 衝突処理
void CFlame::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (other->Layer() == ELayer::eGround)
	{
		float length = mMoveSpeed.Length();
		CVector n = hit.adjust.Normalized();
		float d = CVector::Dot(n, mMoveSpeed);
		mMoveSpeed = (mMoveSpeed - n * d).Normalized() * length;
		Position(Position() + hit.adjust * hit.weight);
	}
}

// 更新
void CFlame::Update()
{
	// 基底クラスの更新処理
	CBillBoardImage::Update();

	// 炎のエフェクトを移動
	CVector move = mMoveSpeed * Times::DeltaTime();
	Position(Position() + move);

	// スケール変更時間を経過していない
	if (mElapsedTime < mFlameScaleAnimTime)
	{
		// 経過時間に合わせて、徐々に炎を大きくする
		float per = mElapsedTime / mFlameScaleAnimTime;
		if (per < 1.0f)
		{
			float scale = Easing::QuadOut(per, 1.0f, 0.0f, 1.0f);
			Scale(CVector::one * scale * mFlameScale);
		}
		else
		{
			Scale(CVector::one * mFlameScale);
		}

		mElapsedTime += Times::DeltaTime();
	}
	// 移動時間が経過したら、削除する
	else
	{
		Scale(CVector::one * mFlameScale);
	}

	// アニメーションが終わったら、削除フラグを立てる
	if (IsEndAnim())
	{
		mIsDeath = true;
	}
}
