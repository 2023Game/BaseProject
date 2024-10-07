#pragma once

// BGMの種類
enum class EBGMType
{
	eNone = -1,

	eTitle,	// タイトル画面のBGM
	eGame,	// ゲーム画面のBGM
	eMenu,	// メニューを開いている時のBGM

	Num
};

// BGMデータ
struct BGMData
{
	EBGMType type;		// BGMの種類
	std::string path;	// BGMのリソースパス
	float baseVolume;	// ベース音量
	bool isLoop;		// ループするかどうか
	int loopBegin;		// ループ開始位置
	int loopLength;		// ループ終了位置
	// リセット処理
	void Reset()
	{
		type = EBGMType::eNone;
		path = "";
		baseVolume = 1.0f;
		isLoop = true;
		loopBegin = 0;
		loopLength = 0;
	}
};
// BGMデータのテーブル
extern const BGMData BGM_DATA[];
// BGMデータ数
extern const int BGM_DATA_COUNT;
