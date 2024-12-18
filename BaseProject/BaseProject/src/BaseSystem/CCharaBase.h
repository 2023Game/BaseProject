#pragma once
#include "CObjectBase.h"

/// <summary>
/// キャラクターのベースクラス
/// </summary>
class CCharaBase : public CObjectBase
{
public:
	// コンストラクタ
	CCharaBase(ETag tag, ETaskPriority prio, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame);
	// デストラクタ
	virtual ~CCharaBase();

	// 最大HPを取得
	int GetMaxHp() const;
	// 現在HPを取得
	int GetHp() const;

	// ダメージを受ける
	virtual void TakeDamage(int damage, CObjectBase* causer);

	// 死亡
	virtual void Death();
	// 死んでいるかどうか
	bool IsDeath() const;

protected:
	int mMaxHp;	// 最大HP
	int mHp;	// 現在HP
};
