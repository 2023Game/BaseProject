#pragma once
#include "CObjectBase.h"
#include "CColor.h"

// コライダー表示クラスのベースクラス
class CColBase : public CObjectBase
{
public:
	// コンストラクタ
	CColBase(const CColor& defaultColor);
	// デストラクタ
	virtual ~CColBase();

	/// <summary>
	/// 選択状態を設定
	/// </summary>
	/// <param name="isSelect">trueなら選択</param>
	void SetSelect(bool isSelect);

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 更新
	void Update() override;

protected:
	/// <summary>
	/// 現在の色を取得
	/// </summary>
	/// <returns></returns>
	CColor GetColor() const;

	bool mIsSelect;			// 選択されているかどうか

private:
	CColor mDefaultColor;	// デフォルト色
	bool mIsCollision;		// 何かと衝突しているかどうか
};
