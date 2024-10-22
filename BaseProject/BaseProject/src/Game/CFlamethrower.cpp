#include "CFlamethrower.h"
#include "CFlame.h"
#include "CInput.h"
#include "Maths.h"

// 炎の発射間隔時間
#define THROW_INTERVAL 0.15f
// 炎の発射方向のブレ幅
#define FLAME_DIR_RAND 0.02f
// 炎の移動速度
#define FLAME_MOVE_SPEED 75.0f
// 炎の色
#define FLAME_COLOR CColor(1.0f, 0.25f, 0.1f)

// コンストラクタ
CFlamethrower::CFlamethrower(CObjectBase* owner, const CMatrix* attach,
	const CVector& offsetPos, const CMatrix& offsetRot)
	: mpOwner(owner)
	, mpAttachMtx(attach)
	, mThrowOffsetPos(offsetPos)
	, mThrowOffsetRot(offsetRot)
	, mElapsedTime(0.0f)
	, mIsThrowing(false)
{
}

// デストラクタ
CFlamethrower::~CFlamethrower()
{
	for (CFlame* flame : mFlames)
	{
		flame->Kill();
	}
}

// 炎を発射開始
void CFlamethrower::Start()
{
	mIsThrowing = true;
	mElapsedTime = 0.0f;
}

// 炎を発射停止
void CFlamethrower::Stop()
{
	mIsThrowing = false;
}

// 炎を発射しているかどうか
bool CFlamethrower::IsThrowing() const
{
	return mIsThrowing;
}

// 発射時のオフセット位置を設定
void CFlamethrower::SetThrowOffsetPos(const CVector& pos)
{
	mThrowOffsetPos = pos;
}

// 発射時のオフセット回転値を設定
void CFlamethrower::SetThrowOffsetRot(const CMatrix& rot)
{
	mThrowOffsetRot = rot;
}

// 炎の発射位置を取得
CVector CFlamethrower::GetThrowPos() const
{
	// アタッチする行列が設定されている場合は、行列の座標を返す
	if (mpAttachMtx != nullptr)
	{
		CVector pos = mpAttachMtx->Position();
		pos += mpAttachMtx->VectorX().Normalized() * mThrowOffsetPos.X()
			+ mpAttachMtx->VectorY().Normalized() * mThrowOffsetPos.Y()
			+ mpAttachMtx->VectorZ().Normalized() * mThrowOffsetPos.Z();
		return pos;
	}
	// 持ち主が設定されている場合は、持ち主の座標を返す
	else if (mpOwner != nullptr)
	{
		return mpOwner->Position() + (mpOwner->Rotation() * mThrowOffsetPos);
	}

	return CVector::zero;
}

// 炎の発射方向を取得
CVector CFlamethrower::GetThrowDir() const
{
	// アタッチする行列が設定されている場合は、行列の正面方向ベクトルを返す
	if (mpAttachMtx != nullptr)
	{
		return mThrowOffsetRot * mpAttachMtx->VectorZ();
	}
	// 持ち主が設定されている場合は、持ち主の正面方向ベクトルを返す
	else if (mpOwner != nullptr)
	{
		return mThrowOffsetRot * mpOwner->VectorZ();
	}

	return mThrowOffsetRot * CVector::forward;
}

// 炎のエフェクトを作成
void CFlamethrower::CreateFlame()
{
	// 炎のエフェクトを作成
	CFlame* flame = new CFlame(ETag::eFlame);

	// 発射位置を取得
	CVector pos = GetThrowPos();// + CVector(0.0f, 10.0f, 0.0f);
	// 発射方向を取得
	CVector dir = GetThrowDir();// + CVector(0.0f, -1.0f, 0.0f);
	// 発射方向をランダムでブラす
	dir.X(dir.X() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	dir.Y(dir.Y() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	dir.Z(dir.Z() + Math::Rand(-FLAME_DIR_RAND, FLAME_DIR_RAND));
	dir.Normalize();
	// 発射位置、方向、移動速度を設定
	flame->Setup(pos, dir, FLAME_MOVE_SPEED);

	// 炎のカラーを設定
	flame->SetColor(FLAME_COLOR);
	// 加算ブレンドにして、炎が発光しているように見せる
	flame->SetBlendType(EBlend::eAdd);

	// 作成した炎のエフェクトをリストに追加
	mFlames.push_back(flame);
}

// 更新
void CFlamethrower::Update()
{
	// 炎を発射していたら
	if (mIsThrowing)
	{
		// 経過時間に応じて、炎のエフェクトを作成
		if (mElapsedTime >= THROW_INTERVAL)
		{
			CreateFlame();
			mElapsedTime -= THROW_INTERVAL;
		}
		mElapsedTime += Times::DeltaTime();
	}

	// 生成済みの炎のエフェクトの削除処理
	auto itr = mFlames.begin();
	auto end = mFlames.end();
	while (itr != end)
	{
		CFlame* flame = *itr;
		// 削除フラグが立っていたら、削除
		if (flame->IsDeath())
		{
			itr = mFlames.erase(itr);
			flame->Kill();
		}
		else
		{
			itr++;
		}
	}
}
