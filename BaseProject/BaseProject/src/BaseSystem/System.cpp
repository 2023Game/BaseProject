#include "System.h"
#include "CColor.h"
#include "CVector.h"
#include <glut.h>

// ゲームの終了フラグ
bool gIsExitGame = false;
// モーションブラーが有効かどうか
bool gIsEnableMotionBlur = false;
// モーションブラーの方向
CVector gMotionBlurDir = CVector(0.0f, 0.0f, 1.0f);
// モーションブラーの幅
float gMotionBlurWidth = 1.0f;
// モーションブラーの反復回数
int gMotionBlurCount = 2;

// ゲームを終了
void System::ExitGame()
{
	gIsExitGame = true;
}

// ゲームを終了するかどうかを返す
bool System::IsExitGame()
{
	return gIsExitGame;
}

// 画面のクリアカラーを設定（rgba）
void System::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

// 画面のクリアカラーを設定（CColor）
void System::SetClearColor(const CColor& color)
{
	SetClearColor(color.R(), color.G(), color.B(), color.A());
}

// モーションブラーの有効無効設定
void System::SetEnableMotionBlur(bool enable)
{
	gIsEnableMotionBlur = enable;
}

// モーションブラーが有効かどうか
bool System::IsEnableMotionBlur()
{
	return gIsEnableMotionBlur;
}

// モーションブラーのパラメータ設定
void System::SetMotionBlur(const CVector& dir, float width, int count)
{
	gMotionBlurDir = dir;
	gMotionBlurWidth = width;
	gMotionBlurCount = count;
}

// モーションブラーのパラメータを取得
void System::GetMotionBlurParam(CVector* outDir, float* outWidth, int* outCount)
{
	*outDir = gMotionBlurDir;
	*outWidth = gMotionBlurWidth;
	*outCount = gMotionBlurCount;
}
