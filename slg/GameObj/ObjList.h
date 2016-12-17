#pragma once

#include <algorithm>
#include <vector>
#include <list>
#include "ICharacter.h"
#include "Utils/Singleton.hpp"

template<typename T, typename C = std::allocator<T> >
class ProtectContainer
{
public:
	ProtectContainer()
		:_isTraverse(false)
	{}

	void add(const T& t) { 
		if (!_isTraverse)
		{
			_container.push_back(t);
			if (_addCb)
			{
				_addCb(t);
			}
		}			
		else
			_tmpAdds.push_back(t);
	}
	void remove(const T& t) {
		if (!_isTraverse)
		{
			_container.erase(std::find(_container.begin(), _container.end(), t));
			if (_removeCb)
			{
				_removeCb(t);
			}
		}			
		else
			_tmpDels.push_back(t);
	}
	void traverseBegin() { _isTraverse = true; }
	
	void traverseEnd() { 
		_isTraverse = false;
		addTemps();
		delTemps();
	}
	void clear() 
	{ 
		_tmpAdds.clear();
		_tmpDels.clear();
		_container.clear(); 
	}
	
	typename C::iterator begin() { return _container.begin(); }
	typename C::iterator end() { return _container.end(); }

	typename C::const_iterator begin() const { return _container.begin(); }
	typename C::const_iterator end() const { return _container.end(); }

	int size() { return _container.size(); }

	void setAddCb(const std::function<void( const T& t)>& func) { _addCb = func; }
	void setRemoveCb(const std::function<void( const T& t)>& func) { _removeCb = func; }

private:
	bool _isTraverse;
	C _container;
	std::vector<T> _tmpAdds;
	std::vector<T> _tmpDels;
	std::function<void(const T& t)> _addCb;
	std::function<void(const T& t)> _removeCb;

	void addTemps() {
		for (auto it : _tmpAdds) {
			add(it);
		}
		_tmpAdds.clear();
	}
	void delTemps() {
		for (auto it : _tmpDels) {
			remove(it);
		}
		_tmpDels.clear();
	}
};

class ObjList
{
public:
	ObjList();
	~ObjList();

	virtual void initTowers()=0;

	void clear();
	void add(ICharacter* pChar);
	void remove(ICharacter* pChar);

	IRoleObject* getUndeadRole(int id) const;
	IRoleObject* getRole(int id) const;
	IRoleObject* findNearestTarget(const IRoleObject* pChar) const;

	Tower* getLeftTower();  // 为了保持使用统一，采用指针
	Tower* getRightTower();
	Tower* getKingTower();
	
	typedef std::vector<ICharacter*>::iterator iterator;
	iterator begin() { return _list.begin(); }
	iterator end() { return _list.end(); }
	void tick(float dt);

	int getPoints() const;
	void addPoint();
	void setMaxPoint();

protected:
	static const int MaxPoint = 3;

	void onAdd( const ICharacter* pChar);
	void onRemove( const ICharacter* pChar);
	void resetTower( bool isEnemy );

	int _points;
	ProtectContainer<ICharacter*, std::vector<ICharacter*> > _list;
	std::map<int, const IRoleObject*> _charMap;

	Tower _leftTower;
	Tower _rightTower;
	Tower _kingTower;

private:
	IRoleObject* findObj(const IRoleObject* pChar) const;
	const IRoleObject* findTower(const IRoleObject* pChar) const;

	struct CrossSort
	{
		bool operator () (const ICharacter* first, const ICharacter* second) const
		{
			if (first != NULL && second != NULL)
			{
				return first->getCrossLevel() > second->getCrossLevel();
			}
			return false;
		}		
	};
};

class PlayerList : public ObjList
{
public:
	virtual void initTowers();
};

class EnemyList : public ObjList 
{
public:
	virtual void initTowers();
};

class ObjListUtility
{
public:
	void setOthersCross(const ICharacter* pChar, int level);
	std::list<ICharacter*> getAll();
private:
};

#define PlayerListIns Singleton<PlayerList>::instance()
#define EnemyListIns Singleton<EnemyList>::instance()
#define ObjListUtils Singleton<ObjListUtility>::instance()
