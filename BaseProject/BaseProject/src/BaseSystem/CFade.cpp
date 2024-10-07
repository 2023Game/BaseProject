#include "CFade.h"

// フェードクラスのインスタンス
CFade* CFade::ms_instance = nullptr;

// コンストラクタ
CFade::CFade()
	: CTask(ETaskPriority::eFade, 0, ETaskPauseType::eSystem, true, true)
	, mFadeColor(CColor::black)
	, mFadeTime(0.0f)
	, mElapsedTime(0.0f)
	, mIsFadeIn(false)
	, mIsFading(false)
{
	// フェード画像を読み込み
	mpFadeImage = new CImage
	(
		"UI/white.png",
		ETaskPriority::eFade, 0,
		ETaskPauseType::eSystem,
		true, false // フェード画像はタスクリストに登録しない
	);
	// フェード画像を画面全体のサイズに変更
	mpFadeImage->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	// フェードカラーを設定
	mpFadeImage->SetColor(mFadeColor);
}

// デストラクタ
CFade::~CFade()
{
	// フェード画像をタスクリストに登録していないので、
	// デストラクタで自分で破棄する
	SAFE_DELETE(mpFadeImage);

	ms_instance = nullptr;
}

// インスタンスを取得
CFade* CFade::Instance()
{
	if (ms_instance == nullptr)
	{
		ms_instance = new CFade();
	}
	return ms_instance;
}

// フェードカラーを設定
void CFade::SetFadeColor(const CColor& color)
{
	CFade* inst = Instance();
	inst->mFadeColor.Set(color.R(), color.G(), color.B());
	inst->mpFadeImage->SetColor(inst->mFadeColor);
}

// フェードイン開始
void CFade::FadeIn(float time)
{
	Instance()->StartFade(time, true);
}

// フェードアウト開始
void CFade::FadeOut(float time)
{
	Instance()->StartFade(time, false);
}

// フェード開始
void CFade::StartFade(float time, bool isFadeIn)
{
	// 各パラメータを設定
	mFadeTime = time;
	mElapsedTime = 0.0f;
	mIsFadeIn = isFadeIn;

	// フェード時間が0かどうかで初期状態のアルファ値を変更
	float alpha = mFadeTime <= 0.0f ? 1.0f : 0.0f;
	// 初期状態のアルファ値を設定
	mFadeColor.A(mIsFadeIn ? 1.0f - alpha : alpha);
	mpFadeImage->SetColor(mFadeColor);

	// フェード時間が0より長い場合は、フェードフラグを立てる
	mIsFading = mFadeTime > 0.0f;
}

// フェード中かどうか
bool CFade::IsFading()
{
	return Instance()->mIsFading;
}

// 更新
void CFade::Update()
{
	// フェード中でなければ、処理しない
	if (!mIsFading) return;

	// フェード時間が経過していない
	if (mElapsedTime < mFadeTime)
	{
		// 経過時間に合わせて、フェードイメージのアルファ値を変更
		float alpha = mElapsedTime / mFadeTime;
		mFadeColor.A(mIsFadeIn ? 1.0f - alpha : alpha);
		mpFadeImage->SetColor(mFadeColor);
		// 経過時間加算
		mElapsedTime += Time::DeltaTime();
	}
	// フェード時間を経過した
	else
	{
		// フェード終了
		mIsFading = false;
		mElapsedTime = mFadeTime;
		mFadeColor.A(mIsFadeIn ? 0.0f : 1.0f);
		mpFadeImage->SetColor(mFadeColor);
	}
}

// 描画
void CFade::Render()
{
	if (mFadeColor.A() == 0.0f) return;
	mpFadeImage->Render();
}
