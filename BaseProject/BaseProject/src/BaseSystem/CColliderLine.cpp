#include "CColliderLine.h"
#include <glut.h>
#include "Maths.h"
#include "CColor.h"

// コンストラクタ
CColliderLine::CColliderLine(CObjectBase* owner, ELayer layer,
	const CVector& v0, const CVector& v1,
	bool isKinematic, float weight)
	: CCollider(owner, layer, EColliderType::eLine, isKinematic, weight)
	, mRadius(0.1f)
{
	// 線分の頂点を設定
	mV[0] = v0;
	mV[1] = v1;
}

// 線分コライダーの設定
void CColliderLine::Set(CObjectBase* owner, ELayer layer,
	const CVector& v0, const CVector& v1)
{
	CCollider::Set(owner, layer);

	// 線分の頂点を設定
	mV[0] = v0;
	mV[1] = v1;
}

// 線分の始点と終点を取得
void CColliderLine::Get(CVector* v0, CVector* v1) const
{
	*v0 = mWV[0];
	*v1 = mWV[1];
}

float CColliderLine::Radius() const
{
	return mRadius;
}

void CColliderLine::Render()
{
	// 現在の行列を退避しておく
	glPushMatrix();

	// 自身の行列を適用
	glMultMatrixf(Matrix().M());
	// アルファブレンドを有効にする
	glEnable(GL_BLEND);
	// ブレンド方法を指定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// ライトオフ
	glDisable(GL_LIGHTING);

	// DIFFUSE赤色設定
	CColor col = CColor::red;
	if (!IsEnable()) col = CColor::gray;
	float* c = (float*)&col;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	glLineWidth(3.0f);
	// 線分を描画
	glBegin(GL_LINES);
	glVertex3f(mV[0].X(), mV[0].Y(), mV[0].Z());
	glVertex3f(mV[1].X(), mV[1].Y(), mV[1].Z());
	glEnd();
	glLineWidth(1.0f);

	// ライトオン
	glEnable(GL_LIGHTING);
	// アルファブレンド無効
	glDisable(GL_ALPHA);

	// 描画前の行列に戻す
	glPopMatrix();
}

// コライダーの情報を更新
void CColliderLine::UpdateCol()
{
	// 頂点のワールド座標を算出
	CMatrix m = Matrix();
	mWV[0] = mV[0] * m;
	mWV[1] = mV[1] * m;

	// バウンディングボックスを更新
	mBounds = CBounds::GetLineBounds(mWV[0], mWV[1]);
}
