#pragma once
#include "CObjectBase.h"

// エフェクトのベースクラス
class CEffect : public CObjectBase
{
public:
	// コンストラクタ
	CEffect(ETag tag);
	// デストラクタ
	virtual ~CEffect();

	// 更新
	void Update() override;
	// 描画
	void Render() override;
private:
};
