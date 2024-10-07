#include "CDebugPrint.h"
#include <stdarg.h>
#include "CText.h"

// �C���X�^���X
CDebugPrint* CDebugPrint::mspInstance = nullptr;

// �R���X�g���N�^
CDebugPrint::CDebugPrint()
	: CTask(ETaskPriority::eDebug, 0, ETaskPauseType::eDefault, true)
{
	// ������`��p�̃N���X�𐶐�
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

// �`�悷��f�o�b�O�������ǉ�
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

// �`�悷�镶��������X�g�ɒǉ�
void CDebugPrint::AddStr(std::string str)
{
	mPrintStrList.push_back(str);
}

// �X�V
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

// �`��
void CDebugPrint::Render()
{
}