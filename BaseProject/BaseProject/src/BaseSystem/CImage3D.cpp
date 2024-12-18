#include "CImage3D.h"

// 3D空間での縮尺（3D空間での1のサイズに相当するピクセル数）
#define WORLD_UNIT_PER_PIXEL 256.0f

//コンストラクタ
CImage3D::CImage3D(std::string path,
	ETag tag,
	ETaskPriority prio, int sortOrder,
	ETaskPauseType pause,
	bool dontDelete, bool addTaskList)
	: CObjectBase(tag, prio, sortOrder, pause, dontDelete, addTaskList)
	, mOffsetPos(0.0f, 0.0f)
	, mUV(0.0f, 0.0f, 1.0f, 1.0f)
	, mWolrdUnitPerPixel(WORLD_UNIT_PER_PIXEL)
	, mpAnimData(nullptr)
	, mAnimNo(0)
	, mElapsedTime(0.0f)
	, mIsBillboard(false)
	, mIsDepthTest(true)
	, mIsDepthMask(false)
	, mIsLighting(false)
{
	// テクスチャ読み込み
	mMaterial.LoadTexture(path, path, false);
	CTexture* tex = mMaterial.Texture();

	// 読み込んだテクスチャの
	CVector2 size;
	size.X(tex->Header().width);
	size.Y(tex->Header().height);
	SetSize(size);

	SetColor(CColor::white);

	// UV設定
	SetUV(CRect(0.0f, 0.0f, 1.0f, 1.0f));
}

//デストラクタ
CImage3D::~CImage3D()
{
}

// 2D空間でのオフセット座標を取得
const CVector2& CImage3D::GetOffsetPos() const
{
	return mOffsetPos;
}

// 2D空間でのオフセット座標を設定
void CImage3D::SetOffsetPos(const float& x, const float& y)
{
	SetOffsetPos(CVector2(x, y));
}

// 2D空間でのオフセット座標を設定（CVector2版）
void CImage3D::SetOffsetPos(const CVector2& pos)
{
	mOffsetPos = pos;
}

// サイズ取得
const CVector2& CImage3D::GetSize() const
{
	return mSize;
}

// サイズ設定
void CImage3D::SetSize(const float& x, const float& y)
{
	SetSize(CVector2(x, y));
}

// サイズ設定（CVector2版）
void CImage3D::SetSize(const CVector2& size)
{
	mSize = size;
	CVector2 s = mSize * (1.0f / mWolrdUnitPerPixel);

	// 三角形の頂点座標設定
	mT[0].Vertex
	(
		CVector(s.X(), s.Y(), 0.0f),
		CVector(-s.X(), -s.Y(), 0.0f),
		CVector(s.X(), -s.Y(), 0.0f)
	);
	mT[1].Vertex
	(
		CVector(-s.X(), s.Y(), 0.0f),
		CVector(-s.X(), -s.Y(), 0.0f),
		CVector(s.X(), s.Y(), 0.0f)
	);
	// 法線をZ軸方向
	mT[0].Normal(CVector(0.0f, 0.0f, 1.0f));
	mT[1].Normal(CVector(0.0f, 0.0f, 1.0f));
}

// UV取得
const CRect& CImage3D::GetUV() const
{
	return mUV;
}

// UV設定
void CImage3D::SetUV(const float& left, const float& top, const float& right, const float& bottom)
{
	CRect rect;
	rect.SetPos(left, top);
	rect.W(right - left);
	rect.H(bottom - top);
	SetUV(rect);
}

// UV設定（CRect版）
void CImage3D::SetUV(const CRect& uv)
{
	mUV = uv;

	float l = uv.X();
	float t = 1.0f - uv.Y();
	float r = l + uv.W();
	float b = t - uv.H();

	mT[0].UV
	(
		CVector(r, t, 0.0f),
		CVector(l, b, 0.0f),
		CVector(r, b, 0.0f)
	);
	mT[1].UV
	(
		CVector(l, t, 0.0f),
		CVector(l, b, 0.0f),
		CVector(r, t, 0.0f)
	);
}

// 3D空間での縮尺を設定
void CImage3D::SetWorldUnitPerPixel(float pixel)
{
	mWolrdUnitPerPixel = pixel;
	SetSize(mSize);
}

// アニメーションデータを設定
void CImage3D::SetAnimData(TexAnimData* animData)
{
	mpAnimData = animData;
	mAnimNo = 0;
	mElapsedTime = 0.0f;

	// アニメーションが有効であれば、
	// 一番最初の画像のUVを設定
	if (IsEnableAnim())
	{
		CRect uv = mMaterial.Texture()->CalcUV
		(
			mpAnimData->row,
			mpAnimData->col,
			0
		);
		SetUV(uv);

		CTexture* tex = mMaterial.Texture();
		CVector2 size;
		float ratio = (float)mpAnimData->col / mpAnimData->row;
		size.X(tex->Header().width);
		size.Y(tex->Header().width * ratio);
		SetSize(size);
	}
}

// アニメーションが終わったかどうか
bool CImage3D::IsEndAnim() const
{
	if (mpAnimData == nullptr) return true;
	int size = mpAnimData->frames.size();
	return mAnimNo == size;
}

// ビルボードのオンオフを設定
void CImage3D::SetBillboard(bool enable)
{
	mIsBillboard = enable;
}

// デプステストのオンオフを設定
void CImage3D::SetDepthTest(bool enable)
{
	mIsDepthTest = enable;
}

// デプステストのオンオフを設定
void CImage3D::SetDepthMask(bool enable)
{
	mIsDepthMask = enable;
}

// ライティングのオンオフを設定
void CImage3D::SetLighting(bool enable)
{
	mIsLighting = enable;
}

// アニメーションを再生できるかどうか
bool CImage3D::IsEnableAnim() const
{
	if (mpAnimData == nullptr) return false;
	if (mpAnimData->frames.size() == 0) return false;
	return true;
}

// 更新
void CImage3D::Update()
{
	// アニメーションが有効であれば
	if (IsEnableAnim())
	{
		int size = mpAnimData->frames.size();
		if (mAnimNo < size)
		{
			TexAnimFrameData data = mpAnimData->frames[mAnimNo];
			// 現在の画像の表示時間を超えたら
			if (mElapsedTime >= data.time)
			{
				// 次の画像へ切り替え
				mElapsedTime -= data.time;

				mAnimNo++;
				if (mAnimNo >= size)
				{
					if (mpAnimData->loop)
					{
						mAnimNo -= size;
					}
					else
					{
						mAnimNo = size;
					}
				}
				CRect uv = mMaterial.Texture()->CalcUV
				(
					mpAnimData->row,
					mpAnimData->col,
					mAnimNo
				);
				SetUV(uv);
			}
			// 経過時間を計測
			mElapsedTime += Times::DeltaTime();
		}
	}
}

// 描画
void CImage3D::Render()
{
	Render(&mMaterial);
}

// 描画（マテリアル指定版）
void CImage3D::Render(CMaterial* mpMaterial)
{
	// 行列の保存
	glPushMatrix();

	// 自身の行列を取得
	CMatrix m = Matrix();

	// ビルボードが有効ならば
	if (mIsBillboard)
	{
		// 常にカメラの方向を向かせる
		CCamera* cam = CCamera::CurrentCamera();
		CMatrix camMtx = cam->GetViewMatrix().Inverse();
		camMtx.Position(CVector::zero);
		m = camMtx * m;
	}

	// 行列を反映
	glMultMatrixf(m.M());

	// オフセット座標を反映
	CVector2 offset = mOffsetPos * (1.0f / mWolrdUnitPerPixel);
	glTranslatef(offset.X(), offset.Y(), 0.0f);

	// 各設定のフラグの状態に合わせて、オフにする
	if (!mIsDepthTest) glDisable(GL_DEPTH_TEST);	// デプステスト
	if (!mIsDepthMask) glDepthMask(false);		// デプス書き込み
	if (!mIsLighting) glDisable(GL_LIGHTING);		// ライティング

	// マテリアル適用
	mpMaterial->Enabled(mColor);
	// 三角形の描画
	mT[0].Render();
	mT[1].Render();
	// マテリアル解除
	mpMaterial->Disabled();

	// 各設定を元に戻す
	glEnable(GL_DEPTH_TEST);	// デプステスト
	glDepthMask(true);			// デプス書き込み
	glEnable(GL_LIGHTING);		// ライティング

	// 行列を戻す
	glPopMatrix();
}
