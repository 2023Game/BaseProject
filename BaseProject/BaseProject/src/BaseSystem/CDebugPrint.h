#pragma once
#include <vector>
#include <string>
#include "CTask.h"

class CText;

// 画面上にデバッグ文字を表示するクラス
class CDebugPrint : public CTask
{
public:
	/// <summary>
	/// 描画するデバッグ文字列を追加
	/// </summary>
	/// <param name="format">書式文字列</param>
	/// <param name="第2引数以降">文字列に変換する引数</param>
	static void Print(const char* format, ...);

private:
	// コンストラクタ
	CDebugPrint();

	// 描画する文字列をリストに追加
	void AddStr(std::string str);

	// 更新
	void Update();
	// 描画
	void Render();

	static CDebugPrint* mspInstance;		// インスタンス
	std::vector<std::string> mPrintStrList;	// 描画文字列のリスト
	CText* mpText;	// 文字列描画用
};