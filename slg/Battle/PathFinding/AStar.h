/**
 * A* 算法
 * author: zhangpanyi@live.com
 * https://github.com/zhangpanyi/a-star.git
 */

#ifndef __ASTAR_H__
#define __ASTAR_H__

#include <list>
#include <vector>
#include <functional>

/**
 * A-Star algorithm
 */
class AStar
{
public:
	/**
	 * 二维坐标
	 */
	struct PathPos
	{
		int x;
		int y;

		PathPos()
			: x(0)
			, y(0)
		{
		}

		PathPos(int _x, int _y)
			: x(_x)
			, y(_y)
		{
		}

		bool operator== (const PathPos &that) const
		{
			return x == that.x && y == that.y;
		}
	};

	typedef std::function<bool(int x, int y)> ObstaclesCheck;

	/**
	 * 搜索参数
	 */
	struct Param
	{
		int		row;
		int		column;

		ObstaclesCheck	isObstacles;

		Param()
			: row(0)
			, column(0)
			, isObstacles(nullptr)
		{
		}

		Param(int _row, int _column, const ObstaclesCheck &_is_canreach)
			: row(_row)
			, column(_column)
			, isObstacles(_is_canreach)
		{

		}
	};

private:
	/**
	 * 路径节点状态
	 */
	enum NodeState
	{
		NOTEXIST,
		IN_OPENLIST,
		IN_CLOSELIST
	};

	/**
	 * 路径节点
	 */
	struct PathNode
	{
		int		g;
		int		h;
		PathPos			pos;
		NodeState		state;
		PathNode*			parent;

		int f() const
		{
			return g + h;
		}

		PathNode()
			: g(0)
			, h(0)
			, parent(nullptr)
			, state(NOTEXIST)
		{}

		PathNode(const PathPos &pos)
			: g(0)
			, h(0)
			, pos(pos)
			, parent(nullptr)
			, state(NOTEXIST)
		{
		}
	};

public:
	AStar();
	~AStar();

public:
	void setAllowCorner(bool isAllow);
	void InitParam(const Param &param);
	std::list<PathPos> Search(const PathPos& start, const PathPos& end, int bodyW = 1, int bodyH = 1);	

private:
	bool IsVlidParam(const PathPos& start, const PathPos& end);
	void clear();
private:
	void PercolateUp(size_t hole);

	int GetNodeIndex(PathNode *node);

	int CalculGValue(PathNode *parent_node, const PathPos &current_pos);
	int CalculHValue(const PathPos &current_pos, const PathPos &end_pos);

	bool canPass(const PathPos &pos, const PathPos &) const;

	void FindCanreachPos(const PathPos &current_pos, std::vector<PathNode*> &nearbyNodes);
	void pushValidNearPos(int iX, int iY, const PathPos &current_pos);

	void HandleFoundNode(PathNode *current_node, PathNode *target_node);
	void HandleNotFoundNode(PathNode *current_node, PathNode *target_node, const PathPos &end_pos);

	PathNode* getNode(PathPos pos);
private:
	const int StepValue = 10;
	const int ObliqueValue = 14;

	bool					_isAllowCorner;
	int				_column;
	int				_row;
	int				_bodyW;
	int				_bodyH;
	std::vector<PathNode>		_maps;
	std::vector<PathNode*>		_nearbyNodes;
	ObstaclesCheck			_isObstacles;
	std::vector<PathNode*>		_openList;
};

#endif