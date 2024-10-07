#pragma once
#include "CSceneBase.h"
class CText;

// 起動時のメニューシーン
class CBootMenuScene : public CSceneBase
{
public:
	// コンストラクタ
	CBootMenuScene();
	// デストラクタ
	~CBootMenuScene();

	// シーン読み込み
	void Load() override;
	// 更新
	void Update() override;

private:
	// 次の項目を選択
	void SelectNextMenu();
	// 前の項目を選択
	void SelectPrevMenu();

	CText* mpText;	// 文字列描画用

	// メニュー情報
	struct MenuInfo
	{
		std::string name;	// 表示名
		EScene scene;		// 遷移先のシーン
	};
	// メニュー情報のリスト
	std::vector<MenuInfo> mMenuInfoList;

	int mSelectIndex;	// 選択中のインデックス値
};