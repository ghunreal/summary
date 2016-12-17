#pragma once

#include "IBattleUI.h"
#include "Utils/Singleton.hpp"

class UIPve : public IBattleUI
{
public:
	UIPve();
	~UIPve();

	virtual void initWithUi(Node* pUi);
};

#define UIPveIns Singleton<UIPve>::instance()

