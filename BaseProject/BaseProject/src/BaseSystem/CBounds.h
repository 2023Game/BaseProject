#pragma once

// バウンディングボックス情報
class CBounds
{
public:
	// コンストラクタ
	CBounds();
	// デストラクタ
	~CBounds();

	/// <summary>
	/// 最小位置と最大位置で設定
	/// </summary>
	/// <param name="min">最小位置</param>
	/// <param name="max">最大位置</param>
	void SetRange(const CVector& min, const CVector& max);	/// <summary>
	/// 中心位置とサイズで設定
	/// </summary>
	/// <param name="center">中心位置</param>
	/// <param name="size">サイズ</param>
	void SetPos(const CVector& center, const CVector& size);

	// 最小位置を設定
	void Min(const CVector& min);
	// 最小位置を取得
	const CVector& Min() const;
	// 最大位置を設定
	void Max(const CVector& max);
	// 最大位置を取得
	const CVector& Max() const;

	// 中心位置を設定
	void Center(const CVector& center);
	// 中心位置を取得
	CVector Center() const;
	// サイズを設定
	void Size(const CVector& size);
	// サイズを取得
	CVector Size() const;

	/// <summary>
	/// 2つのバウンディングボックスが交差するかどうか
	/// </summary>
	/// <param name="b0">1つ目のバウンディングボックス</param>
	/// <param name="b1">2つ目のバウンディングボックス</param>
	/// <returns>trueならば、交差している</returns>
	static bool Intersect(const CBounds& b0, const CBounds& b1);

	/// <summary>
	/// 線分のバウンディングボックスを取得
	/// </summary>
	/// <param name="ls">線分の始点</param>
	/// <param name="le">線分の終点</param>
	/// <returns>バウンディングボックス</returns>
	static CBounds GetLineBounds(const CVector& ls, const CVector& le);
	/// <summary>
	/// 球のバウンディングボックスを取得
	/// </summary>
	/// <param name="pos">球の座標</param>
	/// <param name="rad">球の半径</param>
	/// <returns>バウンディングボックス</returns>
	static CBounds GetSphereBounds(const CVector& pos, float rad);
	/// <summary>
	/// 三角形のバウンディングボックスを取得
	/// </summary>
	/// <param name="t0">三角形の1つ目の頂点</param>
	/// <param name="t1">三角形の2つ目の頂点</param>
	/// <param name="t2">三角形の3つ目の頂点</param>
	/// <returns>バウンディングボックス</returns>
	static CBounds GetTriangleBounds(const CVector& t0, const CVector& t1, const CVector& t2);
	/// <summary>
	/// カプセルのバウンディングボックスを取得
	/// </summary>
	/// <param name="cs">カプセルを構成する線分の始点</param>
	/// <param name="ce">カプセルを構成する線分の終点</param>
	/// <param name="cr">カプセルの半径</param>
	/// <returns>バウンディングボックス</returns>
	static CBounds GetCapsuleBounds(const CVector& cs, const CVector& ce, float cr);

private:
	CVector mMin;	// 最小位置
	CVector mMax;	// 最大位置
};
