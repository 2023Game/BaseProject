#include "CFont.h"
#include <glut.h>
#include "CFontData.h"
#include "CResourceManager.h"

#define DEFAULT_FONT_PATH "C:\\Windows\\Fonts\\msgothic.ttc"

CFont::CFont(const char* path)
	: mpFontData(nullptr)
	, mFontSize(0)
	, mTextAlign(FTGL::TextAlignment::ALIGN_LEFT)
	, mLineLength(100.0f)
	, mLineSpacing(1.0f)
{
	if (path == nullptr || strlen(path) == 0)
	{
		path = DEFAULT_FONT_PATH;
	}

	mpFontData = CResourceManager::Load<CFontData>(path, path);
}

CFont::~CFont()
{
}

void CFont::SetFontSize(int fontSize)
{
	mFontSize = fontSize;
}

void CFont::SetAlignment(FTGL::TextAlignment align)
{
	mTextAlign = align;
}

void CFont::SetLineLength(float length)
{
	mLineLength = length;
}

void CFont::SetLineSpacing(float spacing)
{
	mLineSpacing = spacing;
}

void CFont::Render(const char* str)
{
	mpFontData->SetFontSize(mFontSize);
	mpFontData->SetAlignment(mTextAlign);
	mpFontData->SetLineLength(mLineLength);
	mpFontData->SetLineSpacing(mLineSpacing);
	mpFontData->Render(str);
}

void CFont::Render(const wchar_t* str)
{
	mpFontData->SetFontSize(mFontSize);
	mpFontData->SetAlignment(mTextAlign);
	mpFontData->SetLineLength(mLineLength);
	mpFontData->SetLineSpacing(mLineSpacing);
	mpFontData->Render(str);
}
