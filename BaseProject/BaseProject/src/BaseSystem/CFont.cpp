#include "CFont.h"
#include <glut.h>

#define DEFAULT_FONT_PATH "C:\\Windows\\Fonts\\msgothic.ttc"

CFont::CFont(const char* path)
	: mpFontLayout(nullptr)
{
	if (path == nullptr || strlen(path) == 0)
	{
		path = DEFAULT_FONT_PATH;
	}

	mpFont = new FTPixmapFont(path);
	if (mpFont->Error() == FT_Err_Ok)
	{
		mpFontLayout = new FTSimpleLayout();
		mpFontLayout->SetFont(mpFont);
	}
	else
	{
		delete mpFont;
		mpFont = nullptr;
	}
}

CFont::~CFont()
{
	delete mpFont;
	mpFont = nullptr;
}

void CFont::SetFontSize(int fontSize)
{
	if (mpFont == nullptr) return;
	mpFont->FaceSize(fontSize);
}

void CFont::SetAlignment(FTGL::TextAlignment align)
{
	if (mpFontLayout == nullptr) return;
	mpFontLayout->SetAlignment(align);
}

void CFont::SetLineLength(float length)
{
	if (mpFontLayout == nullptr) return;
	mpFontLayout->SetLineLength(length);
}

void CFont::SetLineSpacing(float spacing)
{
	if (mpFontLayout == nullptr) return;
	mpFontLayout->SetLineSpacing(spacing);
}

void CFont::Render(const char* str)
{
	if (mpFont == nullptr) return;
	mpFontLayout->Render(str);
}

void CFont::Render(const wchar_t* str)
{
	if (mpFont == nullptr) return;
	mpFontLayout->Render(str);
}
