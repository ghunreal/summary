#include "ICharacter.h"

#include "GameObj/ObjList.h"
#include "Battle/BattleManager.h"
#include "Battle/IBattleUI.h"
#include "GameDefine.h"

using namespace Game;

std::vector<Vec2> CharacterMode::getModePts(const Vec2& pos, int gridNum)
{
	CC_ASSERT(gridNum > 0);
	std::vector<Vec2> pts;
	pts.push_back(pos);

	switch (gridNum)
	{
	case 1:
		break;
	case 2:
		pts.push_back(pos + Vec2(TileWidth,0));
		break;
	case 4:
		pts.push_back(pos + Vec2(TileWidth, 0));
		pts.push_back(pos + Vec2(0, TileHeight));
		pts.push_back(pos + Vec2(TileWidth, TileHeight));
		break;

	default:
		CC_ASSERT(false);
		break;
	}
	return pts;
}

Vec2 CharacterMode::getBodyInfo(int gridNum)
{
	Vec2 ret;
	switch (gridNum)
	{
	case 1:
		ret = Vec2(1, 1);
		break;
	case 2:
		ret = Vec2(2, 1);
		break;
	case 4:
		ret = Vec2(2, 2);
		break;

	default:
		CC_ASSERT(false);
		break;
	}
	return ret;
}

Rect CharacterMode::getBodyRect(const Vec2& pos, int gridNum)
{
	Rect rect;
	rect.origin = pos;
	switch (gridNum)
	{
	case 1:
		rect.size.width = TileWidth;
		rect.size.height = TileHeight;
		break;
	case 2:
		rect.size.width = TileWidth*2;
		rect.size.height = TileHeight;
		break;
	case 4:
		rect.size.width = TileWidth * 2;
		rect.size.height = TileHeight * 2;
		break;

	default:
		CC_ASSERT(false);
		break;
	}
	return rect;
}

/**************************************CharacterMode End*********************************************/

IRoleObject::IRoleObject(IObjAction* objAction, RoleAttribute* attribute)
	: _isEnemy(false)
	, _isLeft(false)
	, _playComplete(false)
	, _gridNum(1)
	, _speed(1.0f)
	, _agent(NULL)
	, _drawNode(NULL)
	,m_isBirthing(true)
{
	_objAction = objAction;
	_attribute = attribute;
	//_attribute->initAttribute(this);
}

IRoleObject::~IRoleObject()
{
	if (_agent)
	{
		BattleManagerIns->getNavi()->removeAgent(_agent);
	}
	delete _attribute;
}

void IRoleObject::tick(float dt)
{
	_objAction->tick(dt);
}

const IObjAction& IRoleObject::getRoleAction() const
{
	return *_objAction;
}

IObjAction& IRoleObject::getRoleAction()
{
	return *_objAction;
}

const RoleAttribute& IRoleObject::getAttribute() const
{
	return *_attribute;
}

RoleAttribute& IRoleObject::getAttribute()
{
	return *_attribute;
}

void IRoleObject::setBEnmey(bool isEnemy)
{
	_isEnemy = isEnemy;
}

bool IRoleObject::isEnemy() const
{
	return _isEnemy;
}

ObjList* IRoleObject::getTeamList() const
{
	ObjList* ret = _isEnemy ? (ObjList*)EnemyListIns : PlayerListIns;
	return ret;
}

ObjList* IRoleObject::getEnemyList() const
{
	ObjList* ret = _isEnemy ? (ObjList*)PlayerListIns : EnemyListIns;
	return ret;
}

bool IRoleObject::isDead() const
{
	return _attribute->getHp()<=0;
}

bool IRoleObject::isInAtkRange(const IRoleObject* target) const
{
	if (target != NULL)
	{
		return isInAtkRange(target->getPosition());
	}
	return false;
}

bool IRoleObject::isInAtkRange(const Vec2& pos) const
{
	return getPosition().distance(pos) < 120;
}

bool IRoleObject::isInAtkRange() const
{
	return isInAtkRange(getEnemyList()->findNearestTarget(this));
}

void IRoleObject::setDead()
{
	_objAction->dealDeath();
	this->removeChild(_drawNode);
	_drawNode = NULL;
	if (_agent)
	{
		BattleManagerIns->getNavi()->removeAgent(_agent);
		_agent = NULL;
	}
}

void IRoleObject::setSpeedRatio(float speed)
{
	_speed = speed;
	if (m_armature)
	{
		m_armature->getAnimation()->setSpeedScale(speed);
	}
}

float IRoleObject::getSpeedRatio() const
{
	return _speed;
}

void IRoleObject::playAction(ActionDef::Tag action, int dir)
{
	_lastAction = action;
	_playComplete = false;
	int loop = ActionDef::getLoop(action);
	playWithCB(ActionDef::getName(action, dir), -1, loop);
}

void IRoleObject::changeActionDir(int dir)
{
	playAction(_lastAction, dir);
}

void IRoleObject::createAgent(const Vec2& pos, int collideLv, int weight)
{
	_agent = BattleManagerIns->getNavi()->addAgent(3.0f, ActionDef::toMapNaviPt(pos));
	_agent->setUserData(this);         // 设置用户数据
	_agent->setMaxSpeed(8.0f);           // 最大移动速度
	_agent->setMaxAcceleration(10000.0f); // 最大加速度
	_agent->setCollisionQueryRange(_agent->getRadius()*2.f);
	_agent->setCollideLevel(collideLv);
	_agent->setSeparationWeight(weight);
	_agent->start();
}

void IRoleObject::animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID)
{
	if (movementType == COMPLETE )
	{
		_playComplete = true;
	}
}

void IRoleObject::play(const std::string& name, int durationTo)
{
	GameObject::play(name, _speed, durationTo);
}

void IRoleObject::play(const std::string& name, Action* pAction, int durationTo)
{
	GameObject::play(name, pAction, _speed, durationTo);
}

void IRoleObject::playWithCB(const std::string& name, int durationTo, int loop)
{
	GameObject::playWithCB(name, _speed, durationTo, loop);
}

bool IRoleObject::isCross(const IRoleObject* target) const
{
	if (target != NULL)
	{
		int targetCollide = target->getAttribute().getConsume();
		if (targetCollide > getAttribute().getConsume())
			return false;

		const Vec2& myPos = getPosition();
		const Vec2& targetPos = target->getPosition();
		float distance = myPos.distance(targetPos);
		if (distance < TileHeight)
			return true;
	}
	return false;
}

/**************************************IRoleObject End**********************************************/

ICharacter::ICharacter()
	: IRoleObject(&_charAction, new HeroAttribute)
	, _waitDel(false)
{
	_charAction.initAction(this);
	_charState.initState(this);
}

ICharacter::~ICharacter()
{
}

const CharacterState& ICharacter::getCharState() const
{
	return _charState;
}

CharacterState& ICharacter::getCharState()
{
	return _charState;
}

void ICharacter::setDead()
{
	IRoleObject::setDead();
	this->stop();
	this->runAction(FadeOut::create(2.0f));
	getTeamList()->remove(this);
}

int ICharacter::getCrossLevel() const
{
	return _charAction.getCrossLevel();
}

/**************************************ICharacter End**********************************************/

Tower::Tower()
	: IRoleObject(&_towerAction,new BuildingAttribute)
	, _isInit(false)
{
	_towerAction.initAction(this);
	borned();
	this->retain();
}

void Tower::initTower(const std::string& bg, const std::string& tower, const Vec2& pos, int gridNum, int offset, bool isEnemy)
{
	if (!_isInit)
	{
		setId(20002, isEnemy);
		getAttribute().initAttribute(this);

		_isInit = true;
		Sprite* towerBg = Sprite::create(bg);
		Sprite* towerGun = Sprite::create(tower);
		towerGun->setScale(0.4f);
		towerBg->setPosition(Vec2(offset, 0));
		towerGun->setPosition(Vec2(towerBg->getContentSize().width / 2, towerBg->getContentSize().height / 2));
		towerBg->addChild(towerGun);
		this->addChild(towerBg);

		this->setScaleX(1.6f);
		this->setScaleY(1.4f);
		//initArmatureByLooks(looks);
		setPosition(BattleManagerIns->getMapGrid()->regularMapPos(pos));
		setGridNum(gridNum);
		_towerAction.drawRunGrid(this->getPosition());

		setBEnmey(isEnemy);
		auto maxHp = getAttribute().getMaxHp();
		this->initHpBar(maxHp, maxHp, 0.7f, Vec2(towerBg->getContentSize().width / 2, towerBg->getContentSize().height + 20), towerBg, isEnemy);
		towerBg->setCascadeOpacityEnabled(true);
		this->setCascadeOpacityEnabled(true);
	}

	//createAgent(pos,0);
	setOpacity(255);
	getAttribute().reset();
}

void Tower::initTower(int tid, const Vec2& pos, int gridNum, int offset, bool isEnemy /* = false */)
{
	RoleAttribute& attr = getAttribute();
	if (!_isInit)
	{
		setId(tid, isEnemy);
		attr.initAttribute(this);
		_isInit = true;

		Sprite* towerBg = Sprite::create("res/ui/demo/battle/" + attr.getCardIcon());
		Sprite* towerGun = Sprite::create("res/ui/demo/battle/tower_gun.png");
		towerGun->setScale(0.4f);
		towerBg->setPosition(Vec2(offset, 0));
		towerGun->setPosition(Vec2(towerBg->getContentSize().width / 2, towerBg->getContentSize().height / 2));
		towerBg->addChild(towerGun);
		this->addChild(towerBg);

		this->setScaleX(1.6f);
		this->setScaleY(1.4f);
		//initArmatureByLooks(looks);
		setPosition(BattleManagerIns->getMapGrid()->regularMapPos(pos));
		setGridNum(gridNum);
		_towerAction.drawRunGrid(this->getPosition());

		setBEnmey(isEnemy);
		auto maxHp = attr.getMaxHp();
		this->initHpBar(maxHp, maxHp, 0.7f, Vec2(towerBg->getContentSize().width / 2, towerBg->getContentSize().height + 20), towerBg, isEnemy);
		towerBg->setCascadeOpacityEnabled(true);
		this->setCascadeOpacityEnabled(true);
	}

	//createAgent(pos, 0,0);
	setOpacity(255);
	attr.reset();
}

void Tower::setDead()
{
	IRoleObject::setDead();
	this->stop();
	this->runAction(FadeOut::create(2.0f));

	if (_isEnemy)
	{
		BattleManagerIns->getMapGrid()->getSettingArea()->redraw();
	}
	ObjList* teams = getTeamList();
	if (this != teams->getKingTower())
	{
		teams->addPoint();
	}
	else
	{
		teams->setMaxPoint();
	}
	BattleManagerIns->getCurBattle()->updatePoint(_isEnemy, teams->getPoints());
}
