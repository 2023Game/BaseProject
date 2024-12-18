#pragma once
#include "CObjectBase.h"
#include "CModel.h"

// サボテンの針のクラス
class CCactusNeedle : public CObjectBase
{
public:
	// コンストラクタ
	CCactusNeedle(float speed, float dist);
	// デストラクタ
	~CCactusNeedle();

	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	CModel* mpModel;		// 針のモデルデータ
	CCollider* mpCollider;	// 針のコライダー
	float mMoveSpeed;		// 移動速度
	float mMoveDist;		// 移動できる距離
	float mCurrDist;		// 現在の移動距離
};
