#include "Blend.h"
#include "glut.h"

// 指定したブレンド処理を有効化
void Blend::EnableBlend(EBlend blend)
{
	//ブレンド処理を有効
	glEnable(GL_BLEND);

	//ブレンドタイプによって処理を切り替え
	switch (blend)
	{
		// アルファブレンド
		case EBlend::eAlpha:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		// 加算ブレンド
		case EBlend::eAdd:
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		// 乗算ブレンド
		case EBlend::eMultiply:
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);
			break;
		// 色反転
		case EBlend::eInvert:
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
			break;
	}
}

// ブレンド処理を無効化
void Blend::DisableBlend()
{
	glDisable(GL_BLEND);
}
