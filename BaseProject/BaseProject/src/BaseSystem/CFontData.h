#pragma once
#include "CResource.h"
#include <FTGL/ftgl.h>
#include <string>

// フォントデータのリソース
class CFontData : public CResource
{
	friend CResourceManager;

public:
	void SetDontDelete(bool del) override;

	void SetFontSize(int fontSize);
	void SetAlignment(FTGL::TextAlignment align);
	void SetLineLength(float length);
	void SetLineSpacing(float spacing);

	void Render(const char* str);
	void Render(const wchar_t* str);

private:
	CFontData();
	~CFontData();

	// フォント読み込み
	bool Load(std::string path, bool dontDelete) override;

	FTPixmapFont* mpFont;
	FTSimpleLayout* mpFontLayout;
};