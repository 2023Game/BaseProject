#pragma once
#include "CUIBase.h"

class CUIManager
{
private:
	static CUIManager* msInstance;
	std::vector<CUIBase*> mUIList;

public:
	CUIManager();
	~CUIManager();

	static CUIManager* Instance();
	static void ClearInstance();

	void Add(CUIBase* ui);
	void Remove(CUIBase* ui);
};