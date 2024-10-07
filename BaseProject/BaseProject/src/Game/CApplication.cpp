#include "CApplication.h"
//OpenGL
#include "glut.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CSoundManager.h"
#include "CBGMManager.h"
#include "CSceneManager.h"
#include "CGamePause.h"
#include "CFade.h"

CApplication::~CApplication()
{
}

void CApplication::Start()
{
#if _DEBUG
	// デバッグカメラを作成
	CDebugCamera::DebugCamera();
#endif

	// フェードクラスを作成
	CFade::Instance();
	// ゲームポーズクラスを作成
	new CGamePause();
	// サウンド管理クラスを作成
	CSoundManager::Instance();
	// BGM管理クラスを作成
	CBGMManager::Instance();

#if _DEBUG
	// デバッグモードでは、ブートメニューを最初に開く
	CSceneManager::Instance()->LoadScene(EScene::eBootMenu);
#else
	// リリースモードでは、タイトル画面を最初に開く
	CSceneManager::Instance()->LoadScene(EScene::eTitle);
#endif
}

void CApplication::End()
{
	CSceneManager::ClearInstance();
	CTaskManager::ClearInstance();
	CCollisionManager::ClearInstance();
	CResourceManager::ClearInstance();
	CBGMManager::ClearInstance();
	CSoundManager::ClearInstance();
}

void CApplication::Update()
{
	CTaskManager::Instance()->Delete();
	// タスクの更新処理
	CTaskManager::Instance()->Update();
	// 衝突処理
	CCollisionManager::Instance()->CollisionAll();

	// サウンドの更新
	CSoundManager::Instance()->Update();

	// シーンの更新
	CSceneManager::Instance()->Update();

	// タスクの描画処理
	CTaskManager::Instance()->Render();
	// コライダの描画
	CCollisionManager::Instance()->Render();
}
