#pragma once
#include "CUIBase.h"
#include "CFont.h"
#include "CVector.h"
#include "CColor.h"

enum class ETextAlignH
{
	eLeft,	// ç∂ëµÇ¶
	eCenter,// íÜâõëµÇ¶
	eRight,	// âEëµÇ¶
};

enum class ETextAlignV
{
	eTop,	// è„ëµÇ¶
	eMiddle,// è„â∫íÜâõëµÇ¶
	eBottom,// â∫ëµÇ¶
};

class CText : public CUIBase
{
public:
	CText(CFont* font = nullptr, int fontSize = 24,
		const CVector2& pos = CVector2::zero,
		const CVector2& size = CVector2(400.0f, 300.0f),
		const CColor& color = CColor::black,
		ETaskPriority prio = ETaskPriority::eUI,
		int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eDefault,
		bool dontDelete = false,
		bool addTaskList = true);
	~CText();

	void SetFontSize(int fontSize);

	void SetSize(const CVector2& size);

	void SetTextAlign(ETextAlignH alignH, ETextAlignV alignV);
	void SetTextAlignH(ETextAlignH align);
	void SetTextAlignV(ETextAlignV align);

	void SetEnableShadow(bool enable);
	void SetShadowOffsetPos(const CVector2& offset);
	void SetShadowColor(const CColor& color);

	void SetEnableOutline(bool enable);
	//void SetOutlineWidth(float width);
	void SetOutlineColor(const CColor& color);

	void SetText(const char* format, ...);
	void SetText(const wchar_t* format, ...);

	void Render();

private:
	void CalcLineCount();
	void RenderText(CVector2 pos, const CColor& color, const CColor& outlineColor);

	CFont* mpFont;
	std::wstring mText;
	int mFontSize;
	int mLineCount;
	ETextAlignH mTextAlignH;
	ETextAlignV mTextAlignV;

	bool mIsEnableShadow;
	CVector2 mShadowPos;
	CColor mShadowColor;

	bool mIsEnableOutline;
	float mOutlineWidth;
	CColor mOutlineColor;
};