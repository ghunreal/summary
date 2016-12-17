#include "ObjList.h"

#include "Battle/BattleManager.h"
#include "Battle/MapGrid.h"

using namespace Game;

ObjList::ObjList()
	: _points(0)
{
	_list.setAddCb(CC_CALLBACK_1(ObjList::onAdd,this));
	_list.setRemoveCb(CC_CALLBACK_1(ObjList::onRemove, this));
	_leftTower.setBLeft(true);
}

void ObjList::resetTower(bool isEnemy)
{
	_leftTower.setOpacity(255);
	_leftTower.getAttribute().reset();

	_rightTower.setOpacity(255);
	_rightTower.getAttribute().reset();

	_kingTower.setOpacity(255);
	_kingTower.getAttribute().reset();

	_leftTower.setId(20002, false);
	_rightTower.setId(20002, false);
	_kingTower.setId(20001, false);

	_charMap[_leftTower.getId()] = &_leftTower;
	_charMap[_rightTower.getId()] = &_rightTower;
	_charMap[_kingTower.getId()] = &_kingTower;
}

ObjList::~ObjList()
{
}

void ObjList::clear()
{
	_points = 0;
	for (auto it = _list.begin(); it != _list.end(); ++it)
	{
		(*it)->recycle();
	}
	_list.clear();
	_charMap.clear();
	_leftTower.removeFromParent();
	_rightTower.removeFromParent();
	_kingTower.removeFromParent();
}

void ObjList::add(ICharacter* pChar)
{
	_list.add(pChar);
}

void ObjList::remove(ICharacter* pChar)
{
	pChar->delDelay();
	//_list.remove(pChar);
}

IRoleObject* ObjList::getUndeadRole(int id) const
{
	IRoleObject* role = getRole(id);
	if (role && !role->isDead())
	{
		return role;
	}
	return NULL;
}

IRoleObject* ObjList::getRole(int id) const
{
	auto it = _charMap.find(id);
	if (it != _charMap.end())
	{
		return const_cast<IRoleObject*>(it->second);
	}
	return NULL;
}

void ObjList::onAdd( const ICharacter* pChar)
{
	_charMap[pChar->getId()] = pChar;
}

void ObjList::onRemove( const ICharacter* pChar)
{
	if (pChar)
	{
		_charMap[pChar->getId()] = NULL;
		const_cast<ICharacter*>(pChar)->recycle();
	}
}

void ObjList::tick(float dt)
{
	int size = _list.size();
	_list.traverseBegin();
	std::sort(_list.begin(), _list.end(), CrossSort());
	for (auto it = _list.begin(); it != _list.end(); ++it)
	{
		auto player = *it;
		if (player->isWaitDel())
		{
			_list.remove(player);
			continue;
		}
		if (!player->isDead())
		{
			player->tick(dt);
		}
	}
	_list.traverseEnd();	
}

IRoleObject* ObjList::findNearestTarget(const IRoleObject* pChar) const
{
	IRoleObject* ret = findObj(pChar);
	if (ret == NULL)
	{
		ret = const_cast<IRoleObject*>(findTower(pChar));
	}
	return ret;
}

IRoleObject* ObjList::findObj(const IRoleObject* pChar) const
{
	const Vec2& pos = pChar->getPosition();
	ICharacter* ret = NULL;
	int min = INT_MAX;
	for (auto it = _list.begin(); it != _list.end(); ++it)
	{
		if ((*it)->isDead())
		{
			continue;
		}
		int len = (*it)->getPosition().distanceSquared(pos);
		if (len < min)
		{
			min = len;
			ret = *it;
		}
	}
	return ret;
}

const IRoleObject* ObjList::findTower(const IRoleObject* pChar) const
{
	if (pChar->isLeftDirection())
	{
		if (!_leftTower.isDead())
		{
			return &_leftTower;
		}
	}
	else
	{
		if (!_rightTower.isDead())
		{
			return &_rightTower;
		}
	}
	if (!_kingTower.isDead())
	{
		return &_kingTower;
	}
	return NULL;
}

Tower* ObjList::getLeftTower()
{
	return &_leftTower;
}

Tower* ObjList::getRightTower()
{
	return &_rightTower;
}

Tower* ObjList::getKingTower()
{
	return &_kingTower;
}

int ObjList::getPoints() const
{
	return _points;
}

void ObjList::addPoint()
{
	++_points;
}

void ObjList::setMaxPoint()
{
	_points = MaxPoint;
}

/********************************************ObjList End******************************************************/

void PlayerList::initTowers()
{
	MapGrid* mapGrid = BattleManagerIns->getMapGrid();
	_leftTower.initTower(20002, Vec2(300, 630), 1, 15);
	mapGrid->addChild(&_leftTower, MapGrid::ZOrder::Role);

	_rightTower.initTower(20002, Vec2(840, 630), 1, 15);
	mapGrid->addChild(&_rightTower, MapGrid::ZOrder::Role);

	_kingTower.initTower(20001, Vec2(540, 540), 4, 35);
	mapGrid->addChild(&_kingTower, MapGrid::ZOrder::Role);

	resetTower(false);
}

void EnemyList::initTowers()
{
	MapGrid* mapGrid = BattleManagerIns->getMapGrid();

	_leftTower.initTower(20002, Vec2(300, 1200), 1, 15, true);
	mapGrid->addChild(&_leftTower,MapGrid::ZOrder::Role);

	_rightTower.initTower(20002, Vec2(840, 1200), 1, 15, true);
	mapGrid->addChild(&_rightTower, MapGrid::ZOrder::Role);

	_kingTower.initTower(20001, Vec2(540, 1300), 4, 35, true);
	mapGrid->addChild(&_kingTower, MapGrid::ZOrder::Role);

	resetTower(true);
}

/*******************************************EnemyList End******************************************************/

void ObjListUtility::setOthersCross(const ICharacter* pChar, int level)
{
	if (pChar == NULL)
		return;

	std::list<ICharacter*> list = getAll();
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		ICharacter* player = *it;
		if (player == pChar)
			continue;

		CC_ASSERT(player->getId() != pChar->getId());

		if (!player->isDead()
			&& !player->getCharState().isCross()
			&& pChar->isCross(player))
		{
			player->getCharState().enableCross(true);
			player->getRoleAction().setCrossTarget(pChar->getId(), level);
		}
	}
}

std::list<ICharacter*> ObjListUtility::getAll()
{
	std::list<ICharacter*> list(PlayerListIns->begin(), PlayerListIns->end());
	list.insert(list.end(), EnemyListIns->begin(), EnemyListIns->end());
	return list;
}