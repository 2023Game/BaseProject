#include "CDebugProfiler.h"
#include "CDebugTimer.h"
#include "Time.h"

// CDebugProfilerのインスタンス
CDebugProfiler* CDebugProfiler::mpInstance = nullptr;

// コンストラクタ
CDebugProfiler::CDebugProfiler()
	: CTask(ETaskPriority::eDebug, 0, ETaskPauseType::eDefault, true)
{
}

// デストラクタ
CDebugProfiler::~CDebugProfiler()
{
}

// インスタンス取得
CDebugProfiler* CDebugProfiler::Instance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new CDebugProfiler();
	}
	return mpInstance;
}

// 時間計測開始
void CDebugProfiler::StartTimer(std::string name)
{
	CDebugProfiler* inst = Instance();
	auto& timers = inst->mTimers;

	CDebugTimer* timer = timers[name];
	if (timer == nullptr)
	{
		timer = new CDebugTimer();
		timers[name] = timer;
 	}

	timer->Start();
}

// 時間計測終了
void CDebugProfiler::EndTimer(std::string name)
{
	CDebugProfiler* inst = Instance();
	auto& timers = inst->mTimers;

	CDebugTimer* timer = timers[name];
	if (timer == nullptr) return;

	timer->End();
}

// 計測結果を描画
void CDebugProfiler::Print()
{
	CDebugProfiler* inst = Instance();
	auto& timers = inst->mTimers;

	if (timers.size() > 0)
	{
		CDebugPrint::Print("■■■■■■■ Profiler ■■■■■■■\n");
		CDebugPrint::Print("%.1fFPS( Delta:%f秒 )\n", Time::FPS(), Time::DeltaTime());
		for (auto& timer : timers)
		{
			CDebugTimer* dt = timer.second;
			if (dt->IsEnd())
			{
				CDebugPrint::Print
				(
					"%s:経過時間[%f秒]:割合[%f%%%%]\n",
					timer.first.c_str(),
					dt->Get(),
					(dt->Get() / Time::DeltaTime()) * 100.0f
				);
			}
			else
			{
				CDebugPrint::Print
				(
					"%s:経過時間[%f秒]\n",
					timer.first.c_str(),
					dt->Get()
				);
			}
			dt->Reset();
		}
		CDebugPrint::Print("■■■■■■■■■■■■■■■■■■■\n");
	}
}
