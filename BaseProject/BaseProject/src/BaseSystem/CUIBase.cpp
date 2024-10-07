#include "CUIBase.h"
#include "CUIManager.h"
#include "CInput.h"

CUIBase::CUIBase(ETaskPriority prio, int sortOrder, ETaskPauseType pause, bool dontDelete, bool addTaskList)
	: CTask(prio, sortOrder, pause, dontDelete, addTaskList)
	, mIsTouch(false)
	, mLastMousePos(CVector2::zero)
	, mPosition(0.0f, 0.0f)
	, mSize(100.0f, 100.0f)
	, mCenter(0.0f, 0.0f)
	, mScale(1.0f)
	, mColor(1.0f, 1.0f, 1.0f, 1.0f)
{
	CUIManager::Instance()->Add(this);
}

CUIBase::~CUIBase()
{
	CUIManager::Instance()->Remove(this);
}

//表示座標を設定（CVector2版）
void CUIBase::SetPos(const CVector2& pos)
{
	mPosition = pos;
}

//表示座標を設定
void CUIBase::SetPos(const float& x, const float& y)
{
	SetPos(CVector2(x, y));
}

//表示座標を取得
const CVector2& CUIBase::GetPos() const
{
	return mPosition;
}

//スケール値を設定
void CUIBase::SetScale(const float& scale)
{
	mScale = scale;
}

//スケール値を取得
float CUIBase::GetScale() const
{
	return mScale;
}

//表示サイズを設定（CVector2版）
void CUIBase::SetSize(const CVector2& size)
{
	mSize = size;
}

//表示サイズを設定
void CUIBase::SetSize(const float& x, const float& y)
{
	SetSize(CVector2(x, y));
}

//表示サイズを取得
const CVector2& CUIBase::GetSize() const
{
	return mSize;
}

//中心位置を設定（CVector2版）
void CUIBase::SetCenter(const CVector2& center)
{
	mCenter = center;
}

//中心位置を設定
void CUIBase::SetCenter(const float x, const float y)
{
	SetCenter(CVector2(x, y));
}

//中心位置を取得
const CVector2& CUIBase::GetCenter() const
{
	return mCenter;
}

//表示カラーを設定（CColor版）
void CUIBase::SetColor(const CColor& color)
{
	mColor = color;
}

//表示カラーを設定（アルファ値有り版）
void CUIBase::SetColor(const float& r, const float& g, const float& b, const float& a)
{
	mColor.Set(r, g, b, a);
}

//表示カラーを設定
void CUIBase::SetColor(const float& r, const float& g, const float& b)
{
	mColor.Set(r, g, b);
}

//表示カラーを取得
const CColor& CUIBase::GetColor() const
{
	return mColor;
}

//アルファ値を設定
void CUIBase::SetAlpha(const float& alpha)
{
	mColor.A(alpha);
}

//アルファ値を取得
float CUIBase::GetAlpha() const
{
	return mColor.A();
}

bool CUIBase::CollisionPoint(const CVector2& point)
{
	return CCollider::CollisionRectPoint(CRect(mPosition - mCenter, mSize), point);
}

void CUIBase::OnPointerEnter(const CVector2& pos)
{
}

void CUIBase::OnPointerExit(const CVector2& pos)
{
}

void CUIBase::OnPointerDown(const CVector2& pos)
{
}

void CUIBase::OnPointerUp(const CVector2& pos)
{
}

void CUIBase::OnMove(const CVector2& move)
{
}

void CUIBase::Update()
{
	CVector2 mousePos = CInput::GetMousePos();
	if (IsEnable() && !mIsTouch)
	{
		if (CInput::PushKey(VK_LBUTTON))
		{
			if (CollisionPoint(mousePos))
			{
				mIsTouch = true;
				OnPointerDown(mousePos);
				mLastMousePos = mousePos;
			}
		}
	}
	else
	{
		if (!IsEnable() || CInput::PullKey(VK_LBUTTON))
		{
			mIsTouch = false;
			OnPointerUp(mousePos);
		}
		else
		{
			CVector2 move = mousePos - mLastMousePos;
			if (move.LengthSqr() > 0.0f)
			{
				OnMove(move);
			}
			mLastMousePos = mousePos;
		}
	}

	if (!mIsEnter)
	{
		if (CollisionPoint(mousePos))
		{
			mIsEnter = true;
			OnPointerEnter(mousePos);
		}
	}
	else
	{
		if (!CollisionPoint(mousePos))
		{
			mIsEnter = false;
			OnPointerExit(mousePos);
		}
	}
}
