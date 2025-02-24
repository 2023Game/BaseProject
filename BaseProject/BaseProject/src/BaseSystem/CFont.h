#pragma once
#include <FTGL/ftgl.h>
#include <string>

class CFontData;

class CFont
{
public:
	CFont(const char* path = "");
	~CFont();

	void SetFontSize(int fontSize);
	void SetAlignment(FTGL::TextAlignment align);
	void SetLineLength(float length);
	void SetLineSpacing(float spacing);

	void Render(const char* str);
	void Render(const wchar_t* str);

private:
	CFontData* mpFontData;
	int mFontSize;
	FTGL::TextAlignment mTextAlign;
	float mLineLength;
	float mLineSpacing;
};
