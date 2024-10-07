#ifndef CENEMY_H
#define CENEMY_H
#include "CCharaBase.h"
#include "CCollider.h"
#include "CModel.h"

/*
エネミークラス
キャラクタクラスを継承
*/
class CEnemy : public CCharaBase
{
public:
	// コンストラクタ
	CEnemy();
	// 更新処理
	void Update();
private:
};

#endif
