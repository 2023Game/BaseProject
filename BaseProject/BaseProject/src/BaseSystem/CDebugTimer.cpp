#include "CDebugTimer.h"

// コンストラクタ
CDebugTimer::CDebugTimer()
	: freq({ 0 })
	, startCount({ 0 })
	, endCount({ 0 })
	, isEnd(true)
	, time(0.0f)
{
}

// 時間計測開始
void CDebugTimer::Start()
{
	if (!isEnd) return;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&startCount);
	isEnd = false;
}

// 時間計測終了
void CDebugTimer::End()
{
	if (isEnd) return;

	QueryPerformanceCounter(&endCount);
	isEnd = true;

	time += (float)(endCount.QuadPart - startCount.QuadPart) / freq.QuadPart;
	startCount = { 0 };
	endCount = { 0 };
}

// 計測した時間をリセット
void CDebugTimer::Reset()
{
	isEnd = true;
	time = 0.0f;
	startCount = { 0 };
	endCount = { 0 };
}

// 計測した時間を取得
float CDebugTimer::Get() const
{
	if (isEnd)
	{
		return time;
	}
	else
	{
		LARGE_INTEGER tempCount;
		QueryPerformanceCounter(&tempCount);
		return time + (float)(tempCount.QuadPart - startCount.QuadPart) / freq.QuadPart;
	}
}

// 時間計測を終了しているか
bool CDebugTimer::IsEnd() const
{
	return isEnd;
}
