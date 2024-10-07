#include "CSound.h"
#include "CSoundManager.h"
#include "Maths.h"

// コンストラクタ
CSound::CSound()
	: g_hmmio(0)
	, mpBuf(nullptr)
	, mSampleCount(0)
	, mLoopBegin(0)
	, mLoopLength(0)
	, mBaseVolume(1.0f)
{
	memset(&g_datachunkinfo, 0, sizeof(g_datachunkinfo));
	memset(&g_riffchunkinfo, 0, sizeof(g_riffchunkinfo));
	memset(&g_mmioinfo, 0, sizeof(g_mmioinfo));
	memset(&g_wfx, 0, sizeof(g_wfx));

	CSoundManager::Instance()->Add(this);
}

// デストラクタ
CSound::~CSound()
{
	// 再生中のサウンドを停止
	Stop(-1, 0.0f);

	for (CAudioSource& source : mAudioSources)
	{
		IXAudio2SourceVoice* voice = source.voice;
		voice->DestroyVoice();
	}
	mAudioSources.clear();

	SAFE_DELETE_ARRAY(mpBuf);

	if (g_hmmio != nullptr)
	{
		mmioClose(g_hmmio, 0);
	}

	CSoundManager::Instance()->Remove(this);
}

// ファイル読み込み
bool CSound::Load(std::string path, bool dontDelete)
{
	// 読み込み済みならば、処理しない
	if (mpBuf != nullptr) return true;

	// ファイルパスにリソースディレクトリのパスを追加
	std::string filePath = RES_DIR + path;

	MMCKINFO mmckinfo; PCMWAVEFORMAT pcmwf; MMRESULT mmret;
	memset(&g_mmioinfo, 0x00, sizeof(g_mmioinfo));
	g_hmmio = mmioOpen(const_cast<TCHAR*>(filePath.c_str()), &g_mmioinfo, MMIO_READ);
	if (g_hmmio == NULL)
	{
		return false;
	}

	memset(&g_riffchunkinfo, 0x00, sizeof(g_riffchunkinfo));
	g_riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmret = mmioDescend(g_hmmio, &g_riffchunkinfo, NULL, MMIO_FINDRIFF);
	if (mmret != MMSYSERR_NOERROR)
	{
		if (g_hmmio != nullptr)
		{
			mmioClose(g_hmmio, 0);
			g_hmmio = nullptr;
		}
		return false;
	}
	memset(&mmckinfo, 0x00, sizeof(mmckinfo));
	mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmret = mmioDescend(g_hmmio, &mmckinfo, &g_riffchunkinfo, MMIO_FINDCHUNK);
	if (mmret != MMSYSERR_NOERROR)
	{
		if (g_hmmio != nullptr)
		{
			mmioClose(g_hmmio, 0);
			g_hmmio = nullptr;
		}
		return false;
	}

	if (mmckinfo.cksize >= sizeof(WAVEFORMATEX))
	{
		mmioRead(g_hmmio, (HPSTR)&g_wfx, sizeof(g_wfx));
	}
	else
	{
		mmioRead(g_hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
		memset(&g_wfx, 0x00, sizeof(g_wfx));
		memcpy(&g_wfx, &pcmwf, sizeof(pcmwf));
		g_wfx.cbSize = 0;
	}
	mmioAscend(g_hmmio, &mmckinfo, 0);
	memset(&g_datachunkinfo, 0x00, sizeof(g_datachunkinfo));
	g_datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmret = mmioDescend(g_hmmio, &g_datachunkinfo, &g_riffchunkinfo, MMIO_FINDCHUNK);
	if (mmret != MMSYSERR_NOERROR)
	{
		if (g_hmmio != nullptr)
		{
			mmioClose(g_hmmio, 0);
			g_hmmio = nullptr;
		}
		return false;
	}

	//XAUDIO2_VOICE_STATE state;
	UINT32 buflen; HRESULT hr; //DWORD dw;

	//buflen = g_wfx.nAvgBytesPerSec;
	buflen = g_datachunkinfo.cksize;
	mpBuf = new unsigned char[buflen];
	g_readlen = mmioRead(g_hmmio, (HPSTR)mpBuf, buflen);
	if (g_readlen <= 0)
	{
		if (g_hmmio != nullptr)
		{
			mmioClose(g_hmmio, 0);
			g_hmmio = nullptr;
		}
		return false;
	}

	if (g_hmmio != nullptr)
	{
		mmioClose(g_hmmio, 0);
		g_hmmio = nullptr;
	}

	// 1つ目の音声再生用の情報データを生成しておく
	bool success = CreateAudioSource();
	if (!success)
	{
		return false;
	}

	mSampleCount = g_datachunkinfo.cksize / g_wfx.nBlockAlign;

	return true;
}

// 音声再生用の情報データを作成
bool CSound::CreateAudioSource()
{
	IXAudio2SourceVoice* sv = CSoundManager::Instance()->CreateSourceVoice(&g_wfx);
	if (sv == nullptr)
	{
		return false;
	}

	CAudioSource source;
	source.voice = sv;

	memset(&source.info, 0x00, sizeof(source.info));
	//source.info.Flags = ((UINT32)g_readlen >= g_datachunkinfo.cksize) ? 0 : XAUDIO2_END_OF_STREAM;
	source.info.Flags = XAUDIO2_END_OF_STREAM;
	source.info.AudioBytes = g_readlen;
	source.info.pAudioData = mpBuf;

	// リストに追加
	mAudioSources.push_back(source);

	return true;
}

// 音声を再生していないインデックスを取得
int CSound::FindNotPlayingSourceVoice(bool force) const
{
	int size = mAudioSources.size();
	for (int i = 0; i < size; i++)
	{
		IXAudio2SourceVoice* sv = mAudioSources[i].voice;
		XAUDIO2_VOICE_STATE state;
		sv->GetState(&state);
		if (state.BuffersQueued != 0) continue;

		return i;
	}
	if (force) return 0;
	return -1;
}

// サウンドを再生
int CSound::Play(float volume, bool force, float fadeTime)
{
	// ループ回数を0で再生する
	return PlayLoop(0, true, volume, force, fadeTime);
}

// サウンドをループ再生
int CSound::PlayLoop(int loopCount, bool fromBegin, float volume, bool force, float fadeTime)
{
	int index = FindNotPlayingSourceVoice(force);

	if (index >= 0)
	{
		CAudioSource& source = mAudioSources[index];
		if (force) Stop(index, 0.0f);

		// ループ数がマイナスならば、永続ループに変更
		if (loopCount < 0) loopCount = XAUDIO2_LOOP_INFINITE;
		// ループ数が最大ループ数を超えないようにクランプ
		else loopCount = min(loopCount, XAUDIO2_MAX_LOOP_COUNT);
		// ループ数を設定
		source.info.LoopCount = loopCount;
		if (loopCount == 0)
		{
			source.info.LoopBegin = 0;
			source.info.LoopLength = 0;
		}
		else
		{
			source.info.LoopBegin = mLoopBegin;
			source.info.LoopLength = mLoopLength;
		}

		HRESULT hr;
		if (fromBegin)
		{
			source.voice->FlushSourceBuffers();
		}

		hr = source.voice->SubmitSourceBuffer(&source.info, NULL);
		if (FAILED(hr)) return -1;

		float vol = fadeTime <= 0.0f ? volume : 0.0f;
		hr = source.voice->SetVolume(mBaseVolume * vol);
		if (FAILED(hr)) return -1;

		source.voice->Start();
		//if (fadeTime <= 0.0f)
		//{
		//	source.voice->Start();
		//}
		//else
		{
			AddFadeSound(source.voice, fadeTime, volume, true);
		}
	}

	return index;
}

// サウンドを停止
void CSound::Stop(int index, float fadeTime)
{
	// 停止するインデックスが設定されていたら
	if (index >= 0)
	{
		// 範囲外チェック
		if (index >= mAudioSources.size()) return;

		// 指定されたインデックスのサウンドを停止
		CAudioSource& source = mAudioSources[index];
		if (fadeTime > 0.0f)
		{
			float volume = 1.0f;
			source.voice->GetVolume(&volume);
			AddFadeSound(source.voice, fadeTime, volume, false);
		}
		else
		{
			source.voice->Stop();
		}
	}
	// インデックスが設定されていない
	else
	{
		// 全てのサウンドを停止
		for (CAudioSource& source : mAudioSources)
		{
			if (fadeTime > 0.0f)
			{
				float volume = 1.0f;
				source.voice->GetVolume(&volume);
				AddFadeSound(source.voice, fadeTime, volume, false);
			}
			else
			{
				source.voice->Stop();
			}
		}
	}
}

// サウンドを再生中かどうか
bool CSound::IsPlaying(int index) const
{
	// インデックスが設定されてる
	if (index >= 0)
	{
		// 範囲外チェック
		if (index >= mAudioSources.size()) return false;

		const CAudioSource& source = mAudioSources[index];
		IXAudio2SourceVoice* sv = source.voice;
		XAUDIO2_VOICE_STATE state;
		sv->GetState(&state);

		return state.BuffersQueued != 0;
	}
	// インデックスが設定されていない
	else
	{
		// 全てのサウンドを停止
		for (const CAudioSource& source : mAudioSources)
		{
			IXAudio2SourceVoice* sv = source.voice;
			XAUDIO2_VOICE_STATE state;
			sv->GetState(&state);
			if (state.BuffersQueued != 0) return true;
		}
	}

	return false;
}

// サウンドをフェード開始
void CSound::AddFadeSound(IXAudio2SourceVoice* voice, float fadeTime, float volume, bool isFadeIn)
{
	auto itr = mFadeSounds.begin();
	auto end = mFadeSounds.end();
	while (itr != end)
	{
		CFadeSound& sound = *itr;
		if (sound.voice == voice)
		{
			if (sound.isFadeIn == isFadeIn) return;
			mFadeSounds.erase(itr);
			break;
		}

		itr++;
	}

	CFadeSound newSound;
	newSound.voice = voice;
	newSound.fadeTime = fadeTime;
	newSound.elapsedTime = 0.0f;
	newSound.volume = volume;
	newSound.isFadeIn = isFadeIn;
	mFadeSounds.push_back(newSound);
}

// サウンドを同時再生できる数を設定
void CSound::SetSimultaneousPlayCount(int count)
{
	// 既に設定されている数より小さい値は設定できない
	if (mAudioSources.size() >= count) return;

	// 足りない数の分、AudioSourceを作成する
	int createCount = count - mAudioSources.size();
	for (int i = 0; i < createCount; i++)
	{
		CreateAudioSource();
	}
}

// サウンドのベース音量を設定
void CSound::SetBaseVolume(float volume)
{
	mBaseVolume = Math::Clamp01(volume);
}

// サウンドの音量を設定
void CSound::SetVolume(float volume, int index)
{
	// インデックスが設定されてる
	if (index >= 0)
	{
		// 範囲外チェック
		if (index >= mAudioSources.size()) return;

		const CAudioSource& source = mAudioSources[index];
		IXAudio2SourceVoice* sv = source.voice;
		sv->SetVolume(mBaseVolume * volume);
	}
	// インデックスが設定されていない
	else
	{
		// 全てのサウンドを停止
		for (const CAudioSource& source : mAudioSources)
		{
			IXAudio2SourceVoice* sv = source.voice;
			sv->SetVolume(mBaseVolume * volume);
		}
	}
}

// ループ範囲を設定
void CSound::SetLoopRange(int loopBegin, int loopLength)
{
	mLoopBegin = loopBegin;
	mLoopLength = loopLength;
}

// サウンドのサンプル数を取得
int CSound::GetSampleCount()
{
	return mSampleCount;
}

// 再生中のサウンドの現在のサンプル数を取得
int CSound::GetCurrentSampleCount(int index)
{
	if (!(0 <= index && index < mAudioSources.size())) return 0;

	CAudioSource& source = mAudioSources[index];
	IXAudio2SourceVoice* sv = source.voice;
	XAUDIO2_VOICE_STATE state;
	sv->GetState(&state);
	if (state.BuffersQueued == 0) return 0;

	int sampleMax = source.info.LoopLength == 0
		? GetSampleCount()
		: source.info.LoopLength;
	if (sampleMax == 0) return 0;

	return state.SamplesPlayed % sampleMax;
}

// 更新
void CSound::Update()
{
	//CAudioSource& source = mAudioSources[0];
	//IXAudio2SourceVoice* sv = source.voice;
	//XAUDIO2_VOICE_STATE state;
	//sv->GetState(&state);
	//CDebugPrint::Print("BufferQueued:%d\n", state.BuffersQueued);
	//int samplesPlayed = GetCurrentSampleCount();
	//CDebugPrint::Print("SamplesPlayed:%d\n", samplesPlayed);

	auto itr = mFadeSounds.begin();
	auto end = mFadeSounds.end();
	while (itr != end)
	{
		CFadeSound& sound = *itr;
		if (sound.elapsedTime <= sound.fadeTime)
		{
			float per = sound.elapsedTime / sound.fadeTime;
			if (!sound.isFadeIn) per = 1.0f - per;
			float volume = mBaseVolume * sound.volume * per;
			sound.voice->SetVolume(volume);
			//if (sound.isFadeIn && volume > 0.0f)
			//{
			//	sound.voice->Start();
			//}
			sound.elapsedTime += Time::DeltaTime();
		}
		else
		{
			if (!sound.isFadeIn)
			{
				sound.voice->Stop();
			}
			itr = mFadeSounds.erase(itr);
			continue;
		}

		itr++;
	}
}
