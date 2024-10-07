#include "CExpandButton.h"
#include "CFont.h"
#include "Easing.h"
#include "Maths.h"

#define EXPAND_ANIM_TIME 0.25f

CExpandButton::CExpandButton(const CVector2& pos, const CVector2& size,
	ETaskPriority prio, int sortOrder,
	ETaskPauseType pause, bool dontDelete, bool addTaskList)
	: CButton(pos, size, prio, sortOrder, pause, dontDelete, addTaskList)
	, mEnterScale(1.5f)
	, mPressScale(1.25f)
	, mStartScale(1.0f)
	, mEndScale(1.0f)
	, mElapsedTime(0.0f)
{
	mBaseSize = mSize;
	mCenter = mBaseSize * 0.5f;
}

CExpandButton::~CExpandButton()
{
}

void CExpandButton::OnPointerEnter(const CVector2& pos)
{
	CButton::OnPointerEnter(pos);
}

void CExpandButton::OnPointerExit(const CVector2& pos)
{
	CButton::OnPointerExit(pos);
}

void CExpandButton::OnPointerDown(const CVector2& pos)
{
	CButton::OnPointerDown(pos);
}

void CExpandButton::OnPointerUp(const CVector2& pos)
{
	CButton::OnPointerUp(pos);
}

void CExpandButton::OnMove(const CVector2& move)
{
	CButton::OnMove(move);
}

void CExpandButton::ChangeState(EState state)
{
	if (!IsEnable()) return;
	if (state == mState) return;

	mState = state;

	mStartScale = mScale;
	if (mState == EState::eIdle) mEndScale = 1.0f;
	else if (mState == EState::eOn) mEndScale = mEnterScale;
	else if (mState == EState::ePress) mEndScale = mPressScale;
	mElapsedTime = 0.0f;
}

void CExpandButton::Update()
{
	CButton::Update();

	if (IsEnable())
	{
		if (mElapsedTime < EXPAND_ANIM_TIME)
		{
			float per = Easing::QuintOut(mElapsedTime, EXPAND_ANIM_TIME, 0.0f, 1.0f);
			mScale = Math::Lerp(mStartScale, mEndScale, per);
			mSize = mBaseSize * mScale;

			mElapsedTime += Time::DeltaTime();
		}
		else
		{
			mScale = mEndScale;
		}
	}
	mSize = mBaseSize * mScale;
	mCenter = mSize * 0.5f;
}

void CExpandButton::Render()
{
	CButton::Render();
}
