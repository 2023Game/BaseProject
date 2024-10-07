#pragma once
#include "SceneType.h"
class CTask;

//シーンのベースクラス
class CSceneBase
{
public:
	//コンストラクタ
	CSceneBase(EScene scene);
	//デストラクタ
	virtual ~CSceneBase();

	//シーン読み込み処理(継承先で実装)
	virtual void Load() = 0;
	//シーン更新処理(継承先で実装)
	virtual void Update() = 0;
	//シーンの種類を取得
	EScene GetSceneType() const;
	//シーンに所属するタスクを追加
	void AddTask(CTask* task);

private:
	EScene mSceneType;	//シーンの種類
};
