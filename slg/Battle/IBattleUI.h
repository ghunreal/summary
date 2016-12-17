#pragma once

#include <vector>
#include "cocos2d.h"
#include "BattleData.h"
#include "cocos/ui/CocosGUI.h"
#include "MapGrid.h"
#include "navi.h"

using namespace cocos2d;
using namespace ui;
using namespace Game;
using namespace knavi;

class ICharacter;

class IBattleUI : public Ref
{
public:
	IBattleUI();
	virtual ~IBattleUI()=0;

	virtual void initWithUi(Node* pUi);
	virtual void closeUi();
	Node* getUi() { return _battleUi; }
	void tick(float dt);
	void startCountDown();

	MapGrid* getMapGrid() { return _mapGrid; }
	Navi* getNavi() const { return _navi; }
	void updatePoint(bool isEnemy, int point);
protected:
	static const int SelSize = 4;
	static const int CardUpLen = 10;
	static const int WaitTime = 2000;
	static const int MaxTime = 60*3;
	static const int SummaryTime = 1500;

	int _curCardIdx;
	float _waitTick;
	float _countDown;
	int _leftSeconds;
	float _waitSummary;
	float _playerMp;

	Node* _battleUi;
	Node* _tips;
	EventListenerTouchOneByOne* _touchListener;
	ImageView* _waitCardImg;
	std::vector<const CardHero*> _selCards;
	std::vector<ImageView*> _selCardImages;
	std::list<const CardHero*> _waitList;
	std::list<int> _showList;
	MapGrid* _mapGrid;
	Navi* _navi;

	void initCards();
	virtual void initCardImages();
	void showCardImage(ImageView* image, const CardHero* card);

	void addTouchEvt(cocos2d::Node* pUi);	
	void onSelCard(Ref*, Widget::TouchEventType);
	void resetLastCard();
	void moveWaitCard();
	void setLeftSeconds(int second);
	void createEnemys();
	void updatePlayerMp(float mp);

	// play birth effect.
	void playBirthEffect(ICharacter* pChar, const CardHero& card);
	// ∑≈÷√ø®≈∆
	void placeCard(const Vec2& pos, const CardHero&, bool isRedTeam = false);
	void checkCross(ICharacter* player);
	
	virtual bool onTouchBegan(Touch *touch, Event *event);
	virtual void onTouchMoved(Touch *touch, Event *event);
	virtual void onTouchEnded(Touch *touch, Event *event);
};

