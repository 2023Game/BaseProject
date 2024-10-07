#include "CQuaternion.h"
#include "Maths.h"

const CQuaternion CQuaternion::identity(0.0f, 0.0f, 0.0f);

CQuaternion::CQuaternion(float x, float y, float z, float w)
	: mX(x)
	, mY(y)
	, mZ(z)
	, mW(w)
{
}

CQuaternion::CQuaternion(float x, float y, float z)
{
	Euler(x, y, z);
}

CQuaternion::CQuaternion(const CVector& eulerAngles)
	: CQuaternion(eulerAngles.X(), eulerAngles.Y(), eulerAngles.Z())
{
}

CQuaternion::~CQuaternion()
{
}

CVector CQuaternion::operator*(const CVector& v) const
{
	CVector qv = CVector(mX, mY, mZ);
	CVector t = CVector::Cross(v, qv) * 2.0f;
	return v + t * mW + CVector::Cross(t, qv);
}

CQuaternion CQuaternion::operator*(const CQuaternion& c) const
{
	return CQuaternion
	(
		 mX * c.mW - mY * c.mZ + mZ * c.mY + mW * c.mX,
		 mX * c.mZ + mY * c.mW - mZ * c.mX + mW * c.mY,
		-mX * c.mY + mY * c.mX + mZ * c.mW + mW * c.mZ,
		-mX * c.mX - mY * c.mY - mZ * c.mZ + mW * c.mW
	);
}

CQuaternion CQuaternion::operator*(const float& f) const
{
	return CQuaternion(mX * f, mY * f, mZ * f, mW * f);
}

void CQuaternion::operator*=(const float& f)
{
	mX *= f;
	mY *= f;
	mZ *= f;
	mW *= f;
}

void CQuaternion::Euler(float x, float y, float z)
{
	// 指定された角度をディグリー（度）からラジアンに変換
	x = Math::DegreeToRadian(x);
	y = Math::DegreeToRadian(y);
	z = Math::DegreeToRadian(z);

	float cx = cosf(x * 0.5f);
	float cy = cosf(y * 0.5f);
	float cz = cosf(z * 0.5f);
	float sx = sinf(x * 0.5f);
	float sy = sinf(y * 0.5f);
	float sz = sinf(z * 0.5f);

	//mX = cx * sy * sz + cy * cz * sx;
	//mY = cx * cz * sy - cy * sx * sz;
	//mZ = cx * cy * sz - cz * sx * sy;
	//mW = sx * sy * sz + cx * cy * cz;
	mX = -cx * sy * sz + sx * cy * cz;
	mY =  cx * sy * cz + sx * cy * sz;
	mZ =  sx * sy * cz + cx * cy * sz;
	mW = -sx * sy * sz + cx * cy * cz;
}

void CQuaternion::Euler(const CVector& euler)
{
	Euler(euler.X(), euler.Y(), euler.Z());
}

CVector CQuaternion::Euler() const
{
	//float x2 = mX * mX, y2 = mY * mY, z2 = mZ * mZ, w2 = mW * mW;

	//CVector e;
	//e.X(asinf(2.0f * (mY * mZ + mX * mW)));
	//if (cosf(mZ) != 0.0f)
	//{
	//	e.Y(atanf(-(mX * mZ - mY * mW) / (w2 + z2 - 0.5f)));
	//	e.Z(atanf(-(mX * mY - mZ * mW) / (w2 + y2 - 0.5f)));
	//}
	//else
	//{
	//	e.Y(0.0f);
	//	e.Z(atanf((mX * mY + mZ * mW) / (w2 + x2 - 0.5f)));
	//}

	//// 計算結果をラジアンからディグリー（度）に変換
	//e.X(Math::RadianToDegree(e.X()));
	//e.Y(Math::RadianToDegree(e.Y()));
	//e.Z(Math::RadianToDegree(e.Z()));
	//return e;

	float x = mX, y = mY, z = mZ, w = mW;
	float x2 = x * x, y2 = y* y, z2 = z * z;
	float xy = x * y, xz = x * z, yz = y * z,
		  wx = w * x, wy = w * y, wz = w * z;

	float m00 = 1.0f - (2.0f * y2) - (2.0f * z2);
	float m01 = (2.0f * xy) + (2.0f * wz);
	float m10 = (2.0f * xy) - (2.0f * wz);
	float m11 = 1.0f - (2.0f * x2) - (2.0f * z2);
	float m20 = (2.0f * xz) + (2.0f * wy);
	float m21 = (2.0f * yz) - (2.0f * wx);
	float m22 = 1.0f - (2.0f * x2) - (2.0f * y2);

	float tx, ty, tz;

	if (fabsf(m21 - 1.0f) <= EPSILON)
	{
		tx = M_PI * 0.5f;
		ty = 0.0f;
		tz = atan2f(m10, m00);
	}
	else if (fabsf(m21 + 1.0f) <= EPSILON)
	{
		tx = -M_PI * 0.5f;
		ty = 0.0f;
		tz = atan2f(m10, m00);
	}
	else
	{
		tx = asinf(-m21);
		ty = atan2f(m20, m22);
		tz = atan2f(m01, m11);
	}

	tx = Math::Repeat(Math::RadianToDegree(tx), 360.0f);
	ty = Math::Repeat(Math::RadianToDegree(ty), 360.0f);
	tz = Math::Repeat(Math::RadianToDegree(tz), 360.0f);
	return CVector(tx, ty, tz);
}

void CQuaternion::RotationAxis(const CVector& axis, float angle)
{
	angle = Math::DegreeToRadian(angle);
	float a2 = angle * 0.5f;
	float s = sinf(a2);
	mW = cosf(a2);
	mX = axis.X() * s;
	mY = axis.Y() * s;
	mZ = axis.Z() * s;
}

CMatrix CQuaternion::Matrix() const
{
	float x2 = mX * mX, y2 = mY * mY, z2 = mZ * mZ, w2 = mW * mW;
	CMatrix m
	(
		w2 + x2 - y2 - z2,
		2.0f * (mX * mY - mW * mZ),
		2.0f * (mW * mY + mX * mZ),
		0.0f,

		2.0f * (mW * mZ + mX * mY),
		w2 - x2 + y2 - z2,
		2.0f * (mY * mZ - mW * mX),
		0.0f,

		2.0f * (mX * mZ - mW * mY),
		2.0f * (mY * mZ + mW * mX),
		w2 - x2 - y2 + z2,
		0.0f,

		0.0f, 0.0f, 0.0f, 1.0f
	);
	return m;
}

float CQuaternion::Length() const
{
	return sqrtf(mX * mX + mY * mY + mZ * mZ + mW * mW);
}

float CQuaternion::LengthSqr() const
{
	return mX * mX + mY * mY + mZ * mZ + mW * mW;
}

void CQuaternion::Normalize()
{
	float length = Length();
	if (length == 0.0f) return;
	*this *= (1.0f / length);
}

CQuaternion CQuaternion::Normalized() const
{
	float length = Length();
	if (length == 0.0f) return *this;
	return *this * (1.0f / length);
}

CQuaternion CQuaternion::Inverse() const
{
	float length = LengthSqr();
	if (length == 0.0f) return identity;
	return CQuaternion(-mX, -mY, -mZ, mW) * (1.0f / length);
}

CQuaternion CQuaternion::AngleAxis(float angle, const CVector& axis)
{
	float halfAngle = angle * 0.5f;
	CVector v = axis * sinf(halfAngle);
	return CQuaternion(v.X(), v.Y(), v.Z(), cosf(halfAngle));
}

CQuaternion CQuaternion::FromToRotation(const CVector& fromDir, const CVector& toDir)
{
	CVector from = fromDir.Normalized();
	CVector to = toDir.Normalized();
	float d = CVector::Dot(from, to);

	if (d >= 1.0f)
	{
		return identity;
	}
	else if (d <= -1.0f)
	{
		CVector axis = CVector::Cross(from, CVector::right);
		if (axis.LengthSqr() < EPSILON)
		{
			axis = CVector::Cross(from, CVector::up);
		}
		return AngleAxis(M_PI, axis.Normalized());
	}
	else
	{
		float s = from.Length() * to.Length() + CVector::Dot(from, to);
		if (s != 0.0f)
		{
			CVector v = -CVector::Cross(from, to);
			return CQuaternion(v.X(), v.Y(), v.Z(), s).Normalized();
		}
		else
		{
			return CQuaternion(0.0f, 180.0f, 0.0f);
		}
	}
}

CQuaternion CQuaternion::LookRotation(const CVector& forward)
{
	if (forward.LengthSqr() == 0.0f) return identity;
	return FromToRotation(CVector::forward, forward);
}

CQuaternion CQuaternion::LookRotation(const CVector& forward, const CVector& upwards)
{
	CQuaternion q1 = LookRotation(forward);

	if (CVector::Cross(forward, upwards).LengthSqr() < EPSILON)
	{
		return q1;
	}

	CVector up = q1 * CVector::up;

	CQuaternion q2 = FromToRotation(up, upwards);

	return q2 * q1;
}