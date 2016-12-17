#pragma once

#include "cocos2d.h"
#include "Utils/Singleton.hpp"
#include "navi.h"
#include "BattleData.h"

class IBattleUI;
class ICharacter;
class ObjList;
class MapGrid;

using namespace cocos2d;
using namespace knavi;

namespace Game 
{
	class BattleManager : public Ref
	{
	public:
		/***************jsb begin*******************/
		static BattleManager* getInstance();
		void startPve( Node* pUI );
		void startCountDown();
		void endBattle( int playerStar, int enemyStar );
		void endBattleFromJs();
		/***************jsb end*******************/

		void addRole(ObjList* list, ICharacter* pChar);
		IBattleUI* getCurBattle() const { return _curBattle; }
		MapGrid* getMapGrid() const;
		Navi* getNavi() const;

	private:
		IBattleUI* _curBattle;
	};

	class InterfaceTest : public Ref
	{
	public:

		static InterfaceTest* getInstance();
		void test();
	};

	#define BattleManagerIns Singleton<BattleManager>::instance()
};
