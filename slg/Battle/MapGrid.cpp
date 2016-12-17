#include "MapGrid.h"

#include "GameDefine.h"
#include "GameObj/ICharacter.h"
#include "GameObj/ObjList.h"

static const bool IsDrawGrids = true;
static const bool IsDrawObstacles = true;

PoolDrawNode::PoolDrawNode(int defaultLineWidth)
	:DrawNode(defaultLineWidth)
{

}

PoolDrawNode* PoolDrawNode::create(int defaultLineWidth)
{
	PoolDrawNode* ret = new PoolDrawNode(defaultLineWidth);
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}

	return ret;
}

MapGrid::MapGrid(const GameMapData& mapData, const Vec2& startPos)
	: _mapData(mapData), _startPos(startPos)
{
	initObstacles();
	drawMap();
}

MapGrid::~MapGrid(void)
{
}

bool MapGrid::isInMap(const Vec2& pt) const
{
	if (pt.x < _startPos.x || pt.y < _startPos.y)
		return false;

	Vec2 endPos = _startPos + Vec2(MapWidth,MapHeight);
	if (pt.x > endPos.x || pt.y > endPos.y)
		return false;
	return true;
}

void MapGrid::initSettingArea()
{
	_settingArea = SettingArea::create(this);
	this->addChild(_settingArea, ZOrder::SettingArea);
	_settingArea->setVisible(false);
}

MapGrid* MapGrid::create(const GameMapData& mapData, const Vec2& startPos)
{
	MapGrid* pRet = new MapGrid(mapData, startPos);
	pRet->init();
	return pRet;
}

Vec2 MapGrid::regularMapPos(const Vec2& pos) const
{
	Vec2 regularPos = pos-_startPos;
	int width = _mapData.tileWidth;
	int height = _mapData.tileHeight;
	regularPos.x = ((int)regularPos.x / width)*width+width/2;
	regularPos.y = ((int)regularPos.y / height)*height + height / 2;
	return regularPos + _startPos;
}

std::vector<int> MapGrid::getObstacles(const std::vector<Vec2>& mapPts)
{
	std::vector<int> infos;
	for (auto& it : mapPts)
	{
		int info = getObstacle(it);
		infos.push_back(info);
	}
	return infos;
}

void MapGrid::drawMap()
{
	_drawNode = PoolDrawNode::create(2);
	this->addChild(_drawNode);
	if (IsDrawGrids)
	{
		drawGrids();
	}
	if (IsDrawObstacles)
	{
		drawObstacle();
	}
}

Vec2 MapGrid::mapToGrid(const Vec2& mapPos) const
{
	Vec2 realPos = mapPos - _startPos;
	int iX = realPos.x / _mapData.tileWidth;
	int iY = realPos.y / _mapData.tileHeight;
	return Vec2(iX,iY);
}

Vec2 MapGrid::gridToMap(const Vec2& gridPos) const
{
	Vec2 ret;
	ret.x = gridPos.x * _mapData.tileWidth;
	ret.y = gridPos.y * _mapData.tileHeight;
	return ret + _startPos;
}

void MapGrid::initObstacles()
{
	_obstacles.resize(_mapData.tileColumn*_mapData.tileHeight,0);

	const int row = 2;
	const int column = TileColumn;
	int testArray[row][column] = { { 1, 1, 1, 0, 0, 0, 1, 1, 1 },
								{ 1, 1, 1, 0, 0, 0, 1, 1, 1 } };

	for (int y = 0; y < row; ++y)
	{
		for (int x = 0; x < column / 2; ++x)
		{
			testArray[y][column-x-1] = testArray[y][x];
		}
	}
	
	const int startRow = 9;
	for (int y = 0; y < row; ++y)
	{
		for (int x = 0; x < column; ++x)
		{
			int idx = getGridIdx(x, y + startRow);
			_obstacles[idx] = -testArray[y][x];
		}
	}

	int bottomObstacles[column] = { 1,1,1,1,1,1,1 };
	for (int x = 0; x < column / 2; ++x)
	{
		bottomObstacles[column - x - 1] = bottomObstacles[x];
	}
	for (int x = 0; x < column; ++x)
	{
		int bottom = getGridIdx(x, 0);
		int top = getGridIdx(x, TileRow - 1);
		_obstacles[bottom] = -bottomObstacles[x];
		_obstacles[top] = -bottomObstacles[x];
	}
	for (int y = 0; y < TileRow; ++y)
	{
		int left = getGridIdx(0, y);
		int right = getGridIdx(TileColumn - 1, y);
		_obstacles[left] = -1;
		_obstacles[right] = -1;
	}
}

void MapGrid::drawGrids()
{
	std::vector<int> horizontalVec;
	std::vector<int> verticalVec;
	for (int i = 0; i <= _mapData.tileColumn; ++i)
	{
		horizontalVec.push_back(_startPos.x + i*_mapData.tileWidth);
	}

	for (int i = 0; i <= _mapData.tileRow; ++i)
	{
		verticalVec.push_back(_startPos.y + i*_mapData.tileHeight);
	}
	Vec2 endPos;
	endPos.x = horizontalVec.back();
	endPos.y = verticalVec.back();
	drawGrids(horizontalVec, verticalVec, endPos);
}

void MapGrid::drawGrids(const std::vector<int>& horizontalVec, const std::vector<int>& verticalVec, const Vec2& endPos)
{	
	Color4F color(0.0f, 1.0f, 0.0f, 0.5f);

	// draw horizontal
	for (auto it : verticalVec)
	{
		_drawNode->drawLine(Vec2(_startPos.x, it), Vec2(endPos.x, it), color);
	}

	// draw vertical
	for (auto it : horizontalVec)
	{
		_drawNode->drawLine(Vec2(it, _startPos.y), Vec2(it, endPos.y), color);
	}
}

void MapGrid::drawObstacle()
{
	Color4F color(0.0f, 0.0f, 0.8f, 0.4f);
	for (unsigned int i = 0; i < _obstacles.size(); ++i)
	{
		if (_obstacles[i] != 0)
		{
			drawGrid(i, color);
		}
	}
}

bool MapGrid::isObstacles(int tileX, int tileY) const
{
	int idx = getGridIdx(tileX, tileY);
	if (idx >= 0 && idx < (int)_obstacles.size())
	{
		return _obstacles[idx]!=0;
	}
	return false;
}

int MapGrid::getGridIdx(int iX, int iY) const
{
	return _mapData.tileColumn*iY + iX;
}

Vec2 MapGrid::idxToGrid(int idx)
{
	Vec2 pos;
	pos.x = idx % _mapData.tileColumn;
	pos.y = idx / _mapData.tileColumn;
	return pos;
}

void MapGrid::drawGrid(int gridIdx, const Color4F& color)
{
	Vec2 startPos = idxToGrid(gridIdx);
	startPos = gridToMap(startPos);
	Vec2 endPos = startPos + Vec2(_mapData.tileWidth, _mapData.tileHeight);

	_drawNode->drawSolidRect(startPos, endPos, color);
	//_drawNode->drawLine(startPos, Vec2(startPos.x, endPos.y), color);
	//_drawNode->drawLine(startPos, Vec2(endPos.x, startPos.y), color);
	//_drawNode->drawLine(Vec2(startPos.x, endPos.y), endPos, color);
	//_drawNode->drawLine(Vec2(endPos.x, startPos.y), endPos, color);
}

int MapGrid::getObstacle(const Vec2& mapPos) const
{
	Vec2 gridPos = mapToGrid(mapPos);
	int idx = getGridIdx(gridPos.x, gridPos.y);
	if (idx >= 0 && idx < (int)_obstacles.size())
	{
		return _obstacles[idx];
	}
	return 0;
}

SettingArea* MapGrid::getSettingArea()
{ 
	return _settingArea; 
}

void MapGrid::showSettingArea(bool isVisible)
{ 
	_settingArea->setVisible(isVisible); 
}

/*

void MapGrid::initAStar(int row, int column)
{
	AStar::Param param;
	param.row = row;
	param.column = column;
	param.isObstacles = CC_CALLBACK_2(MapGrid::isObstacles,this);

	_aStar.InitParam(param);
}

std::list<Vec2> MapGrid::findPath(const Vec2& startPt, const Vec2& endPt, int gridNum)
{
	Vec2 gridStart = mapToGrid(startPt);
	Vec2 gridEnd = mapToGrid(endPt);
	AStar::PathPos start(gridStart.x, gridStart.y);
	AStar::PathPos end(gridEnd.x, gridEnd.y);

	std::vector<Vec2> startPts = CharacterMode::getModePts(startPt, gridNum);
	std::vector<Vec2> endPts = CharacterMode::getModePts(endPt, gridNum);
	std::vector<int> startInfos = getObstacles(startPts);
	std::vector<int> endInfos = getObstacles(endPts);
	clearObstacles(startPts);
	clearObstacles(endPts);
	Vec2 body = CharacterMode::getBodyInfo(gridNum);
	std::list<AStar::PathPos> list = _aStar.Search(start, end, body.x, body.y);
	resetObstacles(startPts, startInfos);
	resetObstacles(endPts, endInfos);

	std::list<Vec2> ret;
	for (auto& it : list)
	{
		ret.push_back(gridToMap(Vec2(it.x, it.y)));
	}
	return ret;
}

void MapGrid::setObstacle(const Vec2& grid, int info)
{
	int idx = getGridIdx(grid.x, grid.y);
	if (idx >= 0 && idx < (int)_obstacles.size())
	{
	_obstacles[idx] = info;
	}
}

void MapGrid::clearObstacles(const std::vector<Vec2>& pts)
{
	for (auto& it : pts)
	{
		setObstacle(mapToGrid(it), 0);
	}
}

void MapGrid::resetObstacles(const std::vector<Vec2>& pts, const std::vector<int>& infos)
{
	CC_ASSERT(pts.size() == infos.size());
	int i = 0;
	for (auto& it : pts)
	{
		setObstacle(mapToGrid(it), infos[i++]);
	}
}
*/

/********************************************MapGrid End**********************************************/

SettingArea* SettingArea::create(MapGrid* parent, int defaultLineWidth)
{
	SettingArea* ret = new (std::nothrow) SettingArea();
	if (ret && ret->init())
	{
		ret->setLocalZOrder(MapGrid::ZOrder::SettingArea);
		ret->initArea(parent, defaultLineWidth); 
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}

	return ret;
}

void SettingArea::initArea(MapGrid* parent, int defaultLineWidth)
{
	_pParent = parent;
	_lineWidth = defaultLineWidth;
	_drawNode = PoolDrawNode::create(defaultLineWidth);
	addChild(_drawNode);
	drawArea();
}

void SettingArea::redraw()
{
	removeChild(_drawNode);
	_drawNode = PoolDrawNode::create(_lineWidth);
	addChild(_drawNode);
	drawArea();
}

void SettingArea::drawArea()
{
	drawRect();
	drawLine();
}

void SettingArea::drawRect()
{
	drawKingRect();
	drawTowerRect();
}

void SettingArea::drawKingRect()
{
	Color4F color(1.0f, 1.0f, 0.0f, 0.3f);
	const Vec2& startPos = _pParent->getStartPos();
	int endY = startPos.y + MapHeight;
	int startY = startPos.y + TileHeight*KingStartGrid;
	_drawNode->drawSolidRect(Vec2(startPos.x,startY), Vec2(startPos.x+MapWidth,endY), color);
}

void SettingArea::drawTowerRect()
{
	Color4F color(1.0f, 1.0f, 0.0f, 0.3f);
	const Vec2& startPos = _pParent->getStartPos();
	int endY = startPos.y + TileHeight*KingStartGrid;
	int startY = startPos.y + TileHeight * TowerStartGrid;
	if (!EnemyListIns->getLeftTower()->isDead())
	{
		_drawNode->drawSolidRect(Vec2(startPos.x, startY), Vec2(startPos.x + MapWidth / 2, endY), color);
	}
	if (!EnemyListIns->getRightTower()->isDead())
	{
		_drawNode->drawSolidRect(Vec2(startPos.x + MapWidth / 2, startY), Vec2(startPos.x + MapWidth, endY), color);
	}
}

void SettingArea::drawLine()
{
	drawKingLine();
	drawTowerLine();
}

void SettingArea::drawKingLine()
{
	Color4F color(1.0f, 1.0f, 0.0f, 1.0f);
	const Vec2& startPos = _pParent->getStartPos();
	int endY = startPos.y + MapHeight;
	int midX = startPos.x + MapWidth / 2;

	_drawNode->drawLine(Vec2(midX - 2 * TileWidth, endY), Vec2(midX + 2 * TileWidth, endY), color);
	_drawNode->drawLine(Vec2(midX - 2 * TileWidth, endY), Vec2(midX - 2 * TileWidth, endY-TileHeight), color);
	_drawNode->drawLine(Vec2(midX + 2 * TileWidth, endY), Vec2(midX + 2 * TileWidth, endY - TileHeight), color);
	_drawNode->drawLine(Vec2(startPos.x, endY - TileHeight), Vec2(midX - 2 * TileWidth, endY - TileHeight), color);
	_drawNode->drawLine(Vec2(midX + 2 * TileWidth, endY - TileHeight), Vec2(startPos.x + MapWidth, endY - TileHeight), color);
}

void SettingArea::drawTowerLine()
{
	Color4F color(1.0f, 1.0f, 0.0f, 1.0f);
	Vec2 startPos = _pParent->getStartPos();
	int endY = startPos.y + MapHeight - TileHeight;
	int midY = startPos.y + KingStartGrid*TileHeight;
	startPos.y += TileHeight * TowerStartGrid;
	int endX = startPos.x + MapWidth;
	int midX = startPos.x + MapWidth / 2;
	
	if (!EnemyListIns->getLeftTower()->isDead())
	{
		_drawNode->drawLine(Vec2(startPos), Vec2(startPos.x, endY), color);

		if (!EnemyListIns->getRightTower()->isDead())
		{
			_drawNode->drawLine(Vec2(endX, startPos.y), Vec2(endX, endY), color);
			_drawNode->drawLine(Vec2(startPos), Vec2(endX, startPos.y), color);
		}
		else
		{
			_drawNode->drawLine(Vec2(startPos), Vec2(midX, startPos.y), color);
			_drawNode->drawLine(Vec2(midX, startPos.y), Vec2(midX, midY), color);
			_drawNode->drawLine(Vec2(midX, midY), Vec2(endX, midY), color);
			_drawNode->drawLine(Vec2(endX, midY), Vec2(endX, endY), color);
		}
	}
	else
	{
		if (EnemyListIns->getRightTower()->isDead())
		{
			_drawNode->drawLine(Vec2(startPos.x, midY), Vec2(endX, midY), color);
		}
		else
		{
			_drawNode->drawLine(Vec2(startPos.x, midY), Vec2(midX, midY), color);
			_drawNode->drawLine(Vec2(midX, midY), Vec2(midX, startPos.y), color);
			_drawNode->drawLine(Vec2(midX, startPos.y), Vec2(endX, startPos.y), color);			
			_drawNode->drawLine(Vec2(endX, startPos.y), Vec2(endX, endY), color);
		}
	}
}

bool SettingArea::isSettingPt(const Vec2& mapPt, int gridNum)
{
	std::vector<Vec2> endPts = CharacterMode::getModePts(mapPt, gridNum);
	std::vector<int> infos = _pParent->getObstacles(endPts);
	for (auto it : infos)
	{
		if (it != 0)
			return false;
	}

	if (checkKingArea(endPts)
		&& checkTower(EnemyListIns->getLeftTower(), endPts)
		&& checkTower(EnemyListIns->getRightTower(), endPts))
	{
		return true;
	}
	return false;
}

bool SettingArea::checkKingArea(const std::vector<Vec2>& endPts)
{
	Vec2 startPos = _pParent->getStartPos();
	Vec2 endPos;
	endPos.y = startPos.y + MapHeight;
	endPos.x = startPos.x + MapWidth;
	startPos.y += TileHeight * KingStartGrid;

	for (auto& it : endPts)
	{
		if (isFit(startPos, it, endPos))
			return false;
	}
	return true;
}

bool SettingArea::checkTower(const Tower* tower, const std::vector<Vec2>& endPts)
{
	if (!tower->isDead())
	{
		bool isLeft = tower == EnemyListIns->getLeftTower();
		Vec2 startPos = _pParent->getStartPos();
		Vec2 endPos;
		endPos.y = startPos.y + TileHeight * KingStartGrid;
		endPos.x = isLeft ? (startPos.x + MapWidth / 2) : (startPos.x+MapWidth);
		startPos.y += TileHeight * TowerStartGrid;
		startPos.x = isLeft ? startPos.x : (startPos.x + MapWidth / 2);
		for (auto& it : endPts)
		{
			if (isFit(startPos, it, endPos))
				return false;
		}
	}
	return true;
}

bool SettingArea::lessEqualVec2(const Vec2& first, const Vec2& second)
{
	return first.x <= second.x && first.y <= second.y;
}

bool SettingArea::isFit(const Vec2& first, const Vec2& mid, const Vec2& end)
{
	return lessEqualVec2(first, mid) && lessEqualVec2(mid, end);
}

/********************************************SettingArea End******************************************/

RunningDrawNode* RunningDrawNode::create(int defaultLineWidth)
{
	RunningDrawNode* ret = new (std::nothrow) RunningDrawNode();
	if (ret && ret->init())
	{
		ret->setLocalZOrder(MapGrid::ZOrder::RunningWay);
		ret->_drawNode = PoolDrawNode::create(defaultLineWidth);
		ret->addChild(ret->_drawNode);
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}

	return ret;
}

void RunningDrawNode::drawGrid(const Vec2& startPos)
{
	return;
	Color4F color(0.8f, 0.0f, 0.0f, 0.5f);
	Vec2 endPos = startPos + Vec2(TileWidth, TileHeight);
	_drawNode->drawSolidRect(startPos, endPos, color);
}

/********************************************RunningDrawNode End******************************************/

