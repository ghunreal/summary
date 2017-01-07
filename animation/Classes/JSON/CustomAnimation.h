#pragma once
#include <string>
#include <unordered_map>

#include "cocos2d.h"

USING_NS_CC;
namespace Tool{
	struct FrameImgTime{
		std::string name;
		float			delay;
	};
	typedef std::unordered_map <std::string, std::unordered_map<std::string, std::vector<FrameImgTime>>> Actions_map;

	class CustomAnimation:public Ref
	{
	public:
		static CustomAnimation* getInstance();
		/*
		@param:resPath--> ��Դ·��������·����eg��F:/test/texture/animations/
		roleName--> ��ɫ��Դ�ļ�����
		plistPath --> ��ɫ��ͼ��Դ·�� ����·��
		savePath --> ����json����·�� ����·��
		*/
		static void exportToJson(const std::string& resPath, const std::string& roleName, const std::string& plistPath, const std::string& savePath, int frameRate);

		//void parse(const std::string&);

		Animate* getAnimate(const std::string& roleName, const std::string& actionName, int direction, float fps);
		std::vector<std::string> getActions(const std::string& roleName);
		void init();
	private:
		bool addPlist(const std::string& roleName);
		void initRole(const std::string& roleName);

	private:
		std::string _jsonPath;
		std::unordered_map<std::string, Actions_map> _roleActionMap;
		std::unordered_map<std::string, std::vector<std::string>> _plistMap;
	};
}
