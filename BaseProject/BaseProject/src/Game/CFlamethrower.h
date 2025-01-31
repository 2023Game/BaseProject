#pragma once
#include "CTask.h"
#include "CObjectBase.h"
#include <list>

class CFlame;

// 火炎放射クラス
class CFlamethrower : public CTask
{
public:
	// コンストラクタ
	CFlamethrower(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos = CVector::zero,
		const CMatrix& offsetRot = CMatrix::identity);
	CFlamethrower(CObjectBase* owner, const CMatrix* attach,
		const CVector& offsetPos,
		const CVector& throwDir);
	// デストラクタ
	~CFlamethrower();

	// 炎を発射開始
	void Start();
	// 炎を発射停止
	void Stop();

	// 炎を発射しているかどうか
	bool IsThrowing() const;

	// 発射方向を設定
	void SetThrowDir(const CVector& dir);
	// 発射時のオフセット位置を設定
	void SetThrowOffsetPos(const CVector& pos);
	// 発射時のオフセット回転値を設定
	void SetThrowOffsetRot(const CMatrix& rot);
	// 炎の色を設定
	void SetFlameColor(CColor color);
	// 加算ブレンドを使用するかを設定
	void SetAddBlend(bool use);
	// 炎のスケールの最大値を設定
	void SetFlameScale(float flameScale);
	// 炎のスケール値が最大値になるまでの時間を設定
	void SetFlameScaleAnimTime(float flameScaleAnimTime);
	// 炎の移動速度を設定
	void SetFlameMoveSpeed(float flameMoveSpeed);

	// 更新
	void Update() override;

private:
	// 炎のエフェクトを作成
	void CreateFlame();

	// 炎の発射位置を取得
	CVector GetThrowPos() const;
	// 炎の発射方向を取得
	CVector GetThrowDir() const;

	// 発射した炎のエフェクトのリスト
	std::list<CFlame*> mFlames;
	CObjectBase* mpOwner;		// このエフェクトの持ち主
	const CMatrix* mpAttachMtx;	// エフェクトをアタッチする行列
	CVector mThrowDir;			// 発射方向
	CVector mThrowOffsetPos;	// 発射時のオフセット位置
	CMatrix mThrowOffsetRot;	// 発射時のオフセット回転値
	float mElapsedTime;			// 経過時間計測用
	bool mIsThrowing;			// 炎を発射しているか
	CColor mFlameColor;			// 炎の色
	bool mIsAddBlend;			// 加算ブレンドを使用するかどうか
	float mFlameScale;			// 炎のスケール値の最大値
	float mFlameScaleAnimTime;	// 炎のスケール値が最大値になるまでの時間
	float mFlameMoveSpeed;		// 炎の移動速度
};
