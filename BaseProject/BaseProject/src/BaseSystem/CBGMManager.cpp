#include "CBGMManager.h"

// BGMマネージャーのインスタンス
CBGMManager* CBGMManager::ms_instance = nullptr;

// インスタンスを取得
CBGMManager* CBGMManager::Instance()
{
	if (ms_instance == nullptr)
	{
		ms_instance = new CBGMManager();
	}
	return ms_instance;
}

// インスタンスを破棄
void CBGMManager::ClearInstance()
{
	SAFE_DELETE(ms_instance);
}

// コンストラクタ
CBGMManager::CBGMManager()
	: mState(EState::eIdle)
	, mStateStep(0)
	, mFadeTime(0.0f)
	, mElapsedTime(0.0f)
	, mIsFromBegin(true)
	, mpCurrentBGM(nullptr)
	, mpNextBGM(nullptr)
	, mLoadedBGMType(EBGMType::eNone)
{
	mNextBGMData.fromBegin = true;
	mNextBGMData.fadeTime = 0.0f;
	mNextBGMData.data.Reset();
}

// デストラクタ
CBGMManager::~CBGMManager()
{
	ms_instance = nullptr;
}

// 状態を切り替え
void CBGMManager::ChangeState(EState state)
{
	mState = state;
	mStateStep = 0;
}

// BGMデータを取得
BGMData CBGMManager::GetBGMData(EBGMType type) const
{
	for (int i = 0; i < BGM_DATA_COUNT; i++)
	{
		const BGMData& data = BGM_DATA[i];
		if (data.type == type) return data;
	}

	BGMData ret;
	ret.Reset();
	return ret;
}

// BGMを再生
void CBGMManager::Play(EBGMType type, bool fromBegin, float fadeTime)
{
	// 次に再生するBGMのデータを設定
	mNextBGMData.fromBegin = fromBegin;
	mNextBGMData.fadeTime = fadeTime;
	mNextBGMData.data = GetBGMData(type);
	// 現在が待機状態であれば、BGM読み込み状態へ移行
	if (mState == EState::eIdle)
	{
		ChangeState(EState::eLoading);
	}
}

// 待機状態の更新処理
void CBGMManager::UpdateIdle()
{
}

// BGM読み込み中の更新処理
void CBGMManager::UpdateLoading()
{
	// ステップごとに処理を切り替える
	switch (mStateStep)
	{
		// ステップ0：BGMが有効かチェック
		case 0:
			mIsFromBegin = mNextBGMData.fromBegin;
			mFadeTime = mNextBGMData.fadeTime;
			mElapsedTime = 0.0f;
			mLoadedBGMType = mNextBGMData.data.type;
			mCurrBGMData = mNextBGMData.data;
			// 有効でないBGMであれば、即フェード状態へ移行
			if (mLoadedBGMType == EBGMType::eNone)
			{
				mpNextBGM = nullptr;
				ChangeState(EState::eFade);
			}
			// 有効であれば、次のステップへ
			else
			{
				mStateStep++;
			}
			break;
		// ステップ1：BGMを読み込み
		case 1:
		{
			std::string path = mNextBGMData.data.path;
			mpNextBGM = CResourceManager::Load<CSound>(path, path, true);
			mpNextBGM->SetBaseVolume(mCurrBGMData.baseVolume);
			mStateStep++;
			break;
		}
		// ステップ2：BGM読み込み待ち
		case 2:
			// BGMの読み込みが終われば、フェード状態へ移行
			ChangeState(EState::eFade);
			break;
	}
}

// フェード中の更新処理
void CBGMManager::UpdateFade()
{
	switch (mStateStep)
	{
		// ステップ0：現在再生中のBGMをフェードアウト
		case 0:
			// 現在再生中のBGMが存在しない場合は、次のステップへ
			if (mpCurrentBGM == nullptr)
			{
				mStateStep++;
			}
			else
			{
				// フェード時間を経過していない
				if (mElapsedTime < mFadeTime)
				{
					// 経過時間に合わせてボリュームを下げていく
					float alpha = mElapsedTime / mFadeTime;
					mpCurrentBGM->SetVolume(1.0f - alpha);
					// 経過時間を加算
					mElapsedTime += Time::CalcDeltaTime();
				}
				// 経過時間が経過した
				else
				{
					// ボリュームを0にして停止
					mpCurrentBGM->SetVolume(0.0f);
					mpCurrentBGM->Stop(-1, 0.0f);
					mpCurrentBGM = nullptr;

					mElapsedTime = 0.0f;
					mStateStep++;
				}
			}
			break;
		// ステップ1：次に再生するBGMが存在すれば、ボリューム0で再生開始
		case 1:
			if (mpNextBGM != nullptr)
			{
				mpNextBGM->PlayLoop
				(
					mCurrBGMData.isLoop ? -1 : 0,
					mIsFromBegin,
					0.0f,
					true,
					0.0f
				);
			}
			mStateStep++;
			break;
		// ステップ2：次に再生するBGMをフェードイン
		case 2:
			// 次に再生するBGMが存在しない場合は、次のステップへ
			if (mpNextBGM == nullptr)
			{
				mStateStep++;
			}
			else
			{
				// フェード時間を経過していない
				if (mElapsedTime < mFadeTime)
				{
					// 経過時間に合わせてボリュームを上げていく
					float alpha = mElapsedTime / mFadeTime;
					mpNextBGM->SetVolume(alpha);
					// 経過時間を加算
					mElapsedTime += Time::CalcDeltaTime();
				}
				// 経過時間が経過した
				else
				{
					// ボリュームを1に設定
					mpNextBGM->SetVolume(1.0f);
					// 現在再生中のBGMに次のBGMを設定
					mpCurrentBGM = mpNextBGM;
					// 次のBGMを空にする
					mpNextBGM = nullptr;

					mElapsedTime = 0.0f;
					mStateStep++;
				}
			}
			break;
		case 3:
			// 読み込んだBGMと次に再生するBGMの種類が違う場合は、
			// BGM読み込み状態へ移行
			if (mLoadedBGMType != mNextBGMData.data.type)
			{
				ChangeState(EState::eLoading);
			}
			// 次に再生するBGMが存在しない場合は、待機状態へ移行
			else
			{
				ChangeState(EState::eIdle);
			}
			break;
	}
}

// クロスフェード中の更新処理
void CBGMManager::UpdateCrossFade()
{
	// フェード時間を経過していない
	if (mElapsedTime < mFadeTime)
	{
		float alpha = mElapsedTime / mFadeTime;
		// 現在再生中のBGMが存在すれば、ボリュームを設定
		if (mpCurrentBGM != nullptr)
		{
			mpCurrentBGM->SetVolume(1.0f - alpha);
		}
		// 次に再生するBGMが存在すれば、ボリュームを設定
		if (mpNextBGM != nullptr)
		{
			mpNextBGM->SetVolume(alpha);
		}
		// 経過時間を加算
		mElapsedTime += Time::CalcDeltaTime();
	}
	// 経過時間が経過した
	else
	{
		// 現在再生中のBGMが存在すれば、ボリュームを0にして停止
		if (mpCurrentBGM != nullptr)
		{
			mpCurrentBGM->SetVolume(0.0f);
			mpCurrentBGM->Stop(-1, 0.0f);
		}
		// 次に再生するBGMが存在すれば、
		if (mpNextBGM != nullptr)
		{
			// ボリュームを1に設定
			mpNextBGM->SetVolume(1.0f);
			// 現在再生中のBGMに次のBGMを設定
			mpCurrentBGM = mpNextBGM;
			// 次のBGMを空にする
			mpNextBGM = nullptr;
		}
		// 次に再生するBGMが存在しないならば、現在のBGMを空にする
		else
		{
			mpCurrentBGM = nullptr;
		}

		// 読み込んだBGMと次に再生するBGMの種類が違う場合は、
		// BGM読み込み状態へ移行
		if (mLoadedBGMType != mNextBGMData.data.type)
		{
			ChangeState(EState::eLoading);
		}
		// 次に再生するBGMが存在しない場合は、待機状態へ移行
		else
		{
			ChangeState(EState::eIdle);
		}
	}
}

// 更新
void CBGMManager::Update()
{
	// 現在の状態に合わせて、更新処理を切り替える
	switch (mState)
	{
		// 待機状態
		case EState::eIdle:
			UpdateIdle();
			break;
		// BGM読み込み中
		case EState::eLoading:
			UpdateLoading();
			break;
		// フェード中
		case EState::eFade:
			UpdateFade();
			//UpdateCrossFade();
			break;
	}
}