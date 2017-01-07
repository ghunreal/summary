#include "cocos2d.h"
#include "Preference.h"
#include "common.h"

USING_NS_CC;

namespace Tool{
	std::string Preference::g_rootPath = "";
	int Preference::g_scale = 1;
	int Preference::g_AniSpeed = 1;
	int Preference::g_fps = 8;
	int Preference::g_moveSpeed = 40;

	void Preference::init()
	{
		auto userDefault = UserDefault::getInstance();
		g_scale = userDefault->getIntegerForKey(keyScale, defaultScale);
		g_AniSpeed = userDefault->getIntegerForKey(keyAniSpeed, defaultAniSpeed);
		g_fps = userDefault->getIntegerForKey(keyFps, defaultFps);
		g_moveSpeed = userDefault->getIntegerForKey(keyMoveSpeed, defaultMoveSpeed);

		g_rootPath = FileUtils::getInstance()->getDefaultResourceRootPath();
	}

	void Preference::setScale(int scale)
	{
		g_scale = scale;
		auto userDefault = UserDefault::getInstance();
		userDefault->setIntegerForKey(keyScale, scale);
	}

	void Preference::setAniSpeed(int speed)
	{
		g_AniSpeed = speed;
		auto userDefault = UserDefault::getInstance();
		userDefault->setIntegerForKey(keyAniSpeed, speed);
	}

	void Preference::setFps(int fps)
	{
		g_fps = fps;
		auto userDefault = UserDefault::getInstance();
		userDefault->setIntegerForKey(keyFps, fps);
	}

	void Preference::setMoveSpeed(int speed)
	{
		g_moveSpeed = speed;
		auto ud = UserDefault::getInstance();
		ud->setIntegerForKey(keyMoveSpeed, speed);
	}
}