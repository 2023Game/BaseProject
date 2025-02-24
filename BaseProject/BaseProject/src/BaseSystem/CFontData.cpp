#include "CFontData.h"
#include <glut.h>

CFontData::CFontData()
	: mpFont(nullptr)
	, mpFontLayout(nullptr)
{
}

CFontData::~CFontData()
{
	SAFE_DELETE(mpFont);
}

bool CFontData::Load(std::string path, bool dontDelete)
{
	dontDelete = true;
	if (path.empty()) return false;

	mpFont = new FTPixmapFont(path.c_str());
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

	return true;
}

void CFontData::SetDontDelete(bool del)
{
	CResource::SetDontDelete(true);
}

void CFontData::SetFontSize(int fontSize)
{
	if (mpFont == nullptr) return;
	mpFont->FaceSize(fontSize);
}

void CFontData::SetAlignment(FTGL::TextAlignment align)
{
	if (mpFontLayout == nullptr) return;
	mpFontLayout->SetAlignment(align);
}

void CFontData::SetLineLength(float length)
{
	if (mpFontLayout == nullptr) return;
	mpFontLayout->SetLineLength(length);
}

void CFontData::SetLineSpacing(float spacing)
{
	if (mpFontLayout == nullptr) return;
	mpFontLayout->SetLineSpacing(spacing);
}

void CFontData::Render(const char* str)
{
	if (mpFont == nullptr) return;
	mpFontLayout->Render(str);
}

void CFontData::Render(const wchar_t* str)
{
	if (mpFont == nullptr) return;
	mpFontLayout->Render(str);
}
