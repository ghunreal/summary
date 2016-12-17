#include "Player.h"

#include "Battle/BattleManager.h"
#include "Battle/MapGrid.h"
#include "Utils/VisibleRect.h"
#include "GameObj/ObjList.h"
#include "GameDefine.h"
#include "GameObj/RoleAttribute.h"

using namespace Game;

IMPLEMENT_POOL( Player, 20, 10 );

Player::Player(void)
{
}

Player::~Player(void)
{
}

Player* Player::create(const CardHero& card, const Vec2& pos, bool isEnemy)
{
	Vec2 regularPos = BattleManagerIns->getMapGrid()->regularMapPos(pos);
	Player* player = Player::NEW_Player();
	player->setId(card.getTID(), isEnemy);
	player->getAttribute().initAttribute(player);
	player->initArmatureByLooks(isEnemy ? card.getAnimationRed() : card.getAnimationBlue());
	player->setPosition(regularPos);
	player->setScale(0.7f);
	//player->setLightPosition(VisibleRect::rightTop().x, VisibleRect::rightTop().y, 4000);
	//player->setShadowData(card);
	player->setGridNum(card.getGridsNum());
	player->getRoleAction().drawRunGrid(regularPos);
	player->setBLeft(regularPos.x < VisibleRect::center().x);
	player->setBEnmey(isEnemy);
	player->createAgent(regularPos, card.getConsume(), card.getConsume());
	auto maxHp = player->getAttribute().getMaxHp();
	player->initHpBar(maxHp, maxHp, 1.0f, Point(0, player->getObjectSize().height * 0.25), NULL, isEnemy);
	return player;
}

void Player::recycle()
{
	Player::DEL_Player(this);
}

void Player::setDead()
{
	//m_armature->setDisplayShadow(false);
	ICharacter::setDead();
}

void Player::playAppear()
{
}

void Player::setLightPosition(float x, float y, float z)
{
	m_armature->setLightPosition(x, y, z);
}

//void Player::setShadowData(const CardHero& card)
//{
//	int shadowScaleX = 55, shadowScaleY = -55, shadowX = 0, shadowY = 0, shadowSkew = -30;
//	
//	if (card.getTID() == 2) //bird
//	{
//		shadowScaleY = 50;
//		shadowScaleX = 50;
//		shadowSkew = 0;
//		shadowX = -50;
//		shadowY = -200;
//	}
//	
//	m_armature->setShadowData(shadowScaleX, shadowScaleY, shadowX, shadowY, shadowSkew);
//}
