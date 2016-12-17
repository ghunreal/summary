#include "RoleAttribute.h"

#include "ICharacter.h"
#include "Battle/Data/DataPool.hpp"

RoleAttribute::RoleAttribute()
{
}

RoleAttribute::~RoleAttribute()
{

}

void RoleAttribute::initAttribute(IRoleObject* role)
{	
	_role = role;
	setData();
}

void RoleAttribute::reset()
{
	_hp = getMaxHp();
	_role->updateHpBar(_hp, _hp);
}
	
int RoleAttribute::getHp() const
{
	return _hp;
}

void RoleAttribute::setHp(int hp)
{
	_hp = hp;
	_role->updateHpBar(getMaxHp(), _hp);
}

void RoleAttribute::minusHp(int hp)
{
	return;
	//Avoid the same object more than once dead;
	if (_hp>0)
	{
		_hp -= hp;
		_role->updateHpBar(getMaxHp(), _hp);
		if (_hp <= 0)
		{
			_role->setDead();
		}
	}
}

/***************************************RoleAttribute End*****************************************/

void HeroAttribute::setData()
{
	_cardData = &(CardHeroLoaderRef.getByTID(_role->getCardId()));
	setHp(_cardData->getHP());
}

int HeroAttribute::getMaxHp()const
{
	return _cardData->getHP();
}

int HeroAttribute::getAttack()const
{
	return _cardData->getDamage();
}

int HeroAttribute::getSpeed()const
{
	return _cardData->getSpeed();
}

const std::string& HeroAttribute::getCardIcon()const
{
	return _cardData->getCardIcon();
}

int HeroAttribute::getConsume()const
{
	return _cardData->getConsume();
}

/***************************************HeroAttribute End*****************************************/

void BuildingAttribute::setData()
{
	_buildingData = &(buildingsLoaderRef.getByTID(_role->getCardId()));
	setHp(_buildingData->getHitSpeed());
}

int BuildingAttribute::getMaxHp()const
{
	return _buildingData->getHitpoint32s();
}

int BuildingAttribute::getAttack()const
{
	return _buildingData->getDamage();
}

int BuildingAttribute::getSpeed()const
{
	return 0;
}

const std::string& BuildingAttribute::getCardIcon()const
{
	return _buildingData->getCardIcon();
}

int BuildingAttribute::getConsume() const
{
	return 0;
}

/***************************************BuildingAttribute End*****************************************/


CharacterState::CharacterState(void)
	: _isCross(false)
	, _char(NULL)
{
}

CharacterState::~CharacterState(void)
{
}

void CharacterState::initState(ICharacter* pChar)
{
	_char = pChar;
}

void CharacterState::enableCross(bool isOpen)
{
	_isCross = isOpen;
	log("enableCross: id is %d, cross id %d", _char->getId(), isOpen);
}

bool CharacterState::isCross() const
{
	return _isCross;
}
