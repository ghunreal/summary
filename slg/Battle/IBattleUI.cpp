#include "IBattleUI.h"

#include "Utils/StringHelper.h"
#include "GameObj/ObjList.h"
#include "GameObj/Player.h"
#include "Battle/BattleManager.h"
#include "Utils/UsefulFunc.h"
#include "GameObj/ArrowRain.h"
#include "GameDefine.h"

IBattleUI::IBattleUI()
	: _touchListener(NULL)
	, _tips(NULL)
{
}

IBattleUI::~IBattleUI()
{
	CC_SAFE_RELEASE(_touchListener);
	if (_mapGrid)
	{
		_mapGrid->removeFromParent();
	}
}

void IBattleUI::initWithUi(cocos2d::Node* pUi)
{
	_battleUi = pUi;	
	_waitTick = 0;
	_countDown = 0;
	_waitSummary = SummaryTime;
	_playerMp = 0;
	updatePlayerMp(5);
	_battleUi->getChildByName("img_enemy_point")->setVisible(false);
	_battleUi->getChildByName("img_player_point")->setVisible(false);
	_tips = _battleUi->getChildByName("tips");
	_tips->setOpacity(0);
	_tips->setCascadeOpacityEnabled(true);

	setLeftSeconds(MaxTime);
	initCards();
	initCardImages();
	auto scheduler = Director::getInstance()->getScheduler();
	scheduler->schedule(schedule_selector(IBattleUI::tick),this,0,false);

	_navi = NaviFactoryPtr->createNavi(50);
	_navi->load("res/slg_map.obj");
}

void IBattleUI::closeUi()
{
	EventDispatcher* dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListener(_touchListener);

	Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
	for (auto it : _selCardImages)
	{
		it->setTouchEnabled(false);
	}
}

void IBattleUI::updatePoint(bool isEnemy, int point)
{
	std::string name = isEnemy ? "img_player_point" : "img_enemy_point";
	Sprite* pointSp = dynamic_cast<Sprite*>(_battleUi->getChildByName(name));
	pointSp->setVisible(true);
	pointSp->setTexture(StringHelperIns->format("res/ui/demo/battle/point_%d.png", point));
}

void IBattleUI::startCountDown()
{
	addTouchEvt(_battleUi);

	_countDown = MaxTime;
}

void IBattleUI::addTouchEvt(cocos2d::Node* pUi)
{
	if (!_touchListener)
	{
		_touchListener = EventListenerTouchOneByOne::create();
		CC_SAFE_RETAIN(_touchListener);
		_touchListener->setSwallowTouches(true);
		_touchListener->onTouchBegan = CC_CALLBACK_2(IBattleUI::onTouchBegan, this);
		_touchListener->onTouchMoved = CC_CALLBACK_2(IBattleUI::onTouchMoved, this);
		_touchListener->onTouchEnded = CC_CALLBACK_2(IBattleUI::onTouchEnded, this);
		_touchListener->onTouchCancelled = CC_CALLBACK_2(IBattleUI::onTouchEnded, this);
	}
	EventDispatcher* dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(_touchListener, pUi);

	for (auto it : _selCardImages)
	{
		it->addTouchEventListener(CC_CALLBACK_2(IBattleUI::onSelCard, this));
	}
}

void IBattleUI::tick(float dt)
{
	if (PlayerListIns->getKingTower()->isDead()
		|| EnemyListIns->getKingTower()->isDead())
	{
		_waitSummary -= dt * 1000;
		if (_waitSummary <= 0)
		{
			PlayerListIns->clear();
			EnemyListIns->clear();
			closeUi();
			BattleManagerIns->endBattle(0,0);
		}
		return;
	}
	_navi->update(dt*1000);
	PlayerListIns->tick(dt);
	EnemyListIns->tick(dt);
	if (_waitTick > 0)
	{
		_waitTick -= dt*1000; 
		if (_waitTick <= 0)
		{
			_waitTick = 0;
			moveWaitCard();
		}
	}

	if (_countDown > 0)
	{
		_countDown -= dt;
		if (_leftSeconds-_countDown >= 1 )
		{
			setLeftSeconds(_leftSeconds-1);
		}
		if (_playerMp < 10)
		{
			updatePlayerMp(_playerMp + dt);
		}
	}
}

void IBattleUI::updatePlayerMp(float mp)
{
	mp = 10;
	if ((int)_playerMp != (int)mp)
	{
		TextBMFont* cost = static_cast<TextBMFont*>(_battleUi->getChildByName("lbl_enegy"));
		cost->setString(StringHelperIns->format("*%d",(int)mp));
	}
	_playerMp = mp;
	LoadingBar* bar = static_cast<LoadingBar*>(_battleUi->getChildByName("bar_enegy"));
	bar->setPercent(mp * 10);
}

void IBattleUI::setLeftSeconds(int second)
{
	_leftSeconds = second;
	TextBMFont* time = static_cast<TextBMFont*>(_battleUi->getChildByName("lbl_time"));
	time->setString(UsefulFunc::getFormatTime(_leftSeconds,false,"%d"));

	static int mod = 6;
	if (second != MaxTime && (MaxTime-second) % mod == 0)
	{
		mod += MaxTime - second + 3;
		if (mod > MaxTime - second + 12) mod = MaxTime - second + 6;
		createEnemys();
	}
}

void IBattleUI::createEnemys()
{
	const std::vector<CardHero>& cards = BattleDataIns->getCards();
	std::vector<int> randomList = UsefulFunc::getRandomNumList(2);
	//randomList[0] = 8;
	int rand = UsefulFunc::createRand(1, 3);
	if (rand == 1 || rand == 3)
	{
		auto card = cards[randomList[0]];
		Vec2 pos;
		if (card.getTID() != 10009)
		{
			pos = Vec2(400, 1120);
		}
		else
		{
			pos = PlayerListIns->getKingTower()->getPosition() + Vec2(35, 100);
		}
		placeCard(pos, card, true);
	}

	if (rand == 2 || rand == 3)
	{
		auto card = cards[randomList[1]];
		Vec2 pos;
		if (card.getTID() != 10009)
		{
			pos = Vec2(730, 1120);
		}
		else
		{
			pos = PlayerListIns->getKingTower()->getPosition() + Vec2(35, 100);
		}
		placeCard(pos, card, true);
	}
}

void IBattleUI::onSelCard(Ref* ref, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		ImageView* selCard = static_cast<ImageView*>(ref);
		int curIdx = selCard->getTag();
		if (curIdx != _curCardIdx)
		{
			resetLastCard();
			
			_curCardIdx = selCard->getTag();
			int nowY = _selCardImages[_curCardIdx]->getPositionY();
			_selCardImages[_curCardIdx]->setPositionY(nowY + CardUpLen);
			_selCardImages[_curCardIdx]->getChildByName("img_effect")->setVisible(true);
			BattleManagerIns->getMapGrid()->showSettingArea(true);
		}
	}
}

void IBattleUI::resetLastCard()
{
	if (_curCardIdx >= 0 && _curCardIdx < (int)_selCards.size())
	{
		int nowY = _selCardImages[_curCardIdx]->getPositionY();
		_selCardImages[_curCardIdx]->setPositionY(nowY - CardUpLen);
		_selCardImages[_curCardIdx]->getChildByName("img_effect")->setVisible(false);
	}
}

bool IBattleUI::onTouchBegan(Touch *touch, Event *event)
{
	return true;
}

void IBattleUI::onTouchMoved(Touch *touch, Event *event)
{

}

void IBattleUI::onTouchEnded(Touch *touch, Event *event)
{
	if (_curCardIdx >= 0)
	{
		Vec2 touchPos = touch->getLocation();
		auto mapGrid = BattleManagerIns->getMapGrid();
		if ( !mapGrid->isInMap(touchPos))
			return;

		int gridNum = _selCards[_curCardIdx]->getGridsNum();
		
		touchPos = mapGrid->regularMapPos(touchPos);
		if (_selCards[_curCardIdx]->getTID() != 10009
			&& !mapGrid->getSettingArea()->isSettingPt(touchPos, gridNum))
		{
			return;
		}
		if (_selCards[_curCardIdx]->getConsume() > _playerMp)
		{
			// 提示紫水不足
			_tips->stopAllActions();
			_tips->setPositionY(touchPos.y);
			_tips->setOpacity(255);
			auto pMove = MoveBy::create(1.0f, Vec2(0, 20));
			auto pFade = FadeOut::create(1.5f);
			_tips->runAction(Sequence::createWithTwoActions(pMove, pFade));
			return;
		}
		
		updatePlayerMp(_playerMp - _selCards[_curCardIdx]->getConsume());
		_showList.push_back(_curCardIdx);
		_selCardImages[_curCardIdx]->setVisible(false);

		placeCard(touchPos, *_selCards[_curCardIdx], false);
		_waitList.push_back(_selCards[_curCardIdx]);
		if (_waitTick <= 0)
		{
			_waitTick = WaitTime;
		}
		_curCardIdx = -1;
		BattleManagerIns->getMapGrid()->showSettingArea(false);
	}
}

void IBattleUI::initCards()
{	
	BattleCardsIns->initCards();
	_curCardIdx = -1;
	_selCards.clear();
	_waitList.clear();
	_showList.clear();
	for (int i = 0; i < SelSize; ++i)
	{
		_selCards.push_back(BattleCardsIns->getNext());
	}
	for (int i = SelSize; i < BattleCardsIns->getSize();++i)
	{
		_waitList.push_back(BattleCardsIns->getNext());
	}
}

void IBattleUI::initCardImages()
{
	_selCardImages.clear();
	for (int i = 0; i < SelSize; ++i)
	{
		std::string name = StringHelperIns->format("img_card%d", i + 1);
		ImageView* image = static_cast<ImageView*>(_battleUi->getChildByName(name));

		_selCardImages.push_back(image);
		image->setTag(i);
		showCardImage(image, _selCards[i]);
	}
	_waitCardImg = static_cast<ImageView*>(_battleUi->getChildByName("img_card_wait"));
	showCardImage(_waitCardImg, _waitList.front());
}

void IBattleUI::showCardImage(ImageView* image, const CardHero* card)
{
	image->loadTexture("res/role/card/"+card->getCardIcon());
	TextBMFont* cost = static_cast<TextBMFont*>(image->getChildByName("lbl_cost_enegy"));
	cost->setString(StringHelperIns->toString(card->getConsume()));
	image->getChildByName("img_effect")->setVisible(false);
}

void IBattleUI::moveWaitCard()
{
	const CardHero* curCard = _waitList.front();
	int curIdx = _showList.front();
	int nowY = _selCardImages[curIdx]->getPositionY();
	_selCardImages[curIdx]->setPositionY(nowY - CardUpLen);
	showCardImage(_selCardImages[curIdx], curCard);
	_selCardImages[curIdx]->setVisible(true);
	_selCards[curIdx] = curCard;
	_waitList.pop_front();
	_showList.pop_front();

	if (SelSize+(int)_waitList.size() > BattleCardsIns->getSize())
	{
		_waitTick = WaitTime;
	}
	
	showCardImage(_waitCardImg, _waitList.front());
}

void IBattleUI::placeCard(const Vec2& pos, const CardHero& card, bool isRedTeam /* = false */)
{
	if (card.getTID() != 10009)
	{
		Player* player = Player::create(card, pos, isRedTeam);
		playBirthEffect(player, card);

		ObjList* list = isRedTeam ? (ObjList*)EnemyListIns : (ObjList*)PlayerListIns;
		BattleManagerIns->addRole(list, player);
		checkCross(player);
	}
	else
	{
		ArrowRainIns->play(card, isRedTeam, pos);
	}
}

void IBattleUI::playBirthEffect(ICharacter* pChar, const CardHero& card)
{
	auto barSp = Sprite::create("res/ui/demo/battle/clock.png");
	auto pProgress = ProgressTimer::create(barSp);
	pProgress->setType(ProgressTimer::Type::RADIAL);
	pProgress->setReverseProgress(false);
	pProgress->setMidpoint(Vec2(0.5f, 0.5f));
	pProgress->setPosition(Vec2(0, 80));
	auto pAct = ProgressTo::create(card.getConsume()/2, 100.f);
	auto pRemove = RemoveSelf::create();
	auto pCallAct = CallFunc::create([=](){
		if (pChar)
		{
			pChar->borned();
		}
	});
	pProgress->setScale(0.5f);
	auto pProgessAct = Sequence::create(pAct, pCallAct, pRemove, nullptr);
	pProgessAct->retain();
	//pProgress->runAction();
	pChar->addChild(pProgress);

	//cost effect
	auto costSp = Sprite::create("res/ui/demo/battle/enegy.png");
	costSp->setScale(1.5f);
	costSp->setPosition(Vec2(0, 60));
	costSp->setCascadeOpacityEnabled(true);
	Size spSize = costSp->getContentSize();
	auto txt = Label::createWithSystemFont(StringUtils::format("-%d", card.getConsume()), "res/fonts/arial.ttf", 24);
	txt->setVerticalAlignment(TextVAlignment::CENTER);
	txt->setPosition(Vec2(spSize.width, spSize.height) * 0.5f);
	costSp->addChild(txt);
	auto pMove = MoveBy::create(/*card.getLoadTime()*/1, Vec2(0, 30));
	auto pFade = FadeOut::create(0.5f);
	auto pCall = CallFunc::create([=](){
		pChar->setOpacity(255);
		pProgress->runAction(pProgessAct);
		pProgessAct->release();
	});
	pChar->setOpacity(150);
	costSp->runAction(Sequence::create(pMove, pFade, pCall, RemoveSelf::create(), nullptr));
	pChar->addChild(costSp);
}

void IBattleUI::checkCross(ICharacter* target)
{	
	ObjListUtils->setOthersCross(target, 1);
}
