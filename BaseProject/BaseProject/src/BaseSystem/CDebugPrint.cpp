#include "CDebugPrint.h"
#include <stdarg.h>
#include "CText.h"

// インスタンス
CDebugPrint* CDebugPrint::mspInstance = nullptr;

// コンストラクタ
CDebugPrint::CDebugPrint()
	: CTask(ETaskPriority::eDebug, 0, ETaskPauseType::eDefault, true)
{
	// 文字列描画用のクラスを生成
	mpText = new CText
	(
		nullptr, 24,
		CVector2::zero,
		CVector2(WINDOW_WIDTH, WINDOW_HEIGHT),
		CColor::red,
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		true
	);
}

// 描画するデバッグ文字列を追加
void CDebugPrint::Print(const char* format, ...)
{
	if (mspInstance == nullptr)
	{
		mspInstance = new CDebugPrint();
	}

	char buf[512];
	va_list ap;
	va_start(ap, format);
	vsprintf_s(buf, format, ap);
	va_end(ap);
	mspInstance->AddStr(buf);
}

// 描画する文字列をリストに追加
void CDebugPrint::AddStr(std::string str)
{
	mPrintStrList.push_back(str);
}

// 更新
void CDebugPrint::Update()
{
	std::string str = "";
	for (std::string s : mPrintStrList)
	{
		str += s;
	}
	mpText->SetText(str.c_str());
	mPrintStrList.clear();
}

// 描画
void CDebugPrint::Render()
{
}