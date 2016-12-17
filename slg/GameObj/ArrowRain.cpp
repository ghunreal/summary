#include "ObjList.h"
#include "Battle/MapGrid.h"
#include "Battle/IBattleUI.h"
#include "Battle/BattleManager.h"

#include "ArrowRain.h"

using namespace Game;

IMPLEMENT_POOL(Arrow, 20, 10);

Arrow::Arrow()
:m_moveSpeed(20),
m_armature(NULL)
{
}
Arrow* Arrow::create(const std::string& name, Vec2 pos)
{
	Arrow* arrow = Arrow::NEW_Arrow();
	arrow->initArmature(name);
	arrow->m_DestPos = pos;

	return arrow;
}

void Arrow::initArmature(const std::string& name)
{
	m_armaName = name;
 	m_armature = Armature::create(name);
// 	m_armature->setDisplayShadow(false);

	addChild(m_armature);
}

float Arrow::play(bool isRedTeam, const std::vector<std::string>& actions, float delayTime, int durationTo /* = -1 */)
{
	if (!m_armature)
	{
		return 0;
	}
	auto ani = m_armature->getAnimation();
	if (!ani)
	{
		return 0;
	}
	//ani->setSpeedScale(0.5f);
	int factor = isRedTeam ? -1 : 1;
	//modify destination position.
	m_DestPos -= Vec2(0, factor * 20);
	auto pTower = isRedTeam ?  EnemyListIns->getKingTower(): PlayerListIns->getKingTower() ;
	auto startPos = pTower->getPosition() + Vec2(35, 100 * factor);

	setPosition(startPos + Vec2(20 * (rand_0_1() - 0.5f), 10 * (rand_0_1() - 0.5f)));
	Vec2 s = m_DestPos - startPos;
	float angle = Vec2::angle(Vec2(0, isRedTeam ? -1 : 1), s);
	int dir = s.x > 0 ? 1 : -1 ;
	if (isRedTeam)
	{
		dir = -dir;
	}
	setRotation(dir * CC_RADIANS_TO_DEGREES(angle));

	auto ins = ArmatureDataManager::getInstance();
	auto data = ins->getAnimationData(m_armaName);
	int frameNum = data->getMovement(actions[0])->duration;
	float time = frameNum / 8.0f;

	auto delay = DelayTime::create(rand_0_1() * 0.3 + delayTime);
	auto moveAct = MoveTo::create(time, m_DestPos);
	auto callFun1 = CallFunc::create([=](){
		ani->playWithNames(actions, durationTo, false);
	});
	auto spawn = Spawn::createWithTwoActions(moveAct, callFun1);

	auto fade = FadeOut::create(1.5f);
	auto seq = Sequence::create(Hide::create(), delay,Show::create(), spawn, fade, CallFunc::create([=](){
		this->removeFromParent();
		DEL_Arrow(this);
	}), nullptr);
	runAction(seq);

	return time;
}

/**************************************Arrow End*********************************************/
ArrowRain::ArrowRain()
	:m_num(15),
	m_fallZone(Size(240, 240)),
	m_gridSize(Size(30, 30)),
	m_damage(1),
	m_destPos(Vec2::ZERO)
{
	m_row = m_fallZone.height / m_gridSize.height;
	m_cloumn = m_fallZone.width / m_gridSize.width;
}

void ArrowRain::play(const CardHero& card, bool isRedTeam, const Vec2& destPos)
{
	m_destPos = destPos;
	MapGrid* mapGrid = BattleManagerIns->getMapGrid();
	
	auto zone = DrawNode::create(2);
	//m_zone->drawSolidCircle(destPos, 80, 0.0f, 28, Color4F(1.0f, 1.0f, 1.0f, 0.2f));
	zone->drawCircle(destPos, 120, 0.0f, 40, false, Color4F(1.0f, 1.0f, 1.0f, 0.4f));
	mapGrid->addChild(zone, MapGrid::ZOrder::Role);

	auto coords = getRandIdx();
	float time = 0;
	float effectTime = playEffect(card, destPos);
	for ( unsigned int i = 0; i < coords.size(); ++i)
	{
		auto arrow = Arrow::create("arrow", getPos(coords[i]) + destPos);
		mapGrid->addChild(arrow, MapGrid::ZOrder::Role);
		time = arrow->play(isRedTeam, getActions(isRedTeam), effectTime, 0);
	}
	
	auto pDelay1 = DelayTime::create(time + effectTime);
	auto pDelay2 = DelayTime::create(1.0f);
	auto pCall = CallFunc::create([=](){
		calculateDamage(isRedTeam);
	});
	zone->runAction(Sequence::create(pDelay1, pCall, pDelay2, RemoveSelf::create(), nullptr));
}

Vec2 ArrowRain::getPos(int idx)
{
	int x = idx % m_cloumn - m_cloumn / 2;
	int y = idx / m_cloumn - m_row / 2;
	Vec2 rand((rand_0_1() - 0.5f) * m_gridSize.width, (rand_0_1() - 0.5f) * m_gridSize.height);
	return Vec2((x + 0.5f)*m_gridSize.width, (y + 0.5f)*m_gridSize.height) + rand;
}

std::vector<int> ArrowRain::getRandIdx()
{
	std::vector<int> nums,ret;
	for (int i = 0; i < m_row*m_cloumn;++i)
	{
		nums.push_back(i);
	}

	for (int i = 0; i < m_num;++i)
	{
		int rand = (nums.size() - 1) * rand_0_1();
		ret.push_back(nums[rand]);
		nums.erase(rand + nums.begin());
	}

	return ret;
}

std::vector<std::string> ArrowRain::getActions(bool isRedTeam)
{
	std::vector < std::string > ret;
	char *redTeamAct[] = { "fallreda", "fallredb", "fallredc" };
	char* blueTeamAct[] = { "fallbluea", "fallblueb", "fallbluec" };

	int n = rand_0_1() * 2;
	if (isRedTeam)
	{
		ret.push_back("flyred");
		ret.push_back(redTeamAct[n]);
	}
	else
	{
		ret.push_back("flyblue");
		ret.push_back(blueTeamAct[n]);
	}
	return ret;
}

float ArrowRain::playEffect(const CardHero& card, const Vec2& destPos)
{
	//cost effect
	float moveTime = 1.0f;
	float fadeTime = 0.5f;

	auto costSp = Sprite::create("res/ui/demo/battle/enegy.png");
	costSp->setScale(1.5f);
	costSp->setPosition(Vec2(0, 60));
	costSp->setCascadeOpacityEnabled(true);
	Size spSize = costSp->getContentSize();
	auto txt = Label::createWithSystemFont(StringUtils::format("-%d", card.getConsume()), "res/fonts/arial.ttf", 24);
	txt->setVerticalAlignment(TextVAlignment::CENTER);
	txt->setPosition(Vec2(spSize.width, spSize.height) * 0.5f);
	costSp->addChild(txt);
	auto pMove = MoveBy::create(moveTime, Vec2(0, 30));
	auto pFade = FadeOut::create(fadeTime);
	costSp->runAction(Sequence::create(pMove, pFade, RemoveSelf::create(), nullptr));
	auto pUi = BattleManagerIns->getCurBattle()->getUi();
	costSp->setPosition(destPos);
	pUi->addChild(costSp, MapGrid::ZOrder::Role);

	return moveTime + fadeTime;
}

void ArrowRain::calculateDamage(bool isRedTeam)
{
	Rect box(m_destPos - m_fallZone * 0.5, m_fallZone);
	ObjList* targets = EnemyListIns;
	if (isRedTeam)
	{
		targets = PlayerListIns;
	}
	
	for (auto iter = targets->begin(); iter != targets->end(); ++iter)
	{
		IRoleObject* role = *iter;
		if (!role || role->isDead())
		{
			continue;
		}
		Rect objRect(CharacterMode::getBodyRect(role->getPosition(), role->getGridNum()));
		if (box.intersectsRect(objRect))
		{
			role->getAttribute().minusHp(100);
		}
	}

	auto tower = targets->getLeftTower();
	auto towerSize = tower->getContentSize();
	auto towerAnchor = tower->getAnchorPoint();
	Vec2 towerPos = tower->getPosition() - Vec2(towerSize.width * towerAnchor.x, towerSize.height * towerAnchor.y);
	if (tower && !tower->isDead() && box.intersectsRect(CharacterMode::getBodyRect(towerPos, tower->getGridNum())))
	{
		tower->getAttribute().minusHp(1);
	}

	tower = targets->getRightTower();
	towerSize = tower->getContentSize();
	towerAnchor = tower->getAnchorPoint();
	towerPos = tower->getPosition() - Vec2(towerSize.width * towerAnchor.x, towerSize.height * towerAnchor.y);
	if (tower && !tower->isDead() && box.intersectsRect(CharacterMode::getBodyRect(towerPos, tower->getGridNum())))
	{
		tower->getAttribute().minusHp(1);
	}
	tower = targets->getKingTower();
	towerSize = tower->getContentSize();
	towerAnchor = tower->getAnchorPoint();
	towerPos = tower->getPosition() - Vec2(towerSize.width * towerAnchor.x, towerSize.height * towerAnchor.y);
	if (tower && !tower->isDead() && box.intersectsRect(CharacterMode::getBodyRect(towerPos, tower->getGridNum())))
	{
		tower->getAttribute().minusHp(1);
	}
};