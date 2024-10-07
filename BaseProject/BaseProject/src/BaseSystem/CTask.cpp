#include "CTask.h"
#include "CTaskManager.h"
#include "CSceneManager.h"

//コンストラクタ
CTask::CTask(ETaskPriority prio, int sortOrder,
	ETaskPauseType pause, bool dontDelete, bool addTaskList)
	: mPriority(prio)
	, mSortOrder(sortOrder)
	, mIsSort(false)
	, mIsKill(false)
	, mIsEnable(true)
	, mIsShow(true)
	, mPauseType(pause)
	, mAddTaskList(addTaskList)
{
	//シーン遷移で破棄しないタスクかどうかで
	//所属するシーンのタイプを切り替える
	mSceneType = dontDelete
		? EScene::eNone
		: CSceneManager::Instance()->GetCurrentScene();

	// 自動登録がオンならば
	if (mAddTaskList)
	{
		//タスクを生成した時に
		//タスクマネージャーのタスクリストに追加
		CTaskManager::Instance()->Add(this);
	}
}

//デストラクタ
CTask::~CTask()
{
	if (mAddTaskList)
	{
		//タスクマネージャーのタスクリストから取り除く
		CTaskManager::Instance()->Remove(this);
	}
}

//更新
void CTask::Update()
{
}

//描画
void CTask::Render()
{
}

//優先度を設定
void CTask::SetPriority(ETaskPriority prio)
{
	//既に設定されている優先度であれば処理しない
	if (mPriority == prio) return;

	//優先度を変更して、並び替えフラグを立てる
	mPriority = prio;
	mIsSort = true;
}

//優先度を取得
ETaskPriority CTask::GetPriority() const
{
	return mPriority;
}

//優先度内の順番を設定
void CTask::SetSortOrder(int sortOder)
{
	//既に設定されている順番であれば処理しない
	if (mSortOrder == sortOder) return;

	//順番を変更し、並び替えフラグを立てる
	mSortOrder = sortOder;
	mIsSort = true;
}

//優先度内の順番を取得
int CTask::GetSortOrder() const
{
	return mSortOrder;
}

//ポーズの種類を設定
void CTask::SetPauseType(ETaskPauseType type)
{
	mPauseType = type;
}

//ポーズの種類を取得
ETaskPauseType CTask::GetPauseType() const
{
	return mPauseType;
}

//所属するシーンを設定
void CTask::SetSceneType(EScene scene)
{
	mSceneType = scene;
}

//所属するシーンを取得
EScene CTask::GetSceneType() const
{
	return mSceneType;
}

//タスクを削除
void CTask::Kill()
{
	mIsKill = true;
}

//削除フラグ取得
bool CTask::IsKill() const
{
	return mIsKill;
}

//有効フラグを設定
void CTask::SetEnable(bool enable)
{
	mIsEnable = enable;
}

//有効フラグを取得
bool CTask::IsEnable() const
{
	return mIsEnable;
}

//表示フラグを設定
void CTask::SetShow(bool show)
{
	mIsShow = show;
}

//表示フラグを取得
bool CTask::IsShow() const
{
	// 表示フラグがtrueかつ、
	// 削除フラグが立っていない場合は、表示する
	return mIsShow && !mIsKill;
}
