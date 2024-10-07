#pragma once
#include "CSound.h"
#include "BGMData.h"

// BGM管理クラス
class CBGMManager
{
public:
	// インスタンスを取得
	static CBGMManager* Instance();
	// インスタンスを破棄
	static void ClearInstance();

	/// <summary>
	/// BGMを再生
	/// </summary>
	/// <param name="type">再生するBGMの種類（eNoneを設定するとBGM停止）</param>
	/// <param name="fromBegin">trueならば、BGMを最初から再生する</param>
	/// <param name="fadeTime">フェード時間</param>
	void Play(EBGMType type, bool fromBegin = true, float fadeTime = 0.5f);

	// 更新
	void Update();

private:
	// コンストラクタ
	CBGMManager();
	// デストラクタ
	~CBGMManager();

	// BGMマネージャーの状態
	enum class EState
	{
		eIdle,		// 待機状態
		eLoading,	// BGM読み込み中
		eFade,		// フェード中
	};
	// 状態を切り替え
	void ChangeState(EState state);

	// BGMデータを取得
	BGMData GetBGMData(EBGMType type) const;

	// 待機状態の更新処理
	void UpdateIdle();
	// BGM読み込み中の更新処理
	void UpdateLoading();
	// フェード中の更新処理
	void UpdateFade();
	// クロスフェード中の更新処理
	void UpdateCrossFade();

	// BGMマネージャーのインスタンス
	static CBGMManager* ms_instance;
	EState mState;			// 現在の状態
	int mStateStep;			// 現在の状態内のステップ
	float mFadeTime;		// フェード時間
	float mElapsedTime;		// 経過時間
	bool mIsFromBegin;		// BGMを最初から再生するかどうか
	BGMData mCurrBGMData;

	CSound* mpCurrentBGM;	// 現在再生中のBGM
	CSound* mpNextBGM;		// 次に再生するBGM

	// 次に再生するBGMのデータクラス
	class CNextBGMData
	{
	public:
		bool fromBegin;		// 次に再生するBGMを最初から再生するかどうか
		float fadeTime;		// 次のBGMを再生する時のフェード時間
		BGMData data;		// 次に再生するBGMの情報
	};
	// 次に再生するBGMのデータ
	CNextBGMData mNextBGMData;

	EBGMType mLoadedBGMType;// 読み込んだBGMの種類
};
