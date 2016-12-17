#pragma once

#include "Utils/BlockAllocator.h"
#include "csv_helper.hpp"

class IRoleObject;
class ICharacter;

namespace CsvCoder 
{ 
	class CardHero; 
	class buildings; 
}

class RoleAttribute
{
public:
	RoleAttribute();
	virtual ~RoleAttribute() = 0;
	
	void initAttribute(IRoleObject* role);
	int getHp() const;
	void minusHp(int hp);
	void reset();

	int getMp() const	{ return _mp; }
	virtual int getAttack()const = 0;
	virtual int getMaxHp()const = 0;
	virtual int getSpeed()const = 0;
	virtual const std::string& getCardIcon()const = 0;
	virtual int getConsume() const = 0;
protected:
	int _hp;
	int _mp;
	float _attack;
	float _defence;
	IRoleObject* _role;

	virtual void setData()=0;
	virtual void setHp(int hp);
};


class HeroAttribute : public RoleAttribute
{
public:
	UseBlockAllocator

	virtual int getMaxHp()const;
	virtual int getAttack()const;
	virtual int getSpeed()const;
	virtual const std::string& getCardIcon()const;
	virtual int getConsume() const override;
private:
	const CsvCoder::CardHero* _cardData;
	virtual void setData() override;
};

class BuildingAttribute : public RoleAttribute
{
public:
	UseBlockAllocator

	virtual int getMaxHp()const;
	virtual int getAttack()const;
	virtual int getSpeed()const;
	virtual const std::string& getCardIcon()const;
	virtual int getConsume() const override;
private:
	const CsvCoder::buildings* _buildingData;
	virtual void setData() override;
};


class CharacterState
{
public:
	CharacterState(void);
	~CharacterState(void);

	void initState(ICharacter* pChar);

	void enableCross(bool isOpen);
	bool isCross() const;

private:
	bool _isCross;
	ICharacter* _char;
};

