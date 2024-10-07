#pragma once
#include "CSceneBase.h"
class CColBase;
class CColSphere;
class CColLine;
class CColTriangle;
class CColQuad;

// 衝突判定テストシーン
class CCollisionTestScene : public CSceneBase
{
public:
	// コンストラクタ
	CCollisionTestScene();
	// デストラクタ
	~CCollisionTestScene();

	// シーン読み込み
	void Load() override;

	// コライダーを選択
	void Select(CColBase* col);

	// 更新
	void Update() override;

private:
	CColBase* mpSelectCol;			// 選択しているコライダー
	std::vector<CColBase*> mColList;// 生成済みのコライダーリスト
};