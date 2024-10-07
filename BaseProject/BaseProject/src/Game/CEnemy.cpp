#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"

// コンストラクタ
CEnemy::CEnemy()
	: CCharaBase(ETag::eEnemy, ETaskPriority::eDefault)
{
}

//更新処理
void CEnemy::Update()
{
}
