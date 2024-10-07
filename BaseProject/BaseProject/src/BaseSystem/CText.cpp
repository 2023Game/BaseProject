#include "CText.h"
#include <glut.h>
#include <locale.h>

CText::CText(CFont* font, int fontSize, const CVector2& pos,
	const CVector2& size, const CColor& color,
	ETaskPriority prio, int sortOrder, ETaskPauseType pause,
	bool dontDelete, bool addTaskList)
	: CUIBase(prio, sortOrder, pause, dontDelete, addTaskList)
	, mpFont(font)
	, mText(L"")
	, mFontSize(fontSize)
	, mLineCount(0)
	, mTextAlignH(ETextAlignH::eLeft)
	, mTextAlignV(ETextAlignV::eTop)
	, mIsEnableShadow(false)
	, mShadowPos(CVector2(2.0f, 2.0f))
	, mShadowColor(CColor::black)
	, mIsEnableOutline(false)
	, mOutlineWidth(1.0f)
	, mOutlineColor(CColor::black)
{
	mPosition = pos;
	mSize = size;
	mColor = color;

	if (mpFont == nullptr)
	{
		mpFont = new CFont();
		mpFont->SetFontSize(mFontSize);
		mpFont->SetLineLength(mSize.X());
	}
}

CText::~CText()
{
}

void CText::SetFontSize(int fontSize)
{
	mFontSize = fontSize;

	if (mpFont == nullptr) return;
	mpFont->SetFontSize(fontSize);
}

void CText::SetSize(const CVector2& size)
{
	mSize = size;
	mpFont->SetLineLength(size.X());
}

void CText::SetTextAlign(ETextAlignH alignH, ETextAlignV alignV)
{
	SetTextAlignH(alignH);
	SetTextAlignV(alignV);
}

void CText::SetTextAlignH(ETextAlignH align)
{
	mTextAlignH = align;
	switch (mTextAlignH)
	{
	case ETextAlignH::eLeft:
		mpFont->SetAlignment(FTGL::ALIGN_LEFT);
		break;
	case ETextAlignH::eCenter:
		mpFont->SetAlignment(FTGL::ALIGN_CENTER);
		break;
	case ETextAlignH::eRight:
		mpFont->SetAlignment(FTGL::ALIGN_RIGHT);
		break;
	}
}

void CText::SetTextAlignV(ETextAlignV align)
{
	mTextAlignV = align;
	switch (mTextAlignV)
	{
	case ETextAlignV::eTop:
		break;
	case ETextAlignV::eMiddle:
		break;
	case ETextAlignV::eBottom:
		break;
	}
}

void CText::SetEnableShadow(bool enable)
{
	mIsEnableShadow = enable;
}

void CText::SetShadowOffsetPos(const CVector2& offset)
{
	mShadowPos = offset;
}

void CText::SetShadowColor(const CColor& color)
{
	mShadowColor = color;
}

void CText::SetEnableOutline(bool enable)
{
	mIsEnableOutline = enable;
}

//void CText::SetOutlineWidth(float width)
//{
//	mOutlineWidth = width;
//}

void CText::SetOutlineColor(const CColor& color)
{
	mOutlineColor = color;
}

void CText::SetText(const char* format, ...)
{
	static const int size = 1024;
	char buf[size];
	va_list ap;
	va_start(ap, format);
	vsprintf_s(buf, format, ap);
	va_end(ap);

	wchar_t wbuf[size];
	setlocale(LC_CTYPE, "jpn");
	size_t len = mbstowcs(wbuf, buf, size);
	if (len >= 0)
	{
		mText = wbuf;
	}
	else
	{
		mText = L"";
	}
	CalcLineCount();
}

void CText::SetText(const wchar_t* format, ...)
{
	wchar_t wbuf[256];
	va_list ap;
	va_start(ap, format);
	swprintf_s(wbuf, format, ap);
	va_end(ap);

	mText = wbuf;
	CalcLineCount();
}

void CText::CalcLineCount()
{
	mLineCount = 0;

	int size = mText.size();
	for (int i = 0; i < size; i++)
	{
		if (mText.at(i) == '\n')
		{
			mLineCount++;
		}
	}
}

void CText::RenderText(CVector2 pos, const CColor& color, const CColor& outlineColor)
{
	if (mIsEnableOutline)
	{
		if (pos.X() - mOutlineWidth < 0.0f)
		{
			pos.X(pos.X() + mOutlineWidth);
		}
		glColor4f(outlineColor.R(), outlineColor.G(), outlineColor.B(), mColor.A());

		glRasterPos2f(pos.X() - mOutlineWidth, pos.Y() - mOutlineWidth);
		mpFont->Render(mText.c_str());
		glRasterPos2f(pos.X() + mOutlineWidth, pos.Y() - mOutlineWidth);
		mpFont->Render(mText.c_str());
		glRasterPos2f(pos.X() + mOutlineWidth, pos.Y() + mOutlineWidth);
		mpFont->Render(mText.c_str());
		glRasterPos2f(pos.X() - mOutlineWidth, pos.Y() + mOutlineWidth);
		mpFont->Render(mText.c_str());
	}

	glColor4f(color.R(), color.G(), color.B(), mColor.A());
	glRasterPos2f(pos.X(), pos.Y());
	mpFont->Render(mText.c_str());
}

void CText::Render()
{
	glPushMatrix();

	CVector pos = mPosition;
	if (mTextAlignV == ETextAlignV::eTop)
	{
		pos.Y(pos.Y() + mFontSize);
	}
	else if (mTextAlignV == ETextAlignV::eMiddle)
	{
		pos.Y(pos.Y() + mSize.Y() * 0.5f - (mLineCount - 1) * 0.5f * mFontSize);
	}
	else if (mTextAlignV == ETextAlignV::eBottom)
	{
		pos.Y(pos.Y() + mSize.Y() - mLineCount * mFontSize);
	}

	if (mIsEnableShadow)
	{
		RenderText(pos + mShadowPos, mShadowColor, mShadowColor);
	}

	RenderText(pos, mColor, mOutlineColor);

	glPopMatrix();
}