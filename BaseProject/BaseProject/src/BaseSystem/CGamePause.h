#pragma once
#include "CTask.h"
#include "CInput.h"
#include "CImage.h"

//ゲームポーズ制御クラス
class CGamePause : public CTask
{
private:
	bool mIsPaused;			//ポーズ中かどうか
	int mPauseStep;			//ポーズ処理のステップ
	float mElapsedTime;		//経過時間計測用
	float mPauseTextAlpha;	//PAUSEテキストのアルファ値保存用

	CImage* mFadeBg;		//フェード背景のイメージ
	CImage* mPauseText;		//PAUSEテキストのイメージ

public:
	//コンストラクタ
	CGamePause();
	//デストラクタ
	~CGamePause();

	//更新
	void Update();
	//描画
	void Render();
};