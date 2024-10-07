#include "CSlash.h"

// コンストラクタ
CSlash::CSlash(CObjectBase* owner, const CVector& pos, const CVector& dir,
	float speed, float dist)
	: CObjectBase(ETag::eSlash, ETaskPriority::eEffect, 0, ETaskPauseType::eGame)
	, mpOwner(owner)
	, mKillMoveDist(dist)
	, mMovedDist(0.0f)

{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
	Rotation(CQuaternion::LookRotation(mMoveSpeed, CVector::up));

	mpModel = CResourceManager::Get<CModel>("Slash");
	mpModel->SetupEffectSettings();
}

// デストラクタ
CSlash::~CSlash()
{
}

// 更新
void CSlash::Update()
{
	CVector move = mMoveSpeed * Time::DeltaTime();
	float dist = move.Length();
	if (mMovedDist + dist >= mKillMoveDist)
	{
		dist = mKillMoveDist - mMovedDist;
		move = move.Normalized() * dist;
	}
	Position(Position() + move);

	mMovedDist += dist;
	if (mMovedDist >= mKillMoveDist)
	{
		Kill();
	}
}

// 描画
void CSlash::Render()
{
	mpModel->SetColor(mColor);
	mpModel->Render(Matrix());
}