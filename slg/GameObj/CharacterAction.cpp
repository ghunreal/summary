#include "CharacterAction.h"

#include <math.h>
#include "ObjList.h"
#include "ICharacter.h"
#include "Utils/VisibleRect.h"
#include "GameDefine.h"
#include "Battle/BattleManager.h"
#include "Battle/IBattleUI.h"
#include "Utils/UsefulFunc.h"

using namespace Game;

CrossHanlder::CrossHanlder()
	: _owner(NULL)
{
}

void CrossHanlder::exec()
{
	IRoleObject* target = getTarget();
	if (target)
	{
		_owner->getAgent()->setCollideLevel(0);
		setMovePos(target->getPosition());
		ObjListUtils->setOthersCross(dynamic_cast<ICharacter*>(_owner), _level + 1);
	}
	else
	{
		stopCross();
	}
}

IRoleObject* CrossHanlder::getTarget()
{
	IRoleObject* target = _owner->getTeamList()->getUndeadRole(_targetId);
	if (target == NULL)
	{
		target = _owner->getEnemyList()->getUndeadRole(_targetId);
	}
	return target;
}

void CrossHanlder::tick(float dt)
{
	IRoleObject* target = getTarget();
	if (target)
	{
		const Vec2& targetPos = target->getPosition();
		if (targetPos.distance(_targetPos) >= Distance &&_owner->isCross(target))
		{
			log("CrossHanlder::setMovePos:%d, dir is %d", _owner->getId(), _owner->getDir());
			setMovePos(targetPos);
		}

		const Vec2& nowPos = _owner->getPosition();
		if (_movePos.distance(nowPos) > NearDistance)
		{
			Vec2 dif = _movePos - nowPos;
			int speed = 60;
			int speedX = dif.x == 0 ? 0 : dif.x > 0 ? speed : -speed;
			int speedY = dif.y == 0 ? 0 : dif.y > 0 ? speed : -speed;

			Vec2 curPos = nowPos;
			curPos.x += speedX * dt;
			curPos.y += speedY * dt;
			_owner->setPosition(curPos);
			NaviAgent* agent = _owner->getAgent();
			if (agent)
			{
				agent->changePosition(ActionDef::toMapNaviPt(curPos));
			}
		}
		else
		{
			stopCross();
		}
	}
	else
	{
		stopCross();
	}
}

void CrossHanlder::setCrossTarget(int id, int level)
{
	_targetId = id;
	_level = level;
	exec();
}

void CrossHanlder::setOwner(ICharacter* pChar)
{
	_owner = pChar;
}

int CrossHanlder::getLevel() const
{
	return _level;
}

void CrossHanlder::setMovePos(const Vec2& targetPos)
{
	_targetPos = targetPos;
	const Vec2& nowPos = _owner->getPosition();
	Vec2 dif = nowPos - _targetPos;

	int angle = UsefulFunc::createRand(0, 90);
	int radius = TileWidth;
	int iX = radius*cos(angle*M_PI / 180);
	int iY = radius*sin(angle*M_PI / 180);
	if (dif.x == 0)
	{
		dif.x = UsefulFunc::createRand(0, 1)-0.5f;
	}
	if (dif.y == 0)
	{
		dif.y = UsefulFunc::createRand(0, 1)-0.5f;
	}
	iX = dif.x<0 ? -iX : iX;
	iY = dif.y<0 ? -iY : iY;
	_movePos = _targetPos + Vec2(iX, iY);
	
	NaviPoint3D destination;
	BattleManagerIns->getNavi()->raycast(ActionDef::toMapNaviPt(nowPos), ActionDef::toMapNaviPt(_movePos), destination);
	_movePos = ActionDef::toMapVec2(destination);

	int nowAction = _owner->getRoleAction().getNowAction();
	if (nowAction == ActionDef::Run)
	{
		_owner->getRoleAction().changeMoveTarget(_movePos);	
	}
	else
	{
		_owner->getAgent()->setMoveTarget(ActionDef::toMapNaviPt(_movePos));
	}
}

void CrossHanlder::stopCross()
{
	log("stop cross:%d", _owner->getId());
	_owner->getAgent()->setCollideLevel(_owner->getAttribute().getConsume());
	_owner->getCharState().enableCross(false);
}

/****************************************************************************************************/


IObjAction::IObjAction()
{
}

IObjAction::~IObjAction()
{
	for (auto it = _nextActionList.begin(); it != _nextActionList.end(); ++it )
	{
		delete (*it);
	}
	_nextActionList.clear();
	delete _actionNow;
}

void IObjAction::tick(float dt)
{
	if (isWaitNextAction())
	{
		if (_nextActionList.size() > 0 || autoCreateNext())
		{
			doNext();
		}
		else  if (!_actionNow->isStand() && !_roleObj->isDead())
		{
			replaceAction(ActionFactory<StandAction>::create(_roleObj));
		}
	}
}

void IObjAction::initAction(IRoleObject* pChar)
{
	_roleObj = pChar;
	_actionNow = ActionFactory<StandAction>::create(_roleObj);
}

void IObjAction::pushAction(ActionBase* next)
{
	_nextActionList.push_back(next);
}

void IObjAction::drawRunGrid(const cocos2d::Vec2& pos)
{
	RunningDrawNode* node = _roleObj->getDrawNode();
	if (node == NULL)
	{
		node = RunningDrawNode::create();
		BattleManagerIns->getMapGrid()->addChild(node);
		_roleObj->setDrawNode(node);
	}
	std::vector<Vec2> pts = CharacterMode::getModePts(_roleObj->getPosition(), _roleObj->getGridNum());
	for (auto& pt : pts)
	{
		node->drawGrid(pt);
	}
}

void IObjAction::dealDeath()
{
	replaceAction(ActionFactory<DeadAction>::create(_roleObj));
}

int IObjAction::getNowAction() const
{
	if (_actionNow)
	{
		return _actionNow->getAction();
	}
	return ActionDef::None;
}

void IObjAction::changeMoveTarget(const Vec2& target)
{
	RunAction* now = dynamic_cast<RunAction*>(_actionNow);
	if (now)
	{
		now->setMoveTarget(target);
		now->restartMove();
	}
}

void IObjAction::replaceAction(ActionBase* next)
{
	if (!_roleObj->isDead()
		|| (next->isDead() && !_actionNow->isDead()))
	{
		_nextActionList.clear();
		_nextActionList.push_back(next);
	}
}

bool IObjAction::isWaitNextAction() const
{
	return _actionNow->isStand() || _actionNow->isFinish();
}

/***************************************IObjAction End***********************************************/

void TowerAction::initAction(Tower* pChar)
{
	IObjAction::initAction(pChar);
	_tower = pChar;
}

bool TowerAction::autoCreateNext()
{
	return false;
}

void TowerAction::doNext()
{

}

/***************************************TowerAction End***********************************************/

CharacterAction::CharacterAction(void)
{
	
}

CharacterAction::~CharacterAction(void)
{
}

void CharacterAction::initAction(ICharacter* pChar)
{
	IObjAction::initAction(pChar);
	_character = pChar;
	_cross.setOwner(_character);
}

void CharacterAction::setCrossTarget(int id, int level)
{
	_cross.setCrossTarget(id, level);
}

int CharacterAction::getCrossLevel() const
{
	if (_character->getCharState().isCross())
	{
		return _cross.getLevel();
	}
	return 0;
}

void CharacterAction::tick(float dt)
{
	setCurPos();
	resetLocalZOrder();
	if (!_character->isBirthing())
	{
		if (isWaitNextAction())
		{
			IObjAction::tick(dt);
		}
		else
		{
			_actionNow->doNow(dt);
		}
	}
	if (_character->getCharState().isCross())
	{
		_cross.tick(dt);
	}
}

void CharacterAction::setCurPos()
{
	NaviPoint3D naviPt = _character->getAgent()->getPosition();
	Vec2 pt = ActionDef::toMapVec2(naviPt);
	_character->setPosition(pt);
}

bool CharacterAction::autoCreateNext()
{
	//if (_character->isEnemy()) return false;
	IRoleObject* target = _character->getEnemyList()->findNearestTarget(_character);
	if (target == NULL)
		return false;

	if (_character->isInAtkRange(target) /*&& !target->isBirthing()*/)
	{
		AttackAction* next = ActionFactory<AttackAction>::create(_character);
		next->setTarget(target->getId());
		pushAction(next);
	}
	else
	{
		return createRun(target->getPosition());
	}
	return true;
}

bool CharacterAction::createRun(const Vec2& pos)
{
	if (_character->getCharState().isCross())
		return false;

	log("create run:%d", _character->getId());
	RunAction* next = ActionFactory<RunAction>::create(_character);
	next->setMoveTarget(pos);
	pushAction(next);
	return true;
}

bool CharacterAction::autoCreateAttack()
{
	return false;
}

void CharacterAction::doNext()
{
	ActionBase* next = _nextActionList.front();
	next->exec();
	delete _actionNow;
	_actionNow = next;
	_nextActionList.pop_front();
}

void CharacterAction::resetLocalZOrder()
{
	int zOrder = VisibleRect::top().y - _character->getPositionY();
	if (abs(_character->getLocalZOrder() - zOrder) >= 10)
	{
		_character->setLocalZOrder(zOrder);
	}	
}
