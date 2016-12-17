#pragma once

#include <string>
#include <iostream>

#include "ActionStruct.h"
#include "GameObj.h"
#include "CharacterAction.h"
#include "RoleAttribute.h"
#include "navi.h"

using namespace knavi;

class ObjList;
class RunningDrawNode;

class CharacterMode
{
public:
	static std::vector<Vec2> getModePts(const Vec2& pos,int gridNum);
	static Vec2 getBodyInfo(int gridNum);
	static Rect getBodyRect(const Vec2& pos,int gridNum);
};

class IRoleObject : public RoleObject
{
public:
	IRoleObject(IObjAction* objAction, RoleAttribute* attribute);
	virtual ~IRoleObject() = 0;

	virtual void tick(float dt);

	void setGridNum(int num) { _gridNum = num; }
	int getGridNum() const { return _gridNum; }

	bool isAlive() const { return !isDead() && !isBirthing(); }
	bool isDead() const;
	bool isInAtkRange(const IRoleObject* target) const;
	bool isInAtkRange(const Vec2& pos) const;
	bool isInAtkRange() const;
	virtual void setDead()=0;

	void setBEnmey(bool isEnemy);
	bool isEnemy() const;
	ObjList* getTeamList() const;
	ObjList* getEnemyList() const;

	const IObjAction& getRoleAction() const;
	IObjAction& getRoleAction();

	const RoleAttribute& getAttribute() const;
	RoleAttribute& getAttribute();

	void setDrawNode(RunningDrawNode* drawNode) { _drawNode = drawNode; }
	RunningDrawNode* getDrawNode() const { return _drawNode; }

	void setSpeedRatio(float speed);
	float getSpeedRatio() const;

	void playAction(ActionDef::Tag action, int dir = 0);
	void changeActionDir(int dir);

	void setBLeft(bool isLeft) { _isLeft = isLeft; }
	virtual	bool isLeftDirection() const { return _isLeft; };

	void setDir(int dir) { _curDir = dir; }
	int getDir() const { return _curDir; }

	void createAgent(const Vec2& pos, int collideLv, int weight);
	NaviAgent* getAgent() const { return _agent; }

	bool isPlayComplete() const { return _playComplete; }

	bool isBirthing()const{ return m_isBirthing; }
	void borned(){ m_isBirthing = false; }

	bool isCross(const IRoleObject* target) const;

protected:
	bool _isEnemy;
	bool _isLeft;
	bool _playComplete;
	int _gridNum;
	int _curDir;
	ActionDef::Tag _lastAction;
	float _speed;
	NaviAgent* _agent;

	RunningDrawNode* _drawNode;
	IObjAction* _objAction;
	RoleAttribute* _attribute;

	virtual void play(const std::string& name, int durationTo = -1);
	virtual void play(const std::string& name, Action* pAction, int durationTo = -1);
	virtual void playWithCB(const std::string& name, int durationTo = -1, int loop = -1);
	virtual void animationEvent(cocostudio::Armature *armature, cocostudio::MovementEventType movementType, const std::string& movementID);
private:
	//birthing protection.
	bool m_isBirthing;
};


class ICharacter : public IRoleObject
{	
public:
	ICharacter();
	virtual ~ICharacter()=0;
	virtual void playAppear()=0;

	virtual void recycle()=0;
	virtual void setDead();
	const CharacterState& getCharState() const;
	CharacterState& getCharState();

	void setDestination(const Vec2& pos) { _destination = pos; }
	const Vec2& getDestination() const { return _destination; }

	void setNextRunPos(const Vec2& pos) { _nextPos = pos; }
	const Vec2& getNextRunPos() const { return _nextPos; }
	int getCrossLevel() const;

	bool isWaitDel() const { return _waitDel; }
	void delDelay() { _waitDel = true; }
protected:
	bool _waitDel;
	CharacterAction _charAction;
	CharacterState _charState;
	Vec2 _destination;
	Vec2 _nextPos;
};

class Tower : public IRoleObject
{
public:
	Tower();
	void initTower(const std::string& bg, const std::string& tower, const Vec2& pos, int gridNum, int offset, bool isEnemy = false);
	void initTower(int tid, const Vec2& pos, int gridNum, int offset, bool isEnemy = false);
	virtual void setDead();
protected:
	TowerAction _towerAction;
	bool _isInit;
};
