#include "CGaugeUI3D.h"
#include "CCamera.h"
#include <glut.h>
#include "CImage3D.h"
#include "Maths.h"

#define WORLD_UNIT_PER_PIXEL 32.0f

// コンストラクタ
CGaugeUI3D::CGaugeUI3D(CObjectBase* owner)
	: CObjectBase(ETag::eUI, ETaskPriority::eUI3D, 0, ETaskPauseType::eGame)
	, mpOwner(owner)
	, mpGaugeImg(nullptr)
	, mpWhiteImg(nullptr)
	, mMaxPoint(100)
	, mCurrPoint(mMaxPoint)
	, mPercent(1.0f)
{
	// ゲージのイメージを読み込み
	mpGaugeImg = new CImage3D
	(
		"UI\\gauge.png",
		ETag::eUI,
		ETaskPriority::eUI3D, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpGaugeImg->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
	mpGaugeImg->SetDepthMask(true);
	mGaugeSize = mpGaugeImg->GetSize();

	// 白イメージを読み込み
	mpWhiteImg = new CImage3D
	(
		"UI\\white.png",
		ETag::eUI,
		ETaskPriority::eUI3D, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpWhiteImg->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
}

// デストラクタ
CGaugeUI3D::~CGaugeUI3D()
{
	// 読み込んだイメージを削除
	SAFE_DELETE(mpGaugeImg);
	SAFE_DELETE(mpWhiteImg);

	// 持ち主に削除されたことを伝える
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

// 持ち主を設定
void CGaugeUI3D::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// ポイント残量を反映
void CGaugeUI3D::ApplyPoint()
{
	// 最大値が不正値でなければ
	if (mMaxPoint > 0)
	{
		// 現在値が最大値の何パーセントか求める
		mPercent = Math::Clamp01((float)mCurrPoint / mMaxPoint);
	}
	// 不正値ならば、100%固定
	else
	{
		mPercent = 1.0f;
	}
}

// 最大値を設定
void CGaugeUI3D::SetMaxPoint(int point)
{
	mMaxPoint = point;
	ApplyPoint();
}

// 現在値を設定
void CGaugeUI3D::SetCurrPoint(int point)
{
	mCurrPoint = point;
	ApplyPoint();
}

// ポイント残量の割合を設定（0.0～1.0）
void CGaugeUI3D::SetPercent(float per)
{
	mPercent = Math::Clamp01(per);
}

// 更新
void CGaugeUI3D::Update()
{
	mpGaugeImg->Update();
	mpWhiteImg->Update();
}

// 描画
void CGaugeUI3D::Render()
{
	// 行列の保存
	glPushMatrix();

	// 常にカメラの方向を向かせる
	CCamera* cam = CCamera::CurrentCamera();
	CMatrix m = cam->GetViewMatrix().Inverse();
	m.Position(CVector::zero);
	glMultMatrixf((m * Matrix()).M());

	// ゲージ背景を描画
	mpWhiteImg->SetOffsetPos(CVector2::zero);
	mpWhiteImg->SetSize(mGaugeSize);
	mpWhiteImg->SetColor(CColor::black);
	mpWhiteImg->Render();

	// バーのサイズ、座標、色を
	// ポイント残量の割合に合わせて調整して、バーを描画
	{
		// バーのサイズを調整
		CVector2 barSize = mGaugeSize;
		barSize.X(barSize.X() * mPercent);
		mpWhiteImg->SetSize(barSize);
		// バーの座標を調整
		CVector2 barPos = mGaugeSize - barSize;
		mpWhiteImg->SetOffsetPos(-barPos);
		// バーの色を設定
		CColor barColor = CColor::green;
		if (mPercent <= 0.2f) barColor = CColor::red;
		else if (mPercent <= 0.5f)barColor = CColor::yellow;
		mpWhiteImg->SetColor(barColor);
		// バーを描画
		mpWhiteImg->Render();
	}

	// ゲージ本体を描画
	mpGaugeImg->Render();

	// 行列を戻す
	glPopMatrix();
}
