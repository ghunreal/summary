#include "CsvPraser.h"

/*
	usage
	eg:
	
	std::vector<std::vector<std::string>> fileStr;
	auto csv = CsvPraser::getInstance();
	csv->prase(xxx);
	auto types = fileStr[1];
	for (int i = 2; i < fileStr.size(); ++i)
	{
		auto field = fileStr[i];
		int iV		= atoi(field[0].c_str());
		float fV	= atof(field[1].c_str());
	}
*/
static CsvPraser * s_csvIns = nullptr;

CsvPraser* CsvPraser::getInstance()
{
	if (!s_csvIns)
	{
		s_csvIns = new (std::nothrow) CsvPraser();
	}

	return s_csvIns;
}

CsvPraser::CsvPraser()
{}

void CsvPraser::prase(const std::string& fileName, std::vector<std::vector<std::string>>& out)
{
	out.clear();

	auto str = cocos2d::FileUtils::getInstance()->getStringFromFile("res/config/"  + fileName);
	std::vector<std::string> rows;
	split(str, rows, "\n", 1);
	for (int i = 0; i < rows.size(); ++i)
	{
		std::vector<std::string> fields;
		split(rows[i], fields, ",");
		out.push_back(fields);
	}
}

void CsvPraser::split(const std::string& str, std::vector<std::string>& dest,
	const std::string seperator /* = "" */, int offset/* = 0 */)
{
	dest.clear();

	size_t start = 0;
	size_t end = str.find(seperator, start);
	for (int i = 0; i < offset; ++i)
	{
		start = end + seperator.size();
		end = str.find(seperator, start);
	}

	while (end != std::string::npos)
	{
		std::string strRow = str.substr(start, end - start);
		start = end + seperator.size();
		end = str.find(seperator, start);
		dest.push_back(strRow);
	}
}

void CsvPraser::strToIntArray(const std::string&str, std::string delims, std::vector<int32_t>& out)
{
	out.clear();
	if (str.size() < 1)
	{
		return;
	}

	size_t start = 0;
	size_t end = str.find(delims, start);

	while (end != std::string::npos)
	{
		std::string field = str.substr(start, end - start);
		out.push_back(atoi(field.c_str()));

		start = end + delims.size();
		end = str.find(delims, start);
	}
	out.push_back(atoi(str.substr(start, str.size() - start).c_str()));
}

void CsvPraser::strToFloatArray(const std::string&str, std::string delims, std::vector<float>& out)
{
	out.clear();
	if (str.size() < 1)
	{
		return;
	}

	size_t start = 0;
	size_t end = str.find(delims, start);

	while (end != std::string::npos)
	{
		std::string strRow = str.substr(start, end - start);
		start = end + delims.size();
		end = str.find(delims, start);
		out.push_back(atof(strRow.c_str()));
	}
	out.push_back(atof(str.substr(start, str.size() - start).c_str()));
}