#pragma once
#include "TaskPriority.h"
#include "TaskPauseType.h"
#include "SceneType.h"
class CTaskManager;

//タスククラス
class CTask
{
	friend CTaskManager;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="prio">タスク優先度</param>
	/// <param name="sortOrder">タスク優先度内の順番</param>
	/// <param name="pause">ポーズ種類</param>
	/// <param name="dontDelete">trueにするとシーン遷移で削除されない</param>
	/// <param name="addTaskList">falseにするとタスクリストに自動登録されない</param>
	CTask(ETaskPriority prio = ETaskPriority::eDefault, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eDefault,
		bool dontDelete = false, bool addTaskList = true);
	//デストラクタ
	virtual ~CTask();
	//更新
	virtual void Update();
	//描画
	virtual void Render();

	//優先度を設定
	void SetPriority(ETaskPriority prio);
	//優先度を取得
	ETaskPriority GetPriority() const;
	//優先度内の順番を設定
	void SetSortOrder(int sortOder);
	//優先度内の順番を取得
	int GetSortOrder() const;

	//ポーズの種類を設定
	void SetPauseType(ETaskPauseType type);
	//ポーズの種類を取得
	ETaskPauseType GetPauseType() const;

	//所属するシーンを設定
	void SetSceneType(EScene scene);
	//所属するシーンを取得
	EScene GetSceneType() const;

	//タスクを削除
	virtual void Kill();
	//削除フラグ取得
	bool IsKill() const;

	//有効フラグを設定
	//（オフにすると、更新、描画、衝突判定を停止）
	void SetEnable(bool enable);
	//有効フラグ取得
	bool IsEnable() const;

	//表示フラグを設定
	virtual void SetShow(bool show);
	//表示フラグを取得
	bool IsShow() const;

private:
	ETaskPriority mPriority;	//優先度
	int mSortOrder;		//優先度内での順番
	bool mIsSort;		//ソートの必要があるか
	bool mIsKill;		//削除フラグ
	bool mIsEnable;		//有効フラグ
	bool mIsShow;		//表示フラグ
	ETaskPauseType mPauseType;//ポーズの種類
	bool mAddTaskList;	//タスクリストに追加しているかどうか
	EScene mSceneType;	//所属するシーン種類
};
