#include "CBootMenuScene.h"
#include "CText.h"
#include "CInput.h"
#include "CSceneManager.h"

// コンストラクタ
CBootMenuScene::CBootMenuScene()
	: CSceneBase(EScene::eBootMenu)
	, mSelectIndex(0)
{
	// メニュー情報リストを作成
	mMenuInfoList =
	{
		{ "■メイン", EScene::eNone },
		{ "タイトル画面", EScene::eTitle },
		{ "ゲーム画面", EScene::eGame },
		{ "", EScene::eNone },
		{ "■デバッグ", EScene::eNone },
		{ "衝突判定テスト", EScene::eColTest },
	};

	// 一番先頭の項目が選択できない項目であれば、次の項目を初期選択に変更
	if (mMenuInfoList[mSelectIndex].scene == EScene::eNone)
	{
		SelectNextMenu();
	}
}

// デストラクタ
CBootMenuScene::~CBootMenuScene()
{
}

// シーン読み込み
void CBootMenuScene::Load()
{
	// 起動メニューは背景黒色
	System::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// メニュー表示用のテキストを追加
	mpText = new CText
	(
		nullptr, 24,
		CVector2(100.0f, 100.0f),
		CVector2(WINDOW_WIDTH, WINDOW_HEIGHT),
		CColor::white
	);
}

// 次の項目を選択
void CBootMenuScene::SelectNextMenu()
{
	// 選択可能な項目が来るまで、次へ進める
	// （eNoneの項目は選択不可）
	while (true)
	{
		mSelectIndex = (mSelectIndex + 1) % mMenuInfoList.size();
		if (mMenuInfoList[mSelectIndex].scene != EScene::eNone)
		{
			break;
		}
	}
}

// 前の項目を選択
void CBootMenuScene::SelectPrevMenu()
{
	// 選択可能な項目が来るまで、前に戻る
	// （eNoneの項目は選択不可）
	while (true)
	{
		mSelectIndex = (mSelectIndex - 1 + mMenuInfoList.size()) % mMenuInfoList.size();
		if (mMenuInfoList[mSelectIndex].scene != EScene::eNone)
		{
			break;
		}
	}
}

// 更新
void CBootMenuScene::Update()
{
	// 「↑」キーで前の項目に戻る
	if (CInput::PushKey(VK_UP))
	{
		SelectPrevMenu();
	}
	// 「↓」キーで次の項目へ進める
	else if (CInput::PushKey(VK_DOWN))
	{
		SelectNextMenu();
	}

	// 表示するメニュー情報をテキストに設定
	std::string str = "";
	int size = mMenuInfoList.size();
	for (int i = 0; i < size; i++)
	{
		MenuInfo& info = mMenuInfoList[i];
		str += (i == mSelectIndex) ? ">> " : "   ";
		str += info.name + "\n";
	}
	mpText->SetText(str.c_str());

	// 「Enter」キーもしくは、「SPACE」キーを押したら、
	// 選択したメニューのシーンへ移行
	if (CInput::PushKey(VK_RETURN) || CInput::PushKey(VK_SPACE))
	{
		MenuInfo& select = mMenuInfoList[mSelectIndex];
		CSceneManager::Instance()->LoadScene(select.scene);
	}
}
