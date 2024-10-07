#pragma once
#include "CTask.h"
#include "CColor.h"
#include "CImage.h"

// 画面のフェード処理
class CFade : public CTask
{
public:
	// インスタンスを取得
	static CFade* Instance();

	/// <summary>
	/// フェードカラーを設定
	/// </summary>
	/// <param name="color">設定する色</param>
	static void SetFadeColor(const CColor& color);

	/// <summary>
	/// フェードイン開始
	/// </summary>
	/// <param name="time">フェード時間</param>
	static void FadeIn(float time = 0.25f);
	/// <summary>
	/// フェードアウト開始
	/// </summary>
	/// <param name="time">フェード時間</param>
	static void FadeOut(float time = 0.25f);

	/// <summary>
	/// フェード中かどうか
	/// </summary>
	/// <returns>trueならば、フェード中</returns>
	static bool IsFading();

private:
	// コンストラクタ
	CFade();
	// デストラクタ
	~CFade();

	/// <summary>
	/// フェード開始
	/// </summary>
	/// <param name="time">フェード時間</param>
	/// <param name="isFadeIn">フェードインかどうか</param>
	void StartFade(float time, bool isFadeIn);

	// 更新
	void Update() override;
	// 描画
	void Render() override;

	// フェードクラスのインスタンス
	static CFade* ms_instance;

	CImage* mpFadeImage;	// フェード画像
	CColor mFadeColor;		// フェードカラー
	float mFadeTime;		// フェード時間
	float mElapsedTime;		// 経過時間
	bool mIsFadeIn;			// フェードインかどうか
	bool mIsFading;			// フェード中かどうか
};