
#include "cocos2d.h"
#include "HelperFunc.h"

USING_NS_CC;

namespace Tool{

	static bool cmp(const	std::string& l, const	std::string& r)
	{
		std::string name1, name2;

		size_t pos1 = l.find_last_of('.'), pos2 = r.find_last_of('.');
		if (pos1 != std::string::npos && pos2 != std::string::npos)
		{
			name1 = l.substr(0, pos1);
			name2 = r.substr(0, pos2);
		}

		return atoi(name1.c_str()) < atoi(name2.c_str());
	}

	std::vector<std::string> HelperFunc::GetSubDirFiles(const std::string&	resRootPath)
	{
		std::vector<std::string> ret;
		auto dir = resRootPath + "/*";
		WIN32_FIND_DATA findFileData;
		HANDLE hFind;

		hFind = FindFirstFile(ANSIToUnicode(dir).c_str(), &findFileData);
		if (INVALID_HANDLE_VALUE == hFind)
		{
			printf("GetSubDirFiles failed path = %s\n  error : (%d)\n", resRootPath.c_str(), GetLastError());
			return ret;
		}

		while (true)
		{
			//CCLOG("dictionary: %s", UnicodeToANSI(findFileData.cFileName).c_str());
			if (findFileData.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY && findFileData.cFileName[0] != CHAR('.'))
			{
				ret.push_back(UnicodeToANSI(findFileData.cFileName));
			}

			if (!FindNextFile(hFind, &findFileData))
				break;
		}

		return ret;
	}

	std::vector<std::string> HelperFunc::TraverseFloder(const std::string& path, const std::string& fileType)
	{
		std::vector<std::string> ret;
		auto dir = path + fileType;

		WIN32_FIND_DATA findFileData;
		HANDLE hFind;

		hFind = FindFirstFile(ANSIToUnicode(dir).c_str(), &findFileData);
		if (INVALID_HANDLE_VALUE == hFind)
		{
			printf("FindFirstFile path = %s\n failed (%d)\n",dir.c_str(), GetLastError());
			return ret;
		}

		while (true)
		{
			//CCLOG("filename: %s", UnicodeToANSI(findFileData.cFileName).c_str());
			ret.push_back(UnicodeToANSI(findFileData.cFileName));
			if (!FindNextFile(hFind, &findFileData))
				break;
		}

		FindClose(hFind);

		std::sort(ret.begin(), ret.end(), cmp);
		return ret;
	}

	std::wstring HelperFunc::ANSIToUnicode(const std::string& str)
	{
		int  len = 0;
		len = str.length();
		int  unicodeLen = ::MultiByteToWideChar(CP_ACP,
			0,
			str.c_str(),
			-1,
			NULL,
			0);
		wchar_t *  pUnicode;
		pUnicode = new  wchar_t[unicodeLen + 1];
		memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
		::MultiByteToWideChar(CP_ACP,
			0,
			str.c_str(),
			-1,
			(LPWSTR)pUnicode,
			unicodeLen);
		std::wstring  rt;
		rt = (wchar_t*)pUnicode;
		delete  pUnicode;

		return  rt;
	}

	std::string HelperFunc::UnicodeToANSI(const std::wstring& str)
	{
		char*     pElementText;
		int    iTextLen;
		// wide char to multi char
		iTextLen = WideCharToMultiByte(CP_ACP,
			0,
			str.c_str(),
			-1,
			NULL,
			0,
			NULL,
			NULL);
		pElementText = new char[iTextLen + 1];
		memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
		::WideCharToMultiByte(CP_ACP,
			0,
			str.c_str(),
			-1,
			pElementText,
			iTextLen,
			NULL,
			NULL);
		std::string strText;
		strText = pElementText;
		delete[] pElementText;
		return strText;
	}

	std::wstring HelperFunc::UTF8ToUnicode(const std::string& str)
	{
		int  len = 0;
		len = str.length();
		int  unicodeLen = ::MultiByteToWideChar(CP_UTF8,
			0,
			str.c_str(),
			-1,
			NULL,
			0);
		wchar_t *  pUnicode;
		pUnicode = new  wchar_t[unicodeLen + 1];
		memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
		::MultiByteToWideChar(CP_UTF8,
			0,
			str.c_str(),
			-1,
			(LPWSTR)pUnicode,
			unicodeLen);
		std::wstring  rt;
		rt = (wchar_t*)pUnicode;
		delete  pUnicode;

		return  rt;
	}

	std::string HelperFunc::UnicodeToUTF8(const std::wstring& str)
	{
		char*     pElementText;
		int    iTextLen;
		// wide char to multi char
		iTextLen = WideCharToMultiByte(CP_UTF8,
			0,
			str.c_str(),
			-1,
			NULL,
			0,
			NULL,
			NULL);
		pElementText = new char[iTextLen + 1];
		memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
		::WideCharToMultiByte(CP_UTF8,
			0,
			str.c_str(),
			-1,
			pElementText,
			iTextLen,
			NULL,
			NULL);
		std::string strText;
		strText = pElementText;
		delete[] pElementText;
		return strText;
	}
}