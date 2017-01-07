#pragma	once
#include <vector>
#include <string>

namespace Tool{

	class HelperFunc
	{
	public:
		//遍历制定目录下的png文件
		static	std::vector<std::string> TraverseFloder(const std::string&	path, const std::string& fileType = "/*.png");
		//获取指定路径下的目录文件，非递归(忽略  .  和 .. )
		static	std::vector<std::string> GetSubDirFiles(const std::string& resRootPath);

		static std::wstring ANSIToUnicode(const std::string& str);
		static std::string UnicodeToANSI(const std::wstring& str);
		static std::wstring UTF8ToUnicode(const std::string& str);
		static std::string UnicodeToUTF8(const std::wstring&);
	};
}
