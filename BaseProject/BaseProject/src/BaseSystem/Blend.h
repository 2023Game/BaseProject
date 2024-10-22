#pragma once

// ブレンド処理のタイプ
enum class EBlend
{
	eAlpha,		// アルファブレンド
	eAdd,		// 加算ブレンド
	eMultiply,	// 乗算ブレンド
	eInvert,	// 色反転
};

namespace Blend
{
	/// <summary>
	/// 指定したブレンド処理を有効化
	/// </summary>
	/// <param name="blend">ブレンド処理のタイプ</param>
	void EnableBlend(EBlend blend);
	/// <summary>
	/// ブレンド処理を無効化
	/// </summary>
	void DisableBlend();
}