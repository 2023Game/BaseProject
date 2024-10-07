#include "CSceneManager.h"
#include "CFade.h"
#include "CTitleScene.h"
#include "CGameScene.h"

#include "Test/CBootMenuScene.h"
#include "Test/CCollisionTestScene.h"

// シーンマネージャのインスタンス
CSceneManager* CSceneManager::mpInstance = nullptr;

// インスタンスを取得
CSceneManager* CSceneManager::Instance()
{
	// インスタンスが存在しない場合は新しく生成する
	if (mpInstance == nullptr)
	{
		mpInstance = new CSceneManager();
	}
	return mpInstance;
}

// インスタンスを破棄
void CSceneManager::ClearInstance()
{
	delete mpInstance;
	mpInstance = nullptr;
}

// コンストラクタ
CSceneManager::CSceneManager()
	: mpScene(nullptr)
	, mNextScene(EScene::eNone)
	, mState(ESceneState::eIdle)
	, mStateStep(0)
{
}

// デストラクタ
CSceneManager::~CSceneManager()
{
	// 読み込んでいるシーンがあれば、破棄する
	UnloadScene();
}

// シーンの準備が完了したかどうか
bool CSceneManager::IsReady() const
{
	// 待機状態であれば、準備完了している
	return mState == ESceneState::eIdle;
}

// 現在の状態を取得
ESceneState CSceneManager::GetState() const
{
	return mState;
}

// 現在の状態を切り替え
void CSceneManager::ChangeState(ESceneState state)
{
	mState = state;
	mStateStep = 0;
}

// シーン読み込み
void CSceneManager::LoadScene(EScene scene)
{
	// 既にシーンが読み込まれている場合は、
	// 待機状態でなければ次のシーンを読み込めない
	if (mpScene != nullptr && mState != ESceneState::eIdle) return;

	// 次のシーンを設定
	mNextScene = scene;

	// 既にシーンが読み込まれている場合は、
	// フェードアウト状態へ移行
	if (mpScene != nullptr)
	{
		ChangeState(ESceneState::eFadeOut);
	}
	// 読み込まれていない場合は、そのままシーン読み込み状態へ移行
	else
	{
		ChangeState(ESceneState::eLoading);
	}
}

// 読み込んでいるシーンを破棄
void CSceneManager::UnloadScene()
{
	delete mpScene;
	mpScene = nullptr;
}

// 次のシーンに切り替え
void CSceneManager::ChangeNextScene()
{
	//既にシーンが読み込まれていたら、
	//そのシーンを破棄
	UnloadScene();

	//読み込むシーンの種類に合わせて
	//生成するシーンのクラスを変更する
	switch (mNextScene)
	{
		//タイトルシーン
		case EScene::eTitle:	mpScene = new CTitleScene();	break;
		//ゲームシーン
		case EScene::eGame:		mpScene = new CGameScene();		break;

		//起動時メニュー
		case EScene::eBootMenu:	mpScene = new CBootMenuScene();	break;
		//衝突判定テスト
		case EScene::eColTest:	mpScene = new CCollisionTestScene(); break;
	}

	//シーンを新しく生成できたら、
	//シーン読み込み開始
	if (mpScene != nullptr)
	{
		mpScene->Load();
	}

	// 次に読み込むシーンを初期化
	mNextScene = EScene::eNone;
}

// 現在読み込んでいるシーンの種類を取得
EScene CSceneManager::GetCurrentScene() const
{
	if (mpScene == nullptr) return EScene::eNone;
	return mpScene->GetSceneType();
}

// シーン読み込み状態での更新処理
void CSceneManager::UpdateLoading()
{
	// ステップごとに処理を切り替える
	switch (mStateStep)
	{
		// ステップ0：次のシーンを読み込み
		case 0:
			ChangeNextScene();
			mStateStep++;
			break;
		// ステップ1：読み込み終了待ち
		case 1:
			// 非同期読み込み未対応なため、
			// 即フェードイン状態へ移行
			ChangeState(ESceneState::eFadeIn);
			break;
	}
}

// フェードイン状態での更新処理
void CSceneManager::UpdateFadeIn()
{
	// ステップごとに処理を切り替える
	switch (mStateStep)
	{
		// ステップ0：フェードイン開始
		case 0:
			CFade::FadeIn();
			mStateStep++;
			break;
		// ステップ1：フェードイン終了待ち
		case 1:
			// フェードインが終了したら、待機状態へ移行
			if (!CFade::IsFading())
			{
				ChangeState(ESceneState::eIdle);
			}
			break;
	}
}

// 待機状態での更新処理
void CSceneManager::UpdateIdle()
{
	//シーンが読み込まれていない場合は、処理しない
	if (mpScene == nullptr) return;
	//シーン更新
	mpScene->Update();
}

// フェードアウト状態での更新処理
void CSceneManager::UpdateFadeOut()
{
	// ステップごとに処理を切り替える
	switch (mStateStep)
	{
		// ステップ0：フェードアウト開始
		case 0:
			CFade::FadeOut();
			mStateStep++;
			break;
		// ステップ1：フェードアウト終了待ち
		case 1:
			// フェードアウトが終了したら、シーン読み込み状態へ移行
			if (!CFade::IsFading())
			{
				ChangeState(ESceneState::eLoading);
			}
			break;
	}
}

// 更新
void CSceneManager::Update()
{
	// 現在の状態に合わせて更新処理を切り替える
	switch (mState)
	{
		// シーン読み込み状態
		case ESceneState::eLoading:
			UpdateLoading();
			break;
		// フェードイン状態
		case ESceneState::eFadeIn:
			UpdateFadeIn();
			break;
		// 待機状態
		case ESceneState::eIdle:
			UpdateIdle();
			break;
		// フェードアウト状態
		case ESceneState::eFadeOut:
			UpdateFadeOut();
			break;
	}
}
