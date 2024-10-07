#pragma once
#include "CSceneBase.h"
#include "SceneType.h"

// シーンの状態
enum class ESceneState
{
	eLoading,	// シーン読み込み中
	eFadeIn,	// シーン開始時のフェードイン中
	eIdle,		// 待機中
	eFadeOut,	// シーン終了時のフェードアウト中
};

// シーン管理クラス
class CSceneManager
{
public:
	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static CSceneManager* Instance();
	// インスタンスを破棄
	static void ClearInstance();

	// シーンの準備が完了したかどうか
	bool IsReady() const;

	// 現在の状態を取得
	ESceneState GetState() const;

	// シーン読み込み
	void LoadScene(EScene scene);
	// 読み込んでいるシーンを破棄
	void UnloadScene();

	/// <summary>
	/// 現在読み込んでいるシーンの種類を取得
	/// </summary>
	/// <returns>シーンの種類（eNoneの場合はシーンが読み込まれていない）</returns>
	EScene GetCurrentScene() const;

	// 更新
	void Update();

private:
	// コンストラクタ
	CSceneManager();
	// デストラクタ
	~CSceneManager();

	// 現在の状態を切り替え
	void ChangeState(ESceneState state);

	// 次のシーンに切り替え
	void ChangeNextScene();

	// シーン読み込み状態での更新処理
	void UpdateLoading();
	// フェードイン状態での更新処理
	void UpdateFadeIn();
	// 待機状態での更新処理
	void UpdateIdle();
	// フェードアウト状態での更新処理
	void UpdateFadeOut();

	// シーンマネージャのインスタンス
	static CSceneManager* mpInstance;
	// 現在読み込んでいるシーン
	CSceneBase* mpScene;
	// 次に読み込むシーン
	EScene mNextScene;
	// シーンの現在の状態
	ESceneState mState;
	// 状態内でのステップ
	int mStateStep;
};