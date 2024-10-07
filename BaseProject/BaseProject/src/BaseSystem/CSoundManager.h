#pragma once
#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>
#include <xaudio2.h>
#include <list>

#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib,"winmm.lib")

class CSound;

// サウンド再生管理クラス
class CSoundManager
{
public:
	// インスタンスを取得
	static CSoundManager* Instance();
	// インスタンスを破棄
	static void ClearInstance();

	/// <summary>
	/// 音声データを再生するためのソースボイスを生成
	/// </summary>
	/// <param name="format">音声データのフォーマット</param>
	/// <returns>生成したソースボイスのポインター</returns>
	IXAudio2SourceVoice* CreateSourceVoice(const WAVEFORMATEX* format);

	// サウンドをリストに追加
	void Add(CSound* sound);
	// サウンドをリストから取り除く
	void Remove(CSound* sound);

	// 更新
	void Update();

private:
	// コンストラクタ
	CSoundManager();
	// デストラクタ
	~CSoundManager();

	// サウンドマネージャのインスタンス
	static CSoundManager* mpInstance;
	bool mIsInitialized;		// 初期化済みか
	IXAudio2* mpXAudio;
	IXAudio2MasteringVoice* mpMasterVoice;
	std::list<CSound*> mSounds;	// 生成済みのサウンドのリスト
};