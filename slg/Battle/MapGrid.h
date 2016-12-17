#pragma once

#include <list>
#include <vector>
#include "cocos2d.h"
#include "PathFinding/AStar.h"
#include "Utils/BlockAllocator.h"

USING_NS_CC;

struct GameMapData
{
	int tileWidth;
	int tileHeight;
	int tileRow;
	int tileColumn;
};

class SettingArea;
class Tower;

class PoolDrawNode : public DrawNode
{
public:
	UseBlockAllocator

	PoolDrawNode(int defaultLineWidth=2);
	static PoolDrawNode* create(int defaultLineWidth);
};

class MapGrid : public Node
{
public:
	struct ZOrder
	{
		static const int Base = 0;
		static const int Obstacle = 1;
		static const int RunningWay = 2;
		static const int SettingArea = 3;

		static const int Role = 5;
	};

	MapGrid(const GameMapData& mapData, const Vec2& startPos);
	~MapGrid(void);

	bool isInMap(const Vec2& pt) const;
	void initSettingArea();

	static MapGrid* create( const GameMapData& mapData, const Vec2& startPos=Vec2::ZERO );
	const GameMapData& getMapData() const { return _mapData; }
	const Vec2& getStartPos() const { return _startPos; }

	bool isObstacles(int tileX, int tileY) const;
	Vec2 regularMapPos(const Vec2& pos) const;

	SettingArea* getSettingArea();
	void showSettingArea(bool isVisible);

private:
	friend class  SettingArea;
	
	GameMapData _mapData;
	Vec2 _startPos;	
	std::vector<int> _obstacles; // -1: map obstacle like river; >0: obj obstacle
	PoolDrawNode* _drawNode;
	SettingArea* _settingArea;

	void drawMap();
	Vec2 mapToGrid(const Vec2& mapPos) const;
	Vec2 gridToMap(const Vec2& gridPos) const;

	void initObstacles();
	void drawGrids();
	void drawGrids( const std::vector<int>& horizontalVec, const std::vector<int>& verticalVec, const Vec2& endPos);
	void drawGrid(int gridIdx, const Color4F& color);
	void drawObstacle();
	int getGridIdx(int iX, int iY) const;
	Vec2 idxToGrid(int idx);

	int getObstacle(const Vec2& mapPos) const;
	std::vector<int> getObstacles(const std::vector<Vec2>& mapPts);
/* AStar find
	AStar _aStar;
	void initAStar( int row, int column );
	std::list<Vec2> findPath(const Vec2& startPt, const Vec2& endPt, int gridNum);
	void setObstacle(const Vec2& grid, int info);
	void clearObstacles(const std::vector<Vec2>& pts);
	void resetObstacles(const std::vector<Vec2>& pts, const std::vector<int>& infos);
*/
};

class RunningDrawNode : public Node
{
public:
	static RunningDrawNode* create(int defaultLineWidth = DEFAULT_LINE_WIDTH);
	void drawGrid( const Vec2& mapPos );
private:
	PoolDrawNode* _drawNode;
};

class SettingArea : public Node
{
public:
	static SettingArea* create( MapGrid* parent, int defaultLineWidth = 5);
	void redraw();
	bool isSettingPt(const Vec2& mapPt, int gridNum);
private:
	static const int KingStartGrid = 15;
	static const int TowerStartGrid = 9;

	PoolDrawNode* _drawNode;
	MapGrid* _pParent;
	int _lineWidth;

	void initArea(MapGrid* parent, int defaultLineWidth);
	void drawArea();

	void drawRect();
	void drawKingRect();
	void drawTowerRect();

	void drawLine();
	void drawKingLine();
	void drawTowerLine();

	bool checkKingArea( const std::vector<Vec2>& endPts );
	bool checkTower(const Tower* tower,const std::vector<Vec2>& endPts);

	bool lessEqualVec2(const Vec2& first, const Vec2& second);
	bool isFit(const Vec2& first, const Vec2& mid, const Vec2& end);
};
