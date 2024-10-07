#include "CMatrix.h"
//標準入出力関数のインクルード
#include <stdio.h>
//数学関数のインクルード
#include "Maths.h"
#include "CVector.h"

// 零行列
const CMatrix CMatrix::zero(0.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 0.0f);
// 単位行列
const CMatrix CMatrix::identity(1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f);

// コンストラクタ
CMatrix::CMatrix()
{
	Identity();
}

// コンストラクタ
CMatrix::CMatrix(float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33)
{
	mM[0][0] = m00, mM[0][1] = m01, mM[0][2] = m02, mM[0][3] = m03,
	mM[1][0] = m10, mM[1][1] = m11, mM[1][2] = m12, mM[1][3] = m13,
	mM[2][0] = m20, mM[2][1] = m21, mM[2][2] = m22, mM[2][3] = m23,
	mM[3][0] = m30, mM[3][1] = m31, mM[3][2] = m32, mM[3][3] = m33;
}

//*演算子のオーバーロード
CMatrix CMatrix::operator*(const float &x)
{
	CMatrix t;
	for (int i = 0; i < 4; i++)
	{
		t.mM[i][0] = mM[i][0] * x;
		t.mM[i][1] = mM[i][1] * x;
		t.mM[i][2] = mM[i][2] * x;
		t.mM[i][3] = mM[i][3] * x;
	}
	return t;
}

CMatrix CMatrix::operator+(const CMatrix &m)
{
	CMatrix t;
	for (int i = 0; i < 4; i++)
	{
		t.mM[i][0] = mM[i][0] + m.mM[i][0];
		t.mM[i][1] = mM[i][1] + m.mM[i][1];
		t.mM[i][2] = mM[i][2] + m.mM[i][2];
		t.mM[i][3] = mM[i][3] + m.mM[i][3];
	}
	return t;
}

void CMatrix::operator+=(const CMatrix &m)
{
	for (int i = 0; i < 4; i++)
	{
		mM[i][0] += m.mM[i][0];
		mM[i][1] += m.mM[i][1];
		mM[i][2] += m.mM[i][2];
		mM[i][3] += m.mM[i][3];
	}
}

//クオータニオンで回転行列を設定する
CMatrix CMatrix::Quaternion(float x, float y, float z, float w)
{
	mM[0][0] = x * x - y * y - z * z + w * w;
	mM[0][1] = 2 * x * y - 2 * w * z;
	mM[0][2] = 2 * x * z + 2 * w * y;
	mM[0][3] = 0;
	mM[1][0] = 2 * x * y + 2 * w * z;
	mM[1][1] = -x * x + y * y - z * z + w * w;
	mM[1][2] = 2 * y * z - 2 * w * x;
	mM[1][3] = 0;
	mM[2][0] = 2 * x * z - 2 * w * y;
	mM[2][1] = 2 * y * z + 2 * w * x;
	mM[2][2] = -x * x - y * y + z * z + w * w;
	mM[2][3] = 0;
	mM[3][0] = 0;
	mM[3][1] = 0;
	mM[3][2] = 0;
	mM[3][3] = 1;
	return *this;
}

CVector CMatrix::Position() const
{
	return CVector(mM[3][0], mM[3][1], mM[3][2]);
}

void CMatrix::Position(const CVector& pos)
{
	mM[3][0] = pos.X();
	mM[3][1] = pos.Y();
	mM[3][2] = pos.Z();
}

CVector CMatrix::VectorZ() const
{
	return CVector(mM[2][0], mM[2][1], mM[2][2]);
}

CVector CMatrix::VectorX() const
{
	return CVector(mM[0][0], mM[0][1], mM[0][2]);
}

CVector CMatrix::VectorY() const
{
	return CVector(mM[1][0], mM[1][1], mM[1][2]);
}

CMatrix CMatrix::Transpose() const
{
	CMatrix tmp;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tmp.mM[i][j] = mM[j][i];
		}
	}
	return tmp;
}

CMatrix CMatrix::Inverse() const
{
	CMatrix m;
	float det = 0.0f;
	det +=
		mM[0][0] * mM[1][1] * mM[2][2] * mM[3][3] +
		mM[0][0] * mM[2][1] * mM[3][2] * mM[1][3] +
		mM[0][0] * mM[3][1] * mM[1][2] * mM[2][3];
	det +=
		mM[1][0] * mM[0][1] * mM[3][2] * mM[2][3] +
		mM[1][0] * mM[2][1] * mM[0][2] * mM[3][3] +
		mM[1][0] * mM[3][1] * mM[2][2] * mM[0][3];
	det +=
		mM[2][0] * mM[0][1] * mM[1][2] * mM[3][3] +
		mM[2][0] * mM[1][1] * mM[3][2] * mM[0][3] +
		mM[2][0] * mM[3][1] * mM[0][2] * mM[1][3];
	det +=
		mM[3][0] * mM[0][1] * mM[2][2] * mM[1][3] +
		mM[3][0] * mM[1][1] * mM[0][2] * mM[2][3] +
		mM[3][0] * mM[2][1] * mM[1][2] * mM[0][3];
	det -=
		mM[0][0] * mM[1][1] * mM[3][2] * mM[2][3] +
		mM[0][0] * mM[2][1] * mM[1][2] * mM[3][3] +
		mM[0][0] * mM[3][1] * mM[2][2] * mM[1][3];
	det -=
		mM[1][0] * mM[0][1] * mM[2][2] * mM[3][3] +
		mM[1][0] * mM[2][1] * mM[3][2] * mM[0][3] +
		mM[1][0] * mM[3][1] * mM[0][2] * mM[2][3];
	det -=
		mM[2][0] * mM[0][1] * mM[3][2] * mM[1][3] +
		mM[2][0] * mM[1][1] * mM[0][2] * mM[3][3] +
		mM[2][0] * mM[3][1] * mM[1][2] * mM[0][3];
	det -=
		mM[3][0] * mM[0][1] * mM[1][2] * mM[2][3] +
		mM[3][0] * mM[1][1] * mM[2][2] * mM[0][3] +
		mM[3][0] * mM[2][1] * mM[0][2] * mM[1][3];
	if (det == 0.0f) return m;
	m.mM[0][0] =
		mM[1][1] * mM[2][2] * mM[3][3] +
		mM[2][1] * mM[3][2] * mM[1][3] +
		mM[3][1] * mM[1][2] * mM[2][3] -
		mM[1][1] * mM[3][2] * mM[2][3] -
		mM[2][1] * mM[1][2] * mM[3][3] -
		mM[3][1] * mM[2][2] * mM[1][3];
	m.mM[1][0] =
		mM[1][0] * mM[3][2] * mM[2][3] +
		mM[2][0] * mM[1][2] * mM[3][3] +
		mM[3][0] * mM[2][2] * mM[1][3] -
		mM[1][0] * mM[2][2] * mM[3][3] -
		mM[2][0] * mM[3][2] * mM[1][3] -
		mM[3][0] * mM[1][2] * mM[2][3];
	m.mM[2][0] =
		mM[1][0] * mM[2][1] * mM[3][3] +
		mM[2][0] * mM[3][1] * mM[1][3] +
		mM[3][0] * mM[1][1] * mM[2][3] -
		mM[1][0] * mM[3][1] * mM[2][3] -
		mM[2][0] * mM[1][1] * mM[3][3] -
		mM[3][0] * mM[2][1] * mM[1][3];
	m.mM[3][0] =
		mM[1][0] * mM[3][1] * mM[2][2] +
		mM[2][0] * mM[1][1] * mM[3][2] +
		mM[3][0] * mM[2][1] * mM[1][2] -
		mM[1][0] * mM[2][1] * mM[3][2] -
		mM[2][0] * mM[3][1] * mM[1][2] -
		mM[3][0] * mM[1][1] * mM[2][2];
	m.mM[0][1] =
		mM[0][1] * mM[3][2] * mM[2][3] +
		mM[2][1] * mM[0][2] * mM[3][3] +
		mM[3][1] * mM[2][2] * mM[0][3] -
		mM[0][1] * mM[2][2] * mM[3][3] -
		mM[2][1] * mM[3][2] * mM[0][3] -
		mM[3][1] * mM[0][2] * mM[2][3];
	m.mM[1][1] =
		mM[0][0] * mM[2][2] * mM[3][3] +
		mM[2][0] * mM[3][2] * mM[0][3] +
		mM[3][0] * mM[0][2] * mM[2][3] -
		mM[0][0] * mM[3][2] * mM[2][3] -
		mM[2][0] * mM[0][2] * mM[3][3] -
		mM[3][0] * mM[2][2] * mM[0][3];
	m.mM[2][1] =
		mM[0][0] * mM[3][1] * mM[2][3] +
		mM[2][0] * mM[0][1] * mM[3][3] +
		mM[3][0] * mM[2][1] * mM[0][3] -
		mM[0][0] * mM[2][1] * mM[3][3] -
		mM[2][0] * mM[3][1] * mM[0][3] -
		mM[3][0] * mM[0][1] * mM[2][3];
	m.mM[3][1] =
		mM[0][0] * mM[2][1] * mM[3][2] +
		mM[2][0] * mM[3][1] * mM[0][2] +
		mM[3][0] * mM[0][1] * mM[2][2] -
		mM[0][0] * mM[3][1] * mM[2][2] -
		mM[2][0] * mM[0][1] * mM[3][2] -
		mM[3][0] * mM[2][1] * mM[0][2];
	m.mM[0][2] =
		mM[0][1] * mM[1][2] * mM[3][3] +
		mM[1][1] * mM[3][2] * mM[0][3] +
		mM[3][1] * mM[0][2] * mM[1][3] -
		mM[0][1] * mM[3][2] * mM[1][3] -
		mM[1][1] * mM[0][2] * mM[3][3] -
		mM[3][1] * mM[1][2] * mM[0][3];
	m.mM[1][2] =
		mM[0][0] * mM[3][2] * mM[1][3] +
		mM[1][0] * mM[0][2] * mM[3][3] +
		mM[3][0] * mM[1][2] * mM[0][3] -
		mM[0][0] * mM[1][2] * mM[3][3] -
		mM[1][0] * mM[3][2] * mM[0][3] -
		mM[3][0] * mM[0][2] * mM[1][3];
	m.mM[2][2] =
		mM[0][0] * mM[1][1] * mM[3][3] +
		mM[1][0] * mM[3][1] * mM[0][3] +
		mM[3][0] * mM[0][1] * mM[1][3] -
		mM[0][0] * mM[3][1] * mM[1][3] -
		mM[1][0] * mM[0][1] * mM[3][3] -
		mM[3][0] * mM[1][1] * mM[0][3];
	m.mM[3][2] =
		mM[0][0] * mM[3][1] * mM[1][2] +
		mM[1][0] * mM[0][1] * mM[3][2] +
		mM[3][0] * mM[1][1] * mM[0][2] -
		mM[0][0] * mM[1][1] * mM[3][2] -
		mM[1][0] * mM[3][1] * mM[0][2] -
		mM[3][0] * mM[0][1] * mM[1][2];
	m.mM[0][3] =
		mM[0][1] * mM[2][2] * mM[1][3] +
		mM[1][1] * mM[0][2] * mM[2][3] +
		mM[2][1] * mM[1][2] * mM[0][3] -
		mM[0][1] * mM[1][2] * mM[2][3] -
		mM[1][1] * mM[2][2] * mM[0][3] -
		mM[2][1] * mM[0][2] * mM[1][3];
	m.mM[1][3] =
		mM[0][0] * mM[1][2] * mM[2][3] +
		mM[1][0] * mM[2][2] * mM[0][3] +
		mM[2][0] * mM[0][2] * mM[1][3] -
		mM[0][0] * mM[2][2] * mM[1][3] -
		mM[1][0] * mM[0][2] * mM[2][3] -
		mM[2][0] * mM[1][2] * mM[0][3];
	m.mM[2][3] =
		mM[0][0] * mM[2][1] * mM[1][3] +
		mM[1][0] * mM[0][1] * mM[2][3] +
		mM[2][0] * mM[1][1] * mM[0][3] -
		mM[0][0] * mM[1][1] * mM[2][3] -
		mM[1][0] * mM[2][1] * mM[0][3] -
		mM[2][0] * mM[0][1] * mM[1][3];
	m.mM[3][3] =
		mM[0][0] * mM[1][1] * mM[2][2] +
		mM[1][0] * mM[2][1] * mM[0][2] +
		mM[2][0] * mM[0][1] * mM[1][2] -
		mM[0][0] * mM[2][1] * mM[1][2] -
		mM[1][0] * mM[0][1] * mM[2][2] -
		mM[2][0] * mM[1][1] * mM[0][2];
	return m * (1.0f / det);
}

void CMatrix::Print() {
	printf("%10f %10f %10f %10f\n",
		mM[0][0], mM[0][1], mM[0][2], mM[0][3]);
	printf("%10f %10f %10f %10f\n",
		mM[1][0], mM[1][1], mM[1][2], mM[1][3]);
	printf("%10f %10f %10f %10f\n",
		mM[2][0], mM[2][1], mM[2][2], mM[2][3]);
	printf("%10f %10f %10f %10f\n",
		mM[3][0], mM[3][1], mM[3][2], mM[3][3]);
}

CMatrix CMatrix::Identity()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mM[i][j] = 0.0f;
			if (i == j)
			{
				mM[i][j] = 1.0f;
			}
		}
	}
	//この行列を返す
	return *this;
}

//拡大縮小行列の作成
//Scale(倍率X, 倍率Y, 倍率Z)
CMatrix CMatrix::Scale(float sx, float sy, float sz)
{
	Identity();
	mM[0][0] = sx;
	mM[1][1] = sy;
	mM[2][2] = sz;
	//この行列を返す
	return *this;
}

CMatrix CMatrix::Scale(const CVector& scale)
{
	return Scale(scale.X(), scale.Y(), scale.Z());
}


void CMatrix::M(int row, int col, float value)
{
	mM[row][col] = value;
}

//回転行列（Y軸）の作成
//RotateY(角度)
CMatrix CMatrix::RotateY(float degree) {
	//角度からラジアンを求める
	float rad = Math::DegreeToRadian(degree);
	//単位行列にする
	Identity();
	//Y軸で回転する行列の設定
	mM[0][0] = mM[2][2] = cosf(rad);
	mM[0][2] = -sinf(rad);
	mM[2][0] = -mM[0][2];
	//行列を返す
	return *this;
}

//回転行列（Z軸）の作成
//RotateZ(角度)
CMatrix CMatrix::RotateZ(float degree)
{
	//角度からラジアンを求める
	float rad = Math::DegreeToRadian(degree);
	//単位行列にする
	Identity();
	//Z軸で回転する行列の設定
	mM[0][0] = mM[1][1] = cosf(rad);
	mM[0][1] = sinf(rad);
	mM[1][0] = -mM[0][1];
	//行列を返す
	return *this;
}

CMatrix CMatrix::RotateX(float degree)
{
	//角度からラジアンを求める
	float rad = Math::DegreeToRadian(degree);
	//単位行列にする
	Identity();
	//Z軸で回転する行列の設定
	mM[1][1] = mM[2][2] = cosf(rad);
	mM[1][2] = sinf(rad);
	mM[2][1] = -mM[1][2];
	//行列を返す
	return *this;
}

//移動行列の作成
//Translate(移動量X, 移動量Y, 移動量Z)
CMatrix CMatrix::Translate(float mx, float my, float mz)
{
	Identity();
	mM[3][0] = mx;
	mM[3][1] = my;
	mM[3][2] = mz;
	//この行列を返す
	return *this;
}

CMatrix CMatrix::Translate(const CVector& translation)
{
	return Translate(translation.X(), translation.Y(), translation.Z());
}

//*演算子のオーバーロード
//CMatrix * CMatrix の演算結果を返す
const CMatrix CMatrix::operator*(const CMatrix& m) const {
	CMatrix t;
	for (int i = 0; i < 4; i++)
	{
		t.mM[i][0] = mM[i][0] * m.mM[0][0] + mM[i][1] * m.mM[1][0] + mM[i][2] * m.mM[2][0] + mM[i][3] * m.mM[3][0];
		t.mM[i][1] = mM[i][0] * m.mM[0][1] + mM[i][1] * m.mM[1][1] + mM[i][2] * m.mM[2][1] + mM[i][3] * m.mM[3][1];
		t.mM[i][2] = mM[i][0] * m.mM[0][2] + mM[i][1] * m.mM[1][2] + mM[i][2] * m.mM[2][2] + mM[i][3] * m.mM[3][2];
		t.mM[i][3] = mM[i][0] * m.mM[0][3] + mM[i][1] * m.mM[1][3] + mM[i][2] * m.mM[2][3] + mM[i][3] * m.mM[3][3];
	}
	return t;
}

float CMatrix::M(int r, int c) const
{
	return mM[r][c];
}

float* CMatrix::M() const
{
	return (float*)mM[0];
}

int CMatrix::Size()
{
	return sizeof(mM) / sizeof(float);
}

// 正面ベクトルと上方向ベクトルから回転行列を取得
CMatrix CMatrix::LookRotation(const CVector& forward, const CVector& up)
{
	// 正面ベクトルを正規化
	CVector f = forward.Normalized();
	// 正面ベクトルと上方向ベクトルの外積で横方向ベクトルを取得
	CVector r = CVector::Cross(up, f).Normalized();
	// 正面ベクトルと横方向ベクトルの外積で上方向ベクトルを取得
	CVector u = CVector::Cross(f, r).Normalized();

	// 各ベクトルを行列に設定して返す
	CMatrix m;
	m.mM[0][0] = r.X(); m.mM[0][1] = r.Y(); m.mM[0][2] = r.Z();
	m.mM[1][0] = u.X(); m.mM[1][1] = u.Y(); m.mM[1][2] = u.Z();
	m.mM[2][0] = f.X(); m.mM[2][1] = f.Y(); m.mM[2][2] = f.Z();
	return m;
}
