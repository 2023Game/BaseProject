#pragma once
#include "CObjectBase.h"

// カメラ
class CCamera : public CObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="eye">カメラの視点</param>
	/// <param name="center">カメラの注視点</param>
	/// <param name="isMainCamera">メインカメラかどうか</param>
	CCamera(const CVector& eye, const CVector& center, bool isMainCamera = true);
	// デストラクタ
	virtual ~CCamera();

	// メインカメラを取得
	static CCamera* MainCamera();
	// 現在のカメラを取得
	static CCamera* CurrentCamera();

	/// <summary>
	/// カメラの状態をコピー
	/// </summary>
	/// <param name="copy">コピー元のカメラ</param>
	void CopyCamera(CCamera* copy);

	/// <summary>
	/// 追従するターゲットを設定
	/// </summary>
	/// <param name="target">追従するターゲット</param>
	virtual void SetFollowTargetTf(CTransform* target);

	/// <summary>
	/// 追従時のオフセット座標を設定
	/// </summary>
	/// <param name="offset"></param>
	void SetFollowTargetOffset(const CVector& offset);

	/// <summary>
	/// 注視する位置を設定（注視点のみ）
	/// </summary>
	/// <param name="pos">注視する位置</param>
	void LookAt(const CVector& pos);
	/// <summary>
	/// 注視する位置を設定（注視点 + 上ベクトル）
	/// </summary>
	/// <param name="pos">注視する位置</param>
	/// <param name="up">上ベクトル</param>
	void LookAt(const CVector& pos, const CVector& up);
	/// <summary>
	/// 注視する位置を設定（視点 + 注視点 + 上ベクトル）
	/// </summary>
	/// <param name="eye">カメラの位置</param>
	/// <param name="at">注視する位置</param>
	/// <param name="up">上ベクトル</param>
	/// <param name="updateTargetEye">視点の目標位置も更新するかどうか</param>
	virtual void LookAt(const CVector& eye, const CVector& at, const CVector& up, bool updateTargetEye = true);

	/// <summary>
	/// 画面サイズ変更時のコールバック関数
	/// </summary>
	/// <param name="width">画面の幅</param>
	/// <param name="height">画面の高さ</param>
	void Reshape(int width, int height);
	/// <summary>
	/// 画面の描画エリアを指定
	/// </summary>
	/// <param name="x">横の描画開始位置</param>
	/// <param name="y">縦の描画開始位置</param>
	/// <param name="w">横幅</param>
	/// <param name="h">縦幅</param>
	void Viewport(float x, float y, float w, float h);
	/// <summary>
	/// 透視投影行列を設定
	/// </summary>
	/// <param name="fovy">画角</param>
	/// <param name="aspect">アスペクト比</param>
	/// <param name="zNear">描画最短距離</param>
	/// <param name="zFar">描画最長距離</param>
	void Perspective(float fovy, float aspect, float zNear, float zFar);

	/// <summary>
	/// スクリーン座標からワールド座標へ変換
	/// </summary>
	/// <param name="screenPos">スクリーン座標</param>
	/// <returns>ワールド座標</returns>
	CVector ScreenToWorldPos(const CVector& screenPos);
	/// <summary>
	/// ワールド座標からスクリーン座標へ変換
	/// </summary>
	/// <param name="worldPos">ワールド座標</param>
	/// <returns>スクリーン座標</returns>
	CVector WorldToScreenPos(const CVector& worldPos);

	/// <summary>
	/// このカメラを現在のカメラに設定するかどうか
	/// （解除した場合はメインカメラが現在のカメラに設定される）
	/// </summary>
	/// <param name="isCurrent">trueならば、現在のカメラに設定</param>
	void SetCurrent(bool isCurrent);

	/// <summary>
	/// ビュー行列を取得
	/// </summary>
	/// <returns>ビュー行列</returns>
	const CMatrix& GetViewMatrix() const;
	/// <summary>
	/// プロジェクション行列を取得
	/// </summary>
	/// <returns>プロジェクション行列</returns>
	const CMatrix& GetProjectionMatrix() const;
	/// <summary>
	/// ビューポート行列を取得
	/// </summary>
	/// <returns></returns>
	const CMatrix& GetViewportMatrix() const;

	/// <summary>
	/// 衝突判定を行うコライダーをリストに追加
	/// </summary>
	/// <param name="col"></param>
	void AddCollider(CCollider* col);
	/// <summary>
	/// 衝突判定を行うコライダーをリストから取り除く
	/// </summary>
	/// <param name="col"></param>
	void RemoveCollider(CCollider* col);

	/// <summary>
	/// コライダーとの衝突判定時に押し戻す距離の割合
	/// </summary>
	/// <param name="ratio"></param>
	void SetHitColliderRatio(float ratio);

	// 更新
	void Update() override;

	// 削除
	void Kill() override;

	// カメラ反映
	void Apply();

	// 2D用のカメラ行列を反映
	static void Start2DCamera();
	// 3D用のカメラ行列へ戻す
	static void End2DCamera();

protected:
	// 設定されているコライダーとの衝突結果を反映する
	void ApplyCollision();

	CVector mTargetEye;	// 視点の目標位置
	CVector mEye;		// 視点の実際の位置
	CVector mAt;		// 注視点
	CVector mUp;		// 上ベクトル
	CVector mEyeVec;	// 視点から注視点までのベクトル

	CTransform* mFollowTargetTf;	// 追従するターゲットのCTransfrom
	CVector mFollowOffsetPos;		// 追従ターゲットの位置から視点までのオフセット値

private:
	// カメラを削除
	void DeleteCamera();

	static std::list<CCamera*> spCameraList;// 生成されているカメラのリスト
	static CCamera* spMainCamera;	// メインカメラ
	static CCamera* spCurrentCamera;// 現在のカメラ

	bool mIsMainCamera;				// メインカメラかどうか

	CMatrix mViewMatrix;			// ビュー行列
	CMatrix mProjectionMatrix;		// プロジェクション行列
	CMatrix mViewportMatrix;		// ビューポート行列
	float mFovy;		// カメラの画角
	float mZNear;		// 描画最短距離
	float mZFar;		// 描画最長距離

	// カメラが衝突判定を行うコライダーのリスト
	std::list<CCollider*> mColliders;
	// カメラが衝突時に押し戻す割合
	float mHitColRatio;
};