#pragma once
#include "CButton.h"

class CExpandButton : public CButton
{
public:
	CExpandButton(const CVector2& pos, const CVector2& size,
		ETaskPriority prio = ETaskPriority::eUI,
		int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eDefault,
		bool dontDelete = false,
		bool addTaskList = true);
	~CExpandButton();

	void Update() override;
	void Render() override;

protected:
	void OnPointerEnter(const CVector2& pos) override;
	void OnPointerExit(const CVector2& pos) override;
	void OnPointerDown(const CVector2& pos) override;
	void OnPointerUp(const CVector2& pos) override;
	void OnMove(const CVector2& move) override;

	void ChangeState(EState state) override;

private:
	float mEnterScale;
	float mPressScale;
	float mStartScale;
	float mEndScale;
	float mElapsedTime;
	CVector2 mBaseSize;
};
