#ifndef _ACTION_STRUCT_
#define _ACTION_STRUCT_

#include <vector>
#include <map>
#include <list>
#include <string>
#include "cocos2d.h"
#include "Utils/Singleton.hpp"
#include "navi.h"
#include "Utils/BlockAllocator.h"

USING_NS_CC;
using namespace knavi;

class IRoleObject;

class ActionDef
{
public:
	enum Tag
	{
		None = -1,
		Stand = 0,
		Run = 1,
		Attack = 2,
		Dead = 3,

		MAX,		
	};

	static std::string getName(Tag action,int dir = 0);
	static int getLoop(Tag action);
	
	static int getDir(const cocos2d::Vec2& nowPos, const cocos2d::Vec2& targetPos);

	static NaviPoint3D toMapNaviPt(const Vec2& pt);
	static Vec2 toMapVec2(const NaviPoint3D& naviPt);

private:

	struct sAction
	{
		sAction(int _loop, const std::string& _name)
			: loop(_loop), name(_name)
		{}
		int loop;
		std::string name;
	};

	static bool _isInit;
	static std::map<int, sAction> _actionMaps;

	static void lazyInitMap();
	static NaviPoint3D toNaviPt(const Vec2& pt);
	static Vec2 toVec2(const NaviPoint3D& naviPt);
};


class ActionBase
{
public:
	ActionBase();
	virtual ~ActionBase()=0;
	virtual void exec() = 0;
	virtual void doNow(float dt) = 0;

	bool isStand() const;
	bool isMove()  const;
	bool isDead() const;
	bool isAttack() const; 
	bool isFinish() const;

	virtual int getAction() const=0;
	void setOwner(IRoleObject* owner);
	IRoleObject* getOwner() const;

protected:

	bool _isFinish;
	IRoleObject* _owner;
};

class StandAction : public ActionBase
{
public:
	UseBlockAllocator

	virtual int getAction() const { return ActionDef::Stand; }

	virtual void exec() override;
	virtual void doNow(float dt) override;
private:
};

class RunAction : public ActionBase
{
public:
	UseBlockAllocator

	RunAction();

	virtual int getAction() const { return ActionDef::Run; }
	virtual void exec() override;
	virtual void doNow(float dt) override;
	void setMoveTarget( const cocos2d::Vec2& pt);
	void restartMove();
private:
	static const int ResetTargetTime = 300;

	bool _waitStop;
	bool _stopByCross;
	float _resetTargetTime;

	Vec2 _lastPt;
	Vec2 _targetPt;

	bool isWaitCross();
	bool isNearDestination();
	bool checkResetTarget(float dt);
	void resetDir( const Vec2& nowPt );
	bool resetTarget();
};

class AttackAction : public ActionBase
{
public:
	UseBlockAllocator

	virtual int getAction() const { return ActionDef::Attack; }
	virtual void exec() override;
	virtual void doNow(float dt) override;

	bool isTargetsValid() const;
	void setTarget(int id);
	void setTarget(const std::list<int>& targets);
	// º∆À„…À∫¶
	int getDamage(IRoleObject* pAttacker, IRoleObject* target);
private:
	std::list<int> _targets;
};

class DeadAction : public ActionBase
{
public:
	UseBlockAllocator

	virtual int getAction() const { return ActionDef::Dead; }
	virtual void exec() override;
	virtual void doNow(float dt) override;

private:
};

template<typename T>
class ActionFactory
{
public:
	static T* create(IRoleObject* role)
	{
		if (role == NULL)
		{
			throw std::runtime_error("Empty role");
		}

		T* action = new T();
		action->setOwner(role);
		return action;
	}
};


#endif // defined(_ACTION_STRUCT_)
