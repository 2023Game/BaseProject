#pragma once
#include "CTask.h"
#include "CVector.h"
#include "CColor.h"

class CUIBase : public CTask
{
public:
	CUIBase(ETaskPriority prio = ETaskPriority::eUI, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame,
		bool dontDelete = false, bool addTaskList = true);
	virtual ~CUIBase();

	//表示座標を設定（CVector2版）
	virtual void SetPos(const CVector2& pos);
	//表示座標を設定
	virtual void SetPos(const float& x, const float& y);
	//表示座標を取得
	const CVector2& GetPos() const;

	//スケール値を設定
	virtual void SetScale(const float& scale);
	//スケール値を取得
	float GetScale() const;

	//表示サイズを設定（CVector2版）
	virtual void SetSize(const CVector2& size);
	//表示サイズを設定
	virtual void SetSize(const float& x, const float& y);
	//表示サイズを取得
	const CVector2& GetSize() const;

	//中心位置を設定（CVector2版）
	virtual void SetCenter(const CVector2& center);
	//中心位置を設定
	virtual void SetCenter(const float x, const float y);
	//中心位置を取得
	const CVector2& GetCenter() const;

	//表示カラーを設定（CColor版）
	virtual void SetColor(const CColor& color);
	//表示カラーを設定（アルファ値有り版）
	virtual void SetColor(const float& r, const float& g, const float& b, const float& a);
	//表示カラーを設定
	virtual void SetColor(const float& r, const float& g, const float& b);
	//表示カラーを取得
	const CColor& GetColor() const;

	//アルファ値を設定
	virtual void SetAlpha(const float& alpha);
	//アルファ値を取得
	float GetAlpha() const;

	//更新
	void Update() override;

protected:
	bool CollisionPoint(const CVector2& point);

	virtual void OnPointerEnter(const CVector2& pos);
	virtual void OnPointerExit(const CVector2& pos);
	virtual void OnPointerDown(const CVector2& pos);
	virtual void OnPointerUp(const CVector2& pos);
	virtual void OnMove(const CVector2& move);

protected:
	bool mIsTouch;
	bool mIsEnter;
	CVector2 mLastMousePos;
	CVector2 mPosition;	//表示座標
	CVector2 mSize;		//表示サイズ
	CVector2 mCenter;	//中心位置
	float mScale;
	CColor mColor;		//UIのカラー
};