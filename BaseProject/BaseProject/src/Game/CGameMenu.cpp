#include "CGameMenu.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"

#define MENU_ALPHA 0.75f

CGameMenu::CGameMenu()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eMenu)
	, mSelectIndex(0)
	, mIsOpened(false)
{
	mpBackground = new CImage
	(
		"UI/menu_back.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpBackground->SetCenter(mpBackground->GetSize() * 0.5f);
	mpBackground->SetPos(CVector2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f);
	mpBackground->SetColor(1.0f, 1.0f, 1.0f, MENU_ALPHA);

	int menuItemCount = 3;
	float spaceY = (float)WINDOW_HEIGHT / (menuItemCount + 1);
	for (int i = 0; i < menuItemCount; i++)
	{
		CImage* item = new CImage
		(
			"UI/menu_item.png",
			ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
			false, false
		);
		item->SetCenter(item->GetSize() * 0.5f);
		float posX = (1280.0f - 1024.0f + item->GetSize().X()) * 0.5f + 100.0f;
		item->SetPos(posX, spaceY * (i + 1));
		item->SetColor(1.0f, 1.0f, 1.0f, MENU_ALPHA);
		mMenuItems.push_back(item);
	}

	mpSelectFrame = new CImage
	(
		"UI/menu_item_select.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpSelectFrame->SetCenter(mpSelectFrame->GetSize() * 0.5f);
	mpSelectFrame->SetColor(1.0f, 0.5f, 0.0f, MENU_ALPHA);

	SetEnable(false);
	SetShow(false);
}

CGameMenu::~CGameMenu()
{
}

void CGameMenu::Open()
{
	SetEnable(true);
	SetShow(true);
	mSelectIndex = 0;
	CBGMManager::Instance()->Play(EBGMType::eMenu, false);
	CTaskManager::Instance()->Pause(PAUSE_MENU_OPEN);
}

void CGameMenu::Close()
{
	SetEnable(false);
	SetShow(false);
	CBGMManager::Instance()->Play(EBGMType::eGame, false);
	CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);
}

bool CGameMenu::IsOpened() const
{
	return mIsOpened;
}

void CGameMenu::Decide(int select)
{
	switch (select)
	{
		case 0:
		case 1:
			break;
		case 2:
			Close();
			break;
	}
}

void CGameMenu::Update()
{
	int itemCount = mMenuItems.size();
	if (CInput::PushKey(VK_UP))
	{
		mSelectIndex = (mSelectIndex + itemCount - 1) % itemCount;
	}
	else if (CInput::PushKey(VK_DOWN))
	{
		mSelectIndex = (mSelectIndex + 1) % itemCount;
	}
	else if (CInput::PushKey(VK_RETURN))
	{
		Decide(mSelectIndex);
	}

	mpBackground->Update();
	for (CImage* item : mMenuItems)
	{
		item->Update();
	}
	mpSelectFrame->Update();
}

void CGameMenu::Render()
{
	mpBackground->Render();
	for (int i = 0; i < mMenuItems.size(); i++)
	{
		CImage* item = mMenuItems[i];
		item->Render();

		if (i == mSelectIndex)
		{
			mpSelectFrame->SetPos(item->GetPos());
			mpSelectFrame->Render();
		}
	}
}
