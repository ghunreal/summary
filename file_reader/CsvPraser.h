#pragma 
#include "cocos2d.h"
#include <string>
#include <vector>
#include <cstdint>

class CsvPraser
{
public:
	CsvPraser();

	static CsvPraser* getInstance();
	void prase(const std::string&, std::vector<std::vector<std::string>>&);
	//"1;2;3" --> [1,2,3]
	void strToIntArray(const std::string&str, std::string delims, std::vector<int32_t>&);
	void strToFloatArray(const std::string&str, std::string delims, std::vector<float>&);
private:
	void split(const std::string& str, std::vector<std::string>& dest, const std::string seperator = "\n", int offset = 0);
};

