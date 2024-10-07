#pragma once
#include "CTask.h"
#include <list>

/// <summary>
/// タスク管理クラス
/// </summary>
class CTaskManager
{
public:
	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static CTaskManager* Instance();
	// インスタンスを破棄
	static void ClearInstance();

	/// <summary>
	/// 指定したタスクをリストに追加
	/// </summary>
	/// <param name="task">追加するタスク</param>
	/// <param name="isSort">ソートする時の追加処理かどうか</param>
	void Add(CTask* task, bool isSort = false);
	/// <summary>
	/// 指定したタスクをリストから取り除く
	/// </summary>
	/// <param name="task">取り除くタスク</param>
	/// <param name="isSort">ソートする時の取り除き処理かどうか</param>
	void Remove(CTask* task, bool isSort = false);

	// 削除フラグが立っているタスクを全て削除
	void Delete();
	/// <summary>
	/// 指定したシーンに所属するタスクを全て削除
	/// </summary>
	/// <param name="scene">削除するシーンの種類</param>
	void DeleteInScene(EScene scene);

	// 3Dタスクのリストを取得
	const std::list<CTask*>& Get3dTasks() const;
	// 2Dタスクのリストを取得
	const std::list<CTask*>& Get2dTasks() const;

	/// <summary>
	/// ポーズする
	/// </summary>
	/// <param name="pauseBit"></param>
	void Pause(int pauseBit);
	/// <summary>
	/// ポーズを解除する
	/// </summary>
	/// <param name="pauseBit"></param>
	void UnPause(int pauseBit);
	/// <summary>
	/// ポーズ中かどうか
	/// </summary>
	/// <param name="pauseBit"></param>
	/// <returns></returns>
	bool IsPaused(int pauseBit = 0) const;

	// 更新
	void Update();
	// 描画
	void Render();

private:
	// コンストラクタ
	CTaskManager();
	// デストラクタ
	~CTaskManager();

	// 指定されたリスト内で
	// 削除フラグが立っているタスクを全て削除
	void Delete(std::list<CTask*>& list);

	// タスクの並び替え
	void Sort();

	// 更新するかどうか
	bool IsUpdate(CTask* task) const;

	// 3Dオブジェクトの描画処理
	void Render3d();
	// 2Dオブジェクトの描画処理
	void Render2d();

	// モーションブラー有効時の3Dオブジェクトの描画処理
	void RenderMotionBlur();

	// タスクマネージャのインスタンス
	static CTaskManager* mpInstance;

	// 3Dタスクのリスト
	std::list<CTask*> m3dTasks;
	// 2Dタスクのリスト
	std::list<CTask*> m2dTasks;

	int mPauseBit;	// ポーズのビットフラグ
};
