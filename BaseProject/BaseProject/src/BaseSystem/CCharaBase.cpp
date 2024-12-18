#include "CCharaBase.h"

// コンストラクタ
CCharaBase::CCharaBase(ETag tag, ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(tag, prio, sortOrder, pause)
	, mMaxHp(10)
	, mHp(mMaxHp)
{
}

// デストラクタ
CCharaBase::~CCharaBase()
{
}

// 最大HPを取得
int CCharaBase::GetMaxHp() const
{
	return mMaxHp;
}

// 現在HPを取得
int CCharaBase::GetHp() const
{
	return mHp;
}

// ダメージを受ける
void CCharaBase::TakeDamage(int damage, CObjectBase* causer)
{
	// 既に死亡していたら、ダメージを受けない
	if (IsDeath()) return;

	// 受けたダメージが現在HP以上なら
	if (damage >= mHp)
	{
		// HPを0にして、死亡
		mHp = 0;
		Death();
	}
	// 現在HPの方が多い場合は、ダメージ分減らす
	else
	{
		mHp -= damage;
	}
}

// 死亡
void CCharaBase::Death()
{
}

// 死んでいるかどうか
bool CCharaBase::IsDeath() const
{
	// 現在HPが0ならば、死亡
	return mHp <= 0;
}
