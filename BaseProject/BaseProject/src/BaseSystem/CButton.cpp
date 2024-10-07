#include "CButton.h"
#include "CFont.h"
#include "CText.h"
#include "CImage.h"

#define DEFAULT_BUTTON_COLOR CColor(1.0f, 1.0f, 1.0f)
#define DEFAULT_ENTER_COLOR CColor(0.875f, 0.875f, 0.875f)
#define DEFAULT_PRESS_COLOR CColor(0.75f, 0.75f, 0.75f)
#define DEFAULT_BUTTON_TEXT_COLOR CColor(0.0f, 0.0f, 0.0f)

CButton::CButton(const CVector2& pos, const CVector2& size,
	ETaskPriority prio, int sortOrder,
	ETaskPauseType pause, bool dontDelete, bool addTaskList)
	: CUIBase(prio, sortOrder, pause, dontDelete, addTaskList)
	, mpText(nullptr)
	, mButtonTextSize(32)
	, mButtonText("Button")
	, mButtonTextColor(DEFAULT_BUTTON_TEXT_COLOR)
	, mpNormalImage(nullptr)
	, mpSelectImage(nullptr)
	, mButtonColor(DEFAULT_BUTTON_COLOR)
	, mEnterColor(DEFAULT_ENTER_COLOR)
	, mPressColor(DEFAULT_PRESS_COLOR)
	, mMoveDist(0.0f)
{
	mPosition = pos;
	mSize = size;
}

CButton::~CButton()
{
	SAFE_DELETE(mpText);
	SAFE_DELETE(mpNormalImage);
	SAFE_DELETE(mpSelectImage);
}

void CButton::LoadButtonText(CFont* font, int fontSize, std::string text)
{
	mButtonTextSize = fontSize;
	mpText = new CText
	(
		nullptr, mButtonTextSize,
		mPosition, mSize,
		mButtonTextColor,
		GetPriority(), GetSortOrder(),
		GetPauseType(),
		false, false
	);
	mpText->SetTextAlignH(ETextAlignH::eCenter);
	mpText->SetTextAlignV(ETextAlignV::eMiddle);
}

void CButton::LoadButtonImage(std::string normal, std::string select)
{
	mpNormalImage = new CImage
	(
		normal.c_str(),
		GetPriority(), GetSortOrder(),
		GetPauseType(),
		false, false
	);
	mpSelectImage = new CImage
	(
		select.c_str(),
		GetPriority(), GetSortOrder(),
		GetPauseType(),
		false, false
	);
}

void CButton::SetOnClickFunc(std::function<void()> func)
{
	mOnClickFunc = func;
}

void CButton::OnPointerEnter(const CVector2& pos)
{
}

void CButton::OnPointerExit(const CVector2& pos)
{
}

void CButton::OnPointerDown(const CVector2& pos)
{
}

void CButton::OnPointerUp(const CVector2& pos)
{
	if (mMoveDist <= 10.0f)
	{
		OnClick();
	}
	mMoveDist = 0.0f;
}

void CButton::OnMove(const CVector2& move)
{
	mMoveDist += move.Length();
	//mPosition += move;
}

void CButton::OnClick()
{
	if (!IsEnable()) return;

	// コールバック関数が設定されていたら、
	if (mOnClickFunc != nullptr)
	{
		// コールバック関数を実行
		mOnClickFunc();
	}
}

void CButton::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
}

void CButton::Update()
{
	CUIBase::Update();

	if (mIsTouch)
	{
		mColor = mPressColor;
		ChangeState(EState::ePress);
	}
	else
	{
		if (mIsEnter)
		{
			mColor = mEnterColor;
			ChangeState(EState::eOn);
		}
		else
		{
			mColor = mButtonColor;
			ChangeState(EState::eIdle);
		}
	}

	CVector pos = mPosition;
	if (mpText != nullptr)
	{
		mpText->SetSize(mSize);
		mpText->SetPos(mPosition - mCenter);
	}
	if (mpNormalImage != nullptr && mpSelectImage != nullptr)
	{
		mpNormalImage->SetSize(mSize);
		mpNormalImage->SetPos(mPosition);
		mpNormalImage->SetCenter(mCenter);
		mpSelectImage->SetSize(mSize);
		mpSelectImage->SetPos(mPosition);
		mpSelectImage->SetCenter(mCenter);
	}
}

void CButton::RenderQuad()
{
	glPushMatrix();

	glLoadIdentity();

	//アルファブレンドを有効にする
	glEnable(GL_BLEND);
	//ブレンド方法を指定
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_TRIANGLES);
	// 表示カラーを設定
	glColor4fv((GLfloat*)&mColor);

	float vtxLeft = mPosition.X() - mCenter.X();
	float vtxRight = vtxLeft + mSize.X();
	float vtxBottom = mPosition.Y() - mCenter.Y();
	float vtxTop = vtxBottom + mSize.Y();

	glVertex2d(vtxLeft, vtxTop);
	glVertex2d(vtxLeft, vtxBottom);
	glVertex2d(vtxRight, vtxBottom);

	glVertex2d(vtxLeft, vtxTop);
	glVertex2d(vtxRight, vtxBottom);
	glVertex2d(vtxRight, vtxTop);

	glEnd();

	//テクスチャを解放
	glBindTexture(GL_TEXTURE_2D, 0);
	//アルファブレンドを無効
	glDisable(GL_BLEND);
	//テクスチャを無効
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void CButton::Render()
{
	if (mpNormalImage == nullptr || mpSelectImage == nullptr)
	{
		RenderQuad();
	}
	else
	{
		if (IsEnable() && mState != EState::eIdle)
		{
			mpSelectImage->Render();
		}
		else
		{
			mpNormalImage->Render();
		}
	}

	if (mpText != nullptr)
	{
		mpText->SetText(mButtonText.c_str());
		mpText->Render();
	}
}
