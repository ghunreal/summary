#pragma once

#include <string>
#include <vector>
#include <sstream>

#include "singleton.hpp"

class StringHelper
{
public :
	int ExtractStrings(std::vector<std::string>& strParams, std::string sSource, char delim);
	void trim(std::string& s);
	void ltrim(std::string& s);
	void rtrim(std::string& s);
	std::string format(const char* format, ...);
	std::string format(const char* format, va_list pArgList);
	template<typename T> std::string toString(T arg){std::stringstream ss; ss << arg; return ss.str();}

	// utf8拆分出asc和汉字
	std::vector<std::string> splitUtf8String( const std::string& source );
	bool isAscChar( char ch );
	int getUtf8Len( char firstCh );

private:
	//解析以空格和Tab分隔的字符串
	int ParsePara(std::vector<std::string>& vectString,
              const char* buf,
              int len,
              char chSeparator = '\0');
};

#define StringHelperIns Singleton<StringHelper>::instance()
