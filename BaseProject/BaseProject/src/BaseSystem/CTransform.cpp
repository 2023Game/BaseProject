#include "CTransform.h"

// コンストラクタ
CTransform::CTransform(const CVector& pos, const CQuaternion& rot, const CVector& scale)
	: mpParent(nullptr)
{
	Position(pos);
	Rotation(rot);
	Scale(scale);
}

// コンストラクタ
CTransform::CTransform(const CVector& pos, const CVector& eulerAngles, const CVector& scale)
	: CTransform(pos, CQuaternion(eulerAngles), scale)
{
}

// デストラクタ
CTransform::~CTransform()
{
	// 親が存在すれば、親との繋がりを解除
	if (mpParent != nullptr)
	{
		mpParent->RemoveChild(this);
		mpParent = nullptr;
	}

	// 子が存在すれば、子との繋がりを解除
	auto itr = mChildren.begin();
	auto end = mChildren.end();
	while (itr != end)
	{
		CTransform* child = *itr;
		itr++;
		child->SetParent(nullptr);
	}
	mChildren.clear();
}

// ワールド位置を取得
const CVector& CTransform::Position() const
{
	return mPosition;
}

// ワールド位置を設定
void CTransform::Position(const CVector& pos)
{
	// 親が存在する場合
	if (mpParent != nullptr)
	{
		// ワールド位置はそのまま設定
		mPosition = pos;
		// 自身のワールド位置 - 親のワールド位置 で
		// 自身のローカル位置を求める
		mLocalPosition = mpParent->Rotation().Inverse() * (mPosition - mpParent->Position());
	}
	// 親が存在しない場合
	else
	{
		// ワールド位置、ローカル位置にそのまま設定
		mPosition = pos;
		mLocalPosition = pos;
	}

	// 変更を子に反映
	ApplyChildren();
}

// ワールド位置を設定
void CTransform::Position(float x, float y, float z)
{
	Position(CVector(x, y, z));
}

// ローカル位置を取得
const CVector& CTransform::LocalPosition() const
{
	return mLocalPosition;
}

// ローカル位置を設定
void CTransform::LocalPosition(const CVector& pos)
{
	// 親が存在する場合
	if (mpParent != nullptr)
	{
		// ローカル位置はそのまま設定
		mLocalPosition = pos;
		// 親のワールド位置 + 自身のローカル位置 で
		// 自身のワールド位置を求める
		mPosition = mpParent->Position() + mpParent->Rotation() * mLocalPosition;
	}
	// 親が存在しない場合
	else
	{
		// ローカル位置、ワールド位置にそのまま設定
		mLocalPosition = pos;
		mPosition = pos;
	}

	// 変更を子に反映
	ApplyChildren();
}

// ローカル位置を設定
void CTransform::LocalPosition(float x, float y, float z)
{
	LocalPosition(CVector(x, y, z));
}

// ワールド回転値（クォータニオン）を取得
const CQuaternion& CTransform::Rotation() const
{
	return mRotation;
}

// ローカル回転値（クォータニオン）を取得
const CQuaternion& CTransform::LocalRotation() const
{
	return mLocalRotation;
}

// ワールド回転角度を取得（ディグリー）
CVector CTransform::EulerAngles() const
{
	return mRotation.Euler();
}

// ローカル回転角度を取得（ディグリー）
CVector CTransform::LocalEulerAngles() const
{
	return mLocalRotation.Euler();
}

// ワールド回転値を設定
void CTransform::Rotation(const CQuaternion& rot)
{
	// 親が存在する場合
	if (mpParent != nullptr)
	{
		// ワールド回転値はそのまま設定
		mRotation = rot;
		// 親のワールド回転値の逆クォータニオン * 自身のワールド回転値 で
		// 自身のローカル回転値を求める
		mLocalRotation = mpParent->Rotation().Inverse() * mRotation;
	}
	// 親が存在しない場合
	else
	{
		// ワールド回転値、ローカル回転値にそのまま設定
		mRotation = rot;
		mLocalRotation = rot;
	}

	// 変更を子に反映
	ApplyChildren();
}

// ワールド回転値を設定
void CTransform::Rotation(float x, float y, float z)
{
	Rotation(CQuaternion(x, y, z));
}

// ワールド回転値を設定
void CTransform::Rotation(const CVector& eulerAngles)
{
	Rotation(CQuaternion(eulerAngles));
}

// ローカル回転値を設定
void CTransform::LocalRotation(const CQuaternion& rot)
{
	// 親が存在する場合
	if (mpParent != nullptr)
	{
		// ローカル回転値はそのまま設定
		mLocalRotation = rot;
		// 親のワールド回転値 * 自身のローカル回転値 で
		// 自身のワールド回転値を求める
		mRotation = mpParent->Rotation() * mLocalRotation;
	}
	// 親が存在しない場合
	else
	{
		// ローカル回転値、ワールド回転値にそのまま設定
		mLocalRotation = rot;
		mRotation = rot;
	}

	// 変更を子に反映
	ApplyChildren();
}

// ローカル回転値を設定
void CTransform::LocalRotation(float x, float y, float z)
{
	LocalRotation(CQuaternion(x, y, z));
}

// ローカル回転値を設定
void CTransform::LocalRotation(const CVector& eulerAngles)
{
	LocalRotation(CQuaternion(eulerAngles));
}

// ワールドスケール値を取得
const CVector& CTransform::Scale() const
{
	return mScale;
}

// ローカルスケール値を取得
const CVector& CTransform::LocalScale() const
{
	return mLocalScale;
}

// ワールドスケール値を設定
void CTransform::Scale(const CVector& scale)
{
	// 親が存在する場合
	if (mpParent != nullptr)
	{
		// ワールドスケール値はそのまま設定
		mScale = scale;
		// 自身のワールドスケール値 / 親のワールドスケール値 で
		// 自身のローカルスケール値を求める
		CVector parentScale = mpParent->Scale();
		mLocalScale.X(mScale.X() / parentScale.X());
		mLocalScale.Y(mScale.Y() / parentScale.Y());
		mLocalScale.Z(mScale.Z() / parentScale.Z());
	}
	// 親が存在しない場合
	else
	{
		// ローカルスケール値、ワールドスケール値にそのまま設定
		mScale = scale;
		mLocalScale = scale;
	}

	// 変更を子に反映
	ApplyChildren();
}

// ワールドスケール値を設定
void CTransform::Scale(float x, float y, float z)
{
	Scale(CVector(x, y, z));
}

// ローカルスケール値を設定
void CTransform::LocalScale(const CVector& scale)
{
	// 親が存在する場合
	if (mpParent != nullptr)
	{
		// ローカルスケール値はそのまま設定
		mLocalScale = scale;
		// 親のワールドスケール値 * 自身のローカルスケール値 で
		// 自身のワールドスケール値を求める
		CVector parentScale = mpParent->Scale();
		mScale.X(parentScale.X() * mLocalScale.X());
		mScale.Y(parentScale.Y() * mLocalScale.Y());
		mScale.Z(parentScale.Z() * mLocalScale.Z());
	}
	// 親が存在しない場合
	else
	{
		// ローカルスケール値、ワールドスケール値にそのまま設定
		mLocalScale = scale;
		mScale = scale;
	}

	// 変更を子に反映
	ApplyChildren();
}

// ローカルスケール値を設定
void CTransform::LocalScale(float x, float y, float z)
{
	LocalScale(CVector(x, y, z));
}

// 位置、回転値、スケール値を合わせた行列を取得
CMatrix CTransform::Matrix() const
{
	CMatrix s, r, t;
	s.Scale(mScale);
	r = mRotation.Matrix();
	t.Translate(mPosition);
	return s * r * t;
}

// 正面方向のベクトルを取得
CVector CTransform::VectorZ() const
{
	return mRotation * CVector::forward;
}

// 横方向のベクトルを取得
CVector CTransform::VectorX() const
{
	return mRotation * CVector::right;
}

// 上方向のベクトルを取得
CVector CTransform::VectorY() const
{
	return mRotation * CVector::up;
}

// 平行移動（現在の向きに対しての移動）
void CTransform::Translate(const CVector& translation)
{
	Position(mPosition + mRotation * translation);
}

// 平行移動（現在の向きに対しての移動）
void CTransform::Translate(float x, float y, float z)
{
	Translate(CVector(x, y, z));
}

// 回転
void CTransform::Rotate(const CVector& eulerAngles)
{
	Rotation(mRotation * CQuaternion(eulerAngles));
}

// 回転
void CTransform::Rotate(float x, float y, float z)
{
	Rotate(CVector(x, y, z));
}

// 任意軸回転
void CTransform::RotateAxis(const CVector& axis, float angle)
{
	CQuaternion q;
	q.RotationAxis(axis, angle);
	Rotation(q * mRotation);
}

// ターゲット位置へ向ける
void CTransform::LookAt(const CVector& target)
{
	CVector forward = (target - mPosition).Normalized();
	Rotation(CQuaternion::LookRotation(forward));
}

// 親を設定
void CTransform::SetParent(CTransform* parent)
{
	// 既に設定済みの親なら、処理しない
	if (mpParent == parent) return;

	// 既に他の親が設定されていれば、
	if (mpParent != nullptr)
	{
		// 他の親と親子関係を解除
		mpParent->RemoveChild(this);
		mpParent = nullptr;
		// 親子関係を解除したので、
		// 自身のローカル空間での位置、回転値、スケール値に
		// ワールド空間での各値を設定する
		LocalPosition(mPosition);
		LocalRotation(mRotation);
		LocalScale(mScale);
	}

	// 新しい親を設定
	mpParent = parent;

	// 新しい親が nullptr でなければ、
	if (mpParent != nullptr)
	{
		// 新しい親の子に自身を設定
		mpParent->AddChild(this);
		// 新しい親を設定したので、
		// 自身のローカル空間での位置、回転値、スケール値を再計算する
		// （ワールド空間での各値を設定し直すことで、ローカル空間の値も再計算される）
		Position(mPosition);
		Rotation(mRotation);
		Scale(mScale);
	}
}

// 親を取得
const CTransform* CTransform::GetParent() const
{
	return mpParent;
}

// 子のリストに追加
void CTransform::AddChild(CTransform* child)
{
	auto find = std::find(mChildren.begin(), mChildren.end(), child);
	if (find != mChildren.end()) return;
	mChildren.push_back(child);
}

// 子のリストから取り除く
void CTransform::RemoveChild(CTransform* child)
{
	mChildren.remove(child);
}

// 子に自身の変更を反映
void CTransform::ApplyChildren()
{
	for (CTransform* child : mChildren)
	{
		child->Update();
	}
}

// 更新
void CTransform::Update()
{
	// 親が存在する場合
	if (mpParent != nullptr)
	{
		// ワールド位置は、「親の位置 + 親の回転値 * ローカル位置」で求める
		mPosition = mpParent->Position() + mpParent->Rotation() * mLocalPosition;
		// ワールド回転値は、「親の回転値 * ローカル回転値」で求める
		mRotation = mpParent->Rotation() * mLocalRotation;
		// ワールドスケール値は、「親のスケール値 * ローカル回転値」で求める
		CVector parentScale = mpParent->Scale();
		mScale.X(parentScale.X() * mLocalScale.X());
		mScale.Y(parentScale.Y() * mLocalScale.Y());
		mScale.Z(parentScale.Z() * mLocalScale.Z());
	}
	// 親が存在しない場合
	else
	{
		// ワールド空間での各値とローカル空間での各値と同じ
		mPosition = mLocalPosition;
		mRotation = mLocalRotation;
		mScale = mLocalScale;
	}

	// 変更を子に反映
	ApplyChildren();
}
