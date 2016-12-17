#include "UIPve.h"

#include "GameObj/ObjList.h"
#include "GameObj/Player.h"
#include "Utils/VisibleRect.h"
#include "Utils/StringHelper.h"
#include "Battle/BattleManager.h"
#include "MapGrid.h"
#include "GameDefine.h"

#include "JSGameBind.h"

using namespace ui;

UIPve::UIPve()
{
}

UIPve::~UIPve()
{
}

void UIPve::initWithUi(Node* pUi)
{
	IBattleUI::initWithUi(pUi);
	
	GameMapData mapData;
	mapData.tileColumn = TileColumn;
	mapData.tileWidth = TileWidth;
	mapData.tileRow = TileRow;
	mapData.tileHeight = TileHeight;

	_mapGrid = MapGrid::create(mapData, Vec2(MapStartX, MapStartY));
	auto bg = pUi->getChildByName("_layout")->getChildByName("img_bg");
	bg->addChild(_mapGrid);

	PlayerListIns->initTowers();
	EnemyListIns->initTowers();
	_mapGrid->initSettingArea(); // must after tower initial
}

