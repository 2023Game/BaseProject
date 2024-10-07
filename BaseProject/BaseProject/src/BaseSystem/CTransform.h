#ifndef CTRANSFORM_H
#define CTRANSFORM_H
#include "CVector.h"
#include "CQuaternion.h"
#include <list>

/*
トランスフォームクラス
位置、回転、拡縮、変換行列の基本クラス
*/
class CTransform
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="rot">回転値（クォータニオン）</param>
	/// <param name="scale">スケール値</param>
	CTransform(const CVector& pos = CVector::zero,
		const CQuaternion& rot = CQuaternion::identity,
		const CVector& scale = CVector::one);
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="eulerAngles">回転角度 オイラー角（ディグリー）</param>
	/// <param name="scale">スケール値</param>
	CTransform(const CVector& pos, const CVector& eulerAngles, const CVector& scale);

	// デストラクタ
	~CTransform();

	// ワールド位置を取得
	const CVector& Position() const;
	/// <summary>
	/// ワールド位置を設定
	/// </summary>
	/// <param name="pos">ワールド位置</param>
	void Position(const CVector& pos);
	/// <summary>
	/// ワールド位置を設定
	/// </summary>
	/// <param name="x">ワールド位置 X座標</param>
	/// <param name="y">ワールド位置 Y座標</param>
	/// <param name="z">ワールド位置 Z座標</param>
	void Position(float x, float y, float z);

	// ローカル位置を取得
	const CVector& LocalPosition() const;
	/// <summary>
	/// ローカル位置を設定
	/// </summary>
	/// <param name="pos">ローカル位置</param>
	void LocalPosition(const CVector& pos);
	/// <summary>
	/// ローカル位置を設定
	/// </summary>
	/// <param name="x">ローカル位置 X座標</param>
	/// <param name="y">ローカル位置 Y座標</param>
	/// <param name="z">ローカル位置 Z座標</param>
	void LocalPosition(float x, float y, float z);

	// ワールド回転値（クォータニオン）を取得
	const CQuaternion& Rotation() const;
	// ローカル回転値（クォータニオン）を取得
	const CQuaternion& LocalRotation() const;

	// ワールド回転角度を取得（ディグリー）
	CVector EulerAngles() const;
	// ローカル回転角度を取得（ディグリー）
	CVector LocalEulerAngles() const;

	/// <summary>
	/// ワールド回転値を設定
	/// </summary>
	/// <param name="rot">ワールド回転値（クォータニオン）</param>
	void Rotation(const CQuaternion& rot);
	/// <summary>
	/// ワールド回転値を設定
	/// </summary>
	/// <param name="x">オイラー角 X軸（ディグリー）</param>
	/// <param name="y">オイラー角 Y軸（ディグリー）</param>
	/// <param name="z">オイラー角 Z軸（ディグリー）</param>
	void Rotation(float x, float y, float z);
	/// <summary>
	/// ワールド回転値を設定
	/// </summary>
	/// <param name="eulerAngles">オイラー角（ディグリー）</param>
	void Rotation(const CVector& eulerAngles);

	/// <summary>
	/// ローカル回転値を設定
	/// </summary>
	/// <param name="rot">ローカル回転値（クォータニオン）</param>
	void LocalRotation(const CQuaternion& rot);
	/// <summary>
	/// ローカル回転値を設定
	/// </summary>
	/// <param name="x">オイラー角 X軸（ディグリー）</param>
	/// <param name="y">オイラー角 Y軸（ディグリー）</param>
	/// <param name="z">オイラー角 Z軸（ディグリー）</param>
	void LocalRotation(float x, float y, float z);
	/// <summary>
	/// ローカル回転値を設定
	/// </summary>
	/// <param name="eulerAngles">オイラー角（ディグリー）</param>
	void LocalRotation(const CVector& eulerAngles);


	// ワールドスケール値を取得
	const CVector& Scale() const;
	// ローカルスケール値を取得
	const CVector& LocalScale() const;

	/// <summary>
	/// ワールドスケール値を設定
	/// </summary>
	/// <param name="scale">ワールドスケール値</param>
	void Scale(const CVector& scale);
	/// <summary>
	/// ワールドスケール値を設定
	/// </summary>
	/// <param name="x">ワールドスケール値 X軸</param>
	/// <param name="y">ワールドスケール値 Y軸</param>
	/// <param name="z">ワールドスケール値 Z軸</param>
	void Scale(float x, float y, float z);
	/// <summary>
	/// ローカルスケール値を設定
	/// </summary>
	/// <param name="scale">ローカルスケール値</param>
	void LocalScale(const CVector& scale);
	/// <summary>
	/// ローカルスケール値を設定
	/// </summary>
	/// <param name="x">ローカルスケール値 X軸</param>
	/// <param name="y">ローカルスケール値 Y軸</param>
	/// <param name="z">ローカルスケール値 Z軸</param>
	void LocalScale(float x, float y, float z);

	// 位置、回転値、スケール値を合わせた行列を取得
	virtual CMatrix Matrix() const;

	// 正面方向のベクトルを取得
	CVector VectorZ() const;
	// 横方向のベクトルを取得
	CVector VectorX() const;
	// 上方向のベクトルを取得
	CVector VectorY() const;

	/// <summary>
	/// 平行移動（現在の向きに対しての移動）
	/// </summary>
	/// <param name="translation">移動量</param>
	void Translate(const CVector& translation);
	/// <summary>
	/// 平行移動（現在の向きに対しての移動）
	/// </summary>
	/// <param name="x">X軸の移動量</param>
	/// <param name="y">Y軸の移動量</param>
	/// <param name="z">Z軸の移動量</param>
	void Translate(float x, float y, float z);

	/// <summary>
	/// 回転
	/// </summary>
	/// <param name="eulerAngles">回転角度（ディグリー）</param>
	void Rotate(const CVector& eulerAngles);
	/// <summary>
	/// 回転
	/// </summary>
	/// <param name="x">X軸の回転角度（ディグリー）</param>
	/// <param name="y">Y軸の回転角度（ディグリー）</param>
	/// <param name="z">Z軸の回転角度（ディグリー）</param>
	void Rotate(float x, float y, float z);
	/// <summary>
	/// 任意軸回転
	/// </summary>
	/// <param name="axis">回転軸</param>
	/// <param name="angle">回転角度（ディグリー）</param>
	void RotateAxis(const CVector& axis, float angle);

	/// <summary>
	/// ターゲット位置へ向ける
	/// </summary>
	/// <param name="target">ターゲット位置</param>
	void LookAt(const CVector& target);

	/// <summary>
	/// 親を設定
	/// </summary>
	/// <param name="parent">設定する親のCTransform</param>
	void SetParent(CTransform* parent);
	/// <summary>
	/// 親を取得
	/// </summary>
	/// <returns>設定されている親</returns>
	const CTransform* GetParent() const;

private:
	/// <summary>
	/// 子のリストに追加
	/// </summary>
	/// <param name="child">リストに追加する子のCTransform</param>
	void AddChild(CTransform* child);
	/// <summary>
	/// 子のリストから取り除く
	/// </summary>
	/// <param name="child">リストから取り除く子のCTransform</param>
	void RemoveChild(CTransform* child);

	// 子に自身の変更を反映
	void ApplyChildren();

	// 更新
	void Update();

	CVector mPosition;			// ワールド空間での位置
	CQuaternion mRotation;		// ワールド空間での回転値
	CVector mScale;				// ワールド空間でのスケール値

	CVector mLocalPosition;		// ローカル空間での位置
	CQuaternion mLocalRotation;	// ローカル空間での回転値
	CVector mLocalScale;		// ローカル空間でのスケール値

	CTransform* mpParent;		// 親のCTransform
	std::list<CTransform*> mChildren;	// 子のCTransformのリスト
};

#endif
