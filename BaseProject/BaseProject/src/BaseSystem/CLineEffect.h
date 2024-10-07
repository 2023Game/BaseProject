#pragma once
#include "CEffect.h"
#include "CTexture.h"

class CLinePoint
{
public:
	CVector pos;
	float startWidth;
	float endWidth;
	float elapsed;
	float alpha;
	CLinePoint(const CVector& p, float sw, float ew)
		: pos(p), startWidth(sw), endWidth(ew), elapsed(0.0f), alpha(1.0f) {};
};

// 線分エフェクト
class CLineEffect : public CEffect
{
public:
	// コンストラクタ
	CLineEffect(ETag tag);
	// デストラクタ
	virtual ~CLineEffect();

	// 持ち主を設定
	void SetOwner(CObjectBase* owner);

	// テクスチャ設定
	void SetTexture(std::string texName);

	// UV設定
	void SetBaseUV(const CRect& uv);

	// アニメーションデータを設定
	void SetAnimData(TexAnimData* animData);

	// アニメーションが終わったかどうか
	bool IsEndAnim() const;

	// 線のポイントを追加
	int AddPoint(const CVector& pos, float startWidth, float endWidth);
	// 線のポイントを削除
	void RemovePoint(int index);

	// 線のポイント数を取得
	int GetPointCount() const;

	// 線のポイントの座標を設定
	void SetPos(int index, const CVector& pos);
	// 線のポイントの座標を取得
	CVector GetPos(int index) const;

	//// 線のポイントの幅を設定
	//void SetWidth(int index, float width);
	//// 線のポイントの幅を取得
	//float GetWidth(int index) const;

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// 座標を計算
	CVector CalcPos(const CVector& pos) const;
	// アニメーションを再生できるかどうか
	bool IsEnableAnim() const;

protected:
	// 持ち主
	CObjectBase* mpOwner;

	// 線のテクスチャ
	CTexture* mpTexture;
	// 線のポイントのリスト
	std::vector<CLinePoint*> mPoints;

	// アニメーションデータのリスト
	TexAnimData* mpAnimData;
	// 現在のアニメーション番号
	int mAnimNo;
	// アニメーション用の経過時間計測変数
	float mElapsedTime;

	CRect mBaseUV;
	CRect mUV;
};