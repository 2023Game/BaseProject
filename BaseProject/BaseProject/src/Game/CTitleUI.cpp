#include "CTitleUI.h"
#include "CFont.h"
#include "CText.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"

// 「CLICK TO START」の点滅時間
#define START_TEXT_BLINK_TIME 1.0f
// 「CLICK TO START」の点滅間隔
#define START_TEXT_INTERVAL_TIME 3.0f
// タイトルメニューのアニメーション時間
#define OPEN_ANIM_TIME 0.25f
// タイトルメニューのアニメーション後の待ち時間
#define OPENED_WAIT_TIME 0.5f

// コンストラクタ
CTitleUI::CTitleUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
{
	// タイトルロゴのフォントデータを生成
	mpLogoFont = new CFont("res\\Font\\toroman.ttf");
	mpLogoFont->SetFontSize(128);
	mpLogoFont->SetAlignment(FTGL::TextAlignment::ALIGN_CENTER);
	mpLogoFont->SetLineLength(WINDOW_WIDTH);

	// タイトルロゴのテキストを生成
	mpTitleLogo = new CText
	(
		mpLogoFont, 128,
		CVector2(0.0f, 32.0f),
		CVector2(WINDOW_WIDTH, WINDOW_HEIGHT),
		CColor(0.11f, 0.1f, 0.1f),
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpTitleLogo->SetText("タイトルロゴ");
	mpTitleLogo->SetEnableOutline(true);
	mpTitleLogo->SetOutlineColor(CColor(0.9f, 0.9f, 0.9f));

	// タイトル画面の背景イメージを生成
	mpTitleBg = new CImage
	(
		"UI/title_bg.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);

	// 「CLICK TO START」のテキストを生成
	mpStartText = new CText
	(
		nullptr, 32,
		CVector2(0.0f, -64.0f),
		CVector2(WINDOW_WIDTH, WINDOW_HEIGHT),
		CColor(1.0f, 1.0f, 0.5f),
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpStartText->SetText("- CLICK TO START -");
	mpStartText->SetTextAlignH(ETextAlignH::eCenter);
	mpStartText->SetTextAlignV(ETextAlignV::eBottom);
	mpStartText->SetEnableShadow(true);
	mpStartText->SetShadowColor(CColor(0.25f, 0.125f, 0.0f));
	mpStartText->SetEnableOutline(true);
	mpStartText->SetOutlineColor(CColor(0.0f, 0.0f, 0.0f));

	// [START]ボタンを生成
	CExpandButton* btn1 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 450.0f),
		CVector2(181.0f, 47.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// ボタンの画像を読み込み
	btn1->LoadButtonImage("UI/title_start0.png", "UI/title_start1.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	btn1->SetOnClickFunc(std::bind(&CTitleUI::OnClickStart, this));
	// ボタンは最初は無効化して、スケール値を0にしておく
	btn1->SetEnable(false);
	btn1->SetScale(0.0f);
	// ボタンリストに追加
	mButtons.push_back(btn1);

	// [OPTION]ボタンを生成
	CExpandButton* btn2 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 550.0f),
		CVector2(181.0f, 47.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	btn2->LoadButtonImage("UI/title_option0.png", "UI/title_option1.png");
	btn2->SetOnClickFunc(std::bind(&CTitleUI::OnClickOption, this));
	btn2->SetEnable(false);
	btn2->SetScale(0.0f);
	mButtons.push_back(btn2);

	// [QUIT]ボタンを生成
	CExpandButton* btn3 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 650.0f),
		CVector2(181.0f, 47.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	btn3->LoadButtonImage("UI/title_quit0.png", "UI/title_quit1.png");
	btn3->SetOnClickFunc(std::bind(&CTitleUI::OnClickQuit, this));
	btn3->SetEnable(false);
	btn3->SetScale(0.0f);
	mButtons.push_back(btn3);
}

// デストラクタ
CTitleUI::~CTitleUI()
{
	SAFE_DELETE(mpLogoFont);
	SAFE_DELETE(mpTitleLogo);
	SAFE_DELETE(mpTitleBg);
	SAFE_DELETE(mpStartText);

	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		mButtons[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButtons.clear();
}

// タイトル画面終了か
bool CTitleUI::IsEnd() const
{
	return mIsEnd;
}

// ゲームを開始するか
bool CTitleUI::IsStartGame() const
{
	// 選択項目が1つ目ならば、ゲーム開始
	return mSelectIndex == 0;
}

// ゲームを終了するか
bool CTitleUI::IsExitGame() const
{
	// 選択項目が3つ目ならば、ゲーム終了
	return mSelectIndex == 2;
}

// 状態切り替え
void CTitleUI::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// [START]クリック時のコールバック関数
void CTitleUI::OnClickStart()
{
	if (mIsEnd) return;

	mSelectIndex = 0;
	mIsEnd = true;
}

// [OPTION]クリック時のコールバック関数
void CTitleUI::OnClickOption()
{
	if (mIsEnd) return;

	mSelectIndex = 1;
}

// [QUIT]クリック時のコールバック関数
void CTitleUI::OnClickQuit()
{
	if (mIsEnd) return;

	mSelectIndex = 2;
	mIsEnd = true;
}

// 待機状態
void CTitleUI::UpdateIdle()
{
	// 待機状態をステップ管理
	switch (mStateStep)
	{
		// ステップ0：「CLICK TO START」の点滅待ち
		case 0:
			mpStartText->SetAlpha(1.0f);
			if (mElapsedTime < START_TEXT_INTERVAL_TIME)
			{
				mElapsedTime += Time::DeltaTime();
			}
			else
			{
				mElapsedTime = 0.0f;
				mStateStep = 1;
			}
			break;
		// ステップ1：「CLICK TO START」の点滅
		case 1:
			if (mElapsedTime < START_TEXT_BLINK_TIME)
			{
				// サインカーブで点滅
				float per = sinf(M_PI * mElapsedTime / START_TEXT_BLINK_TIME);
				mpStartText->SetAlpha(1.0f - per);
				mElapsedTime += Time::DeltaTime();
			}
			else
			{
				mElapsedTime = 0.0f;
				mpStartText->SetAlpha(1.0f);
				mStateStep = 0;
			}
			break;
	}

	// フェードが終わっている
	if (!CFade::IsFading())
	{
		// 左クリックすると、メニューを開く
		if (CInput::PushKey(VK_LBUTTON))
		{
			ChangeState(EState::eOpen);
		}
	}
}

// メニューを開く
void CTitleUI::UpdateOpen()
{
	switch (mStateStep)
	{
		// ステップ0：メニューの入場アニメーション
		case 0:
			if (mElapsedTime < OPEN_ANIM_TIME)
			{
				// スケール値を一旦1.0より大きくして、1.0へ戻るイージングアニメーション
				float scale = Easing::BackOut(mElapsedTime, OPEN_ANIM_TIME, 0.0f, 1.0f, 2.0f);
				for (CExpandButton* btn : mButtons)
				{
					btn->SetScale(scale);
				}
				mElapsedTime += Time::DeltaTime();
			}
			else
			{
				for (CExpandButton* btn : mButtons)
				{
					btn->SetScale(1.0f);
				}
				mStateStep++;
				mElapsedTime = 0.0f;
			}
			break;
		// ステップ1：メニュー入場後の待ち
		case 1:
			if (mElapsedTime < OPENED_WAIT_TIME)
			{
				mElapsedTime += Time::DeltaTime();
			}
			else
			{
				// 一定時間待ったら、ボタンをオンにしてタッチできるようにする
				// （誤タッチを防ぐための待ち時間）
				for (CExpandButton* btn : mButtons)
				{
					btn->SetEnable(true);
				}
				ChangeState(EState::eSelect);
			}
			break;
	}
}

// メニュー選択
void CTitleUI::UpdateSelect()
{
}

// フェードアウト
void CTitleUI::UpdateFadeOut()
{
}

// 更新
void CTitleUI::Update()
{
	switch (mState)
	{
		// 待機状態
		case EState::eIdle:
			UpdateIdle();
			break;
		// メニューを開く
		case EState::eOpen:
			UpdateOpen();
			break;
		// メニュー選択
		case EState::eSelect:
			UpdateSelect();
			break;
		// フェードアウト
		case EState::eFadeOut:
			UpdateFadeOut();
			break;
	}

	mpTitleLogo->Update();
	mpTitleBg->Update();
	mpStartText->Update();
	for (CButton* btn : mButtons)
	{
		btn->Update();
	}
}

// 描画
void CTitleUI::Render()
{
	// 描画順：
	// 背景→タイトルロゴ→「CLICK TO START」かメニューボタン

	// 背景描画
	mpTitleBg->Render();
	// タイトルロゴ描画
	mpTitleLogo->Render();

	// 待機状態ならば、「CLICK TO START」を表示
	if (mState == EState::eIdle)
	{
		mpStartText->Render();
	}
	// 待機状態以外は、メニューボタンを表示
	else
	{
		for (CButton* btn : mButtons)
		{
			btn->Render();
		}
	}
}
