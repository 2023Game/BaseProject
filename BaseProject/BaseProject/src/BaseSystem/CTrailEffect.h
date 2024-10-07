#pragma once
#include "CLineEffect.h"

// 軌跡エフェクト
class CTrailEffect : public CLineEffect
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="tag">オブジェクトタグ</param>
	/// <param name="owner">持ち主</param>
	/// <param name="mtx">行列</param>
	/// <param name="offsetPos">現在位置をズラすオフセット値</param>
	/// <param name="updateInterval">更新間隔（時間）</param>
	/// <param name="updateDist">更新間隔（距離）</param>
	/// <param name="startWidth">開始時の軌跡の幅</param>
	/// <param name="endWidth">終了時の軌跡の幅</param>
	/// <param name="showTime">軌跡の表示時間</param>
	CTrailEffect(ETag tag,
		CObjectBase* owner, CMatrix* mtx, CVector offsetPos,
		float updateInterval, float updateDist,
		float startWidth, float endWidth,
		float showTime);
	// デストラクタ
	~CTrailEffect();

	// 持ち主を設定
	void SetOwner(CObjectBase* obj);
	// 追跡する行列を設定
	void SetFollowMatrix(CMatrix* mtx);
	//// 設定された持ち主または行列の位置からのオフセット値を設定
	//void SetOffsetPos(const CVector& pos);

	// 更新
	void Update() override;

private:
	// 現在地を取得
	CVector GetCurrentPos() const;
	// 線の最後のポイントの位置を更新
	void UpdateLastPoint();

	CObjectBase* mpOwner;	// 持ち主のポインター
	CMatrix* mpMatrix;		// 追跡する行列のポインター
	CVector mOffsetPos;		// 現在地からズラすオフセット値
	float mUpdateInterval;	// 更新間隔（時間）
	float mUpdateDistance;	// 更新間隔（距離）
	float mStartWidth;		// 開始時の線の幅
	float mEndWidth;		// 終了時の線の幅
	float mShowTime;		// 表示時間

	CVector mLastPos;		// 前回の位置
	float mElapsedTime;		// 経過時間計測用
};