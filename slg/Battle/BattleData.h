#pragma once

#include <string>
#include <vector>
#include "Utils/Singleton.hpp"
#include "Battle/Data/DataPool.hpp"

using namespace CsvCoder;

namespace Game
{
	class BattleData
	{
	public:
		BattleData();
		~BattleData();

		void setData();
		void setId( int id );
		void setName(const std::string& name);
		void pushItem(int id, int itemId);

		const std::vector<CardHero>& getCards() const { return _cards; }

	private:
		void initCards();
		std::vector<CardHero> _cards;
	};

	class BattleCards
	{
	public:
		void initCards();
		const CardHero* getNext() const;
		int getSize()const { return _cardList.size(); }
		//const CardHero* getCard(int id){ }
	private:
		mutable int _curIdx;
		std::vector<const CardHero*> _cardList;
	};

#define BattleDataIns Singleton<BattleData>::instance()
#define BattleCardsIns Singleton<BattleCards>::instance()

};
