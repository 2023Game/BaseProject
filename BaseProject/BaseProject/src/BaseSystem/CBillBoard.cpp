#include "CBillBoard.h"
//#include "CApplication.h"
#include "CCamera.h"

CBillBoard::CBillBoard(ETag tag, ETaskPauseType pause)
	: CObjectBase(tag, ETaskPriority::eBillboard, 0, pause)
{
}

void CBillBoard::SetSize(const CVector2& size)
{
	//三角形の頂点座標設定
	mT[0].Vertex
	(
		CVector(size.X(), size.Y(), 0.0f),
		CVector(-size.X(), -size.Y(), 0.0f),
		CVector(size.X(), -size.Y(), 0.0f)
	);
	mT[1].Vertex
	(
		CVector(-size.X(), size.Y(), 0.0f),
		CVector(-size.X(), -size.Y(), 0.0f),
		CVector(size.X(), size.Y(), 0.0f)
	);
	//法線をZ軸方向
	mT[0].Normal(CVector(0.0f, 0.0f, 1.0f));
	mT[1].Normal(CVector(0.0f, 0.0f, 1.0f));
}

void CBillBoard::Update()
{
}

void CBillBoard::Render()
{
	Render(&mMaterial);
}

//Render(マテリアルのポインタ)
void CBillBoard::Render(CMaterial* mpMaterial)
{
	//行列の保存
	glPushMatrix();

	//常にカメラの方向を向かせる
	CCamera* cam = CCamera::CurrentCamera();
	CMatrix m = cam->GetViewMatrix().Inverse();
	m.Position(CVector::zero);
	glMultMatrixf((m * Matrix()).M());

	//デプス値の書き込みをオフ
	glDepthMask(false);
	//ライトオフ
	glDisable(GL_LIGHTING);
	//マテリアル適用
	mpMaterial->Enabled(mColor);
	//三角形の描画
	mT[0].Render();
	mT[1].Render();
	//マテリアル解除
	mpMaterial->Disabled();
	//ライトオン
	glEnable(GL_LIGHTING);
	//デプス値の書き込みをオンに戻す
	glDepthMask(true);

	//行列を戻す
	glPopMatrix();
}
