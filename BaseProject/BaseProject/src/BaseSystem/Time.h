#pragma once

namespace Time
{
	// 目標フレームレートを取得
	int TargetFPS();
	// 計算上での1フレームの経過時間を取得
	float CalcDeltaTime();
	// 前回のフレームのFPSを取得
	float FPS();
	// 前回のフレームの経過時間を取得
	float DeltaTime();
}