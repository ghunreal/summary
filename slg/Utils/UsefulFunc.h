#pragma once

#include <vector>
#include <list>
#include <string>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class UsefulFunc
{
public:
	UsefulFunc(void);
	~UsefulFunc(void);

	static float Random0_1();

	// needNeg为是否需要负数
	static int createRand( int min, int max, bool needNeg=false );
	
	// 符合1-100的概率
	static bool fitRate( int rate );

	static bool checkHaveChinese( std::string strbuf );

	static std::vector<std::string> splitString( const std::string& tempStr, const std::string& split );

	static unsigned int getTickCount();

	static std::string getFormatTime( int seconds, bool isShowHour, const std::string& format="%02d" );
	static void getTimeMSFormat( unsigned long long sTime, char* pM, char* pS);

	static bool containsWorldPt( const cocos2d::Node* pNode, const cocos2d::Point& pt );
	static bool containsRect( const cocos2d::Node* pNode, const cocos2d::Rect& rect );
	static bool containsAbPt(const cocos2d::Node* pNode, const cocos2d::Point& pt);

	static std::vector<int> getRandomNumList(int size);

	static std::string UTF8toANSI(const std::string& src);
private:
	static unsigned int lastSeed;
};

