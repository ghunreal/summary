#include "StringHelper.h"
#include <algorithm>
#include <locale>
#include <cctype>
#include <functional>
#include <stdarg.h>

void StringHelper::trim(std::string& s)
{
	ltrim(s);
	rtrim(s);
}

void StringHelper::ltrim(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
}

void StringHelper::rtrim(std::string& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

std::string StringHelper::format(const char* format, va_list pArgList)
{
	#define CC_MAX_STRING_LENGTH (1024*100)
    
    std::string ret;
    
    char* buf = (char*)malloc(CC_MAX_STRING_LENGTH);
    if (buf != nullptr)
    {
        vsnprintf(buf, CC_MAX_STRING_LENGTH, format, pArgList);
        ret = buf;
        free(buf);
    }
    
    return ret;
}

std::string StringHelper::format(const char* format, ...)
{
	#define CC_MAX_STRING_LENGTH (1024*100)
    
    std::string ret;
    
    va_list ap;
    va_start(ap, format);
    
    char* buf = (char*)malloc(CC_MAX_STRING_LENGTH);
    if (buf != nullptr)
    {
        vsnprintf(buf, CC_MAX_STRING_LENGTH, format, ap);
        ret = buf;
        free(buf);
    }
    va_end(ap);
    
    return ret;
}

int StringHelper::ExtractStrings(std::vector<std::string>& strParams,
    std::string sSource, char delim)
{
    return ParsePara(strParams, sSource.data(), sSource.size(), delim);
}

int StringHelper::ParsePara(std::vector<std::string>& vectString,
              const char* buf,
              int len,
              char chSeparator)
{
    if (buf == NULL)
    {
        return vectString.size();
    }

    vectString.clear();

    const char* pStart = buf;
    const char* pEnd = NULL;
    std::string str;
    
    while (true)
    {
        pEnd = (const char*)memchr(pStart, chSeparator, buf + len - pStart);
        if (pEnd >= pStart)
        {
            str.assign(pStart, pEnd);
			trim(str);
            vectString.push_back(str);
        }
        else
        {
            str.assign(pStart, buf + len);
			trim(str);
            vectString.push_back(str);
            break;
        }
        pStart = pEnd + 1;
    }

    return vectString.size();
}

std::vector<std::string> StringHelper::splitUtf8String( const std::string& source )
{
	std::vector<std::string> retVec;
	std::size_t index = 0;
	while ( index < source.size() )
	{
		int size = getUtf8Len(source.at(index));
		std::string str = source.substr(index,size);
		index += size;
		retVec.push_back(str);
	}
	return std::move(retVec);
}

bool StringHelper::isAscChar( char ch )
{
	int num = (ch+0xFF)%0xFF;
	if ( num < 128 )
		return true;
	return false;
}

int StringHelper::getUtf8Len( char ch )
{
	int num = (ch+0xFF)%0xFF;
	if ( num >= 0xF0 )  // 0b11110000
	{
		return 4;
	}
	if ( num >= 0xE0 ) // 0b11100000
	{
		return 3;
	}
	if ( num >= 0xC0 ) // 0b11000000
	{
		return 2;
	}
	return 1;
}

