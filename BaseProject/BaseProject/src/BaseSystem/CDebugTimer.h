#pragma once
#include <windows.h>

// デバッグ用の時間計測クラス
class CDebugTimer
{
public:
	// コンストラクタ
	CDebugTimer();

	// 時間計測開始
	void Start();
	// 時間計測終了
	void End();
	// 計測した時間をリセット
	void Reset();
	// 計測した時間を取得
	float Get() const;
	// 時間計測を終了しているか
	bool IsEnd() const;

private:
	LARGE_INTEGER freq;
	LARGE_INTEGER startCount;
	LARGE_INTEGER endCount;
	bool isEnd;
	float time;
};
