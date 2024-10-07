#include "CBounds.h"

// コンストラクタ
CBounds::CBounds()
{
}

// デストラクタ
CBounds::~CBounds()
{
}

// 最小位置と最大位置で設定
void CBounds::SetRange(const CVector& min, const CVector& max)
{
	mMin = min;
	mMax = max;
}

// 中心位置とサイズで設定
void CBounds::SetPos(const CVector& center, const CVector& size)
{
	mMin = center - size * 0.5f;
	mMax = center + size * 0.5f;
}

// 最小位置を設定
void CBounds::Min(const CVector& min)
{
	mMin = min;
}

// 最小位置を取得
const CVector& CBounds::Min() const
{
	return mMin;
}

// 最大位置を設定
void CBounds::Max(const CVector& max)
{
	mMax = max;
}

// 最大位置を取得
const CVector& CBounds::Max() const
{
	return mMax;
}

// 中心位置を設定
void CBounds::Center(const CVector& center)
{
	SetPos(center, Size());
}

// 中心位置を取得
CVector CBounds::Center() const
{
	return (mMax + mMin) * 0.5f;
}

// サイズを設定
void CBounds::Size(const CVector& size)
{
	SetPos(Center(), size);
}

// サイズを取得
CVector CBounds::Size() const
{
	return (mMax - mMin) * 0.5f;
}

// 2つのバウンディングボックスが交差するかどうか
bool CBounds::Intersect(const CBounds& b0, const CBounds& b1)
{
	return b0.mMin.X() <= b1.mMax.X()
		&& b0.mMax.X() >= b1.mMin.X()
		&& b0.mMin.Y() <= b1.mMax.Y()
		&& b0.mMax.Y() >= b1.mMin.Y()
		&& b0.mMin.Z() <= b1.mMax.Z()
		&& b0.mMax.Z() >= b1.mMin.Z();
}

// 線分のバウンディングボックスを取得
CBounds CBounds::GetLineBounds(const CVector& ls, const CVector& le)
{
	CVector min, max;
	min = ls;
	max = ls;

	min.X(std::min(min.X(), le.X()));
	min.Y(std::min(min.Y(), le.Y()));
	min.Z(std::min(min.Z(), le.Z()));

	max.X(std::max(max.X(), le.X()));
	max.Y(std::max(max.Y(), le.Y()));
	max.Z(std::max(max.Z(), le.Z()));

	CBounds ret;
	ret.SetRange(min, max);
	return ret;
}

// 球のバウンディングボックスを取得
CBounds CBounds::GetSphereBounds(const CVector& pos, float rad)
{
	CVector min = pos - CVector::one * rad;
	CVector max = pos + CVector::one * rad;

	CBounds ret;
	ret.SetRange(min, max);
	return ret;
}

// 三角形のバウンディングボックスを取得
CBounds CBounds::GetTriangleBounds(const CVector& t0, const CVector& t1, const CVector& t2)
{
	CVector min, max;
	min = t0;
	max = t0;

	min.X(std::min(min.X(), t1.X()));
	min.Y(std::min(min.Y(), t1.Y()));
	min.Z(std::min(min.Z(), t1.Z()));
	min.X(std::min(min.X(), t2.X()));
	min.Y(std::min(min.Y(), t2.Y()));
	min.Z(std::min(min.Z(), t2.Z()));

	max.X(std::max(max.X(), t1.X()));
	max.Y(std::max(max.Y(), t1.Y()));
	max.Z(std::max(max.Z(), t1.Z()));
	max.X(std::max(max.X(), t2.X()));
	max.Y(std::max(max.Y(), t2.Y()));
	max.Z(std::max(max.Z(), t2.Z()));

	CBounds ret;
	ret.SetRange(min, max);
	return ret;
}

// カプセルのバウンディングボックスを取得
CBounds CBounds::GetCapsuleBounds(const CVector& cs, const CVector& ce, float cr)
{
	CVector min = cs - CVector::one * cr;
	CVector max = cs + CVector::one * cr;

	CVector cem = ce - CVector::one * cr;
	min.X(std::min(min.X(), cem.X()));
	min.Y(std::min(min.Y(), cem.Y()));
	min.Z(std::min(min.Z(), cem.Z()));

	CVector cep = ce + CVector::one * cr;
	max.X(std::max(max.X(), cep.X()));
	max.Y(std::max(max.Y(), cep.Y()));
	max.Z(std::max(max.Z(), cep.Z()));

	CBounds ret;
	ret.SetRange(min, max);
	return ret;
}
