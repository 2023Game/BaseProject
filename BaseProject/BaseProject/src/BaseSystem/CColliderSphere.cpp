#include "CColliderSphere.h"
#include <glut.h>
#include "CColor.h"

// コンストラクタ
CColliderSphere::CColliderSphere(CObjectBase* owner, ELayer layer, float radius,
	bool isKinematic, float weight)
	: CCollider(owner, layer, EColliderType::eSphere, isKinematic, weight)
	, mRadius(radius)
{
}

// 球コライダーの設定
void CColliderSphere::Set(CObjectBase* owner, ELayer layer, float radius)
{
	CCollider::Set(owner, layer);

	// 半径を設定
	mRadius = radius;
}

// 球の座標と半径を取得
void CColliderSphere::Get(CVector* pos, float* rad) const
{
	*pos = mWPos;
	*rad = mWRadius;
}

void CColliderSphere::Render()
{
	// 現在の行列を退避しておく
	glPushMatrix();

	// 自身の行列を適用
	CMatrix m;
	m.Translate(Position());
	glMultMatrixf((m * Matrix()).M());
	// アルファブレンドを有効にする
	glEnable(GL_BLEND);
	// ブレンド方法を指定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// ライトオフ
	glDisable(GL_LIGHTING);

	// DIFFUSE赤色設定
	CColor col = CColor::red;
	if (!IsEnable()) col = CColor::gray;
	col.A(0.2f);
	float* c = (float*)&col;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	// 球を描画
	glutWireSphere(mRadius, 32, 32);

	// ライトオン
	glEnable(GL_LIGHTING);
	// アルファブレンド無効
	glDisable(GL_ALPHA);

	// 描画前の行列に戻す
	glPopMatrix();
}

// コライダーの情報を更新
void CColliderSphere::UpdateCol()
{
	// 行列を反映した中心位置と半径を計算
	CMatrix m = Matrix();
	mWPos = Position() * m;
	mWRadius = mRadius * m.VectorX().Length();

	// バウンディングボックスを更新
	mBounds = CBounds::GetSphereBounds(mWPos, mWRadius);
}
