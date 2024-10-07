#include "CCamera.h"
#include "glut.h"
#include "Maths.h"
#include "CDebugCamera.h"

std::list<CCamera*> CCamera::spCameraList;
CCamera* CCamera::spMainCamera = nullptr;
CCamera* CCamera::spCurrentCamera = nullptr;

// コンストラクタ
CCamera::CCamera(const CVector& eye, const CVector& center, bool isMainCamera)
	: CObjectBase(ETag::eCamera, ETaskPriority::eCamera)
	, mIsMainCamera(isMainCamera)
	, mFollowTargetTf(nullptr)
	, mFollowOffsetPos(CVector::zero)
	, mFovy(CAMERA_FOVY)
	, mZNear(CAMERA_ZNEAR)
	, mZFar(CAMERA_ZFAR)
	, mHitColRatio(1.0f)
{
	mTargetEye = eye;
	LookAt(eye, center, CVector::up);
	Reshape(WINDOW_WIDTH, WINDOW_HEIGHT);

	// メインカメラが設定されていなければ、
	if (mIsMainCamera && spMainCamera == nullptr)
	{
		// メインカメラに設定
		spMainCamera = this;
		// メインカメラを現在のカメラに設定
		spCurrentCamera = spMainCamera;
	}

	spCameraList.push_back(this);
}

// デストラクタ
CCamera::~CCamera()
{
	// カメラ削除処理
	DeleteCamera();
}

// メインカメラを取得
CCamera* CCamera::MainCamera()
{
	return spMainCamera;
}

// 現在のカメラを取得
CCamera* CCamera::CurrentCamera()
{
#if _DEBUG
	// デバッグカメラが有効であれば、
	if (CDebugCamera::IsOn())
	{
		// デバッグカメラを返す
		return CDebugCamera::DebugCamera();
	}
	// デバッグカメラが無効であれば、
	else
#endif
	{
		// 現在のカメラを返す
		return spCurrentCamera;
	}
}

// カメラを削除
void CCamera::DeleteCamera()
{
	// カメラリストから自分自身を取り除く
	spCameraList.remove(this);

	// 削除されたのがメインカメラであれば、
	// 登録しているメインカメラを解除
	if (spMainCamera == this)
	{
		spMainCamera = nullptr;
	}

	// 現在のカメラが削除された場合は
	if (spCurrentCamera == this)
	{
		// 現在のカメラとは別にメインカメラが存在すれば、
		// メインカメラを現在のカメラに変更
		if (spMainCamera != nullptr)
		{
			spCurrentCamera = spMainCamera;
		}
		// メインカメラが存在しない場合
		else
		{
			spCurrentCamera = nullptr;
			// カメラリストの先頭から調べて
			// メインカメラが存在すれば、
			// そのカメラを現在のカメラに設定する
			for (CCamera* cam : spCameraList)
			{
				if (cam->mIsMainCamera)
				{
					spMainCamera = cam;
					spCurrentCamera = spMainCamera;
					break;
				}
			}
		}
	}
}

// カメラの状態をコピー
void CCamera::CopyCamera(CCamera* copy)
{
	if (copy == nullptr) return;

	Position(copy->Position());
	Rotation(copy->Rotation());
	mViewMatrix = copy->mViewMatrix;
	mProjectionMatrix = copy->mProjectionMatrix;
	mViewportMatrix = copy->mViewportMatrix;
	mFovy = copy->mFovy;
	mZNear = copy->mZNear;
	mZFar = copy->mZFar;
	mTargetEye = copy->mTargetEye;
	mEye = copy->mEye;
	mAt = copy->mAt;
	mUp = copy->mUp;
	mEyeVec = copy->mEyeVec;
}

// 追従するターゲットを設定
void CCamera::SetFollowTargetTf(CTransform* target)
{
	mFollowTargetTf = target;
	if (mFollowTargetTf != nullptr)
	{
		mFollowOffsetPos = mTargetEye - mFollowTargetTf->Position();
	}
}

// 追従時のオフセット座標を設定
void CCamera::SetFollowTargetOffset(const CVector& offset)
{
	if (mFollowTargetTf != nullptr)
	{
		mFollowOffsetPos = offset;
	}
}

// 注視する位置を設定（注視点のみ）
void CCamera::LookAt(const CVector& pos)
{
	LookAt(mEye, pos, mUp);
}

// 注視する位置を設定（注視点 + 上ベクトル）
void CCamera::LookAt(const CVector& pos, const CVector& up)
{
	LookAt(mEye, pos, up);
}

// 注視する位置を設定（視点 + 注視点 + 上ベクトル）
void CCamera::LookAt(const CVector& eye, const CVector& at, const CVector& up, bool updateTargetEye)
{
	if (updateTargetEye) mTargetEye = eye;
	mEye = eye; mAt = at; mUp = up;
	mEyeVec = mAt - mTargetEye;
	mViewMatrix.Identity();
	CVector f = (mEye - mAt).Normalized();
	CVector r = CVector::Cross(mUp, f).Normalized();
	CVector u = CVector::Cross(f, r).Normalized();
	mViewMatrix.M(0, 0, r.X()); mViewMatrix.M(1, 0, r.Y()); mViewMatrix.M(2, 0, r.Z());
	mViewMatrix.M(0, 1, u.X()); mViewMatrix.M(1, 1, u.Y()); mViewMatrix.M(2, 1, u.Z());
	mViewMatrix.M(0, 2, f.X()); mViewMatrix.M(1, 2, f.Y()); mViewMatrix.M(2, 2, f.Z());
	mViewMatrix.M(3, 0, -CVector::Dot(mEye, r));
	mViewMatrix.M(3, 1, -CVector::Dot(mEye, u));
	mViewMatrix.M(3, 2, -CVector::Dot(mEye, f));

	Position(mEye);
	Rotation(CQuaternion::LookRotation(f));

	if (mFollowTargetTf != nullptr)
	{
		mFollowOffsetPos = mTargetEye - mFollowTargetTf->Position();
	}
}

// 画面サイズ変更処理
void CCamera::Reshape(int width, int height)
{
	//画面の描画エリアの指定
	Viewport(0.0f, 0.0f, width, height);

	Perspective
	(
		CAMERA_FOVY,
		(float)width / height,
		CAMERA_ZNEAR,
		CAMERA_ZFAR
	);
}

// 画面の描画エリアを指定
void CCamera::Viewport(float x, float y, float w, float h)
{
	glViewport(x, y, w, h);
	mViewportMatrix.Identity();
	mViewportMatrix.M(0, 0, w * 0.5f);
	mViewportMatrix.M(3, 0, x + w * 0.5f);
	mViewportMatrix.M(1, 1, -h * 0.5f);
	mViewportMatrix.M(3, 1, y + h * 0.5f);
}

// 透視投影行列を設定
void CCamera::Perspective(float fovy, float aspect, float zNear, float zFar)
{
	//各値を設定
	mFovy = fovy;
	mZNear = zNear;
	mZFar = zFar;

	//プロジェクション行列へ切り替え
	glMatrixMode(GL_PROJECTION);
	//行列を初期化して、新しいプロジェクション行列を設定
	glLoadIdentity();
	gluPerspective(mFovy, aspect, mZNear, mZFar);
	//プロジェクション行列を記憶しておく
	glGetFloatv(GL_PROJECTION_MATRIX, mProjectionMatrix.M());

	//モデルビュー行列へ戻す
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// スクリーン座標からワールド座標へ変換
CVector CCamera::ScreenToWorldPos(const CVector& screenPos)
{
	float z = screenPos.Z();
	float d = ((z * (mZFar + mZNear) - 2.0f * mZFar * mZNear) / (mZFar - mZNear) / z);
	CVector4 v =
		mViewportMatrix.Inverse() *
		mProjectionMatrix.Inverse() *
		mViewMatrix.Inverse() *
		CVector4(screenPos.X(), screenPos.Y(), d, 1.0f);
	return CVector(v.X() / v.W(), v.Y() / v.W(), v.Z() / v.W());
}

// ワールド座標からスクリーン座標へ変換
CVector CCamera::WorldToScreenPos(const CVector& worldPos)
{
	CVector4 v =
		mViewMatrix *
		mProjectionMatrix *
		mViewportMatrix *
		CVector4(worldPos, 1.0f);
	return CVector(v.X() / v.W(), v.Y() / v.W(), v.Z() / fabsf(v.W()));
}

// このカメラを現在のカメラに設定するかどうか
void CCamera::SetCurrent(bool isCurrent)
{
	// このカメラを現在のカメラに変更
	if (isCurrent)
	{
		spCurrentCamera = this;
	}
	// 解除時はメインカメラを現在のカメラにする
	else
	{
		spCurrentCamera = spMainCamera;
	}
}

// ビュー行列を取得
const CMatrix& CCamera::GetViewMatrix() const
{
	return mViewMatrix;
}

// プロジェクション行列を取得
const CMatrix& CCamera::GetProjectionMatrix() const
{
	return mProjectionMatrix;
}

// ビューポート行列を取得
const CMatrix& CCamera::GetViewportMatrix() const
{
	return mViewportMatrix;
}

// 衝突判定を行うコライダーをリストに追加
void CCamera::AddCollider(CCollider* col)
{
	mColliders.push_back(col);
}

// 衝突判定を行うコライダーをリストから取り除く
void CCamera::RemoveCollider(CCollider* col)
{
	mColliders.remove(col);
}

// コライダーとの衝突判定時に押し戻す距離の割合
void CCamera::SetHitColliderRatio(float ratio)
{
	mHitColRatio = ratio;
}

// 設定されているコライダーとの衝突結果を反映する
void CCamera::ApplyCollision()
{
	// 注視点から視点までレイを飛ばして、
	// 間に障害物があれば、視点を障害物より手前にズラす
	CHitInfo hit;
	CVector rayStart = mAt;
	CVector rayEnd = mEye;
	float nearDist = 0.0f;
	bool isHit = false;
	// 設定されているコライダーを順番に調べる
	for (CCollider* c : mColliders)
	{
		// レイとコライダーの衝突判定を行う
		if (CCollider::CollisionRay(c, rayStart, rayEnd, &hit))
		{
			// 交点が不整値でなければ、
			if (hit.cross.LengthSqr() != 0.0f)
			{
				// 衝突位置までの距離で一番近い距離を求める
				if (!isHit) nearDist = hit.dist;
				else nearDist = std::min(hit.dist, nearDist);
				isHit = true;
			}
		}
	}

	// レイが衝突していたら、
	// 視点を衝突地点より手前に押し戻す
	if (isHit)
	{
		mEye = rayStart + (rayEnd - rayStart).Normalized() * nearDist * mHitColRatio;
	}
}

// 更新
void CCamera::Update()
{
	// 追従するターゲットが設定されていれば、
	// カメラの位置を追従ターゲットの位置に合わせる
	if (mFollowTargetTf != nullptr)
	{
		CVector eye = mFollowTargetTf->Position() + mFollowOffsetPos;
		Position(eye);
		CVector diff = eye - mTargetEye;
		mTargetEye = eye;
		mEye = mTargetEye;
		mAt = mEye - VectorZ().Normalized() * mEyeVec.Length();
	}

	// 設定されているコライダーと衝突する場合は、
	// カメラの位置を押し出す
	ApplyCollision();

	// 視点、注視点、上ベクトルから各行列を更新
	LookAt(mEye, mAt, mUp, false);
}

// 削除
void CCamera::Kill()
{
	CTask::Kill();
	DeleteCamera();
}

// カメラ反映
void CCamera::Apply()
{
	glMultMatrixf(mViewMatrix.M());
}

// 2D用のカメラ行列を反映
void CCamera::Start2DCamera()
{
	// プロジェクション行列へ切り替えて、
	// 行列の退避と行列を初期化
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	// 2Dの表示エリアを設定
	gluOrtho2D(0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0);

	// モデルビュー行列へ戻して、
	// 行列の退避と行列を初期化
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// 2D描画に必要な設定を行う
	glDisable(GL_DEPTH_TEST);	// デプステストをオフ
	glDisable(GL_LIGHTING);		// ライティングをオフ
	glDisable(GL_CULL_FACE);	// カリング無効
}

// 3D用のカメラ行列へ戻す
void CCamera::End2DCamera()
{
	// 3D描画に必要な処理をオンにする
	glEnable(GL_DEPTH_TEST);	// デプステストをオン
	glEnable(GL_LIGHTING);		// ライティングをオン
	glEnable(GL_CULL_FACE);

	// プロジェクション行列へ切り替えて、
	// 2D描画開始時に退避した行列へ戻す
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	// モデルビュー行列へ戻して、
	// こちらも退避した行列へ戻す
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
