#include "AStar.h"

#include <cassert>
#include <cstring>
#include <algorithm>

#include <iostream>

#define		IS_CLAMP(x, min, max)		((x) >= (min) && (x) < (max))

AStar::AStar()
	: _isAllowCorner(true)
	, _row(0)
	, _column(0)
	, _bodyH(1)
	, _bodyW(1)
{

}

AStar::~AStar()
{
}

void AStar::setAllowCorner(bool isAllow)
{
	_isAllowCorner = isAllow;
}

void AStar::InitParam(const Param &param)
{
	_row = param.row;
	_column = param.column;
	_isObstacles = param.isObstacles;
	_maps.resize(_row * _column);
	_nearbyNodes.reserve(_isAllowCorner ? 8 : 4);
	for ( unsigned int i = 0; i < _maps.size(); ++i)
	{
		auto& pos = _maps[i].pos;
		pos.x = i % _column;
		pos.y = i / _column;
	}
}

std::list<AStar::PathPos> AStar::Search(const PathPos& start, const PathPos& end,int bodyW, int BodyH)
{
	_bodyW = bodyW;
	_bodyH = BodyH;

	std::list<PathPos> paths;
	if (start == end)
	{
		paths.push_back(start);
		paths.push_back(end);
		return paths;
	}
	if (!IsVlidParam(start, end))
	{
		assert(false);
	}
	else
	{
		_openList.clear();

		// 起点放入开启列表
		_openList.push_back(getNode(start));

		while (!_openList.empty())
		{
			// 取出F值最小的节点
			PathNode *current_node = *_openList.begin();
			std::pop_heap(_openList.begin(), _openList.end(), [](const PathNode *a, const PathNode *b)->bool
			{
				return a->f() > b->f();
			});
			_openList.pop_back();
			current_node->state = IN_CLOSELIST;

			// 搜索附近可通行的位置
			FindCanreachPos(current_node->pos, _nearbyNodes);

			for ( auto nearby : _nearbyNodes )
			{
				if (nearby->state == IN_OPENLIST)
				{
					HandleFoundNode(current_node, nearby);
				}
				else
				{
					HandleNotFoundNode(current_node, nearby, end);

					// 找到终点
					if (nearby->pos == end)
					{
						while (nearby->parent)
						{
							paths.push_front(nearby->pos);
							nearby = nearby->parent;
						}
						clear();
						return paths;
					}
				}
			}
		}
	}
	clear();
	return std::list<AStar::PathPos>();
}

bool AStar::IsVlidParam(const PathPos& start, const PathPos& end)
{
	return ( _isObstacles
		&& (end.x >= 0 && end.x < _column)
			&& ( end.y >= 0 && end.y < _row)
			&& (start.x >= 0 && start.x < _column)
			&& (start.y >= 0 && start.y < _row)
			);
}

int AStar::GetNodeIndex(PathNode *node)
{
	size_t index = 0;
	const size_t size = _openList.size();
	while (index < size)
	{
		if (_openList[index] == node)
		{
			return index;
		}
		++index;
	}
	return -1;
}

void AStar::PercolateUp(size_t hole)
{
	size_t parent = 0;
	while (hole > 0)
	{
		parent = (hole - 1) / 2;
		if (_openList[hole]->f() < _openList[parent]->f())
		{
			std::swap(_openList[hole], _openList[parent]);
			hole = parent;
		}
		else
		{
			break;
		}
	}
}

inline int AStar::CalculGValue(PathNode *parent_node, const PathPos &current_pos)
{
	int g_value = ((abs(current_pos.y - parent_node->pos.y) + abs(current_pos.x - parent_node->pos.x)) == 2 ? ObliqueValue : StepValue);
	return g_value += parent_node->g;
}

inline int AStar::CalculHValue(const PathPos &current_pos, const PathPos &end_pos)
{
	unsigned int h_value = abs(end_pos.y - current_pos.y) + abs(end_pos.x - current_pos.x);
	return h_value * StepValue;
}

bool AStar::canPass(const PathPos &pos, const PathPos &current_pos) const
{
	int lx = pos.x;	// left
	int rx = pos.x + _bodyW - 1;		//right
	int ty = pos.y + _bodyH - 1;		//top
	int by = pos.y;							//bottom

	if (!IS_CLAMP(lx, 0, _column) || !IS_CLAMP(rx, 0, _column) || !IS_CLAMP(ty, 0, _row) || !IS_CLAMP(by, 0, _row))
	{
		return false;
	}
	
	bool ret = true;
	int diffx = pos.x - current_pos.x, diffy = pos.y - current_pos.y;

	if (diffy == 0)	//Horizontal movement, check left or right boundary.
	{
		int boundx = diffx > 0 ? rx: lx;
		for (int y = 0; y < _bodyH; ++y)
		{
			ret = ret && !_isObstacles(boundx, pos.y + y);
		}
	}
	else if(diffx == 0)		// vertical movement, check top or bottom boundary.
	{
		int boundY = diffy > 0 ? ty : by;
		for (int x = 0; x < _bodyW; ++x)
		{
			ret = ret && !_isObstacles(lx + x, boundY);
		}
	}
	else
	{
		int barrierNum = 0;

		int fromHx = 0, fromHy = 0, fromVx = 0, fromVy = 0;
		int toHx = 0, toHy = 0, toVx = 0, toVy = 0;

		// right 
		if (diffx > 0)
		{
			fromHx = current_pos.x;
			toHx = rx;
			fromVx = toVx = rx;
		}
		else
		{
			fromHx = lx;
			toHx = current_pos.x + _bodyW - 1;
			fromVx = toVx = lx;
		}
		if (diffy > 0)
		{
			fromHy = toHy = ty;
			fromVy = current_pos.y;
			toVy = ty;
		}
		else
		{
			fromHy = toHy = by;
			fromVy = by;
			toVy = current_pos.y + _bodyH - 1;
		}

		for (int x = fromHx; x <= toHx; ++x)
		{
			barrierNum += _isObstacles(x, fromHy);
		}
		for (int y = fromVy; y < toVy; ++y)
		{
			barrierNum += _isObstacles(fromVx, y);
		}
		
		ret = !barrierNum;
		// special situation. 
		if (barrierNum == 1)
		{
			//int fromHx = 0, fromHy = 0, fromVx = 0, fromVy = 0;

			diffx > 0 ? fromHx = current_pos.x, fromVx = rx : fromHx = rx + 1, fromVx = lx;
			diffy > 0 ? fromHy = ty, fromVy = current_pos.y : fromHy = by, fromVy = current_pos.y;
			//若只有前进方向的方向向量的起点为障碍则允许通过
			ret = _isObstacles(fromHx, fromHy) || _isObstacles(fromVx, fromVy);
		}
	}

	return ret;
}

void AStar::pushValidNearPos(int iX, int iY, const PathPos &current_pos)
{
	PathPos target(iX, iY);
	if (this->canPass(target, current_pos))
	{
		PathNode *node = getNode(target);
		if (node->state != IN_CLOSELIST)
		{
			_nearbyNodes.push_back(node);
		}
	}
}

void AStar::FindCanreachPos(const PathPos &current_pos, std::vector<PathNode*> &nearbyNodes)
{
	nearbyNodes.clear();

	int curX = current_pos.x;
	int curY = current_pos.y;
	pushValidNearPos(curX - 1, curY, current_pos);
	pushValidNearPos(curX + 1, curY, current_pos);
	pushValidNearPos(curX, curY - 1, current_pos);
	pushValidNearPos(curX, curY + 1, current_pos);
	 
	if (_isAllowCorner)
	{
		pushValidNearPos(curX - 1, curY - 1, current_pos);
		pushValidNearPos(curX - 1, curY + 1, current_pos);
		pushValidNearPos(curX + 1, curY - 1, current_pos);
		pushValidNearPos(curX + 1, curY + 1, current_pos);
	}
}

void AStar::HandleFoundNode(PathNode *current_node, PathNode *target_node)
{
	int g_value = CalculGValue(current_node, target_node->pos);
	if (g_value < target_node->g)
	{
		target_node->g = g_value;
		target_node->parent = current_node;

		size_t index = GetNodeIndex(target_node);
		assert(index!=-1);
		PercolateUp(index);		
	}
}

void AStar::HandleNotFoundNode(PathNode *current_node, PathNode *target_node, const PathPos &end_pos)
{
	target_node->parent = current_node;
	target_node->h = CalculHValue(target_node->pos, end_pos);
	target_node->g = CalculGValue(current_node, target_node->pos);
	target_node->state = IN_OPENLIST;

	_openList.push_back(target_node);
	std::push_heap(_openList.begin(), _openList.end(), [](const PathNode *a, const PathNode *b)->bool
	{
		return a->f() > b->f();
	});
}

AStar::PathNode* AStar::getNode(PathPos pos)
{
	int idx = pos.y * _column + pos.x;
	assert(idx >= 0&&idx<(int)_maps.size());
	return &_maps[idx];
}

void AStar::clear()
{
	for_each(_maps.begin(), _maps.end(), [](PathNode& val)->void{ 
		val.state = NOTEXIST; 
		val.parent = NULL; } );
}
