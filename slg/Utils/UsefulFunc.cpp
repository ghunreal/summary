#include "UsefulFunc.h"

#include "math.h"
#include <stdlib.h>
#include <time.h>
#include "GameDefine.h"

using namespace std;

unsigned int UsefulFunc::lastSeed = 0;

unsigned int getUsec()
{
    struct timeval now;  
    gettimeofday(&now,NULL); 
	return (unsigned int)(now.tv_usec+now.tv_sec*1000000);
}

UsefulFunc::UsefulFunc(void)
{
}

UsefulFunc::~UsefulFunc(void)
{
}

int UsefulFunc::createRand( int min, int max, bool needNeg )
{
	CC_ASSERT( min <= max && min >= 0 && max >= 0 );

	if( min == max )
	{
		return min;
	}

	unsigned int seeds = getUsec();
	srand( seeds+lastSeed );
	lastSeed += 100+seeds;

	int dif = max - min + 1;
	int ret = rand()%dif + min;

	if( needNeg && rand()%2 == 0 )
	{
		ret *= -1;
	}

	return ret;
}

float UsefulFunc::Random0_1()
{
	int rand = 	UsefulFunc::createRand( 1, RAND_MAX );
	return rand/(1.0*RAND_MAX);
}

bool UsefulFunc::fitRate( int rate )
{
	int randRate = createRand(1,100);
	return randRate<=rate;
}

bool UsefulFunc::checkHaveChinese( std::string strbuf )
{
	const char * str = strbuf.c_str();
	char c;
	while(1)
	{
		c=*str++;
		if (c==0) break;  // 如果到字符串尾则说明该字符串没有中文字符
		if (c&0x80)        // 如果字符高位为1且下一字符高位也是1则有中文字符
			if (*str & 0x80) return true;
	}
	return false;
}

vector<string> UsefulFunc::splitString( const string& tempStr, const string& split )
{
	vector<string> retVec;

	int sz = tempStr.size() + 1;
	char* p = new char[sz];
	memset(p, 0, sz);
	memcpy(p, tempStr.c_str(), tempStr.size());
	char* token = strtok( p, split.c_str() );
	while( token != NULL )
	{
		string temp = string(token);
		retVec.push_back( temp );
		token = strtok( NULL, split.c_str() );
	}
	delete []p;
	return retVec;
}

unsigned int UsefulFunc::getTickCount()
{
    struct timeval now;  
    gettimeofday(&now,NULL);
	return (unsigned int)(now.tv_sec*1000 + now.tv_usec/1000);
}

std::string UsefulFunc::getFormatTime(int seconds, bool isShowHour, const std::string& format)
{
	std::string colon = format + ":";
	std::string strTime;
	char tmp[32];	

	int hour = seconds / 3600;
	if( isShowHour )
	{
		sprintf(tmp, colon.c_str(), hour);
		strTime += tmp;
	}

	seconds -= hour * 3600;
	int min = seconds / 60;
	sprintf(tmp, colon.c_str(), min);
	strTime += tmp;

	seconds %= 60;
	sprintf(tmp, "%02d", seconds);
	strTime += tmp;
	return strTime;
}

bool UsefulFunc::containsWorldPt( const cocos2d::Node* pNode, const cocos2d::Point& pt )
{
	CC_ASSERT( pNode != NULL ); 

	Vec2 locationInNode = pNode->convertToNodeSpace(pt);
	auto s = pNode->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
        
    return rect.containsPoint(pt);
}

bool UsefulFunc::containsAbPt( const cocos2d::Node* pNode, const cocos2d::Point& pt )
{
	CC_ASSERT( pNode != NULL ); 
	auto s = pNode->getContentSize();
	const Vec2& anchorPt = pNode->getAnchorPoint();
	int iX = pNode->getPosition().x-s.width*anchorPt.x;
	int iY = pNode->getPosition().y-s.height*anchorPt.y;
    Rect rect = Rect(iX, iY, s.width, s.height);
        
    return rect.containsPoint(pt);
}

bool UsefulFunc::containsRect(const cocos2d::Node* pNode, const cocos2d::Rect& rect)
{
	if (containsAbPt(pNode, rect.origin)
		|| containsAbPt(pNode, rect.origin + Vec2(0, rect.size.height))
		|| containsAbPt(pNode, rect.origin + Vec2(rect.size.width, rect.size.height))
		|| containsAbPt(pNode, rect.origin + Vec2(rect.size.width, 0)))
	{
		return true;
	}
	return false;
}

std::vector<int> UsefulFunc::getRandomNumList(int size)
{
	size = size < 1 ? 1 : size;
	std::list<int> initList;
	for (int i = 0; i < size; ++i)
	{
		initList.push_back(i);
	}

	std::vector<int> ret;
	while (initList.size()>0)
	{
		int randomIdx = createRand(0, initList.size()-1);
		auto it = initList.begin();
		while (randomIdx > 0)
		{
			++it;
			--randomIdx;
		}
		ret.push_back(*it);
		initList.erase(it);
	}
	return ret;
}

std::string UsefulFunc::UTF8toANSI(const std::string& str)
{
	std::string ret = "";
	std::string::size_type index = 0;
	while (index < str.size())
	{
		unsigned char ch = str.at(index);
		if ((ch & 0x80) == 0)  // 1 bytes
		{
			ret += ch;
			++index;
		}
		else if ((ch & 0xC0) != 0) // 2 bytes
		{
			CCASSERT(index + 1 < str.size(), "2 bytes utf8 format err");
			unsigned char next = str.at(index + 1);
			CCASSERT((next & 0x80) != 0, "2 bytes utf8 format err");
			ret += (ch & 0x3F) << 6 | (next & 0x3F);
			index += 2;
		}
		else if ((ch & 0xE0) != 0) // 3 bytes
		{
			CCASSERT(index + 2 < str.size(), "3 bytes utf8 format err");
			unsigned char next1 = str.at(index + 1);
			unsigned char next2 = str.at(index + 2);
			CCASSERT((next1 & 0x80) != 0 && (next2 & 0x80) != 0, "3 bytes utf8 format err");
			ret += (ch & 0x0F) << 12 | (next1 & 0x3F) << 6 | (next2 & 0x3F);
			index += 3;
		}
		else
		{
			CCASSERT(false, "utf8 not support 4 bytes");
		}
	}

	return ret;
}