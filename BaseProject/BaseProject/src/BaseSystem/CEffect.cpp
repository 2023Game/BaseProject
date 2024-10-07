#include "CEffect.h"

// コンストラクタ
CEffect::CEffect(ETag tag)
	: CObjectBase(tag, ETaskPriority::eEffect, 0, ETaskPauseType::eGame)
{
}

// デストラクタ
CEffect::~CEffect()
{
}

// 更新
void CEffect::Update()
{
}

// 描画
void CEffect::Render()
{
}
