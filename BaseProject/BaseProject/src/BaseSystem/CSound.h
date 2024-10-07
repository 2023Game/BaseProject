#pragma once
#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>
#include <xaudio2.h>
#include <vector>
#include <list>
#include "CResource.h"

#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib,"winmm.lib")

class CAudioSource;
class CFadeSound;
class CSoundManager;

// サウンド再生クラス
class CSound : public CResource
{
	friend CResourceManager;
	friend CSoundManager;
public:
	/// <summary>
	/// サウンドを再生
	/// </summary>
	/// <param name="volume">ボリューム</param>
	/// <param name="force">再生枠が足りない時に、既に再生中の音声を停止して再生するかどうか</param>
	/// <param name="fadeTime">再生時のフェードイン時間</param>
	/// <returns></returns>
	int Play(float volume = 1.0f, bool force = false, float fadeTime = 0.0f);
	/// <summary>
	/// サウンドをループ再生
	/// </summary>
	/// <param name="loopCount">ループ数（0:ループなし、-1:永続ループ）</param>
	/// <param name="fromBegin">最初から再生するかどうか</param>
	/// <param name="volume">ボリューム</param>
	/// <param name="force">再生枠が足りない時に、既に再生中の音声を停止して再生するかどうか</param>
	/// <param name="fadeTime">再生時のフェードイン時間</param>
	/// <returns></returns>
	int PlayLoop(int loopCount = -1, bool fromBegin = true, float volume = 1.0f, bool force = false, float fadeTime = 0.0f);

	/// <summary>
	/// サウンドを停止
	/// </summary>
	/// <param name="index">停止するインデックス（指定なければ、全て停止）</param>
	/// <param name="fadeTime">停止時のフェードアウト時間</param>
	void Stop(int index = -1, float fadeTime = 0.125f);

	/// <summary>
	/// サウンドを再生中かどうか
	/// </summary>
	/// <returns></returns>
	bool IsPlaying(int index = -1) const;

	/// <summary>
	/// サウンドを同時再生できる数を設定
	/// </summary>
	/// <param name="count">同時再生できる数</param>
	void SetSimultaneousPlayCount(int count);

	/// <summary>
	/// サウンドのベース音量を設定
	/// </summary>
	/// <param name="volume">ベース音量</param>
	void SetBaseVolume(float volume);

	/// <summary>
	/// サウンドの音量を設定
	/// </summary>
	/// <param name="volume">設定する音量</param>
	/// <param name="index">設定するインデックス（指定なければ、全てに設定）</param>
	void SetVolume(float volume, int index = -1);

	/// <summary>
	/// ループ範囲を設定
	/// </summary>
	/// <param name="loopBegin">ループ開始位置</param>
	/// <param name="loopLength">ループの長さ</param>
	void SetLoopRange(int loopBegin, int loopLength);

	/// <summary>
	/// サウンドのサンプル数を取得
	/// </summary>
	/// <returns></returns>
	int GetSampleCount();
	/// <summary>
	/// 再生中のサウンドの現在のサンプル数を取得
	/// </summary>
	/// <returns></returns>
	int GetCurrentSampleCount(int index = 0);

private:
	// コンストラクタ
	CSound();
	// デストラクタ
	~CSound();

	/// <summary>
	/// 音声ファイルを読み込み
	/// </summary>
	/// <param name="path">読み込む音声ファイルのパス</param>
	/// <returns></returns>
	bool Load(std::string path, bool dontDelete) override;

	// 音声再生用の情報データを作成
	bool CreateAudioSource();

	// 音声を再生していないインデックスを取得
	int FindNotPlayingSourceVoice(bool force) const;

	/// <summary>
	/// サウンドをフェード開始
	/// </summary>
	/// <param name="voice">フェード処理を行うサウンド</param>
	/// <param name="fadeTime">フェード時間</param>
	/// <param name="volume">ボリューム</param>
	/// <param name="isFadeIn">フェードインかどうか</param>
	void AddFadeSound(IXAudio2SourceVoice* voice, float fadeTime, float volume, bool isFadeIn);

	// 更新
	void Update();

	// 音声を再生するソースボイスのリスト
	std::vector<CAudioSource> mAudioSources;
	// フェード中のサウンドのリスト
	std::list<CFadeSound> mFadeSounds;

	HMMIO g_hmmio;
	MMIOINFO g_mmioinfo;
	MMCKINFO g_riffchunkinfo;
	MMCKINFO g_datachunkinfo;
	WAVEFORMATEX g_wfx;
	LONG g_readlen;
	unsigned char* mpBuf;
	int mSampleCount;
	int mLoopBegin;
	int mLoopLength;

	float mBaseVolume;	// ベース音量
};

// 音声再生用の情報
class CAudioSource
{
public:
	// 音声を再生するソースボイス
	IXAudio2SourceVoice* voice;
	// 音声の再生情報
	XAUDIO2_BUFFER info;
};

// 音声フェード用の情報
class CFadeSound
{
public:
	IXAudio2SourceVoice* voice;
	float fadeTime;
	float elapsedTime;
	float volume;
	bool isFadeIn;
};
