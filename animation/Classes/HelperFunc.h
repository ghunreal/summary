#pragma	once
#include <vector>
#include <string>

namespace Tool{

	class HelperFunc
	{
	public:
		//�����ƶ�Ŀ¼�µ�png�ļ�
		static	std::vector<std::string> TraverseFloder(const std::string&	path, const std::string& fileType = "/*.png");
		//��ȡָ��·���µ�Ŀ¼�ļ����ǵݹ�(����  .  �� .. )
		static	std::vector<std::string> GetSubDirFiles(const std::string& resRootPath);

		static std::wstring ANSIToUnicode(const std::string& str);
		static std::string UnicodeToANSI(const std::wstring& str);
		static std::wstring UTF8ToUnicode(const std::string& str);
		static std::string UnicodeToUTF8(const std::wstring&);
	};
}
