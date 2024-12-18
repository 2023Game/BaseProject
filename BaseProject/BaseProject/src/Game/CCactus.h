#pragma once
#include "CEnemy.h"

// サボテンの敵クラス
class CCactus : public CEnemy
{
public:
	// コンストラクタ
	CCactus();
	// デストラクタ
	~CCactus();

	// 攻撃中か
	bool IsAttacking() const override;
	// 攻撃開始
	void AttackStart() override;
	// 攻撃終了
	void AttackEnd() override;

	// ダメージを受ける
	void TakeDamage(int damage, CObjectBase* causer) override;
	// 死亡
	void Death() override;
	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 更新
	void Update() override;

private:
	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		// Tポーズ
		eIdle,		// 待機
		eIdleBattle,// 戦闘中の待機
		eWalk,		// 歩行
		eAttack1,	// パンチ攻撃
		eAttack2,	// 針攻撃
		eHit,		// 仰け反り
		eDeath,		// 死亡

		Num
	};

	// プレイヤーの状態
	enum class EState
	{
		eIdle,		// 待機状態
		eChase,		// 追いかける
		eAttack1,	// パンチ攻撃
		eAttack2,	// 針攻撃
		eHit,		// 仰け反り
		eDeath,		// 死亡
	};

	// 戦闘相手の方へ向く
	void LookAtBattleTarget(bool immediate = false);

	// 針を発射
	void ShotNeedle();

	// 状態切り替え
	void ChangeState(int state) override;

	// 待機状態の更新処理
	void UpdateIdle();
	// 追いかける時の更新処理
	void UpdateChase();
	// パンチ攻撃時の更新処理
	void UpdateAttack1();
	// 針攻撃時の更新処理
	void UpdateAttack2();
	// 仰け反り状態の更新処理
	void UpdateHit();
	// 死亡状態の更新処理
	void UpdateDeath();

	bool mIsBattle;					// 戦闘状態か
	float mBattleIdletime;			// 戦闘時の待機時間
	CObjectBase* mpBattleTarget;	// 戦闘相手
	CCollider* mpAttack1Col;		// パンチ攻撃用のコライダー
	int mShotNeedleCount;			// 針を発射した回数
};
