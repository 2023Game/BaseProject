#include "CUIManager.h"

CUIManager* CUIManager::msInstance = nullptr;

CUIManager::CUIManager()
{
}

CUIManager::~CUIManager()
{
}

CUIManager* CUIManager::Instance()
{
	if (msInstance == nullptr)
	{
		msInstance = new CUIManager();
	}
	return msInstance;
}

void CUIManager::ClearInstance()
{
	if (msInstance == nullptr) return;
	delete msInstance;
	msInstance = nullptr;
}

void CUIManager::Add(CUIBase* ui)
{
	auto begin = mUIList.begin();
	auto end = mUIList.end();
	auto find = std::find(begin, end, ui);
	if (find != end) return;

	mUIList.push_back(ui);
}

void CUIManager::Remove(CUIBase* ui)
{
	auto begin = mUIList.begin();
	auto end = mUIList.end();
	auto find = std::find(begin, end, ui);
	if (find == end) return;

	mUIList.erase(find);
}
