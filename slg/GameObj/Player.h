#pragma once

#include "ICharacter.h"
#include "Utils/MemPool.h"

#include "Battle/BattleData.h"
#include "Battle/Data/DataPool.hpp"

using namespace CsvCoder;
using namespace Game;

class Player : public ICharacter
{
public:
	DECLARE_POOL(Player)

	Player(void);
	~Player(void);

	static Player* create(const CardHero& card, const Vec2& pos, bool isEnemy = false);

	virtual void recycle();
	virtual void playAppear();
	virtual void setDead();

	// Param:
	
	void setLightPosition(float x, float y, float z);
	//void setShadowData(const CardHero& card);
protected:
	
};

