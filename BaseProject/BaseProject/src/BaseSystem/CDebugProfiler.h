#pragma once
#include <map>
#include <string>
#include "CTask.h"
class CDebugTimer;

// デバッグ用の処理時間を計測して表示するクラス
class CDebugProfiler : public CTask
{
public:
	// デストラクタ
	~CDebugProfiler();

	// 時間計測開始
	static void StartTimer(std::string name);
	// 時間計測終了
	static void EndTimer(std::string name);

	// 計測結果を描画
	static void Print();

private:
	// CDebugProfilerのインスタンス
	static CDebugProfiler* mpInstance;
	// 処理時間計測中のタイマーのリスト
	std::map<std::string, CDebugTimer*> mTimers;

	// コンストラクタ
	CDebugProfiler();
	// インスタンス取得
	static CDebugProfiler* Instance();
};