#pragma once
#include "CObjectBase.h"

// 測定関連の表示クラス
class CMeasure : public CObjectBase
{
public:
	// コンストラクタ
	CMeasure(int gridCount);
	// デストラクタ
	~CMeasure();

	// 表示するグリッド数を設定
	void SetGridCount(int count);

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	int mGridCount;	// 表示するグリッド数
};
