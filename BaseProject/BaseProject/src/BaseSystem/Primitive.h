#pragma once
#include "CVector.h"
#include "CColor.h"

// 基本図形の描画関連
namespace Primitive
{
	/// <summary>
	/// 線分を描画（3D）
	/// </summary>
	/// <param name="s">始点</param>
	/// <param name="e">終点</param>
	/// <param name="color">色</param>
	/// <param name="lineWidth">線の太さ</param>
	void DrawLine(const CVector& s, const CVector& e, const CColor& color, float lineWidth = 1.0f);
	/// <summary>
	/// 線分を描画（2D）
	/// </summary>
	/// <param name="s">始点</param>
	/// <param name="e">終点</param>
	/// <param name="color">色</param>
	/// <param name="lineWidth">線の太さ</param>
	void DrawLine2D(const CVector2& s, const CVector2& e, const CColor& color, float lineWidth = 1.0f);

	/// <summary>
	/// 球を描画
	/// </summary>
	/// <param name="pos">描画行列</param>
	/// <param name="rad">半径</param>
	/// <param name="color">色</param>
	void DrawSphere(const CMatrix& m, float rad, const CColor& color);
	/// <summary>
	/// ワイヤーフレームの球を描画
	/// </summary>
	/// <param name="pos">描画行列</param>
	/// <param name="rad">半径</param>
	/// <param name="color">色</param>
	void DrawWireSphere(const CMatrix& m, float rad, const CColor& color);

	/// <summary>
	/// 三角形を描画
	/// </summary>
	/// <param name="v0">1つ目の頂点</param>
	/// <param name="v1">2つ目の頂点</param>
	/// <param name="v2">3つ目の頂点</param>
	/// <param name="color">色</param>
	void DrawTriangle(const CVector& v0, const CVector& v1, const CVector& v2, const CColor& color);

	/// <summary>
	/// 板ポリゴンを描画
	/// </summary>
	/// <param name="m">描画行列</param>
	/// <param name="size">縦横のサイズ</param>
	/// <param name="color">色</param>
	void DrawQuad(const CMatrix& m, const CVector2& size, const CColor& color);

	/// <summary>
	/// カプセルを描画
	/// </summary>
	/// <param name="sp">カプセルを構成する線分の始点</param>
	/// <param name="ep">カプセルを構成する線分の終点</param>
	/// <param name="rad">カプセルの半径</param>
	/// <param name="color">色</param>
	void DrawCapsule(const CVector& sp, const CVector& ep, float rad, const CColor& color);

	/// <summary>
	/// ワイヤーフレームのカプセルを描画
	/// </summary>
	/// <param name="sp">カプセルを構成する線分の始点</param>
	/// <param name="ep">カプセルを構成する線分の終点</param>
	/// <param name="rad">カプセルの半径</param>
	/// <param name="color">色</param>
	void DrawWireCapsule(const CVector& sp, const CVector& ep, float rad, const CColor& color);
};