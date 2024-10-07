#pragma once
class CColor;
class CVector;

namespace System
{
	/// <summary>
	/// ゲームを終了
	/// </summary>
	void ExitGame();
	/// <summary>
	/// ゲームを終了するかどうかを返す
	/// </summary>
	/// <returns>trueなら、ゲームを終了</returns>
	bool IsExitGame();

	/// <summary>
	/// 画面のクリアから―を設定（rgba）
	/// </summary>
	/// <param name="r">設定する色の赤要素</param>
	/// <param name="g">設定する色の緑要素</param>
	/// <param name="b">設定する色の青要素</param>
	/// <param name="a">設定する色のアルファ値</param>
	void SetClearColor(float r, float g, float b, float a);
	/// <summary>
	/// 画面のクリアカラーを設定（CColor）
	/// </summary>
	/// <param name="color">設定する色</param>
	void SetClearColor(const CColor& color);

	/// <summary>
	/// モーションブラーの有効無効設定
	/// </summary>
	/// <param name="enable">trueならば、有効</param>
	void SetEnableMotionBlur(bool enable);
	/// <summary>
	/// モーションブラーが有効かどうか
	/// </summary>
	/// <returns>trueならば、有効</returns>
	bool IsEnableMotionBlur();
	/// <summary>
	/// モーションブラーのパラメータ設定
	/// </summary>
	/// <param name="dir">ブラーを掛ける方向</param>
	/// <param name="width">ブラーを掛ける幅</param>
	/// <param name="count">ブラーを掛ける回数</param>
	void SetMotionBlur(const CVector& dir, float width, int count);
	/// <summary>
	/// モーションブラーのパラメータを取得
	/// </summary>
	/// <param name="outDir">ブラーの方向を返す用のポインタ</param>
	/// <param name="outWidth">ブラーの幅を返す用のポインタ</param>
	/// <param name="outCount">ブラーの回数を返す用のポインタ</param>
	void GetMotionBlurParam(CVector* outDir, float* outWidth, int* outCount);
}