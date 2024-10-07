#include "CTextureFont.h"

void CTextureFont::Load(const char* file, int row, int col)
{
	mpTexture = CResourceManager::Load<CTexture>("", file);
	mpTexture->SetRowCol(row, col);
}

void CTextureFont::Draw(float x, float y, float w, float h, char c)
{
	int i = c - 0x20;
	mpTexture->DrawImage(x - w, x + w, y - h, y + h, i);
}

void CTextureFont::Draw(float x, float y, float w, float h, char c[])
{
	for (int i = 0; c[i] != '\0'; i++)
	{
		Draw(x, y, w, h, c[i]);
		x += w * 2;
	}
	return;
}
