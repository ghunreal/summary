#pragma once

#include <list>
#include "ActionStruct.h"
#include "cocos2d.h"

class IRoleObject;
class ICharacter;
class Tower;

class CrossHanlder
{
public:
	CrossHanlder();

	void tick(float dt);
	void setCrossTarget(int id, int level);
	void setOwner(ICharacter* pChar);
	int getLevel() const;
private:

	static const int Distance = 30; // change dir when _targetPos changed > Distance

	int _targetId;
	int _level; // start with 0, the first most insider is 0
	Vec2 _targetPos;
	Vec2 _movePos;
	ICharacter* _owner;
	void exec();
	void setMovePos( const Vec2& targetPos );
	void stopCross();
	IRoleObject* getTarget();
};

class IObjAction
{
public:
	IObjAction();
	~IObjAction();
	virtual void tick(float dt);

	void pushAction(ActionBase* next);
	void replaceAction(ActionBase* next);

	void drawRunGrid(const cocos2d::Vec2& pos);
	void dealDeath();
	virtual void setCrossTarget(int id, int level) {}
	int getNowAction() const;
	void changeMoveTarget(const Vec2& target);
protected:

	ActionBase* _actionNow;
	std::list<ActionBase*> _nextActionList;

	void initAction(IRoleObject* pChar);
	bool isWaitNextAction() const;
	virtual bool autoCreateNext()=0;
	virtual void doNext() =0;

private:
	IRoleObject* _roleObj;
};

class TowerAction : public IObjAction
{
public:
	void initAction(Tower* pChar);
private:	
	Tower* _tower;

	virtual bool autoCreateNext();
	virtual void doNext();
};

class CharacterAction : public IObjAction
{
public:
	CharacterAction(void);
	~CharacterAction(void);

	virtual void tick(float dt);
	void initAction(ICharacter* pChar);
	virtual void setCrossTarget(int id, int level) override;
	int getCrossLevel() const;

private:
	ICharacter* _character;
	CrossHanlder _cross;

	virtual bool autoCreateNext();
	bool autoCreateAttack();
	bool createRun(const cocos2d::Vec2& pos);
	virtual void doNext();

	void setCurPos();
	void resetLocalZOrder();
};

