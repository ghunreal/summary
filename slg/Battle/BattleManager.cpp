#include "BattleManager.h"

#include "cocos2d.h"
#include "JSGameBind.h"
#include "Utils/VisibleRect.h"
#include "GameObj/Player.h"
#include "GameObj/ObjList.h"
#include "UIPve.h"

using namespace Game;

BattleManager* BattleManager::getInstance()
{
	return BattleManagerIns;
}

void BattleManager::startPve(cocos2d::Node* pUI)
{
	_curBattle = UIPveIns;
	_curBattle->initWithUi(pUI);
}

void BattleManager::startCountDown()
{
	_curBattle->startCountDown();
}

void BattleManager::endBattleFromJs()
{
	PlayerListIns->clear();
	EnemyListIns->clear();
	if (_curBattle)
	{
		_curBattle->closeUi();
	}
	_curBattle = NULL;
}

void BattleManager::endBattle(int playerStar, int enemyStar)
{
	_curBattle = NULL;
	jsval v[] = {
		v[0] = INT_TO_JSVAL(100),
		v[1] = INT_TO_JSVAL(playerStar),
		v[2] = INT_TO_JSVAL(enemyStar)
	};
	send_js_msg(3,v);
}

void BattleManager::addRole(ObjList* list, ICharacter* pChar)
{
	if (list == EnemyListIns)
	{
		//pChar->setBEnmey(true);
		pChar->playAction(ActionDef::Stand, 5);
	}
	else
	{
		pChar->playAction(ActionDef::Stand, 1);
	}
	list->add(pChar);
	
	getMapGrid()->addChild(pChar, MapGrid::ZOrder::Role);
}

MapGrid* BattleManager::getMapGrid() const
{ 
	if (_curBattle)
	{
		return _curBattle->getMapGrid();
	}
	return NULL;
}

Navi* BattleManager::getNavi() const
{
	if (_curBattle)
	{
		return _curBattle->getNavi();
	}
	return NULL;
}

InterfaceTest* InterfaceTest::getInstance()
{
	return Singleton<InterfaceTest>::instance();
}

void InterfaceTest::test()
{
	cocos2d::log("InterfaceTest::test");
}
