#include "CSoundManager.h"
#include "CSound.h"
#include "CBGMManager.h"

// サウンドマネージャーのインスタンス
CSoundManager* CSoundManager::mpInstance = nullptr;

// インスタンスを取得
CSoundManager* CSoundManager::Instance()
{
	// インスタンスがなければ、新しく生成する
	if (mpInstance == nullptr)
	{
		// インスタンス生成
		mpInstance = new CSoundManager();
		// 初期化失敗したら、インスタンスを破棄
		if (!mpInstance->mIsInitialized)
		{
			SAFE_DELETE(mpInstance);
		}
	}
	return mpInstance;
}

// インスタンスを破棄
void CSoundManager::ClearInstance()
{
	SAFE_DELETE(mpInstance);
}

// コンストラクタ
CSoundManager::CSoundManager()
	: mIsInitialized(false)
	, mpXAudio(nullptr)
	, mpMasterVoice(nullptr)
{
	HRESULT hr;
	(void)CoInitializeEx(NULL, COINIT_MULTITHREADED);
	hr = XAudio2Create(&mpXAudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED(hr))
	{
		CoUninitialize();
		mpXAudio = nullptr;
		return;
	}

	hr = mpXAudio->CreateMasteringVoice(&mpMasterVoice, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL);
	if (FAILED(hr))
	{
		if (mpXAudio != nullptr) mpXAudio->Release();
		mpXAudio = nullptr;
		mpMasterVoice = nullptr;
		CoUninitialize();
		return;
	}

	// 初期化終了
	mIsInitialized = true;
}

// デストラクタ
CSoundManager::~CSoundManager()
{
	if (mpMasterVoice != nullptr)
	{
		mpMasterVoice->DestroyVoice();
		mpMasterVoice = nullptr;
	}

	if (mpXAudio != nullptr)
	{
		mpXAudio->Release();
		mpXAudio = nullptr;
	}

	CoUninitialize();
}

// 音声データを再生するためのソースボイスを生成
IXAudio2SourceVoice* CSoundManager::CreateSourceVoice(const WAVEFORMATEX* format)
{
	if (mpXAudio == nullptr) return nullptr;

	// ソースボイスを作成
	IXAudio2SourceVoice* ret = nullptr;
	HRESULT hr = mpXAudio->CreateSourceVoice
	(
		&ret, format,
		XAUDIO2_VOICE_NOPITCH /*| XAUDIO2_VOICE_MUSIC*/,
		XAUDIO2_DEFAULT_FREQ_RATIO,
		NULL,
		NULL,
		NULL
	);
	// 作成に失敗した場合は、nullptrを返す
	if (FAILED(hr)) return nullptr;

	return ret;
}

// サウンドをリストに追加
void CSoundManager::Add(CSound* sound)
{
	mSounds.push_back(sound);
}

// サウンドをリストから取り除く
void CSoundManager::Remove(CSound* sound)
{
	mSounds.remove(sound);
}

// 更新
void CSoundManager::Update()
{
	// リストに登録されているサウンドを更新
	for (CSound* sound : mSounds)
	{
		sound->Update();
	}

	// BGMマネージャーの更新
	CBGMManager::Instance()->Update();
}