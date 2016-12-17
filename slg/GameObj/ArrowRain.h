#pragma once
#include "Utils/MemPool.h"
#include "Battle/Data/DataPool.hpp"

using namespace CsvCoder;

class Arrow :public Node
{
public:
	DECLARE_POOL(Arrow)
	Arrow();
	static Arrow* create(const std::string& name, Vec2 pos = Vec2::ZERO);
	float play(bool isRedTeam, const std::vector<std::string>& actions, float delay = 0.0f, int durationTo = -1);
private:
	void initArmature(const std::string& effect);
private:
	float m_moveSpeed;
	Vec2 m_DestPos;
	Armature* m_armature;
	std::string m_armaName;
};

class ArrowRain
{
public:
	ArrowRain();
	float playEffect(const CardHero& card, const Vec2& destPos);
	void play(const CardHero& card, bool isRedTeam, const Vec2& destPos);
	void calculateDamage(bool isRedTeam);
private:
	std::vector<std::string> getActions(bool isRedTeam);
	std::vector<int> getRandIdx();
	Vec2 getPos(int idx);
private:
	int m_row;
	int m_cloumn;
	int m_num;
	Size m_fallZone;
	Size m_gridSize;
	int m_damage;
	Vec2 m_destPos;
};

#define ArrowRainIns Singleton<ArrowRain>::instance()