#pragma once
#include "CVector.h"
#include "CTexture.h"
#include "CUIBase.h"
#include <functional>

class CFont;
class CText;
class CImage;

class CButton : public CUIBase
{
public:
	// コンストラクタ
	CButton(const CVector2& pos, const CVector2& size,
		ETaskPriority prio = ETaskPriority::eUI,
		int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eDefault,
		bool dontDelete = false,
		bool addTaskList = true);
	// デストラクタ
	virtual ~CButton();

	void LoadButtonText(CFont* font, int fontSize, std::string text);
	void LoadButtonImage(std::string normal, std::string select);

	void SetOnClickFunc(std::function<void()> func);

	void Update() override;
	void Render() override;

protected:
	void OnPointerEnter(const CVector2& pos) override;
	void OnPointerExit(const CVector2& pos) override;
	void OnPointerDown(const CVector2& pos) override;
	void OnPointerUp(const CVector2& pos) override;
	void OnMove(const CVector2& move) override;
	virtual void OnClick();

	enum class EState
	{
		eIdle,
		eOn,
		ePress,
	};
	virtual void ChangeState(EState state);

	EState mState;

private:
	void RenderQuad();

	std::function<void()> mOnClickFunc;

	CText* mpText;
	int mButtonTextSize;
	std::string mButtonText;
	CColor mButtonTextColor;

	CImage* mpNormalImage;
	CImage* mpSelectImage;

	CColor mButtonColor;
	CColor mEnterColor;
	CColor mPressColor;

	float mMoveDist;
};
