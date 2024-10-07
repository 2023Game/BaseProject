#define _USE_MATH_DEFINES
#include <cmath>
#include "CGamePause.h"
#include "CTaskManager.h"
#include "Maths.h"

//コンストラクタ
CGamePause::CGamePause()
	: CTask(ETaskPriority::eSystem, 0, ETaskPauseType::eSystem, true)
	, mIsPaused(false)
	, mPauseStep(0)
	, mElapsedTime(0.0f)
	, mPauseTextAlpha(0.0f)
{
	//フェード背景用の白イメージを読み込み
	mFadeBg = new CImage
	(
		"UI/white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eSystem,
		true, false
	);
	//今回は黒背景にするのでカラーを黒(0, 0, 0)にする
	//最初は非表示のままにするのでアルファ値も0
	mFadeBg->SetColor(0.0f, 0.0f, 0.0f, 0.0f);
	//画面全てを覆う設定
	//座標(0, 0)からサイズ(画面幅, 画面高さ)までを指定
	mFadeBg->SetPos(0.0f, 0.0f);
	mFadeBg->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	//PAUSEテキストのイメージ読み込み
	mPauseText = new CImage
	(
		"UI/pause_text.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eSystem,
		true, false
	);
	//こちらも最初は非表示なのでアルファ値を0にしておく
	mPauseText->SetColor(1.0f, 1.0f, 1.0f, 0.0f);
	//画面中央に配置
	const CVector2& size = mPauseText->GetSize();
	CVector2 pos = (CVector2(WINDOW_WIDTH, WINDOW_HEIGHT) - size) * 0.5f;
	mPauseText->SetPos(pos);
}

//デストラクタ
CGamePause::~CGamePause()
{
	//フェード背景のイメージを読み込んでいたら破棄
	SAFE_DELETE(mFadeBg);
	//PAUSEテキストのイメージを読み込んでいたら破棄
	SAFE_DELETE(mPauseText);
}

//更新
void CGamePause::Update()
{
	//ポーズ中ではない場合
	if (!mIsPaused)
	{
		//「P」キーを押すと、ポーズ開始
		if (CInput::PushKey('P'))
		{
			//TaskManagerからポーズを設定する
			CTaskManager::Instance()->Pause(PAUSE_GAME);
			//ポーズ中のフラグも立てる
			mIsPaused = true;
		}
	}
	//ポーズ中
	else
	{
		//フェード背景の表示＆非表示に掛ける時間（秒）
		static const float fadeTime = 0.25f;
		//フェード背景の最大アルファ値
		static const float fadeAlpha = 0.5f;
		//PAUSEテキストのアニメーション時間（秒）
		static const float animTime = 1.0f;

		//ポーズステップごとに処理を分ける
		switch (mPauseStep)
		{
			//ポーズ開始したのでフェード背景を表示
			case 0:
			{
				//フェード背景の表示時間が経過していない
				if (mElapsedTime < fadeTime)
				{
					//経過時間に合わせてアルファ値を設定
					float alpha = mElapsedTime / fadeTime;
					mFadeBg->SetAlpha(fadeAlpha * alpha);
					//1フレームの経過時間を加算（仮）
					mElapsedTime += 0.016f;
				}
				//フェード背景の表示時間が経過した
				else
				{
					//フェード背景を完全に表示して
					//次のステップへ
					mFadeBg->SetAlpha(fadeAlpha);
					mElapsedTime = 0.0f;
					mPauseStep++;
				}
				break;
			}
			//PAUSEテキストの点滅アニメーション再生
			case 1:
			{
				//経過時間に合わせてSinカーブで
				//アルファ値を設定して点滅させる
				float alpha = mElapsedTime / animTime;
				mPauseTextAlpha = sin(M_PI * 2.0f * alpha) * 0.5f + 0.5f;
				mPauseText->SetAlpha(mPauseTextAlpha);

				//経過時間がアニメーション時間を超えたら、
				//0に戻してループさせる
				mElapsedTime += 0.016f;
				if (mElapsedTime >= animTime) mElapsedTime -= animTime;

				//このステップで再度「P」キーが押されたら
				if (CInput::PushKey('P'))
				{
					//ポーズ終了ステップへ移行
					mElapsedTime = 0.0f;
					mPauseStep++;
				}
				break;
			}
			//ポーズ終了するのでポーズ背景を非表示
			case 2:
			{
				// ポーズ背景の表示処理と反対の処理なのでコメントは割愛
				if (mElapsedTime < fadeTime)
				{
					float alpha = mElapsedTime / fadeTime;
					//非表示にしていくので「1.0f-alpha」
					mFadeBg->SetAlpha(fadeAlpha * (1.0f - alpha));
					mPauseText->SetAlpha(mPauseTextAlpha * (1.0f - alpha));
					mElapsedTime += 0.016f;
				}
				else
				{
					//終了時はアルファ値0で非表示
					mFadeBg->SetAlpha(0.0f);
					mPauseText->SetAlpha(0.0f);
					mElapsedTime = 0.0f;
					mPauseStep = 0;
					//TaskManagerからポーズを解除する
					CTaskManager::Instance()->UnPause(PAUSE_GAME);
					//ポーズフラグも下ろす
					mIsPaused = false;
				}
				break;
			}
		}
	}
}

//描画
void CGamePause::Render()
{
	//フェード背景を読み込んでいたら描画
	if (mFadeBg != nullptr)
	{
		mFadeBg->Render();
	}
	//PAUSEテキストを読み込んでいたら描画
	if (mPauseText != nullptr)
	{
		mPauseText->Render();
	}
}