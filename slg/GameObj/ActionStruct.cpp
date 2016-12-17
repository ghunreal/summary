#include "ActionStruct.h"

#include "Utils/StringHelper.h"
#include "ICharacter.h"
#include "Battle/BattleManager.h"
#include "Battle/MapGrid.h"
#include "GameDefine.h"
#include "GameObj/ObjList.h"

using namespace Game;

bool ActionDef::_isInit = false;
std::map<int, ActionDef::sAction> ActionDef::_actionMaps;

void ActionDef::lazyInitMap()
{
	if (!_isInit)
	{
		_isInit = true;
		_actionMaps.insert(std::make_pair(Stand, sAction(1, "idle")));
		_actionMaps.insert(std::make_pair(Run, sAction(1, "run")));
		_actionMaps.insert(std::make_pair(Attack, sAction(0, "attack")));
		_actionMaps.insert(std::make_pair(Dead, sAction(0, "death")));
	}
}

std::string ActionDef::getName(ActionDef::Tag action, int dir)
{
	lazyInitMap();
	auto it = _actionMaps.find(action);
	CC_ASSERT(it != _actionMaps.end());
	if (action != ActionDef::Dead )
	{
		std::string ret = it->second.name;
		ret += StringHelperIns->toString(dir);
		return ret;
	}
	else
	{
		return it->second.name;
	}
}

int ActionDef::getLoop(Tag action)
{
	lazyInitMap();
	auto it = _actionMaps.find(action);
	CC_ASSERT(it != _actionMaps.end());
	return it->second.loop;
}

int ActionDef::getDir(const Vec2& nowPos, const Vec2& targetPos)
{
	if (nowPos == targetPos) return 3;

	Vec2 dif = targetPos - nowPos;
	float angle = atan2f(dif.y, dif.x);

	if (angle >= -M_PI / 8 && angle < M_PI / 8) // right
	{
		return 3;
	}
	else if (angle >= M_PI / 8 && angle < M_PI * 3 / 8) // top-right
	{
		return 2;
	}
	else if (angle >= M_PI * 3 / 8 && angle < M_PI * 5 / 8) // top
	{
		return 1;
	}
	else if (angle >= M_PI * 5 / 8 && angle < M_PI * 7 / 8) // top-left
	{
		return 8;
	}
	else if (angle >= M_PI * 7 / 8 || angle < -M_PI * 7 / 8) // left
	{
		return 7;
	}
	else if (angle >= -M_PI * 7 / 8 && angle < -M_PI * 5 / 8) // bottom-left
	{
		return 6;
	}
	else if (angle >= -M_PI * 5 / 8 && angle < -M_PI * 3 / 8) // bottom
	{
		return 5;
	}
	else if (angle >= -M_PI * 3 / 8 && angle < -M_PI / 8) // bottom-right
	{
		return 4;
	}
	return 3;
}

NaviPoint3D ActionDef::toNaviPt(const Vec2& pt)
{
	return NaviPoint3D(pt.x, .0f, pt.y);
}

Vec2 ActionDef::toVec2(const NaviPoint3D& naviPt)
{
	return Vec2(naviPt.x, naviPt.z);
}

NaviPoint3D ActionDef::toMapNaviPt(const Vec2& pos)
{
	const Vec2& startPos = BattleManagerIns->getMapGrid()->getStartPos();
	Vec2 pt = pos - startPos - Vec2(MapWidth / 2, MapHeight / 2);
	pt.x /= 10;
	pt.y /= 10;
	return toNaviPt(pt);
}

Vec2 ActionDef::toMapVec2(const NaviPoint3D& naviPt)
{
	const Vec2& startPos = BattleManagerIns->getMapGrid()->getStartPos();
	Vec2 pt = ActionDef::toVec2(naviPt);
	pt.x *= 10;
	pt.y *= 10;
	pt += startPos + Vec2(MapWidth / 2, MapHeight / 2);
	return pt;
}

/*************************************** ActionDef end ************************************************************/

ActionBase::ActionBase()
	: _isFinish(false)
{
}

ActionBase::~ActionBase()
{
}

bool ActionBase::isStand() const 
{
	return getAction() == ActionDef::Stand;
}

bool ActionBase::isMove()  const 
{
	return getAction() == ActionDef::Run;
}

bool ActionBase::isDead() const 
{
	return getAction() == ActionDef::Dead;
}

bool ActionBase::isAttack() const
{
	return getAction() == ActionDef::Attack;
}

int ActionBase::getAction() const 
{ 
	return ActionDef::None;
}

bool ActionBase::isFinish() const
{
	return _isFinish;
}

void ActionBase::exec()
{

}

void ActionBase::doNow(float dt)
{

}

void ActionBase::setOwner(IRoleObject* owner)
{
	_owner = owner;
}

IRoleObject* ActionBase::getOwner() const
{
	return _owner;
}

/*************************************** ActionBase end ************************************************************/

void StandAction::exec()
{
	int dir = _owner->getDir();
	if (dir == 0)
	{
		dir = _owner->isEnemy() ? 5 : 1;
		_owner->setDir(dir);
	}	
	_owner->playAction(ActionDef::Stand, dir);
}

void StandAction::doNow(float dt)
{

}

/*************************************** StandAction end ************************************************************/

RunAction::RunAction()
	: _waitStop(false)
{
}

void RunAction::exec()
{
	_owner->setDir(0);
	restartMove();
}

void RunAction::restartMove()
{
	_stopByCross = false;
	_lastPt = _owner->getPosition();
	_resetTargetTime = ResetTargetTime;
	_owner->getAgent()->setMoveTarget(ActionDef::toMapNaviPt(_targetPt));
}

void RunAction::doNow(float dt)
{
	if (_waitStop)
	{
		log("stop walk:%d", _owner->getId());
		_isFinish = _owner->getAgent()->isStopped();
		return;
	}

	if (isWaitCross())
	{
		log("isWaitCross:%d", _owner->getId());
		return;
	}

	if (_owner->isInAtkRange() || isNearDestination())
	{
		_waitStop = true;
		_owner->getAgent()->stopWalk();
		return;
	}

	if (checkResetTarget(dt))
		return;

	resetDir(_owner->getPosition());
}

void RunAction::setMoveTarget(const cocos2d::Vec2& pt)
{
	_targetPt = pt;
}

bool RunAction::isNearDestination()
{
	const Vec2& nowPt = _owner->getPosition();
	Vec2 moveTarget = ActionDef::toMapVec2(_owner->getAgent()->getMoveTarget());
	if (nowPt.distance(moveTarget) <= NearDistance)
	{
		_owner->setPosition(moveTarget);
		_owner->getAgent()->changePosition(ActionDef::toMapNaviPt(moveTarget));
		log("isNearDestination");
		return true;
	}
	return false;
}

bool RunAction::isWaitCross()
{
	ICharacter* player = dynamic_cast<ICharacter*>(_owner);
	if (player != NULL && player->getCharState().isCross())
	{
		if (!_stopByCross)
		{
			_owner->getAgent()->stopWalk();
			_stopByCross = true;
		}
		return true;
	}

	if (_stopByCross && !player->getCharState().isCross())
	{
		restartMove();
		return true;
	}
	return false;
}

void RunAction::resetDir(const Vec2& nowPt)
{
	if (_lastPt.distance(nowPt) >= 0.5f || _owner->getDir() == 0)
	{
		int dir = ActionDef::getDir(_lastPt, nowPt);
		if (_owner->getDir() != dir)
		{
			log("RunAction::resetDir:%d, dir is %d, distance is %f",_owner->getId(), dir, _lastPt.distance(nowPt));
			_owner->setDir(dir);
			_owner->playAction(ActionDef::Run, dir);
		}
	}
	_lastPt = nowPt;
}

bool RunAction::checkResetTarget(float dt)
{
	_resetTargetTime -= dt*1000;
	if (_resetTargetTime <= 0)
	{
		_resetTargetTime = ResetTargetTime;
		return resetTarget();
	}
	return false;
}

bool RunAction::resetTarget()
{
	IRoleObject* target = _owner->getEnemyList()->findNearestTarget(_owner);
	if (!target)
		return false;
	const Vec2& nearPos = target->getPosition();
	if (nearPos != _targetPt)
	{
		const Vec2& curPt = _owner->getPosition();
		if (curPt.distanceSquared(nearPos) < curPt.distanceSquared(_targetPt))
		{
			_targetPt = nearPos;
			restartMove();
			return true;
		}
	}
	return false;
}

/*************************************** RunAction end ************************************************************/

void AttackAction::exec()
{
	if (!_owner->getAgent()->isStopped())
	{
		_owner->getAgent()->stopWalk();
	}

	IRoleObject* target = NULL;
	while (_targets.size()>0)
	{
		int targetId = _targets.front();
		ObjList* enemys = _owner->getEnemyList();
		target = enemys->getUndeadRole(targetId);
		if (target != NULL)
		{
			break;
		}
		else
		{
			_targets.pop_front();
		}
	}
	if (target != NULL)
	{
		int dir = ActionDef::getDir(_owner->getPosition(), target->getPosition());
		_owner->setDir(dir);
		_owner->playAction(ActionDef::Attack, dir);
	}
	else
	{
		_isFinish = true;
	}
}

void AttackAction::doNow(float dt)
{
	if (_owner->isPlayComplete())
	{
		_isFinish = true;
		for (auto it : _targets)
		{
			ObjList* enemys = _owner->getEnemyList();
			auto target = enemys->getUndeadRole(it);
			if (target)
			{
				target->getAttribute().minusHp(getDamage(_owner, target));
			}			
		}
	}
}

bool AttackAction::isTargetsValid() const
{
	if (_targets.empty())
	{
		return false;
	}

	return false;
}

void AttackAction::setTarget(int target)
{
	_targets.clear();
	_targets.push_back(target);
}

void AttackAction::setTarget(const std::list<int>& targets)
{
	_targets = targets;
}

int AttackAction::getDamage(IRoleObject* pAttacker, IRoleObject* target)
{
	// to do 
	RoleAttribute& attackerAttr = pAttacker->getAttribute();

	return attackerAttr.getAttack();
}
/*************************************** AttackAction end ************************************************************/

void DeadAction::exec()
{

}

void DeadAction::doNow(float dt)
{

}

/*************************************** DeadAction end ************************************************************/
