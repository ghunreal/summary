#include "BattleData.h"

#include "cocos2d.h"
#include "JSGameBind.h"
#include "Utils/UsefulFunc.h"

using namespace Game;

BattleData::BattleData()
{
	initCards();
}

BattleData::~BattleData()
{
}

void BattleData::initCards()
{
	// to do
	int ids[9] = { 10001, 10002, 10003, 10004, 10005, 10006, 10007, 10008, 10009};
	for (int i = 0; i < 9; ++i)
	{
		_cards.push_back(CardHeroLoaderRef.getByTID(ids[i]));
	}
}

void BattleData::setData()
{
	JSObject* jsObj = getJSObject("BattleData");
	ScriptingCore* sc = ScriptingCore::getInstance();
	JS::RootedValue retval(sc->getGlobalContext());
	sc->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsObj), "getData", 0, NULL, &retval);
	auto cx = sc->getGlobalContext();
	
	cocos2d::__Dictionary* dataDic;
	jsval_to_ccdictionary(cx, retval, &dataDic);

	cocos2d::__Double* id = (cocos2d::__Double*)dataDic->objectForKey("id");
	setId(id->getValue());

	std::string name = dataDic->valueForKey("name")->_string;
	setName(name);

	sc->executeFunctionWithOwner(OBJECT_TO_JSVAL(jsObj), "getArray", 0, NULL, &retval);
	cocos2d::__Array* ccArray;
	jsval_to_ccarray(cx, retval, &ccArray);
	for (int i = 0; i < ccArray->count(); ++i)
	{
		cocos2d::__Dictionary* testDic = (cocos2d::__Dictionary*)(ccArray->getObjectAtIndex(i));
		cocos2d::__Double* id = (cocos2d::__Double*)testDic->objectForKey("id");
		cocos2d::__Double* item = (cocos2d::__Double*)testDic->objectForKey("item");

		pushItem(id->getValue(), item->getValue());
	}
}

void BattleData::setId(int id)
{
	cocos2d::log( "BattleData::setId %d", id );
}

void BattleData::setName(const std::string& name)
{
	cocos2d::log("BattleData::setName %s", name.c_str());
}

void BattleData::pushItem(int id, int itemId)
{
	cocos2d::log( "BattleData::pushItem %d , item id %d", id, itemId );
}

/****************************************BattleData End***********************************************/

void BattleCards::initCards()
{
	_cardList.clear();
	const std::vector<CardHero>& cards = BattleDataIns->getCards();
	std::vector<int> randomList = UsefulFunc::getRandomNumList(cards.size());
	for (auto it : randomList)
	{
		_cardList.push_back(&cards[it]);
	}
	_curIdx = 0;

	/*int randomList[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	for (auto it : randomList)
	{
	_cardList.push_back(&cards[it - 1]);
	}

	_curIdx = 8;*/
}

const CardHero* BattleCards::getNext() const
{
	auto card = _cardList[_curIdx++];
	_curIdx %= _cardList.size();

	return card;
	/*_curIdx = 8;
	return _cardList[8];*/
}